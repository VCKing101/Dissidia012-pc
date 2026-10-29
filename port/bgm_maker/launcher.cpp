// Starting the game on a mod's track; see launcher.hpp. MIT licence; see
// port/README.md.

#include "launcher.hpp"

#include "d012_mod_manifest.hpp"

#include <SDL3/SDL.h>

#include <cstdlib>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <tlhelp32.h>
#endif

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

#if defined(_WIN32)
constexpr const char *kGameFile = "D012Native.exe";
#else
constexpr const char *kGameFile = "D012Native";
#endif

} // namespace

std::filesystem::path game_program() {
    const char *base = SDL_GetBasePath();
    if (base == nullptr) return {};
    const std::filesystem::path file = manifest::path_from_utf8(base) / kGameFile;
    std::error_code ec;
    return std::filesystem::is_regular_file(file, ec) ? file : std::filesystem::path();
}

bool game_running() {
#if defined(_WIN32)
    // Only looked at: the list of processes' names.
    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return false;
    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof entry;
    bool found = false;
    for (BOOL more = Process32FirstW(snapshot, &entry); more && !found; more = Process32NextW(snapshot, &entry))
        found = _wcsicmp(entry.szExeFile, L"D012Native.exe") == 0;
    CloseHandle(snapshot);
    return found;
#else
    return false;
#endif
}

std::vector<std::pair<std::string, std::string>> game_environment(const ModsFolder &mods, const std::string &track_id) {
    std::vector<std::pair<std::string, std::string>> variables;
    if (!mods.checkout.empty()) {
        // As tools/port/play_d012.ps1 starts a build in a checkout.
        const std::filesystem::path data = mods.checkout / "build" / "port" / "data";
        variables.emplace_back("D012_GAME_DIR", manifest::utf8(mods.checkout / "build" / "port" / "game"));
        const char *chosen_data = std::getenv("D012_DATA_DIR");
        if (chosen_data == nullptr || *chosen_data == '\0') {
            variables.emplace_back("D012_DATA_DIR", manifest::utf8(data));
            variables.emplace_back("D012_MEMORY_STICK_DIR", manifest::utf8(data / "ms0"));
        }
        variables.emplace_back("PSPRECOMP_NO_INTERPRETER", "1");
    }
    if (mods.tell_game) variables.emplace_back("D012_MODS_DIR", manifest::utf8(mods.path));
    variables.emplace_back("D012_TEST_BGM_START", track_id);
    return variables;
}

GameLauncher::~GameLauncher() {
    if (process_ != nullptr) SDL_DestroyProcess(process_);  // frees the handle; the game runs on
}

bool GameLauncher::running() {
    if (process_ == nullptr) return false;
    if (!SDL_WaitProcess(process_, false, nullptr)) return true;
    SDL_DestroyProcess(process_);
    process_ = nullptr;
    return false;
}

bool GameLauncher::start(const ModsFolder &mods, const std::string &track_id, std::string &error) {
    const std::filesystem::path program = game_program();
    if (program.empty()) {
        error = std::string("the game (") + kGameFile + ") is not beside the Battle BGM Maker";
        return false;
    }
    if (running() || game_running()) {
        error = "the game is running; close it first (it reads its mods when it starts)";
        return false;
    }
    SDL_Environment *environment = SDL_CreateEnvironment(true);
    if (environment == nullptr) {
        error = SDL_GetError();
        return false;
    }
    for (const auto &[name, value] : game_environment(mods, track_id))
        SDL_SetEnvironmentVariable(environment, name.c_str(), value.c_str(), true);
    const std::string file = manifest::utf8(program);
    const std::string folder = manifest::utf8(program.parent_path());
    const char *arguments[] = {file.c_str(), nullptr};
    const SDL_PropertiesID properties = SDL_CreateProperties();
    SDL_SetPointerProperty(properties, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, const_cast<char **>(arguments));
    SDL_SetPointerProperty(properties, SDL_PROP_PROCESS_CREATE_ENVIRONMENT_POINTER, environment);
    SDL_SetStringProperty(properties, SDL_PROP_PROCESS_CREATE_WORKING_DIRECTORY_STRING, folder.c_str());
    SDL_SetNumberProperty(properties, SDL_PROP_PROCESS_CREATE_STDIN_NUMBER, SDL_PROCESS_STDIO_NULL);
    SDL_SetNumberProperty(properties, SDL_PROP_PROCESS_CREATE_STDOUT_NUMBER, SDL_PROCESS_STDIO_INHERITED);
    SDL_SetNumberProperty(properties, SDL_PROP_PROCESS_CREATE_STDERR_NUMBER, SDL_PROCESS_STDIO_INHERITED);
    process_ = SDL_CreateProcessWithProperties(properties);
    SDL_DestroyProperties(properties);
    SDL_DestroyEnvironment(environment);
    if (process_ == nullptr) {
        error = std::string("the game cannot be started (") + SDL_GetError() + ")";
        return false;
    }
    return true;
}

} // namespace d012::bgm_maker
