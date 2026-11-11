// Story scenes with Japanese voices (d012_voice_scenes.hpp). MIT licence;
// see port/README.md.

#include "d012_voice_scenes.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <optional>

namespace d012::voice_scenes {
namespace {

#include "d012_voice_scenes_data.inc"

// The text files the fixes below name.
constexpr std::string_view kNarration = "text/M_/event/MOVIE/M_000.bin";
constexpr std::array<std::string_view, 11> kTextFiles = {
    kNarration,
    "text/M_/event/RT/R_UP01.bin",
    "text/M_/event/RT/R_UP02.bin",
    "text/M_/event/RT/R_UP04.bin",
    "text/M_/event/RT/R_UP05.bin",
    "text/M_/event/RT/R_UP07.bin",
    "text/M_/event/RT/R_UP08.bin",
    "text/M_/event/RT/R_UP09.bin",
    "text/M_/event/RT/R_UP10.bin",
    "text/M_/event/RT/R_UP11.bin",
    "text/M_/event/RT/R_UP21.bin",
};

// A line a Japanese script shows that the English text has no words for
// (absent, or blank): short sounds the English release did not voice, and
// two lines it cut. The port's own English, in every language's table where
// that line has none. `file` names a text file; empty, the line is in the
// text chunk of `scene`.
struct Fill {
    std::string_view file;
    std::uint32_t scene;
    std::uint16_t line;
    std::u16string_view text;
};
constexpr Fill kFills[] = {
    {"text/M_/event/RT/R_UP01.bin", 0, 45, u"Hee hee hee hee\u2015"},
    {"text/M_/event/RT/R_UP01.bin", 0, 70, u"Hmph."},
    {"text/M_/event/RT/R_UP01.bin", 0, 105, u"Why me\u2015"},
    {"text/M_/event/RT/R_UP02.bin", 0, 24, u"Aaah!"},
    {"text/M_/event/RT/R_UP02.bin", 0, 99, u"Ugh..."},
    {"text/M_/event/RT/R_UP04.bin", 0, 12, u"Tidus..."},
    {"text/M_/event/RT/R_UP05.bin", 0, 8, u"Ah\u2015"},
    {"text/M_/event/RT/R_UP07.bin", 0, 14, u"Ah!"},
    {"text/M_/event/RT/R_UP07.bin", 0, 16, u"Ngh!"},
    {"text/M_/event/RT/R_UP08.bin", 0, 131, u"Hm?"},
    {"text/M_/event/RT/R_UP08.bin", 0, 193, u"Ngh..."},
    {"text/M_/event/RT/R_UP09.bin", 0, 27, u"Hwaaah-hohohoho!"},
    {"text/M_/event/RT/R_UP09.bin", 0, 67, u"Ngh\u2015"},
    {"text/M_/event/RT/R_UP09.bin", 0, 106, u"Huh?"},
    {"text/M_/event/RT/R_UP10.bin", 0, 98, u"Ngh\u2015"},
    {"text/M_/event/RT/R_UP10.bin", 0, 137, u"Nngh!"},
    // Said slowly, the line showing a little more each time.
    {"text/M_/event/RT/R_UP10.bin", 0, 158, u"Let"},
    {"text/M_/event/RT/R_UP10.bin", 0, 159, u"Let them"},
    {"text/M_/event/RT/R_UP10.bin", 0, 160, u"Let them get"},
    {"text/M_/event/RT/R_UP10.bin", 0, 161, u"Let them get away"},
    {"text/M_/event/RT/R_UP11.bin", 0, 21, u"Hm!!"},
    {"text/M_/event/RT/R_UP11.bin", 0, 23, u"Ah!!"},
    {"text/M_/event/RT/R_UP21.bin", 0, 87, u"Ah..."},
    {"", 0x17C283AEu, 78, u"Gaaah!"},
    {"", 0x4D32ACBEu, 43, u"Ngh\u2015"},
    {"", 0x4D32ACBEu, 47, u"Heh heh heh... Ha ha ha...\nHa ha ha ha ha!"},
    {"", 0x5BF5164Du, 34, u"Let me tell you one thing."},
    {"", 0x6A039567u, 19, u"Ah\u2015"},
    {"", 0x842CDDC2u, 30, u"Huh\u2015?"},
    {"", 0xC5F4FFAAu, 10, u"Huh\u2015"},
};

// A narration line of the Japanese scripts that holds what the English
// release split over several lines (the English script shows them one after
// another, with a recording each; the Japanese one shows the one line over
// one recording): those lines, a row each. Line 70's Japanese words are
// line 72's, and line 39's line 40's.
struct Join {
    std::uint16_t line;
    std::array<std::uint16_t, 3> parts;  // 0 after the last
};
constexpr Join kJoins[] = {
    {1, {1, 2}},          {9, {9, 10}},         {14, {14, 15}},       {17, {17, 18, 19}},  {25, {25, 26}},
    {27, {27, 28, 29}},   {30, {30, 31}},       {38, {38, 39}},       {39, {40}},          {48, {48, 49}},
    {52, {52, 53}},       {55, {55, 56, 57}},   {58, {58, 59, 60}},   {65, {65, 66}},      {67, {67, 68}},
    {70, {72}},           {73, {73, 74}},       {75, {75, 76, 77}},   {78, {78, 79}},      {84, {84, 85}},
    {88, {88, 89}},       {94, {94, 95}},       {97, {97, 98}},       {99, {99, 100}},     {107, {107, 108}},
    {117, {117, 118}},    {119, {119, 120}},    {123, {123, 124}},    {140, {140, 141, 142}}, {143, {143, 144}},
};

// The game's key for hiding message text (main 0x089D68AA; tools/character_text.py).
constexpr std::array<std::uint16_t, 16> kTextKey = {0xED98, 0x90AB, 0x5E28, 0x8442, 0x32B1, 0x2DDA, 0xA657, 0xC8BF,
                                                    0x0033, 0x579A, 0xF227, 0x4B4C, 0x36A7, 0x927D, 0x739D, 0x84CE};

// The script chunk: 30 track offsets at 0x408, each track a count and that
// many 32-byte commands; at 0x484 the voice table's offset, which runs to
// the end of the chunk in 0x40-byte paths.
constexpr std::size_t kTracks = 30;
constexpr std::size_t kTrackTable = 0x408;
constexpr std::size_t kVoiceTableAt = 0x484;
constexpr std::size_t kCommand = 32;
constexpr std::size_t kPath = 0x40;

std::uint32_t le32(const std::uint8_t *p) {
    return static_cast<std::uint32_t>(p[0]) | (static_cast<std::uint32_t>(p[1]) << 8u) |
           (static_cast<std::uint32_t>(p[2]) << 16u) | (static_cast<std::uint32_t>(p[3]) << 24u);
}
std::uint16_t le16(const std::uint8_t *p) { return static_cast<std::uint16_t>(p[0] | (p[1] << 8u)); }
void put32(std::vector<std::uint8_t> &out, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(value >> (8 * i)));
}
void put16(std::vector<std::uint8_t> &out, std::uint16_t value) {
    out.push_back(static_cast<std::uint8_t>(value));
    out.push_back(static_cast<std::uint8_t>(value >> 8u));
}
void set32(std::uint8_t *p, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) p[i] = static_cast<std::uint8_t>(value >> (8 * i));
}
void set16(std::uint8_t *p, std::uint16_t value) {
    p[0] = static_cast<std::uint8_t>(value);
    p[1] = static_cast<std::uint8_t>(value >> 8u);
}

// One message table: u16 count, u16 offset per string (in 2-byte units from
// the table's start; 0: none), each string a u16 byte length and its units,
// unit j of string i XORed with kTextKey[(i + j) & 15].
std::optional<std::vector<std::u16string>> read_table(std::span<const std::uint8_t> data) {
    if (data.size() < 2u) return std::nullopt;
    const std::uint16_t count = le16(data.data());
    if (data.size() < 2u + 2u * std::size_t(count)) return std::nullopt;
    std::vector<std::u16string> strings(count);
    for (std::uint16_t i = 0; i < count; ++i) {
        const std::size_t at = std::size_t(le16(data.data() + 2u + 2u * i)) * 2u;
        if (at == 0u) continue;
        if (at + 2u > data.size()) return std::nullopt;
        const std::size_t bytes = le16(data.data() + at);
        if (bytes % 2u != 0u || at + 2u + bytes > data.size()) return std::nullopt;
        std::u16string &s = strings[i];
        for (std::size_t j = 0; j < bytes / 2u; ++j)
            s.push_back(static_cast<char16_t>(le16(data.data() + at + 2u + 2u * j) ^ kTextKey[(i + j) & 15u]));
    }
    return strings;
}

std::optional<std::vector<std::uint8_t>> write_table(const std::vector<std::u16string> &strings) {
    if (strings.size() > 0xFFFFu) return std::nullopt;
    std::vector<std::uint8_t> out;
    put16(out, static_cast<std::uint16_t>(strings.size()));
    out.resize(2u + 2u * strings.size());
    for (std::size_t i = 0; i < strings.size(); ++i) {
        const std::u16string &s = strings[i];
        if (s.empty()) continue;
        if (out.size() / 2u > 0xFFFFu || s.size() * 2u > 0xFFFFu) return std::nullopt;
        set16(out.data() + 2u + 2u * i, static_cast<std::uint16_t>(out.size() / 2u));
        put16(out, static_cast<std::uint16_t>(s.size() * 2u));
        for (std::size_t j = 0; j < s.size(); ++j)
            put16(out, static_cast<std::uint16_t>(s[j] ^ kTextKey[(i + j) & 15u]));
    }
    return out;
}

// A language container: "mlng", u32 count, u32 offset[count], u32
// size[count], the tables (main 0x08870B10 picks the language's).
std::optional<std::vector<std::vector<std::u16string>>> read_tables(std::span<const std::uint8_t> data) {
    if (data.size() < 8u || std::memcmp(data.data(), "mlng", 4u) != 0) return std::nullopt;
    const std::uint32_t count = le32(data.data() + 4u);
    if (count == 0u || count > 16u || data.size() < 8u + 8u * count) return std::nullopt;
    std::vector<std::vector<std::u16string>> tables;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint32_t at = le32(data.data() + 8u + 4u * i);
        const std::uint32_t size = le32(data.data() + 8u + 4u * count + 4u * i);
        if (at > data.size() || size > data.size() - at) return std::nullopt;
        auto table = read_table(data.subspan(at, size));
        if (!table) return std::nullopt;
        tables.push_back(std::move(*table));
    }
    return tables;
}

std::optional<std::vector<std::uint8_t>> write_tables(const std::vector<std::vector<std::u16string>> &tables) {
    std::vector<std::uint8_t> out(8u + 8u * tables.size());
    std::memcpy(out.data(), "mlng", 4u);
    set32(out.data() + 4u, static_cast<std::uint32_t>(tables.size()));
    for (std::size_t i = 0; i < tables.size(); ++i) {
        auto table = write_table(tables[i]);
        if (!table) return std::nullopt;
        set32(out.data() + 8u + 4u * i, static_cast<std::uint32_t>(out.size()));
        set32(out.data() + 8u + 4u * tables.size() + 4u * i, static_cast<std::uint32_t>(table->size()));
        out.insert(out.end(), table->begin(), table->end());
    }
    return out;
}

// No words: absent, or nothing but spaces before the terminator.
bool blank(const std::u16string &s) {
    return std::all_of(s.begin(), s.end(), [](char16_t c) { return c == 0u || c == u' ' || c == u'\u3000'; });
}

// The words of a string, without its terminator.
std::u16string words(const std::u16string &s) {
    std::u16string out = s;
    while (!out.empty() && out.back() == 0u) out.pop_back();
    return out;
}

void fix_tables(std::vector<std::vector<std::u16string>> &tables, std::string_view file, std::uint32_t scene) {
    for (std::vector<std::u16string> &table : tables) {
        for (const Fill &fill : kFills) {
            if (fill.file != file || (file.empty() && fill.scene != scene)) continue;
            if (fill.line >= table.size() || !blank(table[fill.line])) continue;
            table[fill.line] = std::u16string(fill.text) + u'\0';
        }
        if (file != kNarration) continue;
        // From the lines as the game has them, before any is joined.
        const std::vector<std::u16string> before = table;
        for (const Join &join : kJoins) {
            std::u16string joined;
            bool whole = join.line < table.size();
            for (const std::uint16_t part : join.parts) {
                if (part == 0u) break;
                if (part >= before.size() || blank(before[part])) {
                    whole = false;
                    break;
                }
                if (!joined.empty()) joined.push_back(u'\n');
                joined += words(before[part]);
            }
            if (whole) table[join.line] = joined + u'\0';
        }
    }
}

struct Chunk {
    std::uint32_t kind;
    std::array<std::uint8_t, 4> tag;
    std::span<const std::uint8_t> data;
};

bool is_text(const Chunk &c) { return std::memcmp(c.tag.data(), "text", 4u) == 0; }

// "ARC\x01", u32 count, 8 bytes, then {u32 kind, tag, u32 offset, u32 size}
// per chunk from byte 16.
std::optional<std::vector<Chunk>> read_archive(std::span<const std::uint8_t> data) {
    if (data.size() < 16u || std::memcmp(data.data(), "ARC\x01", 4u) != 0) return std::nullopt;
    const std::uint32_t count = le32(data.data() + 4u);
    if (count == 0u || count > 64u || data.size() < 16u + 16u * count) return std::nullopt;
    std::vector<Chunk> chunks;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint8_t *entry = data.data() + 16u + 16u * i;
        const std::uint32_t at = le32(entry + 8u), size = le32(entry + 12u);
        if (at > data.size() || size > data.size() - at) return std::nullopt;
        Chunk c{le32(entry), {}, data.subspan(at, size)};
        std::memcpy(c.tag.data(), entry + 4u, 4u);
        chunks.push_back(c);
    }
    return chunks;
}

// The script chunk is laid out as expected: its tracks and voice table
// inside it.
bool script_fits(std::span<const std::uint8_t> chunk) {
    if (chunk.size() < kVoiceTableAt + 4u) return false;
    for (std::size_t t = 0; t < kTracks; ++t) {
        const std::uint32_t at = le32(chunk.data() + kTrackTable + 4u * t);
        if (at > chunk.size() || chunk.size() - at < 4u) return false;
        const std::uint32_t count = le32(chunk.data() + at);
        if ((chunk.size() - at - 4u) / kCommand < count) return false;
    }
    return le32(chunk.data() + kVoiceTableAt) <= chunk.size();
}

} // namespace

std::span<const std::uint32_t> scenes() { return kScenes; }
std::span<const VoiceName> voice_names() { return kVoiceNames; }
std::span<const std::string_view> text_files() { return kTextFiles; }

std::vector<std::vector<std::u16string>> read_container(std::span<const std::uint8_t> container) {
    auto tables = read_tables(container);
    return tables ? std::move(*tables) : std::vector<std::vector<std::u16string>>{};
}

std::vector<std::uint8_t> write_container(const std::vector<std::vector<std::u16string>> &tables) {
    auto out = write_tables(tables);
    return out ? std::move(*out) : std::vector<std::uint8_t>{};
}

std::vector<std::uint8_t> fix_text_file(std::string_view name, std::span<const std::uint8_t> own) {
    auto tables = read_tables(own);
    if (!tables) return {};
    fix_tables(*tables, name, 0u);
    auto out = write_tables(*tables);
    return out ? std::move(*out) : std::vector<std::uint8_t>{};
}

// One movie's sound: where its bytes are in the file, scanned pack by pack.
struct MovieSplice::Scan {
    struct Span {
        std::uint64_t file;  // where the sound bytes are in the file
        std::uint32_t size;
        std::uint64_t stream;  // and in the sound
    };
    static constexpr std::size_t kPack = 2048u;
    static constexpr std::size_t kBlock = 64u * kPack;

    Reader *read;
    std::uint64_t next;  // the next pack to scan
    std::uint64_t total{};
    std::vector<Span> spans;
    bool done{};
    bool broken{};

    // Scans the packs that start before file offset `end`.
    void scan_file(std::uint64_t end) {
        while (!done && next < end) scan_block();
    }
    // Scans until the sound reaches `stream_end` bytes, or the file ends.
    void scan_sound(std::uint64_t stream_end) {
        while (!done && total < stream_end) scan_block();
    }

    void scan_block() {
        std::vector<std::uint8_t> block(kBlock);
        block.resize((*read)(next, block) / kPack * kPack);
        if (block.empty()) {
            done = true;
            return;
        }
        for (std::size_t at = 0; at < block.size() && !done; at += kPack) {
            const std::uint8_t *pack = block.data() + at;
            if (pack[0] != 0u || pack[1] != 0u || pack[2] != 1u || (pack[3] != 0xBAu && pack[3] != 0xB9u)) {
                broken = done = true;
                break;
            }
            if (pack[3] == 0xB9u) {  // the end of the stream
                done = true;
                break;
            }
            std::size_t i = 14u + (pack[13] & 7u);
            while (i + 6u <= kPack && pack[i] == 0u && pack[i + 1u] == 0u && pack[i + 2u] == 1u) {
                const std::uint8_t id = pack[i + 3u];
                if (id == 0xB9u) {
                    done = true;
                    break;
                }
                const std::size_t length = (std::size_t(pack[i + 4u]) << 8u) | pack[i + 5u];
                if (i + 6u + length > kPack) {
                    broken = done = true;
                    break;
                }
                if (id == 0xBDu && length >= 3u) {
                    const std::size_t sound = i + 9u + pack[i + 8u] + 4u;  // after the 4 bytes
                    const std::size_t end = i + 6u + length;
                    if (sound < end) {
                        spans.push_back({next + at + sound, static_cast<std::uint32_t>(end - sound), total});
                        total += end - sound;
                    }
                }
                i += 6u + length;
            }
        }
        next += block.size();
    }
};

namespace {

// Where a movie's packs start, or 0 when it is not a movie.
std::uint64_t movie_start(const MovieSplice::Reader &read) {
    std::array<std::uint8_t, 16> head{};
    std::array<std::uint8_t, 4> pack{};
    if (read(0u, head) != head.size() || std::memcmp(head.data(), "PSMF", 4u) != 0) return 0u;
    const std::uint64_t start = (std::uint64_t(head[8]) << 24u) | (std::uint64_t(head[9]) << 16u) |
                                (std::uint64_t(head[10]) << 8u) | head[11];
    if (start == 0u || read(start, pack) != pack.size() || pack != std::array<std::uint8_t, 4>{0, 0, 1, 0xBA})
        return 0u;
    return start;
}

} // namespace

std::unique_ptr<MovieSplice> MovieSplice::open(Reader own, Reader theirs) {
    const std::uint64_t own_start = movie_start(own), their_start = movie_start(theirs);
    if (own_start == 0u || their_start == 0u) return nullptr;
    return std::unique_ptr<MovieSplice>(new MovieSplice(std::move(own), std::move(theirs), own_start, their_start));
}

MovieSplice::MovieSplice(Reader own, Reader theirs, std::uint64_t own_start, std::uint64_t their_start)
    : read_own_(std::move(own)), read_theirs_(std::move(theirs)),
      own_(std::make_unique<Scan>(Scan{&read_own_, own_start})),
      theirs_(std::make_unique<Scan>(Scan{&read_theirs_, their_start})) {}

MovieSplice::~MovieSplice() = default;

bool MovieSplice::broken() const { return own_->broken || theirs_->broken; }

std::size_t MovieSplice::read(std::uint64_t offset, std::span<std::uint8_t> out) {
    const std::size_t got = read_own_(offset, out);
    own_->scan_file(offset + got);
    const auto by_file = [](std::uint64_t at, const Scan::Span &s) { return at < s.file; };
    const auto by_stream = [](std::uint64_t at, const Scan::Span &s) { return at < s.stream; };
    auto it = std::upper_bound(own_->spans.begin(), own_->spans.end(), offset, by_file);
    if (it != own_->spans.begin()) --it;
    for (; it != own_->spans.end() && it->file < offset + got; ++it) {
        const std::uint64_t from = std::max(offset, it->file), to = std::min(offset + got, it->file + it->size);
        if (from >= to) continue;
        // Their sound's bytes [stream, stream + n), where they have them.
        const std::uint64_t stream = it->stream + (from - it->file), n = to - from;
        std::uint8_t *dest = out.data() + (from - offset);
        theirs_->scan_sound(stream + n);
        auto t = std::upper_bound(theirs_->spans.begin(), theirs_->spans.end(), stream, by_stream);
        if (t != theirs_->spans.begin()) --t;
        for (; t != theirs_->spans.end() && t->stream < stream + n; ++t) {
            const std::uint64_t a = std::max(stream, t->stream), b = std::min(stream + n, t->stream + t->size);
            if (a >= b) continue;
            read_theirs_(t->file + (a - t->stream), {dest + (a - stream), static_cast<std::size_t>(b - a)});
        }
    }
    return got;
}

std::vector<std::uint8_t> merge_scene(std::uint32_t scene, std::span<const std::uint8_t> japanese,
                                      std::span<const std::uint8_t> own, std::vector<Voice> &voices) {
    voices.clear();
    const auto theirs = read_archive(japanese);
    const auto ours = read_archive(own);
    if (!theirs || !ours || !script_fits(theirs->front().data)) return {};
    const auto our_text = std::find_if(ours->begin(), ours->end(), is_text);
    const bool their_text = std::any_of(theirs->begin(), theirs->end(), is_text);
    if (their_text != (our_text != ours->end())) return {};
    std::vector<std::uint8_t> text;
    if (their_text) {
        auto tables = read_tables(our_text->data);
        if (!tables) return {};
        fix_tables(*tables, {}, scene);
        auto written = write_tables(*tables);
        if (!written) return {};
        text = std::move(*written);
    }

    // The Japanese archive's header and chunks in order, the text the
    // game's, each chunk from a 16-byte boundary.
    std::vector<std::uint8_t> out(japanese.begin(), japanese.begin() + 16);
    out.resize(16u + 16u * theirs->size());
    std::vector<std::size_t> starts;
    for (std::size_t i = 0; i < theirs->size(); ++i) {
        const Chunk &c = (*theirs)[i];
        const std::span<const std::uint8_t> data = is_text(c) ? std::span<const std::uint8_t>(text) : c.data;
        out.resize((out.size() + 15u) / 16u * 16u);
        std::uint8_t *entry = out.data() + 16u + 16u * i;
        set32(entry, c.kind);
        std::memcpy(entry + 4u, c.tag.data(), 4u);
        set32(entry + 8u, static_cast<std::uint32_t>(out.size()));
        set32(entry + 12u, static_cast<std::uint32_t>(data.size()));
        starts.push_back(out.size());
        out.insert(out.end(), data.begin(), data.end());
    }

    // The voice table, with the names the game has for each line.
    std::uint8_t *script = out.data() + starts.front();
    const std::size_t script_size = theirs->front().data.size();
    for (std::size_t at = le32(script + kVoiceTableAt); at + kPath <= script_size; at += kPath) {
        char *path = reinterpret_cast<char *>(script + at);
        const std::string japanese_name(path, std::find(path, path + kPath, '\0'));
        std::string asked = japanese_name;
        for (const VoiceName &v : kVoiceNames)
            if (japanese_name == v.japanese) {
                asked = v.english;
                std::memset(path, 0, kPath);
                std::memcpy(path, asked.data(), std::min(asked.size(), kPath - 1u));
                break;
            }
        voices.push_back({asked, japanese_name});
    }
    return out;
}

} // namespace d012::voice_scenes
