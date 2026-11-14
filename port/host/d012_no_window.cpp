// When the game's window cannot be made (issue 24), the setup's build tells
// the player why, in a message box, and ends. Before, it ran on with no
// window: the game waited unseen at its first dialog, the autosave notice, and
// each try left another copy of the program running in Task Manager.
//
// PortableKit calls this (GameProfile::renderer_unavailable) after the log
// has its report of what the machine has for Vulkan (gpu/vulkan_report.hpp),
// so the log a player sends says why. A developer's build runs on without a
// window, as test runs on a machine without Vulkan want;
// D012_HEADLESS_FALLBACK=1 does that in the setup's build too.
// D012_NO_RENDER=1 turns the window off on purpose, and never comes here.
//
// MIT licence; see port/README.md.

#include "d012_no_window.hpp"

#include "d012_no_window_rules.hpp"
#include "d012_portable.hpp"

#include <SDL3/SDL.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace d012 {

void renderer_unavailable(const std::string &why) {
#if defined(D012_INSTALLER_BUILD)
    if (no_window_rules::keep_running(std::getenv("D012_HEADLESS_FALLBACK"))) {
        std::cout << "[window] none; running on without one (D012_HEADLESS_FALLBACK)" << std::endl;
        return;
    }
#if defined(_WIN32)
    constexpr auto system = no_window_rules::System::Windows;
#elif defined(__linux__)
    constexpr auto system = no_window_rules::System::Linux;
#else
    constexpr auto system = no_window_rules::System::Other;
#endif
    const std::u8string log = portable::log_file().u8string();
    const std::string text = no_window_rules::message(why, system, std::string(log.begin(), log.end()));
    std::cout << "[window] none; saying so and quitting (D012_HEADLESS_FALLBACK=1 runs on without one)" << std::endl;
    if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Dissidia 012 (PC port)", text.c_str(), nullptr))
        std::cout << "[window] the message could not be shown either: " << SDL_GetError() << std::endl;
    std::fflush(stdout);
    std::fflush(stderr);
    // Not exit(): the disc's read-through and other threads are running, and
    // a half-started system has nothing worth taking down in order.
    std::_Exit(1);
#else
    (void)why;
    std::cout << "[window] none; a developer's build runs on without one" << std::endl;
#endif
}

} // namespace d012
