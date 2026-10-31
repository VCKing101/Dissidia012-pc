// Checking the mod being edited; see checks.hpp. MIT licence; see
// port/README.md.

#include "checks.hpp"

#include "d012_json.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <map>
#include <set>

namespace d012::bgm_maker {

namespace {

using json::Value;

constexpr int kHostRate = 44'100;

void check_others(std::vector<Problem> &problems, const std::string &id, const std::vector<ModSummary> &others) {
    if (id.empty()) return;
    for (const ModSummary &other : others) {
        if (other.id != id) continue;
        problems.push_back({Problem::Level::Error, "mod.id",
                            "the folder \"" + other.folder + "\" already has the id \"" + id +
                                "\"; the port would leave one of them out",
                            0});
    }
}

// m:ss.mmm (h:mm:ss.mmm past an hour), as the Python tool's format_time.
std::string time_text(std::int64_t samples, int rate) {
    const long long millis = rate > 0 ? std::llround(static_cast<double>(samples) * 1000.0 / rate) : 0;
    char text[32];
    if (millis >= 3'600'000)
        std::snprintf(text, sizeof text, "%lld:%02lld:%02lld.%03lld", millis / 3'600'000, millis / 60'000 % 60,
                      millis / 1000 % 60, millis % 1000);
    else
        std::snprintf(text, sizeof text, "%lld:%02lld.%03lld", millis / 60'000, millis / 1000 % 60, millis % 1000);
    return text;
}

// Where the port puts a loop point, in its 44,100 Hz frames (host_audio.cpp).
std::int64_t port_frames(const mod_manifest::LoopPoint &point, int rate) {
    if (point.unit == mod_manifest::LoopPoint::Unit::Microseconds)
        return std::llround(static_cast<double>(point.value) * kHostRate / 1e6);
    if (rate <= 0) return point.value;
    return std::llround(static_cast<double>(point.value) * kHostRate / rate);
}

bool is_ascii(const std::string &text) {
    return std::all_of(text.begin(), text.end(), [](char c) { return static_cast<unsigned char>(c) < 0x80u; });
}

const std::string *text_of(const Value &object, std::string_view name) {
    const Value *value = object.find(name);
    return value != nullptr && value->is(Value::Kind::String) ? &value->text : nullptr;
}

// The tool's checks on what the game would play (`read`), walking the
// manifest itself so the paths name the right entries.
void tool_checks(std::vector<Problem> &problems, const Value &document, const mod_manifest::Manifest &read,
                 const std::vector<ModSummary> &others, const FactsOf &facts) {
    if (!read.loaded) return;
    std::map<std::uint32_t, std::string> other_numbers;
    for (const ModSummary &other : others) other_numbers.insert(other.tracks.begin(), other.tracks.end());
    const Value *music = document.find("music");
    const Value *list = music != nullptr && music->is(Value::Kind::Object) ? music->find("series") : nullptr;
    if (list == nullptr || !list->is(Value::Kind::Array)) return;
    const auto error = [&](std::string path, std::string message) {
        problems.push_back({Problem::Level::Error, std::move(path), std::move(message), 0});
    };
    const auto warning = [&](std::string path, std::string message) {
        problems.push_back({Problem::Level::Warning, std::move(path), std::move(message), 0});
    };
    std::map<std::uint32_t, std::string> numbers;
    std::set<std::string> seen_series;
    std::set<std::string> seen_tracks;
    for (std::size_t s = 0; s < list->items.size(); ++s) {
        const Value &entry = list->items[s];
        const std::string *series_key = entry.is(Value::Kind::Object) ? text_of(entry, "key") : nullptr;
        // A key's first series is the one the game may play; a second is left out.
        if (series_key == nullptr || !seen_series.insert(*series_key).second) continue;
        const auto played_series = std::find_if(read.series.begin(), read.series.end(),
                                                [&](const mod_manifest::Series &x) { return x.key == *series_key; });
        if (played_series == read.series.end()) continue;  // the game leaves it out already
        const Value &tracks = *entry.find("tracks");
        const std::string s_path = "series." + std::to_string(s);
        int playable = 0;
        for (std::size_t t = 0; t < tracks.items.size(); ++t) {
            const Value &item = tracks.items[t];
            const std::string *key = item.is(Value::Kind::Object) ? text_of(item, "key") : nullptr;
            if (key == nullptr || !seen_tracks.insert(*key).second) continue;
            const auto played = std::find_if(played_series->tracks.begin(), played_series->tracks.end(),
                                             [&](const mod_manifest::Track &x) { return x.key == *key; });
            if (played == played_series->tracks.end()) continue;
            const std::string t_path = s_path + ".tracks." + std::to_string(t);
            const std::string context = "track \"" + *key + "\"";
            // Windows takes a \ between folders; the Steam Deck does not find the file.
            if (played->file_text.find('\\') != std::string::npos) {
                error(t_path + ".file", context + " \"file\" must use / between folders (a \\ only works on Windows)");
                error(t_path, context + " is left out");
                continue;
            }
            const std::string track_id = read.id + ":" + *key;
            const std::uint32_t number = mod_manifest::id_number(track_id);
            if (const auto clash = numbers.find(number); clash != numbers.end()) {
                error(t_path + ".key", context + " has the same number as " + clash->second + "; change one key");
                continue;
            }
            if (const auto clash = other_numbers.find(number); clash != other_numbers.end()) {
                error(t_path + ".key",
                      context + " has the same number as " + clash->second + " of another mod; change the key");
                continue;
            }
            numbers.emplace(number, track_id);
            if (const auto known = facts ? facts(played->file_text) : std::nullopt) {
                if (!known->error.empty()) {
                    error(t_path + ".file", context + " (" + played->file_text + ") cannot be played: " + known->error);
                    error(t_path, context + " is left out");
                    continue;
                }
                if (known->samples && known->rate > 0 && *known->samples > known->rate * kMaxTrackSeconds) {
                    error(t_path + ".file", context + " is longer than 30 minutes");
                    continue;
                }
                if (known->samples) {
                    // Only a loop the manifest gives: a header's is inside its stream.
                    const Value *loop = item.find("loop");
                    if (loop != nullptr && loop->is(Value::Kind::Object)) {
                        if (const std::string why = loop_range_problem(played->loop, known->rate, *known->samples);
                            !why.empty()) {
                            error(t_path + ".loop", context + ": " + why);
                            continue;
                        }
                    }
                    if (known->header_samples && *known->header_samples != *known->samples)
                        warning(t_path + ".file", context + ": FFmpeg decodes " + std::to_string(*known->samples) +
                                                      " samples, but the " + known->header_format + " header says " +
                                                      std::to_string(*known->header_samples));
                }
            }
            if (!is_ascii(played->title))
                warning(t_path + ".title", context + ": the game's band shows a character outside ASCII as ?");
            ++playable;
        }
        if (playable == 0)
            error(s_path, "series \"" + *series_key + "\" has no track that can be played; it is left out");
    }
}

} // namespace

std::vector<Problem> check_mod(const ModModel &model, const std::filesystem::path &folder,
                               const std::vector<ModSummary> &others, const FactsOf &facts) {
    std::vector<Problem> problems = check_text(manifest_text(model), model.id, folder, others, facts);
    // The line numbers are in a manifest that is not on disc yet; paths say where.
    for (Problem &problem : problems) problem.line = 0;
    return problems;
}

std::vector<Problem> check_text(const std::string &text, const std::string &id, const std::filesystem::path &folder,
                                const std::vector<ModSummary> &others, const FactsOf &facts) {
    mod_manifest::Result read = mod_manifest::read_text(text, folder);
    std::vector<Problem> problems = std::move(read.problems);
    // As the Python tool: the id against the other mods' once the mod's own
    // members are read, before its music.
    if (read.manifest.loaded) {
        std::vector<Problem> clashes;
        check_others(clashes, id, others);
        const auto music = std::find_if(problems.begin(), problems.end(), [](const Problem &p) {
            return p.path != "mod" && !p.path.starts_with("mod.");
        });
        problems.insert(music, clashes.begin(), clashes.end());
    }
    std::string error;
    if (const auto document = json::parse(text, error)) tool_checks(problems, *document, read.manifest, others, facts);
    if (text.size() > mod_manifest::kMaxManifestBytes)
        problems.push_back({Problem::Level::Error, "mod", "the manifest would be larger than 1 MiB", 0});
    return problems;
}

std::string loop_range_problem(const mod_manifest::Loop &loop, int rate, std::int64_t samples) {
    if (loop.mode != mod_manifest::Loop::Mode::Range || rate <= 0 || samples <= 0) return {};
    const std::int64_t frames = std::llround(static_cast<double>(samples) * kHostRate / rate);
    const std::int64_t first = port_frames(loop.start, rate);
    const std::int64_t last = loop.end ? port_frames(*loop.end, rate) - 1 : frames - 1;
    if (0 <= first && first < last && last < frames) return {};
    return "the loop is outside the file (" + std::to_string(samples) + " samples, " + time_text(samples, rate) +
           "); the port would loop the whole file instead";
}

std::string folder_name_problem(std::string_view name, const std::filesystem::path &mods) {
    if (name.empty()) return "the folder needs a name";
    if (name == "." || name == ".." || name.front() == '.') return "the folder's name may not start with '.'";
    for (const char c : name)
        if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|' ||
            static_cast<unsigned char>(c) < 0x20u)
            return "the folder's name may not contain / \\ : * ? \" < > |";
    std::error_code ec;
    if (std::filesystem::exists(mods / mod_manifest::path_from_utf8(name), ec))
        return "the mods folder already has \"" + std::string(name) + "\"";
    return {};
}

std::vector<const Problem *> problems_at(const std::vector<Problem> &problems, std::string_view path, bool below) {
    std::vector<const Problem *> found;
    for (const Problem &problem : problems) {
        const std::string_view at = problem.path;
        if (at == path || (below && at.size() > path.size() && at.starts_with(path) && at[path.size()] == '.'))
            found.push_back(&problem);
    }
    return found;
}

} // namespace d012::bgm_maker
