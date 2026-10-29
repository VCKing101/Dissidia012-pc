// Names the Battle BGM Maker makes; see names.hpp. MIT licence; see
// port/README.md.

#include "names.hpp"

#include "d012_mod_manifest.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <utility>

namespace d012::bgm_maker::names {
namespace {

// What Python's unicodedata.normalize("NFKD", c).encode("ascii", "ignore")
// leaves of U+00A0..U+024F (made with Python 3.12; code points that leave
// nothing are not listed).
constexpr std::pair<char32_t, const char *> kFold[] = {
    {0x00A0, " "}, {0x00A8, " "}, {0x00AA, "a"}, {0x00AF, " "}, {0x00B2, "2"}, {0x00B3, "3"}, {0x00B4, " "},
    {0x00B8, " "}, {0x00B9, "1"}, {0x00BA, "o"}, {0x00BC, "14"}, {0x00BD, "12"}, {0x00BE, "34"}, {0x00C0, "A"},
    {0x00C1, "A"}, {0x00C2, "A"}, {0x00C3, "A"}, {0x00C4, "A"}, {0x00C5, "A"}, {0x00C7, "C"}, {0x00C8, "E"},
    {0x00C9, "E"}, {0x00CA, "E"}, {0x00CB, "E"}, {0x00CC, "I"}, {0x00CD, "I"}, {0x00CE, "I"}, {0x00CF, "I"},
    {0x00D1, "N"}, {0x00D2, "O"}, {0x00D3, "O"}, {0x00D4, "O"}, {0x00D5, "O"}, {0x00D6, "O"}, {0x00D9, "U"},
    {0x00DA, "U"}, {0x00DB, "U"}, {0x00DC, "U"}, {0x00DD, "Y"}, {0x00E0, "a"}, {0x00E1, "a"}, {0x00E2, "a"},
    {0x00E3, "a"}, {0x00E4, "a"}, {0x00E5, "a"}, {0x00E7, "c"}, {0x00E8, "e"}, {0x00E9, "e"}, {0x00EA, "e"},
    {0x00EB, "e"}, {0x00EC, "i"}, {0x00ED, "i"}, {0x00EE, "i"}, {0x00EF, "i"}, {0x00F1, "n"}, {0x00F2, "o"},
    {0x00F3, "o"}, {0x00F4, "o"}, {0x00F5, "o"}, {0x00F6, "o"}, {0x00F9, "u"}, {0x00FA, "u"}, {0x00FB, "u"},
    {0x00FC, "u"}, {0x00FD, "y"}, {0x00FF, "y"}, {0x0100, "A"}, {0x0101, "a"}, {0x0102, "A"}, {0x0103, "a"},
    {0x0104, "A"}, {0x0105, "a"}, {0x0106, "C"}, {0x0107, "c"}, {0x0108, "C"}, {0x0109, "c"}, {0x010A, "C"},
    {0x010B, "c"}, {0x010C, "C"}, {0x010D, "c"}, {0x010E, "D"}, {0x010F, "d"}, {0x0112, "E"}, {0x0113, "e"},
    {0x0114, "E"}, {0x0115, "e"}, {0x0116, "E"}, {0x0117, "e"}, {0x0118, "E"}, {0x0119, "e"}, {0x011A, "E"},
    {0x011B, "e"}, {0x011C, "G"}, {0x011D, "g"}, {0x011E, "G"}, {0x011F, "g"}, {0x0120, "G"}, {0x0121, "g"},
    {0x0122, "G"}, {0x0123, "g"}, {0x0124, "H"}, {0x0125, "h"}, {0x0128, "I"}, {0x0129, "i"}, {0x012A, "I"},
    {0x012B, "i"}, {0x012C, "I"}, {0x012D, "i"}, {0x012E, "I"}, {0x012F, "i"}, {0x0130, "I"}, {0x0132, "IJ"},
    {0x0133, "ij"}, {0x0134, "J"}, {0x0135, "j"}, {0x0136, "K"}, {0x0137, "k"}, {0x0139, "L"}, {0x013A, "l"},
    {0x013B, "L"}, {0x013C, "l"}, {0x013D, "L"}, {0x013E, "l"}, {0x013F, "L"}, {0x0140, "l"}, {0x0143, "N"},
    {0x0144, "n"}, {0x0145, "N"}, {0x0146, "n"}, {0x0147, "N"}, {0x0148, "n"}, {0x0149, "n"}, {0x014C, "O"},
    {0x014D, "o"}, {0x014E, "O"}, {0x014F, "o"}, {0x0150, "O"}, {0x0151, "o"}, {0x0154, "R"}, {0x0155, "r"},
    {0x0156, "R"}, {0x0157, "r"}, {0x0158, "R"}, {0x0159, "r"}, {0x015A, "S"}, {0x015B, "s"}, {0x015C, "S"},
    {0x015D, "s"}, {0x015E, "S"}, {0x015F, "s"}, {0x0160, "S"}, {0x0161, "s"}, {0x0162, "T"}, {0x0163, "t"},
    {0x0164, "T"}, {0x0165, "t"}, {0x0168, "U"}, {0x0169, "u"}, {0x016A, "U"}, {0x016B, "u"}, {0x016C, "U"},
    {0x016D, "u"}, {0x016E, "U"}, {0x016F, "u"}, {0x0170, "U"}, {0x0171, "u"}, {0x0172, "U"}, {0x0173, "u"},
    {0x0174, "W"}, {0x0175, "w"}, {0x0176, "Y"}, {0x0177, "y"}, {0x0178, "Y"}, {0x0179, "Z"}, {0x017A, "z"},
    {0x017B, "Z"}, {0x017C, "z"}, {0x017D, "Z"}, {0x017E, "z"}, {0x017F, "s"}, {0x01A0, "O"}, {0x01A1, "o"},
    {0x01AF, "U"}, {0x01B0, "u"}, {0x01C4, "DZ"}, {0x01C5, "Dz"}, {0x01C6, "dz"}, {0x01C7, "LJ"}, {0x01C8, "Lj"},
    {0x01C9, "lj"}, {0x01CA, "NJ"}, {0x01CB, "Nj"}, {0x01CC, "nj"}, {0x01CD, "A"}, {0x01CE, "a"}, {0x01CF, "I"},
    {0x01D0, "i"}, {0x01D1, "O"}, {0x01D2, "o"}, {0x01D3, "U"}, {0x01D4, "u"}, {0x01D5, "U"}, {0x01D6, "u"},
    {0x01D7, "U"}, {0x01D8, "u"}, {0x01D9, "U"}, {0x01DA, "u"}, {0x01DB, "U"}, {0x01DC, "u"}, {0x01DE, "A"},
    {0x01DF, "a"}, {0x01E0, "A"}, {0x01E1, "a"}, {0x01E6, "G"}, {0x01E7, "g"}, {0x01E8, "K"}, {0x01E9, "k"},
    {0x01EA, "O"}, {0x01EB, "o"}, {0x01EC, "O"}, {0x01ED, "o"}, {0x01F0, "j"}, {0x01F1, "DZ"}, {0x01F2, "Dz"},
    {0x01F3, "dz"}, {0x01F4, "G"}, {0x01F5, "g"}, {0x01F8, "N"}, {0x01F9, "n"}, {0x01FA, "A"}, {0x01FB, "a"},
    {0x0200, "A"}, {0x0201, "a"}, {0x0202, "A"}, {0x0203, "a"}, {0x0204, "E"}, {0x0205, "e"}, {0x0206, "E"},
    {0x0207, "e"}, {0x0208, "I"}, {0x0209, "i"}, {0x020A, "I"}, {0x020B, "i"}, {0x020C, "O"}, {0x020D, "o"},
    {0x020E, "O"}, {0x020F, "o"}, {0x0210, "R"}, {0x0211, "r"}, {0x0212, "R"}, {0x0213, "r"}, {0x0214, "U"},
    {0x0215, "u"}, {0x0216, "U"}, {0x0217, "u"}, {0x0218, "S"}, {0x0219, "s"}, {0x021A, "T"}, {0x021B, "t"},
    {0x021E, "H"}, {0x021F, "h"}, {0x0226, "A"}, {0x0227, "a"}, {0x0228, "E"}, {0x0229, "e"}, {0x022A, "O"},
    {0x022B, "o"}, {0x022C, "O"}, {0x022D, "o"}, {0x022E, "O"}, {0x022F, "o"}, {0x0230, "O"}, {0x0231, "o"},
    {0x0232, "Y"}, {0x0233, "y"},
};

// The next code point of UTF-8 `text` at `at`; a byte that starts no valid
// sequence is taken alone (and folds to nothing).
char32_t next_code_point(std::string_view text, std::size_t &at) {
    const auto byte = [&](std::size_t i) { return static_cast<unsigned char>(text[i]); };
    const unsigned char first = byte(at);
    std::size_t length = first < 0x80u ? 1u : (first >> 5u) == 6u ? 2u : (first >> 4u) == 14u ? 3u : (first >> 3u) == 30u ? 4u : 0u;
    if (length == 0u || at + length > text.size()) {
        ++at;
        return 0xFFFDu;
    }
    char32_t value = length == 1u ? first : first & (0x7Fu >> length);
    for (std::size_t i = 1; i < length; ++i) {
        if ((byte(at + i) & 0xC0u) != 0x80u) {
            ++at;
            return 0xFFFDu;
        }
        value = (value << 6u) | (byte(at + i) & 0x3Fu);
    }
    at += length;
    return value;
}

bool is_space(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }

std::string strip(std::string_view text, std::string_view chars) {
    const auto first = text.find_first_not_of(chars);
    if (first == std::string_view::npos) return {};
    const auto last = text.find_last_not_of(chars);
    return std::string(text.substr(first, last - first + 1u));
}

std::string rstrip(std::string_view text, std::string_view chars) {
    const auto last = text.find_last_not_of(chars);
    return last == std::string_view::npos ? std::string() : std::string(text.substr(0u, last + 1u));
}

// Python's os.path.splitext of a base name: the extension from the last dot,
// unless the dots lead the name.
std::pair<std::string, std::string> split_extension(std::string_view base) {
    const auto dot = base.rfind('.');
    if (dot == std::string_view::npos || base.find_first_not_of('.') >= dot) return {std::string(base), {}};
    return {std::string(base.substr(0u, dot)), std::string(base.substr(dot))};
}

std::string base_name(std::string_view name) {
    const auto slash = name.find_last_of("/\\");
    return std::string(slash == std::string_view::npos ? name : name.substr(slash + 1u));
}

std::string lower_ascii(std::string text) {
    for (char &c : text)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return text;
}

} // namespace

std::string ascii_fold(std::string_view text) {
    std::string out;
    for (std::size_t at = 0; at < text.size();) {
        const char32_t c = next_code_point(text, at);
        if (c < 0x80u) {
            out += static_cast<char>(c);
        } else if (c >= 0xFF01u && c <= 0xFF5Eu) {
            out += static_cast<char>(c - 0xFEE0u);  // full-width forms
        } else {
            const auto *found = std::lower_bound(std::begin(kFold), std::end(kFold), c,
                                                 [](const auto &entry, char32_t value) { return entry.first < value; });
            if (found != std::end(kFold) && found->first == c) out += found->second;
        }
    }
    return out;
}

std::string key_from_text(std::string_view text, std::string_view fallback) {
    const std::string folded = lower_ascii(ascii_fold(text));
    std::string key;
    bool run = false;
    for (const char c : folded) {
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            key += c;
            run = false;
        } else if (!run) {
            key += '-';
            run = true;
        }
    }
    key = strip(key, "-._");
    key = rstrip(std::string_view(key).substr(0u, std::min(key.size(), mod_manifest::kMaxKeyLength)), "-._");
    if (key.empty() || !std::isalnum(static_cast<unsigned char>(key.front()))) key = std::string(fallback);
    return key;
}

std::string unique_key(std::string_view base, const std::vector<std::string> &taken, const std::set<std::uint32_t> &numbers,
                       std::string_view mod_id) {
    std::string candidate(base);
    for (int counter = 1;; ) {
        const bool number_free =
            mod_id.empty() || !numbers.contains(mod_manifest::id_number(std::string(mod_id) + ":" + candidate));
        if (std::find(taken.begin(), taken.end(), candidate) == taken.end() && number_free &&
            mod_manifest::valid_key(candidate))
            return candidate;
        ++counter;
        const std::string suffix = "-" + std::to_string(counter);
        const std::size_t room = mod_manifest::kMaxKeyLength - suffix.size();
        candidate = rstrip(base.substr(0u, std::min(base.size(), room)), "-._") + suffix;
    }
}

std::string clip_bytes(std::string_view text, std::size_t limit) {
    std::string_view cut = text.substr(0u, std::min(text.size(), limit));
    // Not inside a character: drop a sequence the cut left unfinished.
    std::size_t end = cut.size();
    std::size_t start = end;
    while (start > 0u && (static_cast<unsigned char>(cut[start - 1u]) & 0xC0u) == 0x80u) --start;
    if (start > 0u) {
        const unsigned char lead = static_cast<unsigned char>(cut[start - 1u]);
        const std::size_t length = lead < 0x80u ? 1u : (lead >> 5u) == 6u ? 2u : (lead >> 4u) == 14u ? 3u : 4u;
        if (lead >= 0x80u && end - (start - 1u) < length) end = start - 1u;
    }
    std::string out(cut.substr(0u, end));
    while (!out.empty() && is_space(out.back())) out.pop_back();
    return out;
}

std::string safe_file_name(std::string_view name, std::string &error) {
    const std::string base = base_name(name);
    auto [stem, extension] = split_extension(base);
    extension = lower_ascii(extension);
    if (extension == ".bwav" || extension == ".nus3audio") {
        const std::string kind = extension == ".bwav" ? "BWAV" : "NUS3AUDIO";
        error = "\"" + base + "\": " + kind +
                " files are not supported (FFmpeg cannot decode them); use the BRSTM version of the track";
        return {};
    }
    if (!mod_manifest::is_audio_extension(extension)) {
        error = "\"" + base + "\" is not .mp3, .ogg, .oga, .opus, .flac, .wav, .brstm, .bcstm or .bfstm";
        return {};
    }
    const std::string folded = ascii_fold(stem);
    std::string plain;
    bool run = false;
    for (const char c : folded) {
        const bool keep = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ' ||
                          c == '.' || c == '_' || c == '(' || c == ')' || c == '-';
        if (keep) {
            plain += c;
            run = false;
        } else if (!run) {
            plain += '_';
            run = true;
        }
    }
    plain = strip(plain, " ._");
    if (plain.empty()) plain = "track";
    return plain.substr(0u, std::min<std::size_t>(plain.size(), 80u)) + extension;
}

std::string title_from_file(std::string_view name) {
    std::string stem = split_extension(base_name(name)).first;
    // A leading track number: one to three digits, then space, '.', '_' or '-'.
    std::size_t digits = 0;
    while (digits < stem.size() && digits < 4u && stem[digits] >= '0' && stem[digits] <= '9') ++digits;
    if (digits >= 1u && digits <= 3u) {
        std::size_t after = digits;
        while (after < stem.size() && (stem[after] == ' ' || stem[after] == '.' || stem[after] == '_' || stem[after] == '-'))
            ++after;
        if (after > digits) stem.erase(0u, after);
    }
    std::replace(stem.begin(), stem.end(), '_', ' ');
    std::string title;
    for (std::size_t i = 0; i < stem.size(); ++i) {
        if (is_space(stem[i])) {
            if (!title.empty() && title.back() != ' ') title += ' ';
        } else {
            title += stem[i];
        }
    }
    while (!title.empty() && title.back() == ' ') title.pop_back();
    return title.empty() ? std::string("Track") : title;
}

std::string upper_case(std::string_view text) {
    std::string out(text);
    for (std::size_t i = 0; i < out.size(); ++i) {
        const auto c = static_cast<unsigned char>(out[i]);
        if (c >= 'a' && c <= 'z') {
            out[i] = static_cast<char>(c - 'a' + 'A');
        } else if (c == 0xC3u && i + 1u < out.size()) {
            // Latin-1 small letters U+00E0..U+00FE (not U+00F7, the division sign).
            const auto next = static_cast<unsigned char>(out[i + 1u]);
            if (next >= 0xA0u && next <= 0xBEu && next != 0xB7u) out[i + 1u] = static_cast<char>(next - 0x20u);
            ++i;
        }
    }
    return out;
}

} // namespace d012::bgm_maker::names
