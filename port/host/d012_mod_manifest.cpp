// A mod's manifest read and checked by the game's rules; see
// d012_mod_manifest.hpp. The rules and messages were d012_mods.cpp's
// read_mod, read_loop, track_file and read_stream_header, unchanged. MIT
// licence; see port/README.md.

#include "d012_mod_manifest.hpp"

#include "d012_json.hpp"
#include "d012_nintendo_stream.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iterator>
#include <set>

namespace d012::mod_manifest {
namespace {

using json::Value;
using Level = Problem::Level;

std::string lower(std::string_view text) {
    std::string out(text);
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return out;
}

// Collects one manifest's problems, each with its path and line.
class Report {
public:
    explicit Report(std::vector<Problem> &out) : out_(out) {}
    void error(const Value *at, std::string path, std::string what) { add(Level::Error, at, std::move(path), std::move(what)); }
    void warning(const Value *at, std::string path, std::string what) { add(Level::Warning, at, std::move(path), std::move(what)); }
    void note(const Value *at, std::string path, std::string what) { add(Level::Note, at, std::move(path), std::move(what)); }

private:
    void add(Level level, const Value *at, std::string path, std::string what) {
        out_.push_back({level, std::move(path), std::move(what), at != nullptr ? at->line : 0});
    }
    std::vector<Problem> &out_;
};

std::string member_path(std::string_view path, std::string_view name) {
    return std::string(path) + "." + std::string(name);
}

// A member of `object` of the given kind; reports a wrong kind or, when
// `required`, its absence.
const Value *member(const Value &object, std::string_view name, Value::Kind kind, bool required, Report &report,
                    std::string_view path, std::string_view context) {
    const Value *value = object.find(name);
    if (value == nullptr) {
        if (required) report.error(&object, std::string(path), std::string(context) + " needs \"" + std::string(name) + "\"");
        return nullptr;
    }
    if (!value->is(kind)) {
        report.error(value, member_path(path, name),
                     std::string(context) + " \"" + std::string(name) + "\" must be " + Value::kind_name(kind) + ", not " +
                         Value::kind_name(value->kind));
        return nullptr;
    }
    return value;
}

std::optional<std::string> text_member(const Value &object, std::string_view name, bool required, Report &report,
                                       std::string_view path, std::string_view context,
                                       std::size_t max_length = kMaxKeyLength) {
    const Value *value = member(object, name, Value::Kind::String, required, report, path, context);
    if (value == nullptr) return std::nullopt;
    if (value->text.empty() || value->text.size() > max_length) {
        report.error(value, member_path(path, name),
                     std::string(context) + " \"" + std::string(name) + "\" must be 1 to " + std::to_string(max_length) +
                         " bytes long");
        return std::nullopt;
    }
    return value->text;
}

void warn_unknown(const Value &object, std::initializer_list<std::string_view> known, Report &report,
                  std::string_view path, std::string_view context) {
    for (const auto &[name, value] : object.members) {
        if (std::find(known.begin(), known.end(), name) == known.end())
            report.warning(&value, member_path(path, name),
                           std::string(context) + " has an unknown member \"" + name + "\", which is ignored");
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
bool read_loop(const Value *loop, Loop &out, Report &report, const std::string &path, const std::string &context) {
    if (loop == nullptr) return true;
    if (loop->is(Value::Kind::String)) {
        if (loop->text == "auto") out.mode = Loop::Mode::Auto;
        else if (loop->text == "whole") out.mode = Loop::Mode::Whole;
        else if (loop->text == "none") out.mode = Loop::Mode::None;
        else {
            report.error(loop, path, context + " \"loop\" must be \"auto\", \"whole\", \"none\" or an object");
            return false;
        }
        return true;
    }
    if (!loop->is(Value::Kind::Object)) {
        report.error(loop, path, context + " \"loop\" must be \"auto\", \"whole\", \"none\" or an object");
        return false;
    }
    warn_unknown(*loop, {"start", "end", "start_seconds", "end_seconds"}, report, path, context + " \"loop\"");
    const Value *start = loop->find("start");
    const Value *end = loop->find("end");
    const Value *start_s = loop->find("start_seconds");
    const Value *end_s = loop->find("end_seconds");
    if ((start != nullptr || end != nullptr) && (start_s != nullptr || end_s != nullptr)) {
        report.error(loop, path, context + " \"loop\" gives samples and seconds; use one or the other");
        return false;
    }
    out.mode = Loop::Mode::Range;
    if (start_s != nullptr || end_s != nullptr) {
        const auto seconds = [&](const Value *value, const char *name) -> std::optional<std::int64_t> {
            if (value == nullptr) return std::nullopt;
            if (!value->is(Value::Kind::Number) || !std::isfinite(value->number) || value->number < 0.0 ||
                value->number > 3600.0) {
                report.error(value, member_path(path, name),
                             context + " \"loop\" \"" + name + "\" must be a number of seconds from 0 to 3600");
                return std::int64_t{-1};
            }
            return static_cast<std::int64_t>(std::llround(value->number * 1e6));
        };
        const auto first = seconds(start_s, "start_seconds");
        const auto last = seconds(end_s, "end_seconds");
        if (!first) {
            report.error(loop, path, context + " \"loop\" needs \"start_seconds\"");
            return false;
        }
        if (*first < 0 || (last && *last < 0)) return false;
        if (last && *last <= *first) {
            report.error(end_s, member_path(path, "end_seconds"), context + " \"loop\" ends before it starts");
            return false;
        }
        out.start = {LoopPoint::Unit::Microseconds, *first};
        if (last) out.end = LoopPoint{LoopPoint::Unit::Microseconds, *last};
        return true;
    }
    if (start == nullptr) {
        report.error(loop, path, context + " \"loop\" needs \"start\" (a sample of the file) or \"start_seconds\"");
        return false;
    }
    const auto first = whole_number(*start);
    const auto last = end != nullptr ? whole_number(*end) : std::optional<std::int64_t>{};
    if (!first || *first < 0) {
        report.error(start, member_path(path, "start"),
                     context + " \"loop\" \"start\" must be a whole number of samples, 0 or more");
        return false;
    }
    if (end != nullptr && (!last || *last <= *first)) {
        report.error(end, member_path(path, "end"),
                     context + " \"loop\" \"end\" must be a whole number of samples after \"start\"");
        return false;
    }
    out.start = {LoopPoint::Unit::Samples, *first};
    if (last) out.end = LoopPoint{LoopPoint::Unit::Samples, *last};
    return true;
}

// A track's file: relative to the mod's folder, inside it, there, and of a
// kind host audio reads.
std::optional<std::filesystem::path> track_file(const Value &value, const std::filesystem::path &folder,
                                                Report &report, const std::string &path, const std::string &context) {
    const std::filesystem::path relative = path_from_utf8(value.text);
    if (relative.empty() || relative.is_absolute() || relative.has_root_name() || relative.has_root_directory()) {
        report.error(&value, path, context + " \"file\" must be a path inside the mod's folder");
        return std::nullopt;
    }
    for (const auto &part : relative) {
        if (part == "..") {
            report.error(&value, path, context + " \"file\" must not leave the mod's folder (\"..\")");
            return std::nullopt;
        }
    }
    const std::string extension = lower(utf8(relative.extension()));
    if (extension == ".bwav" || extension == ".nus3audio") {
        report.error(&value, path,
                     context + " \"file\" is a " + extension.substr(1u) +
                         " file, which is not supported (FFmpeg cannot decode it); use the BRSTM version of the track");
        return std::nullopt;
    }
    if (!is_audio_extension(extension)) {
        report.error(&value, path,
                     context + " \"file\" must be .mp3, .ogg, .opus, .flac, .wav, .brstm, .bcstm or .bfstm, not \"" +
                         extension + "\"");
        return std::nullopt;
    }
    const std::filesystem::path full = folder / relative;
    std::error_code ec;
    if (!std::filesystem::is_regular_file(full, ec)) {
        report.error(&value, path, context + " \"file\" " + value.text + " is not in the mod's folder");
        return std::nullopt;
    }
    return full;
}

// A BRSTM, BCSTM or BFSTM track: its header is checked, and "loop": "auto"
// takes the header's loop (from its loop start to the end of the stream, the
// format's loop end) or, without a loop flag, the whole file. An explicit
// loop in the manifest is kept as it is.
bool read_stream_header(const Value &file, const std::filesystem::path &full, Loop &loop, Report &report,
                        const std::string &path, const std::string &context) {
    if (!nintendo_stream::is_stream_extension(lower(utf8(full.extension())))) return true;
    std::string error;
    const auto info = nintendo_stream::read_header(full, error);
    if (!info) {
        report.error(&file, path + ".file", context + " \"file\" " + file.text + ": " + error);
        return false;
    }
    if (loop.mode != Loop::Mode::Auto) return true;
    if (!info->loop_flag) {
        loop.mode = Loop::Mode::Whole;
        return true;
    }
    if (info->loop_start >= info->samples) {
        report.warning(&file, path + ".loop",
                       context + ": the " + info->format + " header's loop starts at sample " +
                           std::to_string(info->loop_start) + ", at or past the end of its " +
                           std::to_string(info->samples) + " samples; the whole file loops");
        loop.mode = Loop::Mode::Whole;
        return true;
    }
    loop.mode = Loop::Mode::Range;
    loop.start = {LoopPoint::Unit::Samples, info->loop_start};
    loop.end.reset();  // the end of the stream, which is where the format's loop ends
    report.note(&file, path + ".loop",
                context + ": the " + info->format + " header's loop, samples " + std::to_string(info->loop_start) +
                    " to " + std::to_string(info->samples) + " at " + std::to_string(info->rate) + " Hz");
    return true;
}

// Problems in a series or a track leave that part out; problems with the mod
// itself leave all of it out (`loaded` false).
void check(const Value &root, const std::filesystem::path &folder, Manifest &result, Report &report) {
    warn_unknown(root, {"format", "id", "name", "version", "author", "description", "music"}, report, "mod", "the mod");
    if (const Value *format = member(root, "format", Value::Kind::Number, false, report, "mod", "the mod")) {
        if (whole_number(*format) != kFormat) {
            report.error(format, "mod.format",
                         "\"format\" is " + std::to_string(format->number) + "; this build reads format " +
                             std::to_string(kFormat));
            return;
        }
    }
    const auto id = text_member(root, "id", true, report, "mod", "the mod");
    if (id && !valid_key(*id)) {
        report.error(root.find("id"), "mod.id",
                     "\"id\" must be lower-case letters, digits, '.', '_' or '-', starting with a letter or digit");
        return;
    }
    const auto name = text_member(root, "name", true, report, "mod", "the mod");
    const auto version = text_member(root, "version", true, report, "mod", "the mod", kMaxVersionLength);
    const auto author = text_member(root, "author", false, report, "mod", "the mod");
    const auto description = text_member(root, "description", false, report, "mod", "the mod", kMaxDescriptionLength);
    if (!id || !name || !version) return;
    result.id = *id;
    result.name = *name;
    result.version = *version;
    result.author = author.value_or("");
    result.description = description.value_or("");
    result.loaded = true;

    const Value *music = member(root, "music", Value::Kind::Object, false, report, "mod", "the mod");
    if (music == nullptr) {
        report.warning(&root, "mod", "the mod adds nothing this build knows (no \"music\")");
        return;
    }
    warn_unknown(*music, {"series"}, report, "music", "\"music\"");
    const Value *series_list = member(*music, "series", Value::Kind::Array, true, report, "music", "\"music\"");
    if (series_list == nullptr) return;
    std::set<std::string> series_keys;
    std::set<std::string> track_keys;
    for (std::size_t s = 0; s < series_list->items.size(); ++s) {
        const Value &entry = series_list->items[s];
        const std::string s_path = "series." + std::to_string(s);
        if (!entry.is(Value::Kind::Object)) {
            report.error(&entry, s_path, "each series must be an object");
            continue;
        }
        const auto key = text_member(entry, "key", true, report, s_path, "a series");
        const std::string context = "series \"" + key.value_or("?") + "\"";
        warn_unknown(entry, {"key", "name", "order", "tracks"}, report, s_path, context);
        const auto series_name = text_member(entry, "name", true, report, s_path, context, kMaxSeriesNameLength);
        if (!key || !series_name) {
            report.error(&entry, s_path, context + " is left out");
            continue;
        }
        if (!valid_key(*key)) {
            report.error(entry.find("key"), s_path + ".key",
                         context + ": \"key\" must be lower-case letters, digits, '.', '_' or '-'");
            continue;
        }
        if (!series_keys.insert(*key).second) {
            report.error(&entry, s_path + ".key", context + " appears twice; the second is left out");
            continue;
        }
        Series series;
        series.key = *key;
        series.name = *series_name;
        if (const Value *order = member(entry, "order", Value::Kind::Number, false, report, s_path, context)) {
            const auto value = whole_number(*order);
            if (!value || *value < -1'000'000 || *value > 1'000'000)
                report.error(order, s_path + ".order",
                             context + ": \"order\" must be a whole number from -1000000 to 1000000; 0 is used");
            else
                series.order = *value;
        }
        const Value *tracks = member(entry, "tracks", Value::Kind::Array, true, report, s_path, context);
        if (tracks == nullptr) continue;
        for (std::size_t t = 0; t < tracks->items.size(); ++t) {
            const Value &item = tracks->items[t];
            const std::string t_path = s_path + ".tracks." + std::to_string(t);
            if (!item.is(Value::Kind::Object)) {
                report.error(&item, t_path, context + ": each track must be an object");
                continue;
            }
            const auto track_key = text_member(item, "key", true, report, t_path, context + ", a track");
            const std::string track_context = "track \"" + track_key.value_or("?") + "\"";
            warn_unknown(item, {"key", "title", "file", "loop"}, report, t_path, track_context);
            const auto title = text_member(item, "title", true, report, t_path, track_context);
            const Value *file = member(item, "file", Value::Kind::String, true, report, t_path, track_context);
            if (!track_key || !title || file == nullptr) {
                report.error(&item, t_path, track_context + " is left out");
                continue;
            }
            if (!valid_key(*track_key)) {
                report.error(item.find("key"), t_path + ".key",
                             track_context + ": \"key\" must be lower-case letters, digits, '.', '_' or '-'");
                continue;
            }
            if (!track_keys.insert(*track_key).second) {
                report.error(&item, t_path + ".key", track_context + " appears twice in the mod; the second is left out");
                continue;
            }
            Track track;
            const auto full = track_file(*file, folder, report, t_path + ".file", track_context);
            if (!full || !read_loop(item.find("loop"), track.loop, report, t_path + ".loop", track_context) ||
                !read_stream_header(*file, *full, track.loop, report, t_path, track_context)) {
                report.error(&item, t_path, track_context + " is left out");
                continue;
            }
            track.key = *track_key;
            track.title = *title;
            track.file_text = file->text;
            track.file = *full;
            series.tracks.push_back(std::move(track));
        }
        if (series.tracks.empty()) {
            report.error(&entry, s_path, context + " has no track that can be played; it is left out");
            continue;
        }
        result.series.push_back(std::move(series));
    }
}

} // namespace

std::string utf8(const std::filesystem::path &path) {
    const std::u8string text = path.u8string();
    return {text.begin(), text.end()};
}

std::filesystem::path path_from_utf8(std::string_view text) {
    return std::filesystem::path(std::u8string(text.begin(), text.end()));
}

bool valid_key(std::string_view key) {
    if (key.empty() || key.size() > kMaxKeyLength) return false;
    if (!std::isalnum(static_cast<unsigned char>(key.front()))) return false;
    return std::all_of(key.begin(), key.end(), [](char c) {
        return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
    });
}

std::uint32_t id_number(std::string_view text) {
    std::uint32_t hash = 0x811C9DC5u;
    for (const char c : text) {
        hash ^= static_cast<std::uint8_t>(c);
        hash *= 0x01000193u;
    }
    return hash;
}

bool is_audio_extension(std::string_view extension) {
    static constexpr std::array<std::string_view, 9> kExtensions = {".mp3", ".ogg",   ".oga",   ".opus", ".flac",
                                                                     ".wav", ".brstm", ".bcstm", ".bfstm"};
    return std::find(kExtensions.begin(), kExtensions.end(), extension) != kExtensions.end();
}

Result read_text(std::string_view text, const std::filesystem::path &folder) {
    Result result;
    Report report(result.problems);
    std::string parse_error;
    const auto document = json::parse(text, parse_error);
    if (!document) {
        report.error(nullptr, "mod", "not valid JSON: " + parse_error);
        return result;
    }
    if (!document->is(Value::Kind::Object)) {
        report.error(&*document, "mod", "the manifest must be an object");
        return result;
    }
    check(*document, folder, result.manifest, report);
    return result;
}

Result read(const std::filesystem::path &folder) {
    const std::filesystem::path manifest = folder / "mod.json";
    std::error_code ec;
    if (!std::filesystem::is_regular_file(manifest, ec)) {
        Result result;
        result.problems.push_back({Level::Error, "mod", "there is no mod.json; the folder is not a mod", 0});
        return result;
    }
    if (std::filesystem::file_size(manifest, ec) > kMaxManifestBytes) {
        Result result;
        result.problems.push_back({Level::Error, "mod", "larger than 1 MiB", 0});
        return result;
    }
    std::ifstream in(manifest, std::ios::binary);
    const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return read_text(text, folder);
}

} // namespace d012::mod_manifest
