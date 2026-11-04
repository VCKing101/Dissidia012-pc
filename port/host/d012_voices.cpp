// Japanese voices from the player's Japanese disc (d012_voices.hpp). MIT
// licence; see port/README.md.

#include "d012_voices.hpp"

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
#include <cstring>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
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

// A Japanese file in place of one of the game's.
struct Pair {
    std::size_t record;  // in the game's index
    std::uint64_t disc;  // where the file is in the Japanese image
    std::uint32_t size;
    std::uint64_t at;  // in the game's PACKAGE.BIN as it sees it
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

// Pairs the Japanese disc's sound files with the game's: the files from
// pairs_from on in the game's PACKAGE.BIN.
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

    std::uint64_t at = s.pairs_from;
    for (const Candidate &c : candidates) {
        const std::uint32_t size = c.japanese->size & kSizeMask;
        if (at + size > s.own_end + kRoom) {
            std::cerr << "[voices] no more room after PACKAGE.BIN; the rest stay English\n";
            break;
        }
        s.pairs.push_back(
            {own_at[c.own_hash], base + std::uint64_t(c.japanese->sector & kSectorMask) * kSector, size, at});
        at += whole_sectors(size);
    }
    s.read.assign(s.pairs.size(), false);
    const auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - started).count();
    std::cout << "[voices] " << s.pairs.size() << " sound files of the Japanese disc paired with the English ones ("
              << named << " files paired by name, " << sound << " of them sound), "
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

// A file of the Japanese disc, read whole in place of one of the game's.
class DiscFileFilter final : public FileFilter {
public:
    explicit DiscFileFilter(IsoImage::Entry entry) : entry_(entry) {}
    std::uint64_t size() const override { return entry_.size; }
    std::size_t read(std::uint64_t offset, std::uint8_t *destination, std::size_t length) override {
        State &s = state();
        if (offset >= entry_.size || !s.disc) return 0u;
        const std::size_t n = std::min<std::uint64_t>(length, entry_.size - offset);
        std::lock_guard guard(s.lock);
        return s.disc->image->read(std::uint64_t(entry_.lba) * kSector + offset, {destination, n});
    }

private:
    IsoImage::Entry entry_;
};

void note_read(std::size_t pair) {
    State &s = state();
    if (pair >= s.read.size() || s.read[pair]) return;
    s.read[pair] = true;
    if (++s.files_read <= 3u || s.files_read % 100u == 0u)
        std::cout << "[voices] the game has read " << s.files_read << " Japanese sound file"
                  << (s.files_read == 1u ? "" : "s") << " (the last " << s.pairs[pair].size << " bytes)" << std::endl;
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
                if (pair != nullptr && offset < pair->at + pair->size && s.disc) {
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
        if (!s.want || !s.disc) return nullptr;
        const std::string name = relative.substr(kMovieFolder.size());
        if (std::find(s.disc->movies.begin(), s.disc->movies.end(), name) == s.disc->movies.end()) return nullptr;
        const auto entry = s.disc->image->find("PSP_GAME/USRDIR/DATA/MOVIE/" + name);
        // The same size, so the same picture: the game's file has a last
        // byte where the Japanese one has, and none after it.
        std::uint8_t probe = 0;
        if (!entry || entry->size == 0u || portablekit::read_open_file(fd, entry->size - 1u, &probe, 1u) != 1u ||
            portablekit::read_open_file(fd, entry->size, &probe, 1u) != 0u)
            return nullptr;
        portablekit::log_once("voices-movie-" + name, "[voices] " + std::string(path) + " plays the Japanese disc's");
        return std::make_shared<DiscFileFilter>(*entry);
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
