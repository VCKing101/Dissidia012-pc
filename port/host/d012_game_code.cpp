// The installer build (D012_INSTALLER_BUILD): a program that carries none of
// the game's code. The installer recompiles it on the player's PC from their
// own disc and puts it beside the program:
//
//   <folder>/game-code/main.dll        the main executable's corpus, a corpus
//                                      library (PortableKit's
//                                      host/corpus_library.hpp)
//   <folder>/game-code/overlays/*.dll  the overlay libraries, found through
//                                      D012_OVERLAY_DIR (host/d012_portable.cpp)
//
// On Linux they are main.so and overlays/*.so.
//
// In the Android app the folder is game-code/ in the app's own storage,
// where the phone compiles the game's code (host/d012_device_compile.cpp).
//
// D012_GAME_CODE_DIR names another folder. PortableKit calls
// register_generated_functions() where a port's build would have linked the
// corpus (portablekit_add_game(... NO_CORPUS)).
//
// On Windows a copy without game-code/main.dll makes it itself before the
// game's code loads (prepare_at_start, the profile's prepare_game): the
// release's zip unpacked and started by itself, which no setup or launcher
// compiled, or a copy whose setup compile failed. The first start's own setup
// screens ask for the disc and then compile, as the setup does; a copy whose
// disc is set up already compiles on those screens at its next start.
//
// MIT licence; see port/README.md.

#include "app_paths.hpp"
#include "corpus_library.hpp"
#include "install/installer.hpp"
#include "install/user_data.hpp"
#include "profile.hpp"

#include "psprecomp/corpus_abi.hpp"

#include <SDL3/SDL.h>

#if defined(_WIN32) && !defined(PORTABLEKIT_ANDROID_APP) && defined(PORTABLEKIT_HAS_RENDERER)
#include "ui/layer.hpp"
#endif

#include <atomic>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>

#if defined(_WIN32) && !defined(PORTABLEKIT_ANDROID_APP)
namespace d012::compile_game {
// host/d012_compile_game.cpp: --compile-game's steps in this process.
int run_here(bool check_only, std::function<void(const std::string &, unsigned)> listener,
             std::function<bool()> stop);
}
#endif

namespace d012::game_code {

namespace {

const char *game_code_override() {
    const char *dir = std::getenv("D012_GAME_CODE_DIR");
    return dir != nullptr && *dir != '\0' ? dir : nullptr;
}

} // namespace

std::filesystem::path directory() {
    if (const char *dir = game_code_override())
        return std::filesystem::path(std::u8string(dir, dir + std::char_traits<char>::length(dir)));
#if defined(PORTABLEKIT_ANDROID_APP)
    return portablekit::install::user_data_directory() / "game-code";
#else
    return portablekit::executable_directory() / "game-code";
#endif
}

namespace {

void say(const std::string &message) {
    std::cerr << "[game-code] " << message << "\n";
    // The program is started from a shortcut, with no console to read.
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Dissidia 012 (PC port)", message.c_str(), nullptr);
}

} // namespace

#if defined(_WIN32) && !defined(PORTABLEKIT_ANDROID_APP)

bool prepare_at_start(portablekit::install::InstallerUi *given, const std::filesystem::path &,
                      const std::filesystem::path &) {
    // Another folder (a developer's test) is not one --compile-game writes.
    if (game_code_override() != nullptr) return true;
    std::error_code ec;
    if (std::filesystem::is_regular_file(directory() / "main.dll", ec)) return true;
    std::cout << "[game-code] " << (directory() / "main.dll").string()
              << " is missing: compiling the game's code from the disc" << std::endl;

    // The setup's screens when the first start's setup asks, or our own.
    std::unique_ptr<portablekit::install::InstallerUi> own;
    if (given == nullptr) own = portablekit::install::make_installer_ui();
    portablekit::install::InstallerUi *ui = given != nullptr ? given : own.get();
    std::string failure;
    std::atomic<bool> cancelled{false};
    int code = 1;
    // The screen stays on: the work takes minutes.
    SDL_DisableScreenSaver();
    const auto work = [&] {
        // Called from the compile's worker threads too: it must not throw.
        // The screens' Cancel (or closing the window) throws from progress();
        // the compile is told to stop instead.
        std::mutex lock;
        code = compile_game::run_here(
            false,
            [&](const std::string &message, unsigned percent) {
                const std::lock_guard<std::mutex> guard(lock);
                failure = message;
                if (!ui || cancelled) return;
                try {
                    // One stage for the whole compile: the screens time a
                    // stage from when its text changes, and the compile's
                    // messages count parts, so its own would restart the
                    // time left at every part. The log has the messages.
                    ui->progress("Compiling the game's code for your PC", percent, 100u);
                } catch (const portablekit::install::InstallCancelled &) {
                    cancelled = true;
                } catch (...) {
                }
            },
            [&] { return cancelled.load(); });
    };
#if defined(PORTABLEKIT_HAS_RENDERER)
    // Esc is the screen's Cancel only while the interface takes input; the
    // setup's earlier screens turn that on, so our own screens must. Otherwise
    // Esc is kept for the game, which opens its menu with it once it starts.
    const bool own_screens = own != nullptr && portablekit::ui::Layer::get().attached();
    if (own_screens) portablekit::ui::Layer::get().set_interactive(true);
#endif
    try {
        if (ui) ui->run_task("Making the game's code from your disc", work);
        else work();
    } catch (const portablekit::install::InstallCancelled &) {
        cancelled = true;
    }
#if defined(PORTABLEKIT_HAS_RENDERER)
    if (own_screens) portablekit::ui::Layer::get().set_interactive(false);
#endif
    SDL_EnableScreenSaver();
    if (cancelled) {
        std::cout << "[game-code] compiling cancelled; the next start goes on from the parts already made"
                  << std::endl;
        return false;
    }
    if (code == 0) return true;
    portablekit::install::report_problem(
        "The game could not be made",
        "Making the game's code from your disc did not succeed:\n\n" + failure +
            "\n\nStart the game again to try again. The log is game-code-build.log in the game's folder.",
        false);
    return false;
}

#endif

} // namespace d012::game_code

namespace psprecomp {

void register_generated_functions(CorpusRuntime &runtime) {
#if defined(_WIN32)
    const std::filesystem::path library = d012::game_code::directory() / "main.dll";
#else
    const std::filesystem::path library = d012::game_code::directory() / "main.so";
#endif
    std::error_code ec;
    if (!std::filesystem::is_regular_file(library, ec)) {
#if defined(PORTABLEKIT_ANDROID_APP)
        // The app makes it before this (host/d012_device_compile.cpp).
        std::cerr << "[game-code] " << library.string() << " is missing" << std::endl;
        return;
#endif
        d012::game_code::say("The game's code is not compiled yet (" + library.string() +
                             " is missing).\nRun the setup again to compile it from your disc.");
        return;
    }
    std::string error;
    const auto corpus =
        portablekit::open_corpus_library(library, portablekit::game().executable_sha256, error);
    if (!corpus) {
        d012::game_code::say("The compiled game code in " + library.string() + " cannot be used: " + error +
                             "\nRun the setup again to compile it from your disc.");
        return;
    }
    corpus->register_all(runtime);
    std::cout << "[game-code] " << library.string() << std::endl;
}

} // namespace psprecomp
