#pragma once

// "Try in game": the game (D012Native, beside this program or around the
// mods folder) started as a process of its own, its battle music selector on a mod's track
// (D012_TEST_BGM_START), reading the mods folder the tool works on. Never
// while the game is running already: it reads its mods when it starts.
// MIT licence; see port/README.md.

#include "mods_folder.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

struct SDL_Process;

namespace d012::bgm_maker {

// D012Native beside this program, else the one whose data/mods `mods` is;
// empty when there is none.
[[nodiscard]] std::filesystem::path game_program(const ModsFolder &mods);

// Whether the mods are those of the Windows version (D012Native.exe beside
// their data folder) seen from Linux, where it runs under Proton: Steam
// starts it, not this program.
[[nodiscard]] bool windows_copy_around(const ModsFolder &mods);

// Whether a D012Native is running (started from here or not).
[[nodiscard]] bool game_running();

// The variables the game is started with, in order.
[[nodiscard]] std::vector<std::pair<std::string, std::string>> game_environment(const ModsFolder &mods,
                                                                                const std::string &track_id);

class GameLauncher {
public:
    GameLauncher() = default;
    ~GameLauncher();  // the game keeps running
    GameLauncher(const GameLauncher &) = delete;
    GameLauncher &operator=(const GameLauncher &) = delete;

    // Starts the game on `track_id` ("<mod id>:<track key>"); false with
    // `error` when it cannot.
    bool start(const ModsFolder &mods, const std::string &track_id, std::string &error);
    // Whether the game started from here is still running.
    [[nodiscard]] bool running();

private:
    SDL_Process *process_{};
};

} // namespace d012::bgm_maker
