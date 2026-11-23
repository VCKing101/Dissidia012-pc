#pragma once

// The Manikin look: each character the game has a manikin of gets one more
// look in the menus, after the last of its others, named "Manikin". It is
// the game's own manikin (costume 0, the P_<code>_ZAKO model, its assist and
// world map versions and its voice bank), which the game's battles already
// load; only the menus are changed. Rules in d012_manikin_rules.hpp.
//
// What is changed:
//   - the looks a character has (main 0x088D5E0C, and the character lists of
//     main 0x088D5AC8, which reaches it inside its own unit) gain bit 0;
//   - the next look (main 0x088D608C: Square on Player Select, Triangle in
//     Customization) goes on from the last to the manikin and then to
//     Normal;
//   - the look labels: "Manikin" is added to the label table of the six menu
//     text packs that have one (served in place of the disc's,
//     d012_package_mods.hpp), and the three tables in the menu overlay that
//     choose a label by costume show it for costume 0;
//   - the battle portrait, which the disc has for every look but the
//     manikins': a manikin shows its character's Normal one.
//
// Left out, with a line in the log, when a disc mod is on, when a mod
// replaces one of the six text packs, or with D012_NO_MANIKIN_LOOK=1 (or
// D012_NO_PACKAGE_MODS=1). A save that has a manikin chosen still plays it
// then: the menus call it Normal, and Square goes on to the next look as
// before.
// MIT licence; see port/README.md.

namespace d012::manikin_look {

// Called once, by mods::install, before package_mods::install: asks to give
// the package the text packs with the label.
void install();

// Whether the look is offered this run. False until the game has opened its
// package.
[[nodiscard]] bool active();

} // namespace d012::manikin_look
