#pragma once

// The Discord status: what the player is doing (in the menus, a Quick Battle,
// an online battle, who fights whom) on their Discord profile while the
// Discord app runs on the same PC, with a switch on the menu's System page.
// See d012_discord.cpp. MIT licence; see port/README.md.

namespace portablekit {
class HleRegistrar;
}

namespace d012::discord {

// Starts the connection's thread and adds the System page's rows. Once.
void install(portablekit::HleRegistrar &hle);

} // namespace d012::discord
