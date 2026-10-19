#pragma once

// Mods: content that the port adds to the game, never in place of the game's
// own. A mod is a folder in <data dir>/mods/ with a mod.json manifest; the
// format is docs/port/mod-folder.md. The first thing mods add is battle
// music: series (a source game, such as another Final Fantasy) and their
// tracks, which come after the game's own in the BGM choice.
//
// Every series and track has a stable id made from the mod's id and the key
// the manifest gives it ("<mod id>:<key>"), never from the order mods load
// in, and a 32-bit number derived from that id for places that hold only a
// number (a save, a guest-side table). Turning other mods on or off changes
// neither.
//
// A track is played as a stand-in ATRAC3plus file the game opens like one of
// its own BGM files (bgm_path), which PortableKit's sceAtrac decodes from the
// host file (PortableKit host/audio/host_audio.hpp). MIT licence; see
// port/README.md.

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace portablekit {
class HleRegistrar;
}

namespace d012::mods {

struct Mod {
    std::string id;
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::filesystem::path folder;
    bool enabled{};
    bool loaded{};  // its manifest was valid
};

struct Track {
    std::string id;         // "<mod id>:<track key>"
    std::uint32_t number{}; // FNV-1a of `id`; the host track id
    std::string key;
    std::string title;
    std::filesystem::path file;
    std::string series_id;
    std::string mod_id;
};

struct Series {
    std::string id;         // "<mod id>:<series key>"
    std::uint32_t number{}; // FNV-1a of `id`
    std::string key;
    std::string name;
    std::int64_t order{};   // series sort by order, then id, after the game's own
    std::string mod_id;
    std::vector<std::size_t> tracks;  // into Catalogue::tracks, in the manifest's order
};

struct Catalogue {
    std::vector<Mod> mods;        // every mod folder found, by folder name
    std::vector<Series> series;   // enabled mods' series, in display order
    std::vector<Track> tracks;
};

// What install() found; empty before it runs, with D012_NO_MODS, or with no
// mods folder.
[[nodiscard]] const Catalogue &catalogue();

// A track by its id ("mod:key") or by its number in hexadecimal ("1A2B3C4D"
// or "0x1A2B3C4D"); null when there is none.
[[nodiscard]] const Track *find_track(std::string_view id);

// The path the game opens to play `track` as one of its BGM files:
// disc0:/PSP_GAME/USRDIR/DATA/BGM/MD<number, 8 hex digits>.AT3 (fits the
// BGM table's 16-byte file name field).
[[nodiscard]] std::string bgm_path(const Track &track);

// Decodes the track on a worker thread, so that it starts without a pause.
void prefetch(const Track &track);

// Loads the mods and serves their tracks to the game. Called once, from the
// profile's HLE hook.
void install(portablekit::HleRegistrar &hle);

// FNV-1a, 32 bits, of `text`: how ids become numbers.
[[nodiscard]] std::uint32_t id_number(std::string_view text);

} // namespace d012::mods
