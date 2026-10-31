#pragma once

// Downloadable content (d012_dlc.cpp): a line in the log at start saying what
// the game will find in ms0:/PSP/GAME/ULUS10566, and a warning for DLC in a
// folder the game never reads (ULES01505, the European release's).

namespace portablekit {
class HleRegistrar;
}

namespace d012::dlc {

void install(portablekit::HleRegistrar &hle);

} // namespace d012::dlc
