#pragma once

// Bartz's moves from the first Dissidia in the game: the part of the port
// that gives the builder (d012_original_moves.hpp) the player's disc and
// hands what it builds to the game. Nothing is installed and nothing on the
// disc changes: the files are built once, kept in the data folder's cache,
// and served in place of the package's own (d012_package_mods.hpp).
//
// The moves are left out, with a line in the log saying why, when
//   - the disc's files are not the ones the recipe was made for (a disc mod
//     that changes Bartz or a fighter he borrows from);
//   - a mod's package replaces one of the files the build changes;
//   - the build does not come to the bytes the recipe says it must;
//   - a disc mod is on (d012_disc_mod.hpp);
//   - D012_NO_ORIGINAL_MOVES=1 or D012_NO_PACKAGE_MODS=1 is set.
// Online, both players must have them or both be without (net::battle's
// must_match). Bartz learns them at the levels the first game gave them
// (1, 3, 1, 28, 1, 19, 1, 38); D012_TEST_ORIGINAL_MOVES=1 gives all eight
// from level 1, for testing. MIT licence; see port/README.md.

#include "d012_mod_abilities.hpp"

#include <vector>

namespace d012::original_moves {

// Called once, by mods::install, before package_mods::install and
// mod_abilities::install: asks to give the package its files, and adds the
// abilities to `grants` (they are given only when the game's ability table
// has them, so nothing is given when the moves are left out).
void install(std::vector<mod_abilities::Grant> &grants);

// Whether the moves are in the game this run. False until the game has
// opened its package.
[[nodiscard]] bool active();

// His looks from DLC (the official DLC's fourth, costume packs): each one the
// game opens as his fighter's model is given the moves' animations as it is
// read, as his disc looks are; one that cannot take them (another skeleton,
// other animations) is drawn as his first look in a battle, menus aside.
// Nothing changes on the memory stick. Called once, before dlc_packs::install,
// so that it sees the packs' files before they are served. Off with
// D012_NO_ORIGINAL_MOVES_LOOKS=1.
void install_looks();

} // namespace d012::original_moves
