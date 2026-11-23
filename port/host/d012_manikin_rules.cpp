// The Manikin look's rules; see d012_manikin_rules.hpp. MIT licence; see
// port/README.md.

#include "d012_manikin_rules.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <iterator>
#include <string>

namespace d012::manikin {
namespace {

// The game's key for hiding message text (main 0x089D68AA; tools/character_text.py).
constexpr std::array<std::uint16_t, 16> kTextKey = {0xED98, 0x90AB, 0x5E28, 0x8442, 0x32B1, 0x2DDA, 0xA657, 0xC8BF,
                                                    0x0033, 0x579A, 0xF227, 0x4B4C, 0x36A7, 0x927D, 0x739D, 0x84CE};
constexpr char kPackMagic[8] = {'m', 'e', 's', 's', ' ', 'p', 'a', 'k'};
constexpr std::size_t kPackHeader = 16u;
constexpr std::size_t kPackAlign = 16u;
constexpr std::uint8_t kPackFill = 0x77u;
// Looks 0 to 13, as the game's next-look step counts them.
constexpr int kLooks = 14;
// The model code of each character with a manikin (objentry.bin's P_<code>_ZAKO rows).
constexpr std::string_view kManikinCodes[] = {
    "ONE100", "ONE200", "TWO100", "TWO200", "THR100", "THR200", "FOR100", "FOR200", "FIV100", "FIV200",
    "SIX100", "SIX200", "SEV100", "SEV200", "EHT100", "EHT200", "NIN100", "NIN200", "TEN100", "TEN200",
    "GST100", "GST200", "FOR110", "FIV210", "SEV110", "EHT110", "TEN110", "GST110", "GST120", "GST130",
};

std::uint16_t le16(const std::uint8_t *p) { return static_cast<std::uint16_t>(p[0] | (p[1] << 8u)); }
std::uint32_t le32(const std::uint8_t *p) {
    return static_cast<std::uint32_t>(p[0]) | (static_cast<std::uint32_t>(p[1]) << 8u) |
           (static_cast<std::uint32_t>(p[2]) << 16u) | (static_cast<std::uint32_t>(p[3]) << 24u);
}
void put16(std::uint8_t *p, std::uint16_t value) {
    p[0] = static_cast<std::uint8_t>(value);
    p[1] = static_cast<std::uint8_t>(value >> 8u);
}
void put32(std::uint8_t *p, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) p[i] = static_cast<std::uint8_t>(value >> (8 * i));
}

// The look-label table, told by its shape rather than its words: 26
// strings, the second block repeating the first's (Normal, Alt 1; Alt 2 and
// DLC 1 to 9) and two dashes at the end.
bool is_label_table(std::span<const std::uint8_t> table) {
    const std::vector<std::u16string> s = table_strings(table);
    if (s.size() != static_cast<std::size_t>(kLabelStrings)) return false;
    if (std::any_of(s.begin(), s.end(), [](const std::u16string &text) { return text.empty(); })) return false;
    if (s[0] != s[2] || s[1] != s[3] || s[4] != s[14] || s[24] != s[25] || s[0] == s[1]) return false;
    for (int k = 0; k < 9; ++k)
        if (s[5 + k] != s[15 + k]) return false;
    return true;
}

// The table with `label` as one more string at its end.
std::optional<std::vector<std::uint8_t>> with_string(std::span<const std::uint8_t> table, std::u16string_view label) {
    const std::uint16_t count = le16(table.data());
    if (table.size() % 2u != 0u) return std::nullopt;
    const std::size_t old_head = 2u + 2u * count;
    std::vector<std::uint8_t> out(old_head + 2u);
    put16(out.data(), static_cast<std::uint16_t>(count + 1u));
    // Every string moves one unit on, past the new offset.
    for (std::uint16_t i = 0; i < count; ++i) {
        const std::uint16_t offset = le16(table.data() + 2u + 2u * i);
        put16(out.data() + 2u + 2u * i, offset == 0u ? 0u : static_cast<std::uint16_t>(offset + 1u));
    }
    out.insert(out.end(), table.begin() + static_cast<std::ptrdiff_t>(old_head), table.end());
    std::u16string units(label);
    units.push_back(u'\0');
    if (out.size() / 2u + 1u + units.size() > 0xFFFFu) return std::nullopt;
    put16(out.data() + 2u + 2u * count, static_cast<std::uint16_t>(out.size() / 2u));
    const std::size_t at = out.size();
    out.resize(at + 2u + 2u * units.size());
    put16(out.data() + at, static_cast<std::uint16_t>(2u * units.size()));
    for (std::size_t j = 0; j < units.size(); ++j)
        put16(out.data() + at + 2u + 2u * j,
              static_cast<std::uint16_t>(units[j] ^ kTextKey[(static_cast<std::size_t>(count) + j) & 15u]));
    return out;
}

} // namespace

bool has_manikin(int character) { return (character >= 1 && character <= 22) || (character >= 24 && character <= 31); }

std::uint32_t with_manikin(int character, std::uint32_t mask) {
    return mask != 0u && has_manikin(character) ? mask | 1u : mask;
}

int next_look(int current, std::uint16_t mask) {
    for (int step = 1; step <= kLooks; ++step) {
        const int look = (current + step) % kLooks;
        if (look == current) break;
        if (look == 0 && (mask & 1u) == 0u) continue;
        if ((mask & (1u << look)) != 0u) return look;
    }
    return current;
}

std::vector<std::u16string> table_strings(std::span<const std::uint8_t> table) {
    std::vector<std::u16string> out;
    if (table.size() < 2u) return out;
    const std::uint16_t count = le16(table.data());
    if (table.size() < 2u + 2u * std::size_t{count}) return {};
    for (std::uint16_t i = 0; i < count; ++i) {
        const std::size_t offset = 2u * std::size_t{le16(table.data() + 2u + 2u * i)};
        std::u16string text;
        if (offset != 0u) {
            if (offset + 2u > table.size()) return {};
            const std::size_t bytes = le16(table.data() + offset);
            if (offset + 2u + bytes > table.size()) return {};
            for (std::size_t j = 0; j < bytes / 2u; ++j) {
                const auto unit =
                    static_cast<char16_t>(le16(table.data() + offset + 2u + 2u * j) ^ kTextKey[(i + j) & 15u]);
                if (unit == u'\0') break;
                text.push_back(unit);
            }
        }
        out.push_back(std::move(text));
    }
    return out;
}

std::vector<std::pair<std::uint32_t, std::uint32_t>> pack_tables(std::span<const std::uint8_t> pack) {
    if (pack.size() < kPackHeader || std::memcmp(pack.data(), kPackMagic, sizeof kPackMagic) != 0) return {};
    const std::uint16_t count = le16(pack.data() + 10u);
    if (pack.size() < kPackHeader + 8u * std::size_t{count}) return {};
    std::vector<std::pair<std::uint32_t, std::uint32_t>> tables;
    std::uint64_t end = kPackHeader + 8u * std::size_t{count};
    for (std::uint16_t k = 0; k < count; ++k) {
        const std::uint32_t offset = le32(pack.data() + kPackHeader + 8u * k);
        const std::uint32_t size = le32(pack.data() + kPackHeader + 8u * k + 4u);
        // In order, one after another, inside the file: the only layout the
        // port rewrites.
        if (offset < end || std::uint64_t{offset} + size > pack.size()) return {};
        end = std::uint64_t{offset} + size;
        tables.emplace_back(offset, size);
    }
    return tables;
}

std::optional<std::vector<std::uint8_t>> with_manikin_label(std::span<const std::uint8_t> pack,
                                                            std::u16string_view label) {
    const auto tables = pack_tables(pack);
    if (tables.empty()) return std::nullopt;
    // Only the fill between the tables and after the last one, which the
    // rewrite lays out again: anything else there and the file is left alone.
    for (std::size_t k = 0; k < tables.size(); ++k) {
        const std::size_t from = tables[k].first + tables[k].second;
        const std::size_t to = k + 1u < tables.size() ? tables[k + 1u].first : pack.size();
        if (to - from >= kPackAlign) return std::nullopt;
        for (std::size_t i = from; i < to; ++i)
            if (pack[i] != kPackFill) return std::nullopt;
    }
    std::vector<std::vector<std::uint8_t>> bodies;
    bool changed = false;
    for (const auto &[offset, size] : tables) {
        const auto table = pack.subspan(offset, size);
        if (is_label_table(table)) {
            const auto grown = with_string(table, label);
            if (!grown) return std::nullopt;
            bodies.push_back(*grown);
            changed = true;
        } else {
            bodies.emplace_back(table.begin(), table.end());
        }
    }
    if (!changed) return std::nullopt;
    // The header and whatever lies before the first table as they are, then
    // the tables, each on a 16-byte boundary.
    std::vector<std::uint8_t> out(pack.begin(), pack.begin() + tables.front().first);
    for (std::size_t k = 0; k < bodies.size(); ++k) {
        while (out.size() % kPackAlign != 0u) out.push_back(kPackFill);
        put32(out.data() + kPackHeader + 8u * k, static_cast<std::uint32_t>(out.size()));
        put32(out.data() + kPackHeader + 8u * k + 4u, static_cast<std::uint32_t>(bodies[k].size()));
        out.insert(out.end(), bodies[k].begin(), bodies[k].end());
    }
    while (out.size() % kPackAlign != 0u) out.push_back(kPackFill);
    return out;
}

std::optional<std::string> portrait_instead(std::string_view path) {
    constexpr std::string_view kFolder = "/battle/chara_image/";
    const std::size_t folder = path.find(kFolder);
    if (folder == std::string_view::npos) return std::nullopt;
    const std::size_t name = folder + kFolder.size();
    if (path.find('/', name) != std::string_view::npos) return std::nullopt;
    // P_ + a six-letter code, as every fighter's row has it.
    const std::string_view file = path.substr(name);
    constexpr std::string_view kManikin = "_ZAKO";
    if (file.size() < 2u + 6u + kManikin.size() || file.substr(0u, 2u) != "P_" || file.substr(8u, kManikin.size()) != kManikin)
        return std::nullopt;
    if (std::find(std::begin(kManikinCodes), std::end(kManikinCodes), file.substr(2u, 6u)) == std::end(kManikinCodes))
        return std::nullopt;
    const std::string_view tail = file.substr(8u + kManikin.size());
    if (tail != ".gim" && tail != "_2.gim") return std::nullopt;
    std::string out(path.substr(0u, name + 8u));
    out += "_1P";
    out += tail;
    return out;
}

std::vector<std::uint16_t> disc_table(const LabelTable &table) {
    std::vector<std::uint16_t> out = {table.normal, table.normal, table.alt1, table.alt2};
    for (int k = 0; k < 9; ++k) out.push_back(static_cast<std::uint16_t>(table.dlc1 + k));
    return out;
}

} // namespace d012::manikin
