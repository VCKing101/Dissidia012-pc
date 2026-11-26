#pragma once

// Who is where, on the game's own Online Lobby screen (d012_lobby_status.cpp).
// MIT licence; see port/README.md.

#include "d012_decomp_harness.hpp"

namespace d012::lobby_status {

// From the override of main 0x088D44EC (place a texts table's texts on their
// parts), after the game's own: local versus's (d012_versus_menu.cpp) when
// local versus is on, this module's own otherwise.
void texts_placed(Guest *g, const decomp::CallState &in);

} // namespace d012::lobby_status
