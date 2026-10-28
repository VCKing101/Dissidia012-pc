// The mods folder and its mods; see mods_folder.hpp. MIT licence; see
// port/README.md.

#include "mods_folder.hpp"

#include "d012_json.hpp"
#include "d012_mod_manifest.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iterator>

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

std::optional<std::filesystem::path> env_path(const char *name) {
    const char *value = std::getenv(name);
    if (value == nullptr || *value == '\0') return std::nullopt;
    return manifest::path_from_utf8(value);
}

std::filesystem::path program_folder() {
    const char *base = SDL_GetBasePath();
    return base != nullptr ? manifest::path_from_utf8(base) : std::filesystem::path();
}

// build/port/data of the checkout `folder` is in, as play_d012.ps1 and
// setup.ps1 use it, when there is one.
std::optional<std::filesystem::path> checkout_data(std::filesystem::path folder) {
    std::error_code ec;
    for (int up = 0; up < 8 && !folder.empty(); ++up) {
        if (std::filesystem::is_regular_file(folder / "port" / "CMakeLists.txt", ec) &&
            std::filesystem::is_directory(folder / "build" / "port" / "data", ec))
            return folder / "build" / "port" / "data";
        if (folder == folder.parent_path()) break;
        folder = folder.parent_path();
    }
    return std::nullopt;
}

} // namespace

ModsFolder find_mods_folder(const std::optional<std::filesystem::path> &chosen) {
    if (chosen) return {*chosen, "the command line"};
    if (auto mods = env_path("D012_MODS_DIR")) return {*mods, "D012_MODS_DIR"};
    if (auto data = env_path("D012_DATA_DIR")) return {*data / "mods", "D012_DATA_DIR"};
    const std::filesystem::path program = program_folder();
    std::error_code ec;
#if defined(D012_INSTALLER_BUILD)
    if (!program.empty()) return {program / "data" / "mods", "the game's data folder"};
#else
    if (!program.empty() && std::filesystem::is_regular_file(program / "game" / "EBOOT.ELF", ec))
        return {program / "data" / "mods", "the game's data folder"};
    if (auto data = checkout_data(program)) return {*data / "mods", "this checkout's build/port/data"};
#endif
    // The game's per-user data folder (PortableKit host/install/user_data.cpp,
    // with the profile's names in host/d012_profile.cpp).
    if (char *pref = SDL_GetPrefPath("Decomp-Dissidia", "D012Native")) {
        std::filesystem::path data = manifest::path_from_utf8(pref);
        SDL_free(pref);
        return {data / "mods", "the game's data folder"};
    }
    return {std::filesystem::current_path(ec) / "mods", "the current folder"};
}

std::vector<ModSummary> list_mods(const std::filesystem::path &mods) {
    std::vector<ModSummary> result;
    std::error_code ec;
    for (const auto &entry : std::filesystem::directory_iterator(mods, ec)) {
        if (!entry.is_directory(ec)) continue;
        ModSummary summary;
        summary.folder = manifest::utf8(entry.path().filename());
        if (summary.folder.empty() || summary.folder.front() == '.') continue;
        const std::filesystem::path file = entry.path() / "mod.json";
        if (!std::filesystem::is_regular_file(file, ec)) {
            summary.error = "there is no mod.json";
        } else if (std::filesystem::file_size(file, ec) > manifest::kMaxManifestBytes) {
            summary.error = "mod.json is larger than 1 MiB";
        } else {
            std::ifstream in(file, std::ios::binary);
            const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            std::string error;
            const auto document = json::parse(text, error);
            if (!document) {
                summary.error = "mod.json is not valid JSON: " + error;
            } else if (!document->is(json::Value::Kind::Object)) {
                summary.error = "mod.json is not an object";
            } else {
                const auto text_of = [&](const char *name) {
                    const json::Value *value = document->find(name);
                    return value != nullptr && value->is(json::Value::Kind::String) ? value->text : std::string();
                };
                summary.id = text_of("id");
                summary.name = text_of("name");
                summary.version = text_of("version");
                const json::Value *music = document->find("music");
                const json::Value *series = music != nullptr ? music->find("series") : nullptr;
                if (series != nullptr && series->is(json::Value::Kind::Array) && !summary.id.empty()) {
                    for (const json::Value &s : series->items) {
                        const json::Value *tracks = s.find("tracks");
                        if (tracks == nullptr || !tracks->is(json::Value::Kind::Array)) continue;
                        for (const json::Value &t : tracks->items) {
                            const json::Value *key = t.find("key");
                            if (key == nullptr || !key->is(json::Value::Kind::String)) continue;
                            const std::string id = summary.id + ":" + key->text;
                            summary.tracks[manifest::id_number(id)] = id;
                        }
                    }
                }
            }
        }
        result.push_back(std::move(summary));
    }
    std::sort(result.begin(), result.end(), [](const ModSummary &a, const ModSummary &b) { return a.folder < b.folder; });
    return result;
}

} // namespace d012::bgm_maker
