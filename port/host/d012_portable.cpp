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

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

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
        const bool portable = !folder.empty() && std::filesystem::is_regular_file(folder / "game" / "EBOOT.ELF", ec);
#if defined(D012_RELEASE_BUILD)
        const bool no_interpreter = true;
#else
        const bool no_interpreter = portable;
#endif
        if (no_interpreter && !is_set(kNoInterpreter) && !is_set(kInterpreter)) set(kNoInterpreter, "1");
        if (!portable) return;

        if (!is_set(kGameDir)) set(kGameDir, utf8(folder / "game"));
        if (!is_set(kDataDir)) set(kDataDir, utf8(folder / "data"));
        const std::string data = std::getenv(kDataDir);
        std::filesystem::create_directories(std::filesystem::path(std::u8string(data.begin(), data.end())), ec);
        if (!is_set(kMemoryStickDir))
            set(kMemoryStickDir, utf8(std::filesystem::path(std::u8string(data.begin(), data.end())) / "ms0"));
#if defined(D012_INSTALLER_BUILD)
        if (!is_set(kOverlayDir)) set(kOverlayDir, utf8(d012::game_code::directory() / "overlays"));
#endif
        std::cout << "[portable] game " << std::getenv(kGameDir) << ", data " << data << ", memory stick "
                  << std::getenv(kMemoryStickDir) << ", interpreter " << (is_set(kNoInterpreter) ? "off" : "on")
                  << std::endl;
    }
};

const PortableLayout layout;

} // namespace
} // namespace d012::portable
