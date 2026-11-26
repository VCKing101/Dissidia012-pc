// The game's code compiled when the game starts, on the setup's screens: a
// Windows copy that has none (d012_game_code.cpp), a Linux copy that just
// updated itself (d012_update_install.cpp), the Android app
// (d012_device_compile.cpp).
//
// MIT licence; see port/README.md.

#pragma once

#include <functional>
#include <string>

namespace portablekit::install {
class InstallerUi;
}

namespace d012::compile_screens {

// What the compile (compile_game::run_here or run_in_app) is given. Both are
// called from its worker threads too.
struct Hooks {
    // Its messages: the last one is the reason when it fails.
    std::function<void(const std::string &message, unsigned percent)> listener;
    // True once the player cancelled: the compile stops between parts.
    std::function<bool()> stop;
};

enum class Outcome { Done, Failed, Cancelled };

struct Result {
    Outcome outcome = Outcome::Failed;
    std::string message;  // with Failed: why
};

// Runs `compile` under `title` on `given` (the first start's setup screens) or
// on screens of its own when it is null. The compile's progress shows as one
// stage: the screens time what is left from when the stage's text last
// changed, and its messages count parts. Cancel, Esc (Android's Back) and
// closing the window stop it; the parts already made stay in the cache for
// the next start. `compile` returns the compile's code (0: done) and may first
// show steps of its own with ui->progress (ui is null without screens). An
// exception from it fails with its text. The screen stays on throughout.
Result run(portablekit::install::InstallerUi *given, const std::string &title,
           const std::function<int(portablekit::install::InstallerUi *ui, const Hooks &hooks)> &compile);

} // namespace d012::compile_screens
