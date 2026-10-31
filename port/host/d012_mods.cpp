// Mods: loading <data dir>/mods, checking the manifests (by the rules in
// d012_mod_manifest.cpp, which the Battle BGM Maker shares), and serving mod
// tracks to the game as BGM files. The manifest format is
// docs/port/mod-folder.md. A broken mod is logged and left out; it never stops
// the game, and nothing here replaces a file of the game's. MIT licence; see
// port/README.md.

#include "d012_mods.hpp"

#include "d012_json.hpp"
#include "d012_mod_manifest.hpp"

#include "audio/host_audio.hpp"
#include "hle/hle_common.hpp"
#include "install/user_data.hpp"
#include "portablekit/hle_extension.hpp"
#include "profile.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>

namespace d012::mods {
namespace {

namespace audio = portablekit::audio;
using json::Value;

// The game's BGM folder on the disc, as the battle opens its music
// ("DISC0:/PSP_GAME/USRDIR/DATA/BGM/U_13BATTLE.AT3", traced with
// D012_TRACE_IO), without the device.
constexpr std::string_view kBgmFolder = "PSP_GAME/USRDIR/DATA/BGM/";
// The frame size of the game's 96 kbps BGM (that track's "fmt " chunk), which
// stand-ins use unless they replace a file whose own size is known.
constexpr std::uint32_t kGameBgmBlockAlign = 560u;

Catalogue &mutable_catalogue() {
    static Catalogue instance;
    return instance;
}

std::string utf8(const std::filesystem::path &path) { return portablekit::install::path_to_utf8(path); }

std::string lower(std::string_view text) {
    std::string out(text);
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return out;
}

// One problem of a file the mods are read from, in the log:
// "[mods] error: <where>, line N: what", warnings the same way, and a note as
// "[mods] <where>, what" on standard output.
void log_problem(const mod_manifest::Problem &problem, const std::string &where) {
    using Level = mod_manifest::Problem::Level;
    if (problem.level == Level::Note) {
        std::cout << "[mods] " << where << ", " << problem.message << std::endl;
        return;
    }
    std::cerr << "[mods] " << (problem.level == Level::Error ? "error" : "warning") << ": " << where;
    if (problem.line > 0) std::cerr << ", line " << problem.line;
    std::cerr << ": " << problem.message << "\n";
}

audio::HostLoopPoint host_loop_point(const mod_manifest::LoopPoint &point) {
    return {point.unit == mod_manifest::LoopPoint::Unit::Samples ? audio::HostLoopPoint::Unit::Samples
                                                                 : audio::HostLoopPoint::Unit::Microseconds,
            point.value};
}

audio::HostTrack::Loop host_loop_mode(mod_manifest::Loop::Mode mode) {
    switch (mode) {
    case mod_manifest::Loop::Mode::Whole: return audio::HostTrack::Loop::Whole;
    case mod_manifest::Loop::Mode::None: return audio::HostTrack::Loop::None;
    case mod_manifest::Loop::Mode::Range: return audio::HostTrack::Loop::Range;
    case mod_manifest::Loop::Mode::Auto: break;
    }
    return audio::HostTrack::Loop::Auto;
}

struct LoadedTrack {
    Track track;
    audio::HostTrack audio;
    std::string file_text;  // as the manifest gives it
};
struct LoadedSeries {
    Series series;
    std::vector<LoadedTrack> tracks;
};
struct LoadedMod {
    Mod mod;
    std::vector<LoadedSeries> series;
};

// Reads and checks one mod folder's manifest by the shared rules
// (d012_mod_manifest.cpp) and logs its problems. Problems in a series or a
// track leave that part out; problems with the mod itself leave all of it
// out (`loaded` false).
LoadedMod read_mod(const std::filesystem::path &folder) {
    LoadedMod result;
    result.mod.folder = folder;
    const std::string where = utf8(folder.filename()) + "/mod.json";
    const mod_manifest::Result read = mod_manifest::read(folder);
    for (const auto &problem : read.problems) log_problem(problem, where);
    const mod_manifest::Manifest &manifest = read.manifest;
    if (!manifest.loaded) return result;
    result.mod.id = manifest.id;
    result.mod.name = manifest.name;
    result.mod.version = manifest.version;
    result.mod.author = manifest.author;
    result.mod.description = manifest.description;
    result.mod.loaded = true;
    for (const auto &entry : manifest.series) {
        LoadedSeries series;
        series.series.key = entry.key;
        series.series.name = entry.name;
        series.series.order = entry.order;
        series.series.mod_id = manifest.id;
        series.series.id = manifest.id + ":" + entry.key;
        series.series.number = id_number(series.series.id);
        for (const auto &item : entry.tracks) {
            LoadedTrack track;
            track.track.key = item.key;
            track.track.title = item.title;
            track.track.file = item.file;
            track.track.mod_id = manifest.id;
            track.track.series_id = series.series.id;
            track.track.id = manifest.id + ":" + item.key;
            track.track.number = id_number(track.track.id);
            track.audio.file = item.file;
            track.audio.loop = host_loop_mode(item.loop.mode);
            track.audio.loop_start = host_loop_point(item.loop.start);
            if (item.loop.end) track.audio.loop_end = host_loop_point(*item.loop.end);
            track.audio.name = track.track.id;
            track.file_text = track.track.mod_id + "/" + item.file_text;
            series.tracks.push_back(std::move(track));
        }
        result.series.push_back(std::move(series));
    }
    return result;
}

// mods/enabled.json: {"mods": {"<mod id>": true or false}}. A mod it does
// not name is on.
std::map<std::string, bool> read_enabled(const std::filesystem::path &mods) {
    using Level = mod_manifest::Problem::Level;
    std::map<std::string, bool> switches;
    const std::filesystem::path file = mods / "enabled.json";
    std::error_code ec;
    if (!std::filesystem::exists(file, ec)) return switches;
    const auto log = [](Level level, const Value *at, const std::string &what) {
        log_problem({level, {}, what, at != nullptr ? at->line : 0}, "enabled.json");
    };
    std::ifstream in(file, std::ios::binary);
    const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    std::string error;
    const auto document = json::parse(text, error);
    if (!document || !document->is(Value::Kind::Object)) {
        log(Level::Error, nullptr, document ? "must be an object" : "not valid JSON: " + error);
        log(Level::Warning, nullptr, "every mod is on, as with no enabled.json");
        return switches;
    }
    for (const auto &[name, value] : document->members)
        if (name != "mods") log(Level::Warning, &value, "enabled.json has an unknown member \"" + name + "\", which is ignored");
    const Value *list = document->find("mods");
    if (list == nullptr) return switches;
    if (!list->is(Value::Kind::Object)) {
        log(Level::Error, list, std::string("enabled.json \"mods\" must be ") + Value::kind_name(Value::Kind::Object) +
                                    ", not " + Value::kind_name(list->kind));
        return switches;
    }
    for (const auto &[id, value] : list->members) {
        if (!value.is(Value::Kind::Bool)) {
            log(Level::Error, &value, "\"" + id + "\" must be true or false; the mod stays on");
            continue;
        }
        switches[id] = value.boolean;
    }
    return switches;
}

std::filesystem::path mods_directory() {
    if (const char *dir = portablekit::env("MODS_DIR"); dir != nullptr && *dir != '\0')
        return portablekit::install::path_from_utf8(dir);
    try {
        return portablekit::install::user_data_directory() / "mods";
    } catch (const std::exception &error) {
        std::cerr << "[mods] no data directory (" << error.what() << "); no mods\n";
        return {};
    }
}

void load() {
    Catalogue &result = mutable_catalogue();
    const std::filesystem::path mods = mods_directory();
    std::error_code ec;
    if (mods.empty() || !std::filesystem::is_directory(mods, ec)) {
        std::cout << "[mods] no mods folder (" << utf8(mods) << ")\n";
        return;
    }
    const auto switches = read_enabled(mods);
    std::vector<std::filesystem::path> folders;
    for (const auto &entry : std::filesystem::directory_iterator(mods, ec))
        if (entry.is_directory(ec)) folders.push_back(entry.path());
    std::sort(folders.begin(), folders.end());

    std::vector<LoadedMod> loaded;
    std::set<std::string> ids;
    for (const auto &folder : folders) {
        LoadedMod mod = read_mod(folder);
        if (mod.mod.loaded && !ids.insert(mod.mod.id).second) {
            std::cerr << "[mods] error: " << utf8(folder.filename()) << "/mod.json: another folder already has the id \""
                      << mod.mod.id << "\"; this one is left out\n";
            mod.mod.loaded = false;
            mod.series.clear();
        }
        if (mod.mod.loaded) {
            const auto found = switches.find(mod.mod.id);
            mod.mod.enabled = found == switches.end() || found->second;
        }
        loaded.push_back(std::move(mod));
    }
    for (const auto &[id, on] : switches)
        if (!ids.contains(id)) std::cerr << "[mods] warning: enabled.json names \"" << id << "\", which no mod has\n";

    // Tracks of enabled mods, then series in display order. Numbers must be
    // unique: on the (unlikely) clash of two ids, the one that sorts later is
    // left out, whatever order the mods were found in.
    std::map<std::uint32_t, std::string> numbers;
    for (const auto &mod : loaded)
        for (const auto &series : mod.series)
            for (const auto &track : series.tracks)
                if (mod.mod.enabled) {
                    auto [at, inserted] = numbers.emplace(track.track.number, track.track.id);
                    if (!inserted && track.track.id < at->second) at->second = track.track.id;
                }
    std::vector<LoadedSeries> series_all;
    for (auto &mod : loaded) {
        if (mod.mod.enabled) {
            for (auto &series : mod.series) series_all.push_back(std::move(series));
        }
        mod.series.clear();
        result.mods.push_back(mod.mod);
    }
    std::sort(series_all.begin(), series_all.end(), [](const LoadedSeries &a, const LoadedSeries &b) {
        return a.series.order != b.series.order ? a.series.order < b.series.order : a.series.id < b.series.id;
    });
    const bool host_audio = audio::host_audio_available();
    for (auto &series : series_all) {
        for (auto &track : series.tracks) {
            if (numbers.at(track.track.number) != track.track.id) {
                std::cerr << "[mods] error: track " << track.track.id << " has the same number as "
                          << numbers.at(track.track.number) << "; rename one of their keys. It is left out\n";
                continue;
            }
            // Without host audio (said once, in install()) no track can play.
            if (!host_audio) continue;
            std::string error;
            if (!audio::probe_host_audio(track.audio.file, error)) {
                std::cerr << "[mods] error: track " << track.track.id << " (" << track.file_text
                          << ") cannot be played: " << error << ". It is left out\n";
                continue;
            }
            if (!audio::register_host_track(track.track.number, track.audio)) continue;
            series.series.tracks.push_back(result.tracks.size());
            result.tracks.push_back(track.track);
        }
        if (!series.series.tracks.empty()) result.series.push_back(std::move(series.series));
    }

    std::size_t enabled = 0u;
    for (const auto &mod : result.mods) {
        if (!mod.loaded) continue;
        if (mod.enabled) ++enabled;
        std::cout << "[mods] " << mod.id << " " << mod.version << " (" << utf8(mod.folder.filename()) << ")"
                  << (mod.enabled ? "" : ", off in enabled.json") << "\n";
    }
    std::cout << "[mods] " << enabled << " of " << result.mods.size() << " mod folder(s) on: " << result.series.size()
              << " series, " << result.tracks.size() << " track(s) of music added" << std::endl;
}

std::optional<std::uint32_t> parse_hex(std::string_view text) {
    if (text.starts_with("0x") || text.starts_with("0X")) text.remove_prefix(2u);
    if (text.empty() || text.size() > 8u) return std::nullopt;
    std::uint32_t value{};
    const auto [end, ec] = std::from_chars(text.data(), text.data() + text.size(), value, 16);
    if (ec != std::errc{} || end != text.data() + text.size()) return std::nullopt;
    return value;
}

const Track *find_number(std::uint32_t number) {
    for (const Track &track : catalogue().tracks)
        if (track.number == number) return &track;
    return nullptr;
}

// The frame size in the "fmt " chunk of the WAVE file open as `fd`, or the
// game's usual one.
std::uint32_t block_align_of(std::uint32_t fd) {
    std::vector<std::uint8_t> header(4096u);
    header.resize(portablekit::read_open_file(fd, 0u, header.data(), header.size()));
    const auto le32 = [&](std::size_t at) {
        return static_cast<std::uint32_t>(header[at]) | (static_cast<std::uint32_t>(header[at + 1u]) << 8u) |
               (static_cast<std::uint32_t>(header[at + 2u]) << 16u) | (static_cast<std::uint32_t>(header[at + 3u]) << 24u);
    };
    if (header.size() < 12u || le32(0u) != 0x46464952u || le32(8u) != 0x45564157u) return kGameBgmBlockAlign;
    for (std::size_t at = 12u; at + 8u <= header.size();) {
        const std::uint32_t size = le32(at + 4u);
        if (le32(at) == 0x20746D66u && size >= 14u && at + 8u + 14u <= header.size()) {
            const std::uint32_t align = header[at + 8u + 12u] | (header[at + 8u + 13u] << 8u);
            return align != 0u ? align : kGameBgmBlockAlign;
        }
        at += 8u + size + (size & 1u);
    }
    return kGameBgmBlockAlign;
}

// D012_TEST_BGM_OVERRIDE: the next BGM file the game opens plays this track.
// A test switch only: the shipped feature adds tracks and replaces none.
struct Override {
    const Track *track{};
    bool used{};
};
Override &test_override() {
    static Override state;
    return state;
}

void set_up_test_override() {
    const char *value = portablekit::env("TEST_BGM_OVERRIDE");
    if (value == nullptr || *value == '\0') return;
    const Track *track = find_track(value);
    if (track == nullptr) {
        std::cerr << "[mods] " << portablekit::env_name("TEST_BGM_OVERRIDE") << "=" << value
                  << ": no such mod track; the game's own music plays\n";
        return;
    }
    test_override().track = track;
    std::cout << "[mods] test switch " << portablekit::env_name("TEST_BGM_OVERRIDE") << ": the next BGM the game opens plays "
              << track->id << " (" << track->title << ")" << std::endl;
}

std::shared_ptr<portablekit::hle_extension::FileFilter> serve_bgm(std::string_view path, std::uint32_t fd);

// D012_TEST_MODS_DECODE=1: decodes every track at start-up and logs its
// length, level and, from its zero crossings, the pitch of a pure tone, to
// check the decoders on test tones without playing each one. A test switch.
void test_decode_all() {
    if (portablekit::env("TEST_MODS_DECODE") == nullptr) return;
    for (const Track &track : catalogue().tracks) {
        const auto pcm = audio::host_track_pcm(track.number);
        if (!pcm || pcm->failed) {
            std::cout << "[mods] decode check " << track.id << ": failed" << std::endl;
            continue;
        }
        std::int64_t crossings = 0;
        double energy = 0.0;
        int peak = 0;
        const auto &samples = pcm->samples;
        for (std::size_t i = 0; i < samples.size(); i += 2u) {
            const int left = samples[i];
            energy += static_cast<double>(left) * left;
            peak = std::max(peak, std::abs(left));
            if (i >= 2u && (samples[i - 2u] < 0) != (left < 0)) ++crossings;
        }
        const double seconds = static_cast<double>(pcm->frames()) / audio::kHostAudioRate;
        std::cout << "[mods] decode check " << track.id << ": " << pcm->codec << " " << pcm->source_rate << " Hz x"
                  << pcm->source_channels << ", " << seconds << " s, peak " << peak << ", rms "
                  << std::sqrt(energy / pcm->frames()) << ", about " << crossings / (2.0 * seconds) << " Hz"
                  << std::endl;
        // What the game would read at the track's own path.
        const std::string path = bgm_path(track);
        const auto file = serve_bgm(path, 0u);
        std::uint8_t head[12]{};
        if (file) file->read(0u, head, sizeof head);
        std::cout << "[mods] decode check " << path << ": "
                  << (file && std::memcmp(head, "RIFF", 4) == 0 && std::memcmp(head + 8, "WAVE", 4) == 0
                          ? "a stand-in of " + std::to_string(file->size()) + " bytes"
                          : std::string("nothing served"))
                  << std::endl;
    }
}

std::shared_ptr<portablekit::hle_extension::FileFilter> serve_bgm(std::string_view path, std::uint32_t fd) {
    if (!path.starts_with("disc0:/")) return nullptr;
    const std::string relative = lower(path.substr(7u));
    const std::string folder = lower(kBgmFolder);
    if (!relative.starts_with(folder) || !relative.ends_with(".at3")) return nullptr;
    const std::string name = relative.substr(folder.size(), relative.size() - folder.size() - 4u);
    if (fd == 0u) {
        // A mod track at its own path, MD<number>.AT3 (14 characters, so it fits
        // the BGM table's 16-byte name field), which is never on the disc.
        if (!name.starts_with("md")) return nullptr;
        const auto number = parse_hex(std::string_view(name).substr(2u));
        const Track *track = number && name.size() == 10u ? find_number(*number) : nullptr;
        if (track == nullptr) {
            portablekit::log_once("mods-unknown-" + name, "[mods] the game asked for " + std::string(path) +
                                                              ", which no mod track has");
            return nullptr;
        }
        auto file = audio::virtual_atrac_file(track->number, kGameBgmBlockAlign);
        std::cout << "[mods] " << path << " plays " << track->id << (file ? "" : " (it cannot be decoded)") << std::endl;
        return file;
    }
    Override &test = test_override();
    if (test.track == nullptr || test.used) return nullptr;
    test.used = true;
    auto file = audio::virtual_atrac_file(test.track->number, block_align_of(fd));
    std::cout << "[mods] test switch: " << path << " plays " << test.track->id << " instead"
              << (file ? "" : "; it cannot be decoded, so the game's own plays") << std::endl;
    return file;
}

} // namespace

std::uint32_t id_number(std::string_view text) { return mod_manifest::id_number(text); }

const Catalogue &catalogue() { return mutable_catalogue(); }

const Track *find_track(std::string_view id) {
    for (const Track &track : catalogue().tracks)
        if (track.id == id) return &track;
    if (const auto number = parse_hex(id)) return find_number(*number);
    return nullptr;
}

std::string bgm_path(const Track &track) {
    char name[16];
    std::snprintf(name, sizeof name, "MD%08X", track.number);
    return "disc0:/" + std::string(kBgmFolder) + name + ".AT3";
}

void prefetch(const Track &track) { audio::prefetch_host_track(track.number); }

void install(portablekit::HleRegistrar &) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    if (portablekit::env("NO_MODS") != nullptr) {
        std::cout << "[mods] off (" << portablekit::env_name("NO_MODS") << ")\n";
        return;
    }
    if (!audio::host_audio_available())
        std::cout << "[mods] this build cannot play host audio files, or "
                  << portablekit::env_name("NO_HOST_AUDIO") << " is set: mod music is left out\n";
    load();
    set_up_test_override();
    test_decode_all();
    if (!catalogue().tracks.empty()) portablekit::add_file_provider(&serve_bgm);
}

} // namespace d012::mods
