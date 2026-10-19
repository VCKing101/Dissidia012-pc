// Mods: loading <data dir>/mods, checking the manifests, and serving mod
// tracks to the game as BGM files. The manifest format is
// docs/port/mod-folder.md. A broken mod is logged and left out; it never stops
// the game, and nothing here replaces a file of the game's. MIT licence; see
// port/README.md.

#include "d012_mods.hpp"

#include "d012_json.hpp"
#include "d012_nintendo_stream.hpp"

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
constexpr std::size_t kMaxManifestBytes = 1u << 20u;
constexpr std::size_t kMaxNameLength = 64u;
constexpr int kFormat = 1;

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

// Ids and keys: lower case letters, digits, '.', '_' and '-', starting with a
// letter or digit, at most 64 characters.
bool valid_key(std::string_view key) {
    if (key.empty() || key.size() > kMaxNameLength) return false;
    if (!std::isalnum(static_cast<unsigned char>(key.front()))) return false;
    return std::all_of(key.begin(), key.end(), [](char c) {
        return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
    });
}

const std::set<std::string> &audio_extensions() {
    static const std::set<std::string> extensions = {".mp3", ".ogg",   ".oga",   ".opus",  ".flac",
                                                     ".wav", ".brstm", ".bcstm", ".bfstm"};
    return extensions;
}

// Collects one manifest's problems, each with its line.
class Report {
public:
    explicit Report(std::string where) : where_(std::move(where)) {}
    void error(const Value *at, const std::string &what) { line(at, "error", what); ++errors_; }
    void warning(const Value *at, const std::string &what) { line(at, "warning", what); }
    [[nodiscard]] int errors() const noexcept { return errors_; }
    [[nodiscard]] const std::string &where() const noexcept { return where_; }

private:
    void line(const Value *at, const char *kind, const std::string &what) {
        std::cerr << "[mods] " << kind << ": " << where_;
        if (at != nullptr && at->line > 0) std::cerr << ", line " << at->line;
        std::cerr << ": " << what << "\n";
    }
    std::string where_;
    int errors_{};
};

// A member of `object` of the given kind; reports a wrong kind or, when
// `required`, its absence.
const Value *member(const Value &object, std::string_view name, Value::Kind kind, bool required, Report &report,
                    std::string_view context) {
    const Value *value = object.find(name);
    if (value == nullptr) {
        if (required) report.error(&object, std::string(context) + " needs \"" + std::string(name) + "\"");
        return nullptr;
    }
    if (!value->is(kind)) {
        report.error(value, std::string(context) + " \"" + std::string(name) + "\" must be " + Value::kind_name(kind) +
                                ", not " + Value::kind_name(value->kind));
        return nullptr;
    }
    return value;
}

std::optional<std::string> text_member(const Value &object, std::string_view name, bool required, Report &report,
                                       std::string_view context, std::size_t max_length = kMaxNameLength) {
    const Value *value = member(object, name, Value::Kind::String, required, report, context);
    if (value == nullptr) return std::nullopt;
    if (value->text.empty() || value->text.size() > max_length) {
        report.error(value, std::string(context) + " \"" + std::string(name) + "\" must be 1 to " +
                                std::to_string(max_length) + " bytes long");
        return std::nullopt;
    }
    return value->text;
}

void warn_unknown(const Value &object, std::initializer_list<std::string_view> known, Report &report,
                  std::string_view context) {
    for (const auto &[name, value] : object.members) {
        if (std::find(known.begin(), known.end(), name) == known.end())
            report.warning(&value, std::string(context) + " has an unknown member \"" + name + "\", which is ignored");
    }
}

std::optional<std::int64_t> whole_number(const Value &value) {
    if (!value.is(Value::Kind::Number) || !std::isfinite(value.number) || value.number != std::floor(value.number) ||
        std::fabs(value.number) > 9.0e15)
        return std::nullopt;
    return static_cast<std::int64_t>(value.number);
}

// A track's "loop": "auto", "whole", "none", or {"start", "end"} in samples
// of the file, or {"start_seconds", "end_seconds"}.
bool read_loop(const Value *loop, audio::HostTrack &track, Report &report, const std::string &context) {
    if (loop == nullptr) return true;
    if (loop->is(Value::Kind::String)) {
        if (loop->text == "auto") track.loop = audio::HostTrack::Loop::Auto;
        else if (loop->text == "whole") track.loop = audio::HostTrack::Loop::Whole;
        else if (loop->text == "none") track.loop = audio::HostTrack::Loop::None;
        else {
            report.error(loop, context + " \"loop\" must be \"auto\", \"whole\", \"none\" or an object");
            return false;
        }
        return true;
    }
    if (!loop->is(Value::Kind::Object)) {
        report.error(loop, context + " \"loop\" must be \"auto\", \"whole\", \"none\" or an object");
        return false;
    }
    warn_unknown(*loop, {"start", "end", "start_seconds", "end_seconds"}, report, context + " \"loop\"");
    const Value *start = loop->find("start");
    const Value *end = loop->find("end");
    const Value *start_s = loop->find("start_seconds");
    const Value *end_s = loop->find("end_seconds");
    if ((start != nullptr || end != nullptr) && (start_s != nullptr || end_s != nullptr)) {
        report.error(loop, context + " \"loop\" gives samples and seconds; use one or the other");
        return false;
    }
    track.loop = audio::HostTrack::Loop::Range;
    if (start_s != nullptr || end_s != nullptr) {
        const auto seconds = [&](const Value *value, const char *name) -> std::optional<std::int64_t> {
            if (value == nullptr) return std::nullopt;
            if (!value->is(Value::Kind::Number) || !std::isfinite(value->number) || value->number < 0.0 ||
                value->number > 3600.0) {
                report.error(value, context + " \"loop\" \"" + name + "\" must be a number of seconds from 0 to 3600");
                return std::int64_t{-1};
            }
            return static_cast<std::int64_t>(std::llround(value->number * 1e6));
        };
        const auto first = seconds(start_s, "start_seconds");
        const auto last = seconds(end_s, "end_seconds");
        if (!first) {
            report.error(loop, context + " \"loop\" needs \"start_seconds\"");
            return false;
        }
        if (*first < 0 || (last && *last < 0)) return false;
        if (last && *last <= *first) {
            report.error(end_s, context + " \"loop\" ends before it starts");
            return false;
        }
        track.loop_start = {audio::HostLoopPoint::Unit::Microseconds, *first};
        if (last) track.loop_end = audio::HostLoopPoint{audio::HostLoopPoint::Unit::Microseconds, *last};
        return true;
    }
    if (start == nullptr) {
        report.error(loop, context + " \"loop\" needs \"start\" (a sample of the file) or \"start_seconds\"");
        return false;
    }
    const auto first = whole_number(*start);
    const auto last = end != nullptr ? whole_number(*end) : std::optional<std::int64_t>{};
    if (!first || *first < 0) {
        report.error(start, context + " \"loop\" \"start\" must be a whole number of samples, 0 or more");
        return false;
    }
    if (end != nullptr && (!last || *last <= *first)) {
        report.error(end, context + " \"loop\" \"end\" must be a whole number of samples after \"start\"");
        return false;
    }
    track.loop_start = {audio::HostLoopPoint::Unit::Samples, *first};
    if (last) track.loop_end = audio::HostLoopPoint{audio::HostLoopPoint::Unit::Samples, *last};
    return true;
}

// A track's file: relative to the mod's folder, inside it, there, and of a
// kind host audio reads.
std::optional<std::filesystem::path> track_file(const Value &value, const std::filesystem::path &folder,
                                                Report &report, const std::string &context) {
    const std::filesystem::path relative = portablekit::install::path_from_utf8(value.text);
    if (relative.empty() || relative.is_absolute() || relative.has_root_name() || relative.has_root_directory()) {
        report.error(&value, context + " \"file\" must be a path inside the mod's folder");
        return std::nullopt;
    }
    for (const auto &part : relative) {
        if (part == "..") {
            report.error(&value, context + " \"file\" must not leave the mod's folder (\"..\")");
            return std::nullopt;
        }
    }
    const std::string extension = lower(utf8(relative.extension()));
    if (extension == ".bwav" || extension == ".nus3audio") {
        report.error(&value, context + " \"file\" is a " + extension.substr(1u) +
                                 " file, which is not supported (FFmpeg cannot decode it); use the BRSTM version of "
                                 "the track");
        return std::nullopt;
    }
    if (!audio_extensions().contains(extension)) {
        report.error(&value, context + " \"file\" must be .mp3, .ogg, .opus, .flac, .wav, .brstm, .bcstm or .bfstm, "
                                       "not \"" + extension + "\"");
        return std::nullopt;
    }
    const std::filesystem::path full = folder / relative;
    std::error_code ec;
    if (!std::filesystem::is_regular_file(full, ec)) {
        report.error(&value, context + " \"file\" " + value.text + " is not in the mod's folder");
        return std::nullopt;
    }
    return full;
}

// A BRSTM, BCSTM or BFSTM track: its header is checked, and "loop": "auto"
// takes the header's loop (from its loop start to the end of the stream, the
// format's loop end) or, without a loop flag, the whole file. An explicit
// loop in the manifest is kept as it is.
bool read_stream_header(const Value &file, const std::filesystem::path &path, audio::HostTrack &track, Report &report,
                        const std::string &context) {
    if (!nintendo_stream::is_stream_extension(lower(utf8(path.extension())))) return true;
    std::string error;
    const auto info = nintendo_stream::read_header(path, error);
    if (!info) {
        report.error(&file, context + " \"file\" " + file.text + ": " + error);
        return false;
    }
    if (track.loop != audio::HostTrack::Loop::Auto) return true;
    if (!info->loop_flag) {
        track.loop = audio::HostTrack::Loop::Whole;
        return true;
    }
    if (info->loop_start >= info->samples) {
        report.warning(&file, context + ": the " + info->format + " header's loop starts at sample " +
                                  std::to_string(info->loop_start) + ", at or past the end of its " +
                                  std::to_string(info->samples) + " samples; the whole file loops");
        track.loop = audio::HostTrack::Loop::Whole;
        return true;
    }
    track.loop = audio::HostTrack::Loop::Range;
    track.loop_start = {audio::HostLoopPoint::Unit::Samples, info->loop_start};
    track.loop_end.reset();  // the end of the stream, which is where the format's loop ends
    std::cout << "[mods] " << report.where() << ", " << context << ": the " << info->format << " header's loop, samples " << info->loop_start
              << " to " << info->samples << " at " << info->rate << " Hz" << std::endl;
    return true;
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

// Reads and checks one mod folder's manifest. Problems in a series or a
// track leave that part out; problems with the mod itself leave all of it
// out (`loaded` false).
LoadedMod read_mod(const std::filesystem::path &folder) {
    LoadedMod result;
    result.mod.folder = folder;
    const std::string where = utf8(folder.filename()) + "/mod.json";
    Report report(where);
    const std::filesystem::path manifest = folder / "mod.json";
    std::error_code ec;
    if (!std::filesystem::is_regular_file(manifest, ec)) {
        report.error(nullptr, "there is no mod.json; the folder is not a mod");
        return result;
    }
    if (std::filesystem::file_size(manifest, ec) > kMaxManifestBytes) {
        report.error(nullptr, "larger than 1 MiB");
        return result;
    }
    std::ifstream in(manifest, std::ios::binary);
    const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    std::string parse_error;
    const auto document = json::parse(text, parse_error);
    if (!document) {
        report.error(nullptr, "not valid JSON: " + parse_error);
        return result;
    }
    if (!document->is(Value::Kind::Object)) {
        report.error(&*document, "the manifest must be an object");
        return result;
    }
    const Value &root = *document;
    warn_unknown(root, {"format", "id", "name", "version", "author", "description", "music"}, report, "the mod");
    if (const Value *format = member(root, "format", Value::Kind::Number, false, report, "the mod")) {
        if (whole_number(*format) != kFormat) {
            report.error(format, "\"format\" is " + std::to_string(format->number) + "; this build reads format " +
                                     std::to_string(kFormat));
            return result;
        }
    }
    const auto id = text_member(root, "id", true, report, "the mod");
    if (id && !valid_key(*id)) {
        report.error(root.find("id"), "\"id\" must be lower-case letters, digits, '.', '_' or '-', starting with a "
                                      "letter or digit");
        return result;
    }
    const auto name = text_member(root, "name", true, report, "the mod");
    const auto version = text_member(root, "version", true, report, "the mod", 32u);
    const auto author = text_member(root, "author", false, report, "the mod");
    const auto description = text_member(root, "description", false, report, "the mod", 1024u);
    if (!id || !name || !version) return result;
    result.mod.id = *id;
    result.mod.name = *name;
    result.mod.version = *version;
    result.mod.author = author.value_or("");
    result.mod.description = description.value_or("");
    result.mod.loaded = true;

    const Value *music = member(root, "music", Value::Kind::Object, false, report, "the mod");
    if (music == nullptr) {
        report.warning(&root, "the mod adds nothing this build knows (no \"music\")");
        return result;
    }
    warn_unknown(*music, {"series"}, report, "\"music\"");
    const Value *series_list = member(*music, "series", Value::Kind::Array, true, report, "\"music\"");
    if (series_list == nullptr) return result;
    std::set<std::string> series_keys;
    std::set<std::string> track_keys;
    for (const Value &entry : series_list->items) {
        if (!entry.is(Value::Kind::Object)) {
            report.error(&entry, "each series must be an object");
            continue;
        }
        const auto key = text_member(entry, "key", true, report, "a series");
        const std::string context = "series \"" + key.value_or("?") + "\"";
        warn_unknown(entry, {"key", "name", "order", "tracks"}, report, context);
        const auto series_name = text_member(entry, "name", true, report, context, 48u);
        if (!key || !series_name) {
            report.error(&entry, context + " is left out");
            continue;
        }
        if (!valid_key(*key)) {
            report.error(entry.find("key"), context + ": \"key\" must be lower-case letters, digits, '.', '_' or '-'");
            continue;
        }
        if (!series_keys.insert(*key).second) {
            report.error(&entry, context + " appears twice; the second is left out");
            continue;
        }
        LoadedSeries series;
        series.series.key = *key;
        series.series.name = *series_name;
        series.series.mod_id = *id;
        series.series.id = *id + ":" + *key;
        series.series.number = id_number(series.series.id);
        if (const Value *order = member(entry, "order", Value::Kind::Number, false, report, context)) {
            const auto value = whole_number(*order);
            if (!value || *value < -1'000'000 || *value > 1'000'000)
                report.error(order, context + ": \"order\" must be a whole number from -1000000 to 1000000; 0 is used");
            else
                series.series.order = *value;
        }
        const Value *tracks = member(entry, "tracks", Value::Kind::Array, true, report, context);
        if (tracks == nullptr) continue;
        for (const Value &item : tracks->items) {
            if (!item.is(Value::Kind::Object)) {
                report.error(&item, context + ": each track must be an object");
                continue;
            }
            const auto track_key = text_member(item, "key", true, report, context + ", a track");
            const std::string track_context = "track \"" + track_key.value_or("?") + "\"";
            warn_unknown(item, {"key", "title", "file", "loop"}, report, track_context);
            const auto title = text_member(item, "title", true, report, track_context);
            const Value *file = member(item, "file", Value::Kind::String, true, report, track_context);
            if (!track_key || !title || file == nullptr) {
                report.error(&item, track_context + " is left out");
                continue;
            }
            if (!valid_key(*track_key)) {
                report.error(item.find("key"), track_context + ": \"key\" must be lower-case letters, digits, '.', '_' or '-'");
                continue;
            }
            if (!track_keys.insert(*track_key).second) {
                report.error(&item, track_context + " appears twice in the mod; the second is left out");
                continue;
            }
            LoadedTrack track;
            const auto path = track_file(*file, folder, report, track_context);
            if (!path || !read_loop(item.find("loop"), track.audio, report, track_context) ||
                !read_stream_header(*file, *path, track.audio, report, track_context)) {
                report.error(&item, track_context + " is left out");
                continue;
            }
            track.track.key = *track_key;
            track.track.title = *title;
            track.track.file = *path;
            track.track.mod_id = *id;
            track.track.series_id = series.series.id;
            track.track.id = *id + ":" + *track_key;
            track.track.number = id_number(track.track.id);
            track.audio.file = *path;
            track.file_text = track.track.mod_id + "/" + file->text;
            track.audio.name = track.track.id;
            series.tracks.push_back(std::move(track));
        }
        if (series.tracks.empty()) {
            report.error(&entry, context + " has no track that can be played; it is left out");
            continue;
        }
        result.series.push_back(std::move(series));
    }
    return result;
}

// mods/enabled.json: {"mods": {"<mod id>": true or false}}. A mod it does
// not name is on.
std::map<std::string, bool> read_enabled(const std::filesystem::path &mods) {
    std::map<std::string, bool> switches;
    const std::filesystem::path file = mods / "enabled.json";
    std::error_code ec;
    if (!std::filesystem::exists(file, ec)) return switches;
    Report report("enabled.json");
    std::ifstream in(file, std::ios::binary);
    const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    std::string error;
    const auto document = json::parse(text, error);
    if (!document || !document->is(Value::Kind::Object)) {
        report.error(nullptr, document ? "must be an object" : "not valid JSON: " + error);
        report.warning(nullptr, "every mod is on, as with no enabled.json");
        return switches;
    }
    warn_unknown(*document, {"mods"}, report, "enabled.json");
    const Value *list = member(*document, "mods", Value::Kind::Object, false, report, "enabled.json");
    if (list == nullptr) return switches;
    for (const auto &[id, value] : list->members) {
        if (!value.is(Value::Kind::Bool)) {
            report.error(&value, "\"" + id + "\" must be true or false; the mod stays on");
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

std::uint32_t id_number(std::string_view text) {
    std::uint32_t hash = 0x811C9DC5u;
    for (const char c : text) {
        hash ^= static_cast<std::uint8_t>(c);
        hash *= 0x01000193u;
    }
    return hash;
}

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
