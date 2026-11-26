// The game's code compiled when the game starts, on the setup's screens
// (d012_compile_screens.hpp).
//
// MIT licence; see port/README.md.

#include "d012_compile_screens.hpp"

#include "install/installer.hpp"

#include <SDL3/SDL.h>

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "ui/layer.hpp"
#endif

#include <atomic>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>

namespace d012::compile_screens {

namespace {

namespace install = portablekit::install;

// The heading under the title while the compile runs.
constexpr const char *kStage = "Compiling the game's code";
// What Cancel does, under it: the parts made stay in the cache.
constexpr const char *kCancelNote = "Cancelling keeps the parts made so far: the next start goes on from them.";

} // namespace

Result run(install::InstallerUi *given, const std::string &title,
           const std::function<int(install::InstallerUi *ui, const Hooks &hooks)> &compile) {
    std::unique_ptr<install::InstallerUi> own;
    if (given == nullptr) own = install::make_installer_ui();
    install::InstallerUi *ui = given != nullptr ? given : own.get();

    std::mutex lock;
    std::string last;
    std::atomic<bool> cancelled{false};
    int code = 1;
    const Hooks hooks{
        [&](const std::string &message, unsigned percent) {
            // From the compile's worker threads too, so it must not throw:
            // the screens' Cancel throws from progress(), and the compile is
            // told to stop instead.
            const std::lock_guard<std::mutex> guard(lock);
            last = message;
            if (ui == nullptr || cancelled) return;
            try {
                ui->progress(kStage, percent, 100u);
            } catch (const install::InstallCancelled &) {
                cancelled = true;
            } catch (...) {
            }
        },
        [&] { return cancelled.load(); },
    };
    const auto work = [&] {
        try {
            code = compile(ui, hooks);
        } catch (const install::InstallCancelled &) {
            // From a step `compile` showed before the compile.
            cancelled = true;
        } catch (const std::exception &e) {
            const std::lock_guard<std::mutex> guard(lock);
            last = e.what();
            code = 1;
        }
    };

    // The work takes minutes.
    SDL_DisableScreenSaver();
#if defined(PORTABLEKIT_HAS_RENDERER)
    // Esc is the screens' Cancel, and the pad and the touch screen reach them,
    // only while the interface takes input. The setup's earlier screens turn
    // that on; screens of our own must, or Esc is kept for the game, which
    // opens its menu with it once it starts.
    portablekit::ui::Layer &layer = portablekit::ui::Layer::get();
    const bool own_screens = own != nullptr && layer.attached();
    if (own_screens) layer.set_interactive(true);
#endif
    try {
        if (ui != nullptr) ui->run_task(title, work, kCancelNote);
        else work();
    } catch (const install::InstallCancelled &) {
        cancelled = true;
    }
#if defined(PORTABLEKIT_HAS_RENDERER)
    if (own_screens) layer.set_interactive(false);
#endif
    SDL_EnableScreenSaver();

    // Cancelled as the last part finished: the code is made, so it is used.
    if (code == 0) return {Outcome::Done, {}};
    if (cancelled) {
        std::cout << "[game-code] compiling cancelled; the next start goes on from the parts already made"
                  << std::endl;
        return {Outcome::Cancelled, {}};
    }
    return {Outcome::Failed, last};
}

} // namespace d012::compile_screens
