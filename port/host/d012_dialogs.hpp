#pragma once

// The game's message, save-data and keyboard dialogs in the look of the
// game's own menus, drawn with pieces of the game's menu pictures read from
// the player's disc (PortableKit's dialog skin, ui/dialog_skin.hpp), with the
// game's own menu sounds. D012_NO_NATIVE_DIALOGS=1 keeps PortableKit's look;
// D012_NO_DIALOG_SOUNDS=1 keeps them silent.

#include "../decomp/guest.h"

namespace portablekit {
class HleRegistrar;
}

namespace d012::dialogs {

void install(portablekit::HleRegistrar &hle);
// From an override on the game's thread: plays the sounds a dialog asked for
// since the game last polled one of its own (a port's own question over a
// game screen, which the game does not poll).
void play_queued_sounds(Guest *g);

} // namespace d012::dialogs
