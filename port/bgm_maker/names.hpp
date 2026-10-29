#pragma once

// Names the Battle BGM Maker makes: keys from titles and series names, plain
// file names for imported files, titles from file names. The rules of the
// Python tool (port/tools/bgm_maker: manifest.py key_from_text, unique_key,
// clip_bytes; server.py safe_file_name, title_from_file), so both make the
// same names for the same files. MIT licence; see port/README.md.

#include <cstdint>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace d012::bgm_maker::names {

// `text` with accents taken off and anything else outside ASCII dropped
// (Python's NFKD fold, for Latin letters and full-width forms).
[[nodiscard]] std::string ascii_fold(std::string_view text);

// A valid key made from a title or name: "Blinded by Light!" -> "blinded-by-light".
[[nodiscard]] std::string key_from_text(std::string_view text, std::string_view fallback = "track");

// `base`, or base-2, base-3... so that it is not in `taken`, and with
// `mod_id`, so that the track's number "<mod id>:<key>" is not in `numbers`.
[[nodiscard]] std::string unique_key(std::string_view base, const std::vector<std::string> &taken,
                                     const std::set<std::uint32_t> &numbers = {}, std::string_view mod_id = {});

// `text` cut to at most `limit` UTF-8 bytes, never inside a character, with
// trailing white space taken off.
[[nodiscard]] std::string clip_bytes(std::string_view text, std::size_t limit);

// The plain name an imported file gets in music/: its own name in ASCII
// letters, digits, space and . _ - ( ), with its audio extension in lower
// case. Empty, with `error` saying why, when it is not a kind of file mods take.
[[nodiscard]] std::string safe_file_name(std::string_view name, std::string &error);

// A title from a file's name, for files with no title tag: no leading track
// number, _ as space ("03_Blinded_by_Light.brstm" -> "Blinded by Light").
[[nodiscard]] std::string title_from_file(std::string_view name);

// The game's style for series names: upper case (ASCII and Latin-1 letters).
[[nodiscard]] std::string upper_case(std::string_view text);

} // namespace d012::bgm_maker::names
