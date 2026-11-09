#pragma once

// Communication Mode battles on the port's lockstep (docs/port/communication-mode.md,
// section 11). The game's lobby, set-up and battle stay its own; the battle's
// per-frame exchange of the two players' pads (main 0x0890D664) is the port's:
// both players' full pads (analog sticks included) over the port's UDP link,
// with its input delay, instead of the game's 16-bit word over PSP ad hoc.
// MIT licence; see port/README.md.

#include "psprecomp/runtime.hpp"
#include "../decomp/guest.h"

#include <cstdint>

namespace portablekit {
class HleRegistrar;
}

namespace d012::comm_battle {

// Logs, during a battle on the link, the first read of each PSP clock from
// each place in the game's code; D012_CLOCK_OFFSET_S=N (a test switch) moves
// this copy's system time N seconds on. Makes the log unbuffered when this
// copy is set up for network battles.
void install(portablekit::HleRegistrar &hle);

// D012_NET_BATTLE or ad hoc play (d012_net.hpp, configured()) turns it on;
// D012_NO_NET_BATTLE=1 keeps the game's own exchange even then. Read at the
// game's first call of a battle handler and again as each battle starts, so
// online play turned on in the menu counts from the next battle.
[[nodiscard]] bool enabled();

// A battle is running on the port's link, or watched on its stream (Group
// Battle's members who are not fighting): linked or watching, and the game's
// session still running it (a battle the game ends by itself, such as on a
// lost session, may never say so to the port). Each fighter's camera then
// takes its own player's right stick, from the bytes 10 and 11 of that
// player's injected sample (d012_camera.cpp), the same on every copy.
[[nodiscard]] bool active(Guest *g);
// This copy's player, 0 or 1, while fighting on the link; -1 otherwise (a
// watcher has none).
[[nodiscard]] int local_player(Guest *g);

// Rollback's two points between dispatches: a battle frame's state saved as
// its handler is about to run, and the restore a wrong guess asked for, done
// once the handler has returned (the harness's post-dispatch hook).
void before_dispatch(psprecomp::Runtime &rt, std::uint32_t pc, const psprecomp::AllegrexContext &ctx);
void after_dispatch(psprecomp::Runtime &rt);

} // namespace d012::comm_battle
