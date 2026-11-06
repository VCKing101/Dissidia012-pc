#pragma once

// The game's message, save-data and keyboard dialogs in the look of the
// game's own menus, drawn with pieces of the game's menu pictures read from
// the player's disc (PortableKit's dialog skin, ui/dialog_skin.hpp), with the
// game's own menu sounds. D012_NO_NATIVE_DIALOGS=1 keeps PortableKit's look;
// D012_NO_DIALOG_SOUNDS=1 keeps them silent.

namespace portablekit {
class HleRegistrar;
}

namespace d012::dialogs {

void install(portablekit::HleRegistrar &hle);

} // namespace d012::dialogs
