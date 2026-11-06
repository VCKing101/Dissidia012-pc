// Circle on the title screen's menu (New Game, Load Game, Extras) asks
// "Close the game?"; Yes closes it as the port's menu's Quit does
// (docs/port/title-quit.md). On a PSP circle does nothing there.
//
// The title screen is a step of the boot sequence in the menu overlay; its
// handler (menu 0x089DF14C) asks main 0x08872088 for player 1's pad from
// menu 0x089DF330 once a frame, and only while its menu takes input: not
// while it fades in or out, and not once a row is chosen. Nothing in the menu
// overlay can be overridden, so the port overrides that main function and
// acts only on that caller.
//
// The question is PortableKit's message dialog (ui/message_dialog.hpp), drawn
// in the game's look by the native dialogs. The game reads a neutral pad
// while it is up and the title's handler goes on asking for the pad, which is
// where the dialog's sounds are played: the game polls none of its own
// dialogs meanwhile. D012_NO_TITLE_QUIT=1: circle does nothing there.
//
// MIT licence; see port/README.md.

#include "d012_decomp_harness.hpp"
#include "d012_dialogs.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "hle/hle_common.hpp"
#include "ui/message_dialog.hpp"
#include "ui/save_list_dialog.hpp"
#include "ui/text_input.hpp"
#include "ui/ui.hpp"
#endif

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace d012::title_quit {
namespace {

#if defined(PORTABLEKIT_HAS_RENDERER)

constexpr std::uint32_t kPlayerPad = 0x08872088u;   // main: player 1's pad
constexpr std::uint32_t kTitleMenuPoll = 0x089DF330u;  // menu: the title's menu asks for it
constexpr std::uint32_t kCircle = 0x2000u;            // PSP_CTRL_CIRCLE

namespace ui = portablekit::ui;
using Clock = std::chrono::steady_clock;

struct State {
    bool circle_held{};
    Clock::time_point last_poll{};
};

State &state() {
    static State s;
    return s;
}

// Nothing of PortableKit's is over the game.
bool screen_clear() {
    return !ui::game_message_open() && !ui::game_save_list_open() && !ui::text_input_open() &&
           !ui::menu_over_game() && !ui::panel_over_game();
}

void ask() {
    ui::MessageRequest request;
    request.text = "Close the game?";
    request.question = true;
    request.default_no = true;
    request.allow_back = true;
    const bool opened = ui::open_game_message(std::move(request), [](ui::MessageAnswer answer) {
        if (answer != ui::MessageAnswer::Yes) return;
        std::cout << "[title] closing the game from the title screen" << std::endl;
        ui::request_quit();
    });
    if (opened) std::cout << "[title] asked whether to close the game" << std::endl;
}

// main 0x08872088: player 1's pad, for the game as before. From the title's
// menu, a circle pressed since its last frame asks the question.
void run_player_pad(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    decomp::call_original(g, kPlayerPad, regs);
    out.v0 = regs.v0;
    if (in.gpr[31] != kTitleMenuPoll) return;

    dialogs::play_queued_sounds(g);
    State &s = state();
    const Clock::time_point now = Clock::now();
    const bool held = (portablekit::first_pad_sample_now().buttons & kCircle) != 0u;
    // After another screen (the Load list backed out of with circle, say), a
    // circle still held is not a new press.
    const bool fresh = now - s.last_poll < std::chrono::milliseconds(200);
    const bool pressed = fresh && held && !s.circle_held;
    s.circle_held = held;
    s.last_poll = now;
    if (pressed && screen_clear()) ask();
}

struct Install {
    Install() {
        if (std::getenv("D012_NO_TITLE_QUIT") != nullptr) {
            std::cout << "[title] D012_NO_TITLE_QUIT: circle does nothing on the title screen\n";
            return;
        }
        static const decomp::Override overrides[] = {
            {kPlayerPad, "title_quit_player_pad", decomp::Module::Main, decomp::Returns::Integer, &run_player_pad},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install;

#endif

} // namespace
} // namespace d012::title_quit
