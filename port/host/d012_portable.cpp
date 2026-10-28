// The portable layout: a folder holding the program with the game and its data
// beside it, as tools/port/package_deck.py assembles it for the Steam Deck.
//
//   <folder>/D012Native.exe, its DLLs, overlays/
//   <folder>/game/EBOOT.ELF, <folder>/game/disc.iso
//   <folder>/data/           settings.ini, pipeline cache, mods/, ms0/
//
// When <folder>/game/EBOOT.ELF exists next to the running executable, each of
// these that the environment does not already set is set, before main() runs:
//
//   D012_GAME_DIR            <folder>/game
//   D012_DATA_DIR            <folder>/data
//   D012_MEMORY_STICK_DIR    <data dir>/ms0 (the data dir in effect)
//   PSPRECOMP_NO_INTERPRETER 1, unless D012_INTERPRETER is set: a packaged
//                            build runs its recompiled code only, as
//                            tools/port/play_d012.ps1 does
//   D012_OVERLAY_DIR         <folder>/game-code/overlays, in the installer
//                            build only (host/d012_game_code.cpp)
//
// The installer build always uses the layout, game/ or not: what the
// installer's `--install <iso>` makes (EBOOT.ELF, and settings.ini naming the
// player's disc image wherever it is) goes in data/ beside the saves, as
// PortableKit lays out a data directory of its own, so D012_GAME_DIR is set
// only when game/EBOOT.ELF is there. Started from a shortcut it has no console
// (it is built as a windowed program), so its output goes to
// <data>/logs/D012Native.log, the previous run's to D012Native-previous.log.
//
// A variable already set always wins, so run_d012.py, play_d012.ps1 and the
// test tools, which set all of them, behave as before. A build directory's
// bin/ has no game/ beside it, so a developer build is unchanged too. A build
// configured with PORTABLEKIT_RELEASE=ON (D012_RELEASE_BUILD) turns the
// interpreter off by default wherever it runs.
//
// It works through the environment because PortableKit reads these paths
// there (host/main.cpp, host/install/user_data.cpp) and has no hook that runs
// before it locates the game; nothing here is specific to the framework's
// internals.
//
// MIT licence; see port/README.md.

#include "app_paths.hpp"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(D012_INSTALLER_BUILD)
namespace d012::game_code {
std::filesystem::path directory();  // host/d012_game_code.cpp
}
#endif

namespace d012::portable {
namespace {

// The profile's prefix (d012_profile.cpp, env_prefix). Spelled out because
// this runs during static initialisation, before anything may ask the profile.
constexpr const char *kGameDir = "D012_GAME_DIR";
constexpr const char *kDataDir = "D012_DATA_DIR";
constexpr const char *kMemoryStickDir = "D012_MEMORY_STICK_DIR";
constexpr const char *kInterpreter = "D012_INTERPRETER";
constexpr const char *kNoInterpreter = "PSPRECOMP_NO_INTERPRETER";
constexpr const char *kOverlayDir = "D012_OVERLAY_DIR";

bool is_set(const char *name) {
    const char *value = std::getenv(name);
    return value != nullptr && *value != '\0';
}

// PortableKit reads these values as UTF-8 (install::path_from_utf8).
std::string utf8(const std::filesystem::path &path) {
    const std::u8string text = path.u8string();
    return {text.begin(), text.end()};
}

void set(const char *name, const std::string &value) {
#if defined(_WIN32)
    _putenv_s(name, value.c_str());
#else
    setenv(name, value.c_str(), 0);
#endif
}

#if defined(D012_INSTALLER_BUILD) && defined(_WIN32)
// A windowed program started from a shortcut has no standard output: keep
// what it prints, for the player's bug reports. Output a parent redirected
// (the installer, the test tools) is left where it goes.
void log_to_file_without_console(const std::filesystem::path &logs) {
    const HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out != nullptr && out != INVALID_HANDLE_VALUE) return;
    std::error_code ec;
    std::filesystem::create_directories(logs, ec);
    const std::filesystem::path log = logs / "D012Native.log";
    std::filesystem::rename(log, logs / "D012Native-previous.log", ec);
    if (std::freopen(log.string().c_str(), "w", stdout) != nullptr) std::setvbuf(stdout, nullptr, _IOLBF, 4096);
    std::freopen(log.string().c_str(), "a", stderr);
}
#endif

struct PortableLayout {
    PortableLayout() {
        try {
            apply();
        } catch (const std::exception &e) {
            std::cerr << "[portable] " << e.what() << "\n";
        }
    }

    static void apply() {
        const std::filesystem::path folder = portablekit::executable_directory();
        std::error_code ec;
        const bool game_folder = !folder.empty() && std::filesystem::is_regular_file(folder / "game" / "EBOOT.ELF", ec);
#if defined(D012_INSTALLER_BUILD)
        const bool portable = !folder.empty();
#else
        const bool portable = game_folder;
#endif
#if defined(D012_RELEASE_BUILD)
        const bool no_interpreter = true;
#else
        const bool no_interpreter = portable;
#endif
        if (no_interpreter && !is_set(kNoInterpreter) && !is_set(kInterpreter)) set(kNoInterpreter, "1");
        if (!portable) return;

        if (game_folder && !is_set(kGameDir)) set(kGameDir, utf8(folder / "game"));
        if (!is_set(kDataDir)) set(kDataDir, utf8(folder / "data"));
        const std::string data = std::getenv(kDataDir);
        std::filesystem::create_directories(std::filesystem::path(std::u8string(data.begin(), data.end())), ec);
#if defined(D012_INSTALLER_BUILD) && defined(_WIN32)
        log_to_file_without_console(std::filesystem::path(std::u8string(data.begin(), data.end())) / "logs");
#endif
        if (!is_set(kMemoryStickDir))
            set(kMemoryStickDir, utf8(std::filesystem::path(std::u8string(data.begin(), data.end())) / "ms0"));
#if defined(D012_INSTALLER_BUILD)
        if (!is_set(kOverlayDir)) set(kOverlayDir, utf8(d012::game_code::directory() / "overlays"));
#endif
        const char *game = std::getenv(kGameDir);
        std::cout << "[portable] game " << (game != nullptr ? game : "<data dir>") << ", data " << data << ", memory stick "
                  << std::getenv(kMemoryStickDir) << ", interpreter " << (is_set(kNoInterpreter) ? "off" : "on")
                  << std::endl;
    }
};

const PortableLayout layout;

} // namespace
} // namespace d012::portable
