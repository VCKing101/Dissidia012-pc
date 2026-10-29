// Saving a mod; see saving.hpp. MIT licence; see port/README.md.

#include "saving.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iterator>
#include <set>

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

std::optional<std::int64_t> samples_from_facts(const std::string &file, const void *context) {
    const auto &facts = *static_cast<const FactsOf *>(context);
    if (!facts) return std::nullopt;
    const auto known = facts(file);
    return known && known->error.empty() ? known->samples : std::nullopt;
}

std::optional<std::string> read_file(const std::filesystem::path &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::nullopt;
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

bool write_file(const std::filesystem::path &path, const std::string &text) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    out.write(text.data(), static_cast<std::streamsize>(text.size()));
    out.close();
    return static_cast<bool>(out);
}

std::string stamp_now() {
    const std::time_t now = std::time(nullptr);
    std::tm local{};
#if defined(_WIN32)
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    char text[32];
    std::strftime(text, sizeof text, "%Y%m%d-%H%M%S", &local);
    return text;
}

// The old mod.json into .backups/mod-<date>-<time>.json; only the last
// kKeptBackups are kept. Its path relative to the mod, or empty on failure.
std::string back_up(const std::filesystem::path &mod, const std::string &old, std::string &error) {
    const std::filesystem::path folder = mod / ".backups";
    std::error_code ec;
    std::filesystem::create_directories(folder, ec);
    const std::string stamp = stamp_now();
    std::string name = "mod-" + stamp + ".json";
    for (int counter = 2; std::filesystem::exists(folder / name, ec); ++counter)
        name = "mod-" + stamp + "-" + std::to_string(counter) + ".json";
    if (!write_file(folder / name, old)) {
        error = "the old mod.json cannot be kept in .backups (" + manifest::utf8(folder / name) + ")";
        return {};
    }
    std::vector<std::string> kept;
    for (const auto &entry : std::filesystem::directory_iterator(folder, ec)) {
        const std::string file = manifest::utf8(entry.path().filename());
        if (file.starts_with("mod-") && file.ends_with(".json")) kept.push_back(file);
    }
    // Oldest first: by time, then by the counter of saves in the same second
    // ("mod-<stamp>.json", then "-2", "-3"... "-10").
    const auto order = [](const std::string &file) {
        const std::string stem = file.substr(4u, file.size() - 9u);
        const std::size_t dash = stem.find('-', 9u);
        const std::string stamp = dash == std::string::npos ? stem : stem.substr(0u, dash);
        const long counter = dash == std::string::npos ? 1 : std::strtol(stem.c_str() + dash + 1, nullptr, 10);
        return std::pair(stamp, counter);
    };
    std::sort(kept.begin(), kept.end(), [&](const std::string &a, const std::string &b) { return order(a) < order(b); });
    for (std::size_t i = 0; i + kKeptBackups < kept.size(); ++i)
        std::filesystem::remove(folder / manifest::path_from_utf8(kept[i]), ec);
    return ".backups/" + name;
}

// A new mod's folder, with the files that waited in `staging` moved in.
bool make_folder(const std::filesystem::path &target, const std::filesystem::path &staging, std::string &error) {
    std::error_code ec;
    std::filesystem::create_directories(target.parent_path(), ec);
    if (std::filesystem::exists(target, ec)) {
        error = "the mods folder already has \"" + manifest::utf8(target.filename()) + "\"";
        return false;
    }
    if (!staging.empty() && std::filesystem::is_directory(staging, ec)) {
        std::filesystem::rename(staging, target, ec);
        if (ec) {
            // Another drive: copied, then the staging folder goes.
            ec.clear();
            std::filesystem::copy(staging, target, std::filesystem::copy_options::recursive, ec);
            if (ec) {
                error = "the mod's files cannot be copied into " + manifest::utf8(target) + " (" + ec.message() + ")";
                std::filesystem::remove_all(target, ec);
                return false;
            }
            std::filesystem::remove_all(staging, ec);
        }
    }
    std::filesystem::create_directories(target / "music", ec);
    if (ec) {
        error = "the folder " + manifest::utf8(target) + " cannot be made (" + ec.message() + ")";
        return false;
    }
    return true;
}

} // namespace

bool has_errors(const std::vector<Problem> &problems) {
    return std::any_of(problems.begin(), problems.end(),
                       [](const Problem &p) { return p.level == Problem::Level::Error; });
}

SaveResult save_mod(const SaveRequest &request) {
    SaveResult result;
    const std::filesystem::path target = request.mods / manifest::path_from_utf8(request.folder);
    const std::filesystem::path files = request.is_new ? request.staging : target;
    result.text = manifest_text(request.model, samples_from_facts, &request.facts);
    result.problems = check_mod(request.model, files, request.others, request.facts);
    if (request.is_new) {
        if (const std::string why = folder_name_problem(request.folder, request.mods); !why.empty())
            result.problems.push_back({Problem::Level::Error, "mod.folder", why, 0});
    }
    if (has_errors(result.problems)) return result;

    if (request.is_new && !make_folder(target, request.staging, result.error)) return result;
    const std::filesystem::path file = target / "mod.json";
    std::error_code ec;
    const auto old = std::filesystem::is_regular_file(file, ec) ? read_file(file) : std::nullopt;
    if (old && *old == result.text) {
        result.unchanged = true;
    } else {
        if (old) {
            result.backup = back_up(target, *old, result.error);
            if (result.backup.empty()) return result;
        }
        std::filesystem::path partial = file;
        partial += ".part";
        if (!write_file(partial, result.text)) {
            result.error = manifest::utf8(partial) + " cannot be written";
            std::filesystem::remove(partial, ec);
            return result;
        }
        std::filesystem::rename(partial, file, ec);
        if (ec) {
            result.error = manifest::utf8(file) + " cannot be replaced (" + ec.message() + ")";
            std::filesystem::remove(partial, ec);
            return result;
        }
    }
    result.saved = true;

    // Copied in, then left out (a removed track): no longer needed.
    std::set<std::string> used;
    for (const SeriesModel &series : request.model.series)
        for (const TrackModel &track : series.tracks) used.insert(track.file);
    const std::filesystem::path from = request.is_new ? request.staging : target;
    for (const std::filesystem::path &added : request.added) {
        const std::u8string generic = added.lexically_relative(from).generic_u8string();
        const std::string relative(generic.begin(), generic.end());
        if (relative.empty() || relative.starts_with("..") || used.contains(relative)) continue;
        const std::filesystem::path now = target / manifest::path_from_utf8(relative);
        if (std::filesystem::remove(now, ec)) result.deleted.push_back(now);
    }
    return result;
}

} // namespace d012::bgm_maker
