#pragma once

// A mod's manifest (mod.json) read and checked by the game's rules, the
// format of docs/port/mod-folder.md. Shared by the game (d012_mods.cpp, which
// logs the problems and plays the tracks) and the Battle BGM Maker
// (port/bgm_maker/, which shows them), so the tool checks a mod with the code
// the game runs. Nothing here logs, plays or needs PortableKit. MIT licence;
// see port/README.md.

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace d012::mod_manifest {

inline constexpr int kFormat = 1;
inline constexpr std::size_t kMaxManifestBytes = 1u << 20u;
inline constexpr std::size_t kMaxKeyLength = 64u;
inline constexpr std::size_t kMaxSeriesNameLength = 48u;
inline constexpr std::size_t kMaxShortNameLength = 16u;
inline constexpr std::size_t kMaxVersionLength = 32u;
inline constexpr std::size_t kMaxDescriptionLength = 1024u;

// What the game does with a problem: an error leaves the part (or the whole
// mod) out, a warning is only said, a note is information (the game prints
// it to its standard output).
struct Problem {
    enum class Level { Error, Warning, Note };
    Level level{};
    // Where, as the Battle BGM Maker's Python tool names it: "mod", "mod.id",
    // "music", "series.0", "series.0.tracks.1.loop", ...
    std::string path;
    std::string message;
    int line{};  // in mod.json, from 1; 0 when none
};

struct LoopPoint {
    enum class Unit { Samples, Microseconds };
    Unit unit{Unit::Samples};
    std::int64_t value{};
};

// "auto" (the file's loop tags or stream header, else the whole file),
// "whole", "none", or a range: from `start` to `end` (absent: the end of the
// file). For a BRSTM, BCSTM or BFSTM, "auto" has already become the header's
// loop (or Whole) here.
struct Loop {
    enum class Mode { Auto, Whole, None, Range };
    Mode mode{Mode::Auto};
    LoopPoint start;
    std::optional<LoopPoint> end;
};

struct Track {
    std::string key;
    std::string title;
    std::string file_text;       // as the manifest gives it
    std::filesystem::path file;  // the mod's folder / file_text
    Loop loop;
};

struct Series {
    std::string key;
    std::string name;
    std::string short_name;  // "short": the name's start in the PSP / PPSSPP export ("FFXVI"); may be empty
    std::int64_t order{};
    std::vector<Track> tracks;  // only those the game plays
};

struct Manifest {
    bool loaded{};  // the mod itself was valid; series and tracks may still be left out
    std::string id;
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::vector<Series> series;  // only those the game shows
};

struct Result {
    Manifest manifest;
    std::vector<Problem> problems;  // in the order the game finds them
};

// Reads `folder`/mod.json as the game does.
[[nodiscard]] Result read(const std::filesystem::path &folder);
// The same for a manifest's text, its files looked up in `folder`.
[[nodiscard]] Result read_text(std::string_view text, const std::filesystem::path &folder);

// Ids and keys: lower case letters, digits, '.', '_' and '-', starting with a
// letter or digit, at most 64 characters.
[[nodiscard]] bool valid_key(std::string_view key);

// FNV-1a, 32 bits, of `text`: how ids ("<mod id>:<key>") become numbers.
[[nodiscard]] std::uint32_t id_number(std::string_view text);

// The audio file kinds a track may be (lower case, with the dot).
[[nodiscard]] bool is_audio_extension(std::string_view extension);

// UTF-8 text of a path, and back.
[[nodiscard]] std::string utf8(const std::filesystem::path &path);
[[nodiscard]] std::filesystem::path path_from_utf8(std::string_view text);

} // namespace d012::mod_manifest
