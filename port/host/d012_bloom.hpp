#pragma once

// Bloom: the soft glow the game lays over battles and the world map, which the
// player can turn off on the menu's Video page. MIT licence; see
// port/README.md.

namespace portablekit {
class HleRegistrar;
}

namespace d012::bloom {

// True unless the player turned bloom off (settings.ini game.bloom=0, or
// D012_BLOOM=0, which wins).
[[nodiscard]] bool enabled();
// Adds the Video page's row. The override that leaves the bloom out goes in
// from this file's static initialiser.
void install(portablekit::HleRegistrar &hle);

} // namespace d012::bloom
