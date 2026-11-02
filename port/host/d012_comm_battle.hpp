#pragma once

// Communication Mode battles on the port's lockstep (docs/port/communication-mode.md,
// section 11). The game's lobby, set-up and battle stay its own; the battle's
// per-frame exchange of the two players' pads (main 0x0890D664) is the port's:
// both players' full pads (analog sticks included) over the port's UDP link,
// with its input delay, instead of the game's 16-bit word over PSP ad hoc.
// MIT licence; see port/README.md.

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

// D012_NET_BATTLE (d012_net.hpp) turns it on; D012_NO_NET_BATTLE=1 keeps the
// game's own exchange even then.
[[nodiscard]] bool enabled();

// A battle is running on the port's link: linked, and the game's session
// still running it (a battle the game ends by itself, such as on a lost
// session, may never say so to the port). Each fighter's camera then takes
// its own player's right stick, from the bytes 10 and 11 of that player's
// injected sample (d012_camera.cpp), the same on both copies.
[[nodiscard]] bool active(Guest *g);

} // namespace d012::comm_battle
