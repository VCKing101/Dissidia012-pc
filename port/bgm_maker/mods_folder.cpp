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
#include <set>

#if defined(__linux__)
#include <sys/stat.h>
#endif

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

std::optional<std::filesystem::path> env_path(const char *name) {
    const char *value = std::getenv(name);
    if (value == nullptr || *value == '\0') return std::nullopt;
    return manifest::path_from_utf8(value);
}

#if defined(_WIN32)
constexpr const char *kGameFile = "D012Native.exe";
#else
constexpr const char *kGameFile = "D012Native";
#endif

std::filesystem::path program_folder() {
    const char *base = SDL_GetBasePath();
    return base != nullptr ? manifest::path_from_utf8(base) : std::filesystem::path();
}

// The AppImage file this program runs from, when it does: its runtime sets
// APPIMAGE and APPDIR, the folder it is unpacked to, which holds this
// program (a program started from one may have them too).
std::filesystem::path appimage_file() {
#if defined(__linux__)
    const auto image = env_path("APPIMAGE");
    const auto appdir = env_path("APPDIR");
    if (!image || !appdir) return {};
    const std::string base = manifest::utf8(program_folder().lexically_normal());
    std::string inside = manifest::utf8(appdir->lexically_normal());
    if (!inside.empty() && inside.back() != '/') inside += '/';
    if (base.rfind(inside, 0) != 0) return {};
    return *image;
#else
    return {};
#endif
}

// The file the folder opened last time is kept in.
std::filesystem::path remembered_file() {
    char *pref = SDL_GetPrefPath("Decomp-Dissidia", "BgmMaker");
    if (pref == nullptr) return {};
    std::filesystem::path file = manifest::path_from_utf8(pref) / "last-mods-folder.txt";
    SDL_free(pref);
    return file;
}

std::optional<std::filesystem::path> remembered_folder() {
    const std::filesystem::path file = remembered_file();
    if (file.empty()) return std::nullopt;
    std::ifstream in(file, std::ios::binary);
    std::string text;
    if (!std::getline(in, text) || text.empty()) return std::nullopt;
    if (text.back() == '\r') text.pop_back();
    std::filesystem::path folder = manifest::path_from_utf8(text);
    std::error_code ec;
    if (!std::filesystem::is_directory(folder, ec)) return std::nullopt;
    return folder;
}

#if defined(__linux__)
std::filesystem::path home_folder() {
    auto home = env_path("HOME");
    return home ? *home : std::filesystem::path();
}

std::filesystem::path data_home() {
    if (auto data = env_path("XDG_DATA_HOME")) return *data;
    const std::filesystem::path home = home_folder();
    return home.empty() ? home : home / ".local" / "share";
}

// The folder the Linux setup's menu entry starts the game in (its Path= line;
// tools/port/linux_setup/AppRun writes it).
std::optional<std::filesystem::path> setup_menu_folder() {
    const std::filesystem::path data = data_home();
    if (data.empty()) return std::nullopt;
    std::ifstream in(data / "applications" / "dissidia012-pc.desktop", std::ios::binary);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.rfind("Path=", 0) == 0) return manifest::path_from_utf8(line.substr(5));
    }
    return std::nullopt;
}

// The folders of the programs named D012Native or D012Native.exe in Steam's
// shortcuts.vdf files (its non-Steam games; binary VDF, where a text field is
// 0x01, its name, 0, its value, 0). Only read.
std::vector<std::filesystem::path> steam_shortcut_folders() {
    std::vector<std::filesystem::path> roots;
    const std::filesystem::path home = home_folder();
    if (!data_home().empty()) roots.push_back(data_home() / "Steam" / "userdata");
    if (!home.empty()) {
        roots.push_back(home / ".steam" / "steam" / "userdata");
        roots.push_back(home / ".var" / "app" / "com.valvesoftware.Steam" / ".local" / "share" / "Steam" / "userdata");
    }
    std::vector<std::filesystem::path> folders;
    std::error_code ec;
    for (const std::filesystem::path &root : roots) {
        for (auto user = std::filesystem::directory_iterator(root, ec); !ec && user != std::filesystem::directory_iterator();
             user.increment(ec)) {
            const std::filesystem::path file = user->path() / "config" / "shortcuts.vdf";
            std::error_code size_ec;
            const auto size = std::filesystem::file_size(file, size_ec);
            if (size_ec || size > 16u * 1024u * 1024u) continue;
            std::ifstream in(file, std::ios::binary);
            const std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            for (std::size_t at = data.find('\x01'); at != std::string::npos; at = data.find('\x01', at + 1)) {
                if (at + 5 > data.size()) break;
                std::string name = data.substr(at + 1, 4);
                for (char &c : name) c = static_cast<char>(c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c);
                if (name != std::string("exe\0", 4)) continue;
                const std::size_t end = data.find('\0', at + 5);
                if (end == std::string::npos) break;
                std::string value = data.substr(at + 5, end - at - 5);
                while (!value.empty() && (value.front() == '"' || value.front() == ' ')) value.erase(value.begin());
                while (!value.empty() && (value.back() == '"' || value.back() == ' ')) value.pop_back();
                const std::filesystem::path program = manifest::path_from_utf8(value);
                if (program.filename() == "D012Native" || program.filename() == "D012Native.exe")
                    folders.push_back(program.parent_path());
            }
        }
        ec.clear();
    }
    return folders;
}

// When the copy in `folder` was last played: the newest of its log, settings
// and saves; 0 when it has none.
std::int64_t last_played(const std::filesystem::path &folder) {
    std::int64_t newest = 0;
    const std::filesystem::path data = folder / "data";
    for (const std::filesystem::path &path : {data / "logs" / "D012Native.log", data / "settings.ini",
                                              data / "ms0" / "PSP" / "SAVEDATA"}) {
        struct stat info {};
        if (::stat(path.c_str(), &info) == 0) newest = std::max<std::int64_t>(newest, info.st_mtime);
    }
    return newest;
}
#endif

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

std::filesystem::path tool_folder() {
    const std::filesystem::path image = appimage_file();
    return image.empty() ? program_folder() : image.parent_path();
}

bool from_appimage() { return !appimage_file().empty(); }

std::filesystem::path game_in(const std::filesystem::path &folder) {
    if (folder.empty()) return {};
    const std::filesystem::path file = folder / kGameFile;
    std::error_code ec;
    return std::filesystem::is_regular_file(file, ec) ? file : std::filesystem::path();
}

std::vector<GameCopy> find_game_copies() {
    std::vector<GameCopy> copies;
#if defined(__linux__)
    std::vector<std::filesystem::path> folders;
    if (auto folder = setup_menu_folder()) folders.push_back(*folder);
    for (const std::filesystem::path &folder : steam_shortcut_folders()) folders.push_back(folder);
    std::error_code ec;
    if (const std::filesystem::path home = home_folder(); !home.empty())
        for (auto it = std::filesystem::directory_iterator(home / "Games", ec);
             !ec && it != std::filesystem::directory_iterator(); it.increment(ec))
            folders.push_back(it->path());
    std::set<std::filesystem::path> seen;
    for (const std::filesystem::path &found : folders) {
        std::error_code canonical_ec;
        std::filesystem::path folder = std::filesystem::weakly_canonical(found, canonical_ec);
        if (canonical_ec) folder = found.lexically_normal();
        if (!folder.has_filename()) folder = folder.parent_path();
        if (!seen.insert(folder).second || !std::filesystem::is_directory(folder / "data", ec)) continue;
        GameCopy copy{folder};
        if (game_in(folder).empty()) {
            if (!std::filesystem::is_regular_file(folder / "D012Native.exe", ec)) continue;
            copy.windows = true;
        }
        copy.last_played = last_played(folder);
        copies.push_back(std::move(copy));
    }
    std::stable_sort(copies.begin(), copies.end(),
                     [](const GameCopy &a, const GameCopy &b) { return a.last_played > b.last_played; });
#endif
    return copies;
}

void remember_mods_folder(const std::filesystem::path &folder) {
    const std::filesystem::path file = remembered_file();
    if (file.empty()) return;
    std::ofstream out(file, std::ios::binary | std::ios::trunc);
    out << manifest::utf8(folder) << "\n";
}

ModsFolder find_mods_folder(const std::optional<std::filesystem::path> &chosen) {
    const std::filesystem::path program = tool_folder();
    std::error_code ec;
    // A build in a checkout (not one with the game beside it) starts the game
    // as play_d012.ps1 does, whichever mods folder is used.
    std::filesystem::path checkout;
#if defined(D012_INSTALLER_BUILD)
    // Installed beside the game, or (an AppImage) put in a game's folder.
    const bool game_beside = !game_in(program).empty();
#else
    const bool game_beside = !program.empty() && std::filesystem::is_regular_file(program / "game" / "EBOOT.ELF", ec);
    if (!game_beside)
        if (auto data = checkout_data(program)) checkout = data->parent_path().parent_path().parent_path();
#endif
    const auto found = [&](std::filesystem::path path, std::string by, bool tell) {
        return ModsFolder{std::move(path), std::move(by), tell, checkout};
    };
    if (chosen) return found(*chosen, "the command line", true);
    if (auto mods = env_path("D012_MODS_DIR")) return found(*mods, "D012_MODS_DIR", false);
    if (auto data = env_path("D012_DATA_DIR")) return found(*data / "mods", "D012_DATA_DIR", false);
    if (game_beside) return found(program / "data" / "mods", "the game's data folder", false);
    if (!checkout.empty()) return found(checkout / "build" / "port" / "data" / "mods", "this checkout's build/port/data", false);
    // With no game beside it: the folder opened last time, then the copy of
    // the game played last, the window asking when there are others.
    if (auto folder = remembered_folder()) return found(*folder, "opened last time", true);
    if (const std::vector<GameCopy> copies = find_game_copies(); !copies.empty()) {
        ModsFolder mods = found(copies.front().folder / "data" / "mods",
                                copies.size() > 1 ? "the copy of the game played last" : "the game found on this computer",
                                false);
        mods.ask = copies.size() > 1;
        return mods;
    }
    // The game's per-user data folder (PortableKit host/install/user_data.cpp,
    // with the profile's names in host/d012_profile.cpp).
    if (char *pref = SDL_GetPrefPath("Decomp-Dissidia", "D012Native")) {
        std::filesystem::path data = manifest::path_from_utf8(pref);
        SDL_free(pref);
#if defined(__linux__)
        // A copy of the game on Linux keeps its data beside it, so this one
        // is only a place to start from.
        return found(data / "mods", "no copy of the game was found: Open another... picks its data/mods", false);
#else
        return found(data / "mods", "the game's data folder", false);
#endif
    }
    return found(std::filesystem::current_path(ec) / "mods", "the current folder", true);
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
