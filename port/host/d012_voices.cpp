// Japanese voices from the player's Japanese disc (d012_voices.hpp). MIT
// licence; see port/README.md.

#include "d012_voices.hpp"

#include "d012_voice_scenes.hpp"
#include "hle/hle_common.hpp"
#include "install/user_data.hpp"
#include "kernel/iso_image.hpp"
#include "portablekit/hle_extension.hpp"
#include "profile.hpp"
#include "psprecomp/guest_memory.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace d012::voices {
namespace {

using portablekit::IsoImage;
using portablekit::hle_extension::FileFilter;

// As the game opens them (D012_TRACE_IO), without the device, lower case.
constexpr std::string_view kIndexPath = "psp_game/usrdir/data/package_info.bin";
constexpr std::string_view kPackagePath = "psp_game/usrdir/data/package.bin";
constexpr std::string_view kMovieFolder = "psp_game/usrdir/data/movie/";
constexpr std::string_view kJapaneseDiscId = "NPJH-50377";
// The Options screen's text (menu overlay 0x08AB9CB8 loads "text/JP/menu/
// option.bin" with the language folder rewritten): the only language of it
// on the USA and European discs is English.
constexpr std::string_view kOptionText = "text/en/menu/option.bin";

constexpr const char *kDiscKey = "voices.japanese_disc";
constexpr const char *kLanguageKey = "voices.language";

constexpr std::uint64_t kSector = IsoImage::kSectorSize;
// PACKAGE_INFO.BIN's record: the name hash, the start sector in its low 20
// bits, and the size in the low 28 bits with the data-install category above.
constexpr std::uint32_t kSectorMask = 0xFFFFFu;
constexpr std::uint32_t kSizeMask = 0x0FFFFFFFu;
constexpr std::size_t kHeader = 16u;
constexpr std::size_t kRecord = 12u;
// Where the game keeps PACKAGE_INFO.BIN while it runs (main 0x08871938).
constexpr std::uint32_t kGameIndex = 0x09B26BC0u;
// The game's PACKAGE.BIN is followed by this much room for the port's files:
// its size is taken once, when the game opens it at boot, and a disc may be
// chosen later. The index's sector field (20 bits) allows 2 GB in all.
constexpr std::uint64_t kRoom = 512ull * 1024u * 1024u;
// The longest name the game hashes: its path buffers are 64 bytes.
constexpr std::size_t kLongestName = 63u;
// The game's key for hiding message text (main 0x089D68AA; tools/character_text.py).
constexpr std::array<std::uint16_t, 16> kTextKey = {0xED98, 0x90AB, 0x5E28, 0x8442, 0x32B1, 0x2DDA, 0xA657, 0xC8BF,
                                                    0x0033, 0x579A, 0xF227, 0x4B4C, 0x36A7, 0x927D, 0x739D, 0x84CE};

std::string lower(std::string_view text) {
    std::string out(text);
    for (char &c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

std::uint32_t le32(const std::uint8_t *p) {
    return static_cast<std::uint32_t>(p[0]) | (static_cast<std::uint32_t>(p[1]) << 8u) |
           (static_cast<std::uint32_t>(p[2]) << 16u) | (static_cast<std::uint32_t>(p[3]) << 24u);
}
std::uint16_t le16(const std::uint8_t *p) { return static_cast<std::uint16_t>(p[0] | (p[1] << 8u)); }
void put32(std::uint8_t *p, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) p[i] = static_cast<std::uint8_t>(value >> (8 * i));
}
void put16(std::uint8_t *p, std::uint16_t value) {
    p[0] = static_cast<std::uint8_t>(value);
    p[1] = static_cast<std::uint8_t>(value >> 8u);
}

std::array<std::uint32_t, 256> crc_table() {
    std::array<std::uint32_t, 256> table{};
    for (std::uint32_t byte = 0; byte < 256u; ++byte) {
        std::uint32_t value = byte << 24u;
        for (int bit = 0; bit < 8; ++bit) value = (value & 0x80000000u) ? (value << 1u) ^ 0x04C11DB7u : value << 1u;
        table[byte] = value;
    }
    return table;
}

// The game's name hash (main 0x08871288): CRC-32/BZIP2 of the lower-case name.
std::uint32_t name_hash(std::string_view name) {
    static const auto table = crc_table();
    std::uint32_t crc = 0xFFFFFFFFu;
    for (const char c : name)
        crc = (crc << 8u) ^ table[(crc >> 24u) ^ static_cast<std::uint8_t>(std::tolower(static_cast<unsigned char>(c)))];
    return ~crc;
}

// CRC is linear: two names of the same length that differ only in "jp"/"en"
// have hashes that differ by the CRC, from zero, of a message holding the
// difference of those two bytes followed by as many zeros as the rest of
// the name, whatever the name is. deltas[n]: the language code followed by
// n more characters.
std::array<std::uint32_t, kLongestName + 1u> language_deltas() {
    static const auto table = crc_table();
    std::array<std::uint32_t, kLongestName + 1u> deltas{};
    for (std::size_t after = 0; after <= kLongestName; ++after) {
        std::uint32_t crc = 0;
        const auto feed = [&](std::uint8_t byte) { crc = (crc << 8u) ^ table[(crc >> 24u) ^ byte]; };
        feed('j' ^ 'e');
        feed('p' ^ 'n');
        for (std::size_t i = 0; i < after; ++i) feed(0u);
        deltas[after] = crc;
    }
    return deltas;
}

struct Record {
    std::uint32_t hash;
    std::uint32_t sector;
    std::uint32_t size;  // with the category nibble
};

std::optional<std::vector<Record>> parse_index(const std::vector<std::uint8_t> &bytes) {
    if (bytes.size() < kHeader || std::memcmp(bytes.data() + 4u, "pack", 4u) != 0) return std::nullopt;
    const std::uint32_t count = le32(bytes.data() + 8u);
    if (bytes.size() < kHeader + std::size_t(count) * kRecord) return std::nullopt;
    std::vector<Record> records(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint8_t *at = bytes.data() + kHeader + std::size_t(i) * kRecord;
        records[i] = {le32(at), le32(at + 4u), le32(at + 8u)};
    }
    return records;
}

bool sorted_contains(const std::vector<std::uint32_t> &sorted, std::uint32_t value) {
    return std::binary_search(sorted.begin(), sorted.end(), value);
}

std::uint64_t whole_sectors(std::uint64_t bytes) { return (bytes + kSector - 1u) / kSector * kSector; }

// The player's Japanese disc, opened and checked.
struct Disc {
    std::filesystem::path path;
    std::unique_ptr<IsoImage> image;
    IsoImage::Entry package{};
    std::vector<std::uint8_t> index;  // its PACKAGE_INFO.BIN
    std::vector<std::string> movies;  // lower case
};

// A Japanese file in place of one of the game's: a file of the Japanese
// disc, or one the port made from both discs (a story scene, a text file).
struct Pair {
    enum class Kind { Sound, Scene, Text };
    std::size_t record;  // in the game's index
    std::uint64_t disc;  // where the file is in the Japanese image, without `bytes`
    std::uint32_t size;
    std::uint64_t at;  // in the game's PACKAGE.BIN as it sees it
    std::shared_ptr<const std::vector<std::uint8_t>> bytes{};
    Kind kind{Kind::Sound};
};

struct State {
    std::mutex lock;  // reads of the Japanese image
    std::optional<Disc> disc;
    bool want{};      // voices.language is japanese
    bool from_env{};  // <prefix>_JAPANESE_VOICES: nothing is saved

    // From the first open of the game's PACKAGE_INFO.BIN.
    bool built{};
    bool building{};
    std::vector<std::uint8_t> own_bytes;
    std::vector<Record> own;
    std::uint64_t own_end{};  // the game's PACKAGE.BIN ends (a whole sector)
    // The Options text with the port's rows, after the game's PACKAGE.BIN.
    std::size_t text_record{SIZE_MAX};
    std::vector<std::uint8_t> text_source;  // the game's own
    std::vector<std::uint8_t> text;
    std::uint64_t text_at{};
    std::uint64_t pairs_from{};  // where the Japanese files start
    std::vector<Pair> pairs;     // by `at`
    bool applied{};              // the game's index points at the Japanese files
    std::size_t files_read{};
    std::vector<bool> read;  // by pair
};
State &state() {
    static State instance;
    return instance;
}

std::filesystem::path data_directory() { return portablekit::install::user_data_directory(); }

void save_setting(const char *key, const std::string &value) {
    State &s = state();
    if (s.from_env) return;
    const auto dir = data_directory();
    auto entries = portablekit::install::read_settings_file(dir);
    entries[key] = value;
    portablekit::install::write_settings_file(dir, entries);
}

std::optional<Disc> open_disc(const std::filesystem::path &path, std::string &error) {
    std::error_code code;
    if (!std::filesystem::is_regular_file(path, code)) {
        error = "The file is not there.";
        return std::nullopt;
    }
    Disc disc;
    disc.path = path;
    try {
        disc.image = std::make_unique<IsoImage>(path);
    } catch (const std::exception &e) {
        error = std::string("It cannot be read (") + e.what() + ").";
        return std::nullopt;
    }
    std::string disc_id;
    if (const auto umd = disc.image->find("UMD_DATA.BIN")) {
        std::array<std::uint8_t, 10> id{};
        if (disc.image->read(std::uint64_t(umd->lba) * kSector, id) == id.size()) disc_id.assign(id.begin(), id.end());
    }
    const auto index = disc.image->find("PSP_GAME/USRDIR/DATA/PACKAGE_INFO.BIN");
    const auto package = disc.image->find("PSP_GAME/USRDIR/DATA/PACKAGE.BIN");
    if (disc_id != kJapaneseDiscId || !index || !package) {
        error = "This is not the Japanese release (NPJH50377).";
        return std::nullopt;
    }
    disc.package = *package;
    disc.index.resize(index->size);
    disc.index.resize(disc.image->read(std::uint64_t(index->lba) * kSector, disc.index));
    for (const std::string &name : disc.image->list("PSP_GAME/USRDIR/DATA/MOVIE")) disc.movies.push_back(lower(name));
    return disc;
}

// Bytes of the game's PACKAGE.BIN as it is on the disc: what is read.
std::size_t read_own(std::uint64_t offset, std::span<std::uint8_t> bytes) {
    State &s = state();
    const bool was_building = s.building;
    s.building = true;
    const std::int64_t got = portablekit::read_guest_file("disc0:/PSP_GAME/USRDIR/DATA/PACKAGE.BIN", offset,
                                                          bytes.data(), bytes.size());
    s.building = was_building;
    return got < 0 ? 0u : static_cast<std::size_t>(got);
}

std::vector<std::uint8_t> own_file(std::size_t record) {
    const Record &r = state().own[record];
    std::vector<std::uint8_t> bytes(r.size & kSizeMask);
    if (read_own(std::uint64_t(r.sector & kSectorMask) * kSector, bytes) != bytes.size()) bytes.clear();
    return bytes;
}

// Files of one disc, by record: read in one pass where they lie together
// (the story scenes do, on both discs), else one by one in their order.
// `read` reads bytes at an offset of that disc's PACKAGE.BIN.
std::map<std::uint32_t, std::vector<std::uint8_t>>
read_files(const std::vector<const Record *> &records,
           const std::function<std::size_t(std::uint64_t, std::span<std::uint8_t>)> &read) {
    std::vector<const Record *> sorted = records;
    std::sort(sorted.begin(), sorted.end(),
              [](const Record *a, const Record *b) { return (a->sector & kSectorMask) < (b->sector & kSectorMask); });
    std::map<std::uint32_t, std::vector<std::uint8_t>> files;
    if (sorted.empty()) return files;
    const auto start = [](const Record *r) { return std::uint64_t(r->sector & kSectorMask) * kSector; };
    std::uint64_t first = start(sorted.front()), end = first;
    for (const Record *r : sorted) end = std::max(end, start(r) + (r->size & kSizeMask));
    constexpr std::uint64_t kOnePass = 64ull * 1024u * 1024u;
    std::vector<std::uint8_t> span;
    if (end - first <= kOnePass) {
        span.resize(end - first);
        span.resize(read(first, span));
    }
    for (const Record *r : sorted) {
        std::vector<std::uint8_t> &bytes = files[r->hash];
        bytes.resize(r->size & kSizeMask);
        if (start(r) - first + bytes.size() <= span.size())
            std::memcpy(bytes.data(), span.data() + (start(r) - first), bytes.size());
        else if (read(start(r), bytes) != bytes.size())
            bytes.clear();
    }
    return files;
}

// "voice/jp/..." as the game asks for it: in the English folder.
std::string english_name(std::string_view path) {
    std::string name(path);
    if (name.starts_with("voice/jp/")) name.replace(6u, 2u, "en");
    return name;
}

// The story scenes whose script differs between the discs, as the Japanese
// disc has them with the game's text, the voice lines they play, and the
// text files fixed for them (d012_voice_scenes.hpp): added to `chosen`, in
// place of what the pairing by name found.
void story(const std::vector<Record> &japanese, const std::map<std::uint32_t, std::size_t> &own_at,
           std::map<std::size_t, Pair> &chosen) {
    State &s = state();
    std::map<std::uint32_t, const Record *> theirs;
    for (const Record &r : japanese) theirs[r.hash] = &r;
    const std::uint64_t base = std::uint64_t(s.disc->package.lba) * kSector;
    const auto disc_offset = [&](const Record &r) { return base + std::uint64_t(r.sector & kSectorMask) * kSector; };
    std::size_t scenes = 0, missing = 0, unlike = 0, lines = 0, texts = 0;
    std::uint64_t bytes = 0;
    std::vector<const Record *> our_records, their_records;
    std::vector<voice_scenes::Voice> same_name, renamed;
    for (const std::uint32_t hash : voice_scenes::scenes()) {
        const auto mine = own_at.find(hash);
        const auto other = theirs.find(hash);
        if (mine == own_at.end() || other == theirs.end()) continue;
        our_records.push_back(&s.own[mine->second]);
        their_records.push_back(other->second);
    }
    const auto our_files = read_files(our_records, read_own);
    const auto their_files = read_files(their_records, [&](std::uint64_t offset, std::span<std::uint8_t> out) {
        std::lock_guard guard(s.lock);
        return s.disc->image->read(base + offset, out);
    });
    for (const std::uint32_t hash : voice_scenes::scenes()) {
        const auto mine = own_at.find(hash);
        const auto other = theirs.find(hash);
        if (mine == own_at.end() || other == theirs.end()) {
            ++missing;
            continue;
        }
        std::vector<voice_scenes::Voice> voices;
        auto merged = voice_scenes::merge_scene(hash, their_files.at(hash), our_files.at(hash), voices);
        if (merged.empty()) {
            ++unlike;
            continue;
        }
        bytes += merged.size();
        const auto size = static_cast<std::uint32_t>(merged.size());
        chosen[mine->second] = {mine->second, 0u, size, 0u,
                                std::make_shared<const std::vector<std::uint8_t>>(std::move(merged)),
                                Pair::Kind::Scene};
        ++scenes;
        for (const voice_scenes::Voice &v : voices) (v.asked == v.japanese ? same_name : renamed).push_back(v);
    }
    // Each line the Japanese file it is, whatever the pairing by name found
    // for that name; a line served under another name last, so that no
    // scene's table naming that file itself (a line it never plays) wins.
    for (const auto *list : {&same_name, &renamed})
        for (const voice_scenes::Voice &v : *list) {
            const auto asked = own_at.find(name_hash(english_name(v.asked)));
            const auto file = theirs.find(name_hash(v.japanese));
            if (asked == own_at.end() || file == theirs.end()) continue;
            if (!chosen.contains(asked->second) || chosen[asked->second].disc != disc_offset(*file->second)) ++lines;
            chosen[asked->second] = {asked->second, disc_offset(*file->second), file->second->size & kSizeMask, 0u};
        }
    // Test only: <prefix>_TEST_SCENE=<scene>=<scene>[,...] plays the second
    // merged scene where the game asks for the first (hashes in hex), to
    // reach a scene from the start of Story Mode.
    if (const char *swaps = portablekit::env("TEST_SCENE")) {
        std::string_view rest(swaps);
        while (!rest.empty()) {
            const std::string_view item = rest.substr(0, rest.find(','));
            rest = item.size() < rest.size() ? rest.substr(item.size() + 1u) : std::string_view{};
            const std::size_t eq = item.find('=');
            if (eq == std::string_view::npos) continue;
            const auto hex = [](std::string_view text) {
                return static_cast<std::uint32_t>(std::strtoul(std::string(text).c_str(), nullptr, 16));
            };
            const auto from = own_at.find(hex(item.substr(0, eq)));
            const auto to = own_at.find(hex(item.substr(eq + 1u)));
            if (from == own_at.end() || to == own_at.end() || !chosen.contains(to->second) ||
                !chosen[to->second].bytes) {
                std::cerr << "[voices] test scene " << item << ": not two scenes of the game's\n";
                continue;
            }
            Pair swapped = chosen[to->second];
            swapped.record = from->second;
            chosen[from->second] = swapped;
            std::cout << "[voices] test: scene " << item.substr(eq + 1u) << " plays where the game asks for "
                      << item.substr(0, eq) << std::endl;
        }
    }
    for (const std::string_view name : voice_scenes::text_files()) {
        const auto mine = own_at.find(name_hash(name));
        if (mine == own_at.end()) continue;
        auto fixed = voice_scenes::fix_text_file(name, own_file(mine->second));
        if (fixed.empty()) continue;
        const auto size = static_cast<std::uint32_t>(fixed.size());
        chosen[mine->second] = {mine->second, 0u, size, 0u,
                                std::make_shared<const std::vector<std::uint8_t>>(std::move(fixed)), Pair::Kind::Text};
        ++texts;
    }
    std::cout << "[voices] " << scenes << " story scenes as the Japanese disc has them (" << bytes / 1024u
              << " KB), " << lines << " voice lines set by them, " << texts << " text files fixed for them";
    if (missing != 0u) std::cout << "; " << missing << " scenes not on both discs";
    if (unlike != 0u) std::cout << "; " << unlike << " not as expected, left English";
    std::cout << std::endl;
}

// Pairs the Japanese disc's sound files with the game's, and adds the story
// scenes: the files from pairs_from on in the game's PACKAGE.BIN.
void pair() {
    State &s = state();
    s.pairs.clear();
    s.read.clear();
    if (!s.disc || !s.built || s.own.empty()) return;
    const auto started = std::chrono::steady_clock::now();
    const auto japanese = parse_index(s.disc->index);
    if (!japanese) {
        std::cerr << "[voices] the Japanese disc's index is not as expected; the voices stay English\n";
        return;
    }
    std::vector<std::uint32_t> own_hashes, japanese_hashes;
    std::map<std::uint32_t, std::size_t> own_at;
    for (std::size_t i = 0; i < s.own.size(); ++i) {
        own_hashes.push_back(s.own[i].hash);
        own_at[s.own[i].hash] = i;
    }
    for (const Record &r : *japanese) japanese_hashes.push_back(r.hash);
    std::sort(own_hashes.begin(), own_hashes.end());
    std::sort(japanese_hashes.begin(), japanese_hashes.end());
    // Files of one disc only: the language folders' files, and what one
    // build has that the other has not.
    std::vector<std::uint32_t> own_only;
    std::set_difference(own_hashes.begin(), own_hashes.end(), japanese_hashes.begin(), japanese_hashes.end(),
                        std::back_inserter(own_only));

    // A Japanese-only file whose hash, as an English name of exactly one
    // length, is an English-only file of the game's.
    static const auto deltas = language_deltas();
    struct Candidate {
        const Record *japanese;
        std::uint32_t own_hash;
    };
    std::vector<Candidate> candidates;
    std::map<std::uint32_t, int> claimed;
    for (const Record &r : *japanese) {
        if (sorted_contains(own_hashes, r.hash)) continue;
        std::uint32_t match = 0;
        int matches = 0;
        for (const std::uint32_t delta : deltas)
            if (sorted_contains(own_only, r.hash ^ delta)) {
                match = r.hash ^ delta;
                ++matches;
            }
        if (matches != 1) continue;
        candidates.push_back({&r, match});
        ++claimed[match];
    }
    std::erase_if(candidates, [&](const Candidate &c) { return claimed[c.own_hash] != 1; });
    const std::size_t named = candidates.size();

    // Sound only: the language folders also hold text and pictures, which
    // stay the game's own.
    std::sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
        return (a.japanese->sector & kSectorMask) < (b.japanese->sector & kSectorMask);
    });
    const std::uint64_t base = std::uint64_t(s.disc->package.lba) * kSector;
    {
        std::lock_guard guard(s.lock);
        std::erase_if(candidates, [&](const Candidate &c) {
            std::uint8_t magic[4]{};
            if (s.disc->image->read(base + std::uint64_t(c.japanese->sector & kSectorMask) * kSector, magic) !=
                sizeof magic)
                return true;
            return std::memcmp(magic, "RIFF", 4) != 0 && std::memcmp(magic, "SSCF", 4) != 0 &&
                   std::memcmp(magic, "VOLD", 4) != 0;
        });
    }
    const std::size_t sound = candidates.size();

    // What each record of the game's becomes.
    std::map<std::size_t, Pair> chosen;
    for (const Candidate &c : candidates) {
        const std::size_t record = own_at[c.own_hash];
        chosen[record] = {record, base + std::uint64_t(c.japanese->sector & kSectorMask) * kSector,
                          c.japanese->size & kSizeMask, 0u};
    }
    // <prefix>_JAPANESE_SCENES=0: the game's own scenes with the Japanese
    // voices, as before (lines run into each other).
    if (const char *scenes = portablekit::env("JAPANESE_SCENES"); scenes == nullptr || std::string_view(scenes) != "0")
        story(*japanese, own_at, chosen);
    else
        std::cout << "[voices] the story scenes stay the game's own (" << portablekit::env_name("JAPANESE_SCENES")
                  << "=0)" << std::endl;

    // After the Options text: the Japanese disc's files in its order, then
    // the port's.
    std::vector<Pair> placed;
    for (const auto &[record, p] : chosen) placed.push_back(p);
    std::stable_sort(placed.begin(), placed.end(), [](const Pair &a, const Pair &b) {
        return std::make_tuple(a.bytes != nullptr, a.disc) < std::make_tuple(b.bytes != nullptr, b.disc);
    });
    std::uint64_t at = s.pairs_from;
    for (Pair &p : placed) {
        if (at + p.size > s.own_end + kRoom) {
            std::cerr << "[voices] no more room after PACKAGE.BIN; the rest stay English\n";
            break;
        }
        p.at = at;
        s.pairs.push_back(p);
        at += whole_sectors(p.size);
    }
    s.read.assign(s.pairs.size(), false);
    const auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - started).count();
    std::cout << "[voices] " << s.pairs.size() << " files of the Japanese disc in place of the English ones ("
              << named << " sound files paired by name, " << sound << " of them sound), "
              << (at - s.pairs_from) / (1024u * 1024u) << " MB, in " << ms << " ms" << std::endl;
}

// The record words of the game's index entry `i`: the Japanese file's with
// `pair`, else the game's own.
void record_words(std::size_t i, const Pair *pair, std::uint32_t &sector, std::uint32_t &size) {
    const State &s = state();
    if (pair != nullptr) {
        sector = static_cast<std::uint32_t>(pair->at / kSector);
        // Category 0: never served from the data install, which holds the
        // English file under this hash.
        size = pair->size;
    } else {
        sector = s.own[i].sector;
        size = s.own[i].size;
    }
}

// The Options text with the port's strings (numbers kTextFirst on) after the
// game's. A string is a u16 byte length and 16-bit units, unit j of string i
// XORed with kTextKey[(i + j) & 15]; the table is a u16 count and a u16
// offset per string in 2-byte units from its start (0: no string).
std::vector<std::uint8_t> option_text(const std::vector<std::uint8_t> &source) {
    if (source.size() < 2u) return {};
    const std::uint16_t count = le16(source.data());
    if (count == 0u || count > kTextFirst || source.size() < 2u + 2u * count) return {};
    const std::u16string confirm = {0x001E, 0x8001};  // the confirm button's picture, as in string 42
    const auto text = [](std::string_view ascii) { return std::u16string(ascii.begin(), ascii.end()); };
    std::vector<std::u16string> added(kTextCount);
    added[kTextDiscTitle - kTextFirst] = text("Japanese Voice Disc");
    added[kTextDiscValue - kTextFirst] = disc_label();
    added[kTextDiscHelp - kTextFirst] =
        text("The disc image of the Japanese release (NPJH50377)\nthat Japanese voices come from.\nChoose one with ") +
        confirm + text(".");
    added[kTextLanguageTitle - kTextFirst] = text("Voice Language");
    added[kTextLanguageHelp - kTextFirst] =
        text("Set the language characters speak in.\nJapanese uses the voices of the Japanese disc image.");

    const std::uint16_t new_count = kTextFirst + kTextCount;
    const std::size_t shift_units = new_count - count;
    std::vector<std::uint8_t> out(2u + 2u * new_count);
    put16(out.data(), new_count);
    for (std::uint16_t i = 0; i < count; ++i) {
        const std::uint16_t offset = le16(source.data() + 2u + 2u * i);
        put16(out.data() + 2u + 2u * i, offset == 0u ? 0u : static_cast<std::uint16_t>(offset + shift_units));
    }
    out.insert(out.end(), source.begin() + 2u + 2u * count, source.end());
    if (out.size() % 2u != 0u) out.push_back(0u);
    for (std::uint16_t k = 0; k < kTextCount; ++k) {
        const std::uint16_t i = kTextFirst + k;
        std::u16string units = added[k];
        units.push_back(0u);
        if (i == kTextDiscValue) units.resize(kDiscValueUnits, 0u);
        if (out.size() / 2u > 0xFFFFu) return {};
        put16(out.data() + 2u + 2u * i, static_cast<std::uint16_t>(out.size() / 2u));
        const std::size_t at = out.size();
        out.resize(at + 2u + 2u * units.size());
        put16(out.data() + at, static_cast<std::uint16_t>(2u * units.size()));
        for (std::size_t j = 0; j < units.size(); ++j)
            put16(out.data() + at + 2u + 2u * j, static_cast<std::uint16_t>(units[j] ^ kTextKey[(i + j) & 15u]));
    }
    return out;
}

// The index the game reads instead of its own: the Options text moved to
// the port's copy, and with Japanese voices the paired records to the
// Japanese files.
std::vector<std::uint8_t> game_index() {
    State &s = state();
    std::vector<std::uint8_t> bytes = s.own_bytes;
    const auto set = [&](std::size_t i, std::uint32_t sector, std::uint32_t size) {
        std::uint8_t *record = bytes.data() + kHeader + i * kRecord;
        put32(record + 4u, sector);
        put32(record + 8u, size);
    };
    if (s.text_record != SIZE_MAX && !s.text.empty())
        set(s.text_record, static_cast<std::uint32_t>(s.text_at / kSector), static_cast<std::uint32_t>(s.text.size()));
    s.applied = s.want && !s.pairs.empty();
    if (s.applied)
        for (const Pair &p : s.pairs) {
            std::uint32_t sector = 0, size = 0;
            record_words(p.record, &p, sector, size);
            set(p.record, sector, size);
        }
    return bytes;
}

void build(const std::vector<std::uint8_t> &own_bytes) {
    State &s = state();
    s.built = true;
    const auto own = parse_index(own_bytes);
    if (!own) {
        std::cerr << "[voices] the game's index of PACKAGE.BIN is not as expected; nothing is added\n";
        return;
    }
    s.own_bytes = own_bytes;
    s.own = *own;
    std::uint64_t own_end = 0;
    for (const Record &r : s.own)
        own_end =
            std::max<std::uint64_t>(own_end, std::uint64_t(r.sector & kSectorMask) * kSector + (r.size & kSizeMask));
    s.own_end = whole_sectors(own_end);

    // The Options text, read through the game's own PACKAGE.BIN.
    const std::uint32_t text_hash = name_hash(kOptionText);
    for (std::size_t i = 0; i < s.own.size(); ++i)
        if (s.own[i].hash == text_hash) s.text_record = i;
    if (s.text_record != SIZE_MAX) {
        const Record &r = s.own[s.text_record];
        s.text_source.resize(r.size & kSizeMask);
        s.building = true;
        const std::int64_t got = portablekit::read_guest_file("disc0:/PSP_GAME/USRDIR/DATA/PACKAGE.BIN",
                                                              std::uint64_t(r.sector & kSectorMask) * kSector,
                                                              s.text_source.data(), s.text_source.size());
        s.building = false;
        if (got != static_cast<std::int64_t>(s.text_source.size())) s.text_source.clear();
        s.text = option_text(s.text_source);
        if (s.text.empty()) std::cerr << "[voices] the Options text is not as expected; its rows have no text\n";
    }
    s.text_at = s.own_end;
    s.pairs_from = whole_sectors(s.text_at + s.text.size());
    pair();
}

class BytesFilter final : public FileFilter {
public:
    explicit BytesFilter(std::vector<std::uint8_t> bytes) : bytes_(std::move(bytes)) {}
    std::uint64_t size() const override { return bytes_.size(); }
    std::size_t read(std::uint64_t offset, std::uint8_t *destination, std::size_t length) override {
        if (offset >= bytes_.size()) return 0u;
        const std::size_t n = std::min<std::uint64_t>(length, bytes_.size() - offset);
        std::memcpy(destination, bytes_.data() + offset, n);
        return n;
    }

private:
    std::vector<std::uint8_t> bytes_;
};

// A story movie with Japanese voices: the game's file, its picture and all,
// with the sound of the Japanese disc's. The picture differs in some (the end
// credits, for one), so the Japanese file cannot play as it is; the sound is
// the same length, or a frame longer.
class MovieFilter final : public FileFilter {
public:
    using Reader = voice_scenes::MovieSplice::Reader;
    // `own` reads the game's movie; with `theirs` (the Japanese disc's) its
    // sound is theirs, without it (a test) the movie is the game's alone.
    // Null when either is not a movie as expected.
    static std::shared_ptr<MovieFilter> open(Reader own, std::optional<IsoImage::Entry> theirs) {
        std::unique_ptr<voice_scenes::MovieSplice> splice;
        if (theirs) {
            splice = voice_scenes::MovieSplice::open(
                own, [entry = *theirs](std::uint64_t at, std::span<std::uint8_t> out) -> std::size_t {
                    State &s = state();
                    if (!s.disc || at >= entry.size) return 0u;
                    std::lock_guard guard(s.lock);
                    return s.disc->image->read(std::uint64_t(entry.lba) * kSector + at,
                                               out.first(std::min<std::uint64_t>(out.size(), entry.size - at)));
                });
            if (!splice) return nullptr;
        }
        // Its size: the last byte there is.
        std::uint64_t low = 0, high = 1;
        std::uint8_t probe = 0;
        while (own(high, {&probe, 1u}) == 1u && high < (1ull << 40u)) high *= 2u;
        while (high - low > 1u) {
            const std::uint64_t middle = low + (high - low) / 2u;
            (own(middle, {&probe, 1u}) == 1u ? low : high) = middle;
        }
        if (own(0u, {&probe, 1u}) != 1u) return nullptr;
        return std::make_shared<MovieFilter>(low + 1u, std::move(own), std::move(splice));
    }

    MovieFilter(std::uint64_t size, Reader own, std::unique_ptr<voice_scenes::MovieSplice> splice)
        : size_(size), own_(std::move(own)), splice_(std::move(splice)) {}

    std::uint64_t size() const override { return size_; }

    std::size_t read(std::uint64_t offset, std::uint8_t *destination, std::size_t length) override {
        if (!splice_) return own_(offset, {destination, length});
        const std::size_t got = splice_->read(offset, {destination, length});
        if (!warned_ && splice_->broken()) {
            warned_ = true;
            std::cerr << "[voices] a movie's packs are not as expected; part of its sound may stay English\n";
        }
        return got;
    }

private:
    std::uint64_t size_;
    Reader own_;
    std::unique_ptr<voice_scenes::MovieSplice> splice_;
    bool warned_{};
};

void note_read(std::size_t pair) {
    State &s = state();
    if (pair >= s.read.size() || s.read[pair]) return;
    s.read[pair] = true;
    const Pair &p = s.pairs[pair];
    if (p.kind != Pair::Kind::Sound) {
        char hash[16];
        std::snprintf(hash, sizeof hash, "%08X", s.own[p.record].hash);
        std::cout << "[voices] the game has read the "
                  << (p.kind == Pair::Kind::Scene ? "story scene " : "text file ") << hash
                  << (p.kind == Pair::Kind::Scene ? " of the Japanese disc" : " fixed for the Japanese scenes")
                  << std::endl;
        return;
    }
    // <prefix>_TRACE_VOICES=1: each one, by the hash of the name the game
    // asked for and where it is on the Japanese disc.
    static const bool trace = portablekit::env("TRACE_VOICES") != nullptr;
    ++s.files_read;
    if (trace) {
        char line[96];
        std::snprintf(line, sizeof line, "[voices] read %08X: the Japanese disc's file at byte %llu, %u bytes",
                      s.own[p.record].hash, static_cast<unsigned long long>(p.disc), p.size);
        std::cout << line << std::endl;
    } else if (s.files_read <= 3u || s.files_read % 100u == 0u) {
        std::cout << "[voices] the game has read " << s.files_read << " Japanese sound file"
                  << (s.files_read == 1u ? "" : "s") << " (the last " << p.size << " bytes)" << std::endl;
    }
}

// The game's PACKAGE.BIN, then the port's Options text and the paired
// Japanese files after it.
class PackageFilter final : public FileFilter {
public:
    explicit PackageFilter(std::uint32_t fd) : fd_(fd) {}
    std::uint64_t size() const override { return state().own_end + kRoom; }
    std::size_t read(std::uint64_t offset, std::uint8_t *destination, std::size_t length) override {
        State &s = state();
        const std::uint64_t end = size();
        std::size_t done = 0;
        while (done < length && offset < end) {
            std::uint8_t *out = destination + done;
            std::size_t want = static_cast<std::size_t>(std::min<std::uint64_t>(length - done, end - offset));
            const auto clip = [&](std::uint64_t stop) {
                want = static_cast<std::size_t>(std::min<std::uint64_t>(want, stop - offset));
            };
            if (offset < s.own_end) {
                clip(s.own_end);
                const std::size_t got = portablekit::read_open_file(fd_, offset, out, want);
                std::memset(out + got, 0, want - got);
            } else if (offset >= s.text_at && offset < s.text_at + s.text.size()) {
                clip(s.text_at + s.text.size());
                std::memcpy(out, s.text.data() + (offset - s.text_at), want);
            } else {
                auto next = std::upper_bound(s.pairs.begin(), s.pairs.end(), offset,
                                             [](std::uint64_t at, const Pair &p) { return at < p.at; });
                const Pair *pair = next == s.pairs.begin() ? nullptr : &*std::prev(next);
                if (pair != nullptr && offset < pair->at + pair->size && pair->bytes) {
                    clip(pair->at + pair->size);
                    note_read(static_cast<std::size_t>(pair - s.pairs.data()));
                    std::memcpy(out, pair->bytes->data() + (offset - pair->at), want);
                } else if (pair != nullptr && offset < pair->at + pair->size && s.disc) {
                    clip(pair->at + pair->size);
                    note_read(static_cast<std::size_t>(pair - s.pairs.data()));
                    std::lock_guard guard(s.lock);
                    const std::size_t got = s.disc->image->read(pair->disc + (offset - pair->at), {out, want});
                    std::memset(out + got, 0, want - got);
                } else {
                    // Padding to the next file's sector, and the room after.
                    clip(next == s.pairs.end() ? end : next->at);
                    std::memset(out, 0, want);
                }
            }
            done += want;
            offset += want;
        }
        return done;
    }

private:
    std::uint32_t fd_;
};

std::shared_ptr<FileFilter> serve(std::string_view path, std::uint32_t fd) {
    State &s = state();
    if (fd == 0u || s.building || !path.starts_with("disc0:/")) return nullptr;
    const std::string relative = lower(path.substr(7u));
    if (relative == kIndexPath) {
        if (!s.built) {
            std::vector<std::uint8_t> bytes(256u * 1024u);
            bytes.resize(portablekit::read_open_file(fd, 0u, bytes.data(), bytes.size()));
            build(bytes);
        }
        if (s.own.empty()) return nullptr;
        return std::make_shared<BytesFilter>(game_index());
    }
    if (relative == kPackagePath) return s.own.empty() ? nullptr : std::make_shared<PackageFilter>(fd);
    if (relative.starts_with(kMovieFolder)) {
        std::string name = relative.substr(kMovieFolder.size());
        MovieFilter::Reader own = [fd](std::uint64_t at, std::span<std::uint8_t> out) {
            return portablekit::read_open_file(fd, at, out.data(), out.size());
        };
        // Test only: <prefix>_TEST_MOVIE=<name> plays that movie wherever
        // the game opens one, with Japanese voices or without.
        static const char *test_movie = portablekit::env("TEST_MOVIE");
        if (test_movie != nullptr) {
            name = lower(test_movie);
            own = [file = "disc0:/PSP_GAME/USRDIR/DATA/MOVIE/" + std::string(test_movie)](
                      std::uint64_t at, std::span<std::uint8_t> out) -> std::size_t {
                State &st = state();
                const bool was_building = st.building;
                st.building = true;
                const std::int64_t got = portablekit::read_guest_file(file, at, out.data(), out.size());
                st.building = was_building;
                return got < 0 ? 0u : static_cast<std::size_t>(got);
            };
        }
        std::optional<IsoImage::Entry> entry;
        if (s.want && s.disc &&
            std::find(s.disc->movies.begin(), s.disc->movies.end(), name) != s.disc->movies.end())
            entry = s.disc->image->find("PSP_GAME/USRDIR/DATA/MOVIE/" + name);
        if (entry && entry->size == 0u) entry.reset();
        if (!entry && test_movie == nullptr) return nullptr;
        auto movie = MovieFilter::open(own, entry);
        portablekit::log_once("voices-movie-" + name,
                              "[voices] " + std::string(path) +
                                  (test_movie != nullptr ? " (test: " + name + ")" : std::string()) +
                                  (!movie   ? " is not a movie as expected; it plays as it is"
                                   : entry ? " plays with the Japanese disc's sound"
                                           : " plays as the game has it"));
        return movie;
    }
    return nullptr;
}

// Moves the game's index records in its memory to the Japanese files, or
// back to its own.
void apply(psprecomp::GuestMemory &memory, bool japanese) {
    State &s = state();
    if (s.pairs.empty() || s.own.empty()) return;
    if (memory.load32(kGameIndex + 4u) != 0x6B636170u /* "pack" */ ||
        memory.load32(kGameIndex + 8u) != s.own.size()) {
        portablekit::log_once("voices-index",
                              "[voices] the game's index is not where it was; the voices change at the next start");
        return;
    }
    for (const Pair &p : s.pairs) {
        std::uint32_t sector = 0, size = 0;
        record_words(p.record, japanese ? &p : nullptr, sector, size);
        const std::uint32_t at = kGameIndex + static_cast<std::uint32_t>(kHeader + p.record * kRecord);
        if (memory.load32(at) != s.own[p.record].hash) continue;
        memory.store32(at + 4u, sector);
        memory.store32(at + 8u, size);
    }
    s.applied = japanese;
    std::cout << "[voices] the voices are now " << (japanese ? "Japanese" : "English")
              << " (in what the game loads from now on)" << std::endl;
}

} // namespace

bool disc_ready() { return state().disc.has_value(); }

std::u16string disc_label() {
    const State &s = state();
    if (!s.disc) return u"Not set";
    std::u16string name;
    for (const char c : portablekit::install::path_to_utf8(s.disc->path.filename()))
        name.push_back(static_cast<unsigned char>(c) < 0x80u ? static_cast<char16_t>(c) : u'?');
    if (name.size() > kDiscValueUnits - 1u) name = name.substr(0, kDiscValueUnits - 4u) + u"...";
    return name;
}

std::optional<std::string> check_disc(const std::filesystem::path &image) {
    std::string error;
    if (!open_disc(image, error)) return error;
    return std::nullopt;
}

std::optional<std::string> choose_disc(psprecomp::GuestMemory &memory, const std::filesystem::path &image) {
    State &s = state();
    std::string error;
    auto disc = open_disc(image, error);
    if (!disc) {
        std::cout << "[voices] " << portablekit::install::path_to_utf8(image) << ": " << error << std::endl;
        return error;
    }
    if (s.applied) apply(memory, false);
    {
        std::lock_guard guard(s.lock);
        s.disc = std::move(disc);
    }
    std::cout << "[voices] Japanese disc " << portablekit::install::path_to_utf8(image) << std::endl;
    save_setting(kDiscKey, portablekit::install::path_to_utf8(image));
    pair();
    s.text = option_text(s.text_source);
    if (s.want) apply(memory, true);
    return std::nullopt;
}

bool japanese() { return state().want && disc_ready(); }

void set_japanese(psprecomp::GuestMemory &memory, bool on) {
    State &s = state();
    if (s.want != on) {
        s.want = on;
        save_setting(kLanguageKey, on ? "japanese" : "english");
    }
    if (s.applied != on) apply(memory, on);
}

void install(portablekit::HleRegistrar &) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    State &s = state();
    std::filesystem::path disc;
    if (const char *value = portablekit::env("JAPANESE_VOICES"); value != nullptr) {
        s.from_env = true;
        if (std::string_view(value) != "0") {
            disc = portablekit::install::path_from_utf8(value);
            s.want = true;
        }
    } else {
        const auto dir = data_directory();
        const auto settings = portablekit::install::read_settings_file(dir);
        if (const auto found = settings.find(kDiscKey); found != settings.end() && !found->second.empty()) {
            disc = portablekit::install::path_from_utf8(found->second);
            if (disc.is_relative()) disc = dir / disc;
        }
        const auto language = settings.find(kLanguageKey);
        s.want = language != settings.end() && language->second == "japanese";
    }
    if (!disc.empty()) {
        std::string error;
        s.disc = open_disc(disc, error);
        std::cout << "[voices] Japanese disc " << portablekit::install::path_to_utf8(disc) << ": "
                  << (s.disc ? "ready" : error) << "; voices " << (s.want && s.disc ? "Japanese" : "English")
                  << std::endl;
    }
    portablekit::add_file_provider(&serve);
}

} // namespace d012::voices
