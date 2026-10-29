// The Battle BGM Maker without its window; see cli.hpp. MIT licence; see
// port/README.md.

#include "cli.hpp"

#include "audio_file.hpp"
#include "importer.hpp"
#include "measurer.hpp"
#include "names.hpp"
#include "saving.hpp"

#include "d012_json.hpp"
#include "d012_mod_manifest.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <vector>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

// A program without a console of its own: run from one, it writes to that
// one (output sent to a file or a pipe needs nothing).
void attach_console() {
#if defined(_WIN32)
    const HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out != nullptr && out != INVALID_HANDLE_VALUE) return;
    if (!AttachConsole(ATTACH_PARENT_PROCESS)) return;
    std::FILE *ignored = nullptr;
    freopen_s(&ignored, "CONOUT$", "w", stdout);
    freopen_s(&ignored, "CONOUT$", "w", stderr);
    std::cout.clear();
    std::cerr.clear();
#endif
}

struct Mod {
    std::filesystem::path folder;
    ModModel model;
    std::string text;  // mod.json as on disc; empty when there is none
};

bool load(const std::filesystem::path &folder, bool may_be_new, Mod &mod) {
    mod.folder = folder;
    const std::filesystem::path file = folder / "mod.json";
    std::error_code ec;
    if (!std::filesystem::is_regular_file(file, ec)) {
        if (!may_be_new) {
            std::cerr << manifest::utf8(folder) << " has no mod.json\n";
            return false;
        }
        // As the window does for a folder without a manifest.
        const std::string name = manifest::utf8(folder.filename());
        if (manifest::valid_key(name)) mod.model.id = name;
        mod.model.name = name;
        mod.model.version = "1.0.0";
        return true;
    }
    std::ifstream in(file, std::ios::binary);
    mod.text.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    std::string error;
    const auto document = json::parse(mod.text, error);
    if (!document) {
        std::cerr << manifest::utf8(file) << " is not valid JSON: " << error << "\n";
        return false;
    }
    mod.model = model_from_document(*document);
    return true;
}

std::vector<ModSummary> others_of(const std::filesystem::path &folder) {
    std::vector<ModSummary> others;
    const std::string name = manifest::utf8(folder.filename());
    for (ModSummary &summary : list_mods(folder.parent_path()))
        if (summary.folder != name) others.push_back(std::move(summary));
    return others;
}

// Every track's file read to the end, as Check & save does.
std::map<std::string, AudioInfo> measure(const Mod &mod) {
    std::map<std::string, AudioInfo> infos;
    for (const SeriesModel &series : mod.model.series)
        for (const TrackModel &track : series.tracks) {
            if (track.file.empty() || infos.contains(track.file)) continue;
            const std::filesystem::path full = mod.folder / manifest::path_from_utf8(track.file);
            std::error_code ec;
            AudioInfo info;
            if (std::filesystem::is_regular_file(full, ec)) info = measure_audio(full);
            else info.error = "the file is not in the mod's folder";
            infos.emplace(track.file, std::move(info));
        }
    return infos;
}

FactsOf facts_from(const std::map<std::string, AudioInfo> &infos) {
    std::map<std::string, FileFacts> facts;
    for (const auto &[file, info] : infos) facts.emplace(file, facts_of(info));
    return [facts = std::move(facts)](const std::string &file) -> std::optional<FileFacts> {
        const auto found = facts.find(file);
        return found != facts.end() ? std::optional(found->second) : std::nullopt;
    };
}

void print_problems(const std::vector<Problem> &problems) {
    for (const Problem &p : problems) {
        const char *level = p.level == Problem::Level::Error     ? "error"
                            : p.level == Problem::Level::Warning ? "warning"
                                                                 : "note";
        std::cout << level << ": " << p.path;
        if (p.line > 0) std::cout << ", line " << p.line;
        std::cout << ": " << p.message << "\n";
    }
}

void print_tracks(const Mod &mod, const std::map<std::string, AudioInfo> &infos) {
    for (const SeriesModel &series : mod.model.series)
        for (const TrackModel &track : series.tracks) {
            const auto found = infos.find(track.file);
            std::cout << "track " << track.key << ": " << track.file;
            if (found == infos.end() || !found->second.error.empty())
                std::cout << ", cannot be read" << (found != infos.end() ? ": " + found->second.error : std::string());
            else
                std::cout << ", " << found->second.samples.value_or(0) << " samples at " << found->second.decoded_rate
                          << " Hz (" << describe(found->second) << ")";
            std::cout << "\n";
        }
}

// Saves `mod` (checked first); prints the problems and what was done.
int save(Mod &mod, const std::vector<std::filesystem::path> &added) {
    const auto infos = measure(mod);
    SaveRequest request;
    request.mods = mod.folder.parent_path();
    request.folder = manifest::utf8(mod.folder.filename());
    request.model = mod.model;
    request.facts = facts_from(infos);
    request.others = others_of(mod.folder);
    request.added = added;
    const SaveResult result = save_mod(request);
    print_problems(result.problems);
    if (!result.error.empty()) {
        std::cerr << "not saved: " << result.error << "\n";
        return 1;
    }
    if (!result.saved) {
        std::cerr << "not saved: the mod has problems\n";
        return 1;
    }
    std::cout << (result.unchanged ? "saved (no change): " : "saved: ") << manifest::utf8(mod.folder / "mod.json");
    if (!result.backup.empty()) std::cout << " (the old one is in " << result.backup << ")";
    std::cout << "\n";
    return 0;
}

int check(const std::filesystem::path &folder) {
    Mod mod;
    if (!load(folder, false, mod)) return 1;
    const auto infos = measure(mod);
    const std::vector<Problem> problems = check_text(mod.text, mod.model.id, folder, others_of(folder), facts_from(infos));
    print_problems(problems);
    print_tracks(mod, infos);
    return has_errors(problems) ? 1 : 0;
}

int import(const std::filesystem::path &folder, const std::string &series_key,
           const std::vector<std::filesystem::path> &files) {
    if (!manifest::valid_key(series_key)) {
        std::cerr << "\"" << series_key << "\" is not a series key (lower-case letters, digits, . _ -)\n";
        return 2;
    }
    Mod mod;
    if (!load(folder, true, mod)) return 1;
    std::error_code ec;
    std::filesystem::create_directories(folder, ec);
    int series = -1;
    for (int s = 0; s < static_cast<int>(mod.model.series.size()); ++s)
        if (mod.model.series[s].key == series_key) series = s;
    if (series < 0) {
        SeriesModel made;
        made.key = series_key;
        made.name = names::upper_case(series_key);
        for (const SeriesModel &other : mod.model.series) made.order = std::max(made.order, other.order + 10);
        mod.model.series.push_back(std::move(made));
        series = static_cast<int>(mod.model.series.size()) - 1;
    }
    Importer importer;
    importer.add(files, folder, series, 0);
    std::vector<ImportResult> results;
    while (results.size() < files.size()) {
        for (ImportResult &result : importer.take()) results.push_back(std::move(result));
        if (results.size() < files.size()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    const std::vector<ModSummary> others = others_of(folder);
    std::vector<std::filesystem::path> added;
    int status = 0;
    for (const ImportResult &result : results) {
        if (!result.error.empty()) {
            std::cerr << "not added: " << result.error << "\n";
            status = 1;
            continue;
        }
        added.push_back(result.copy);
        // As the window: a key unique in the mod, its number too (against other mods' as well).
        std::vector<std::string> taken;
        std::set<std::uint32_t> numbers;
        for (const SeriesModel &s : mod.model.series)
            for (const TrackModel &t : s.tracks) {
                taken.push_back(t.key);
                if (manifest::valid_key(mod.model.id)) numbers.insert(manifest::id_number(mod.model.id + ":" + t.key));
            }
        for (const ModSummary &other : others)
            for (const auto &[number, id] : other.tracks) numbers.insert(number);
        TrackModel track;
        track.title = result.title;
        track.file = result.file;
        track.key = names::unique_key(names::key_from_text(track.title, "track"), taken, numbers,
                                      manifest::valid_key(mod.model.id) ? std::string_view(mod.model.id)
                                                                        : std::string_view());
        track.loop.mode = result.info.loop ? LoopModel::Mode::Auto : LoopModel::Mode::Whole;
        std::cout << "added " << track.key << ": " << track.file << " (\"" << track.title << "\")\n";
        mod.model.series[series].tracks.push_back(std::move(track));
    }
    if (status != 0) {
        for (const auto &file : added) std::filesystem::remove(file, ec);
        return status;
    }
    status = save(mod, added);
    if (status != 0)  // nothing saved: the copies go again
        for (const auto &file : added) std::filesystem::remove(file, ec);
    return status;
}

bool whole_number(const char *text, std::int64_t &value) {
    char *end = nullptr;
    value = std::strtoll(text, &end, 10);
    return end != text && *end == '\0' && value >= 0;
}

int set_loop(const std::filesystem::path &folder, const std::string &key, int count, char **values) {
    Mod mod;
    if (!load(folder, false, mod)) return 1;
    TrackModel *track = nullptr;
    for (SeriesModel &series : mod.model.series)
        for (TrackModel &t : series.tracks)
            if (t.key == key) track = &t;
    if (track == nullptr) {
        std::cerr << "the mod has no track \"" << key << "\"\n";
        return 2;
    }
    LoopModel loop;
    const std::string first = values[0];
    std::int64_t start = 0, end = 0;
    if (count == 1 && first == "auto") loop.mode = LoopModel::Mode::Auto;
    else if (count == 1 && first == "whole") loop.mode = LoopModel::Mode::Whole;
    else if (count == 1 && first == "none") loop.mode = LoopModel::Mode::None;
    else if (whole_number(values[0], start) && (count == 1 || (count == 2 && whole_number(values[1], end)))) {
        loop.mode = LoopModel::Mode::Custom;
        loop.start = start;
        if (count == 2) loop.end = end;
    } else {
        std::cerr << "a loop is auto, whole, none, or a start (and an end) in samples of the file\n";
        return 2;
    }
    track->loop = loop;
    return save(mod, {});
}

} // namespace

int run_command_line(int argc, char **argv) {
    if (argc < 2) return -1;
    const std::string command = argv[1];
    if (command != "--check" && command != "--import" && command != "--set-loop") return -1;
    attach_console();
    const auto usage = [] {
        std::cerr << "BgmMaker --check <mod folder>\n"
                     "BgmMaker --import <mod folder> <series key> <file>...\n"
                     "BgmMaker --set-loop <mod folder> <track key> auto|whole|none|<start> [<end>]\n";
        return 2;
    };
    if (argc < 3) return usage();
    std::filesystem::path folder = std::filesystem::absolute(manifest::path_from_utf8(argv[2])).lexically_normal();
    if (folder.filename().empty()) folder = folder.parent_path();  // given with a trailing slash
    if (command == "--check") return argc == 3 ? check(folder) : usage();
    if (command == "--import") {
        if (argc < 5) return usage();
        std::vector<std::filesystem::path> files;
        for (int i = 4; i < argc; ++i) files.push_back(std::filesystem::absolute(manifest::path_from_utf8(argv[i])));
        return import(folder, argv[3], files);
    }
    if (argc < 5 || argc > 6) return usage();
    return set_loop(folder, argv[3], argc - 4, argv + 4);
}

} // namespace d012::bgm_maker
