#pragma once

// Abilities a mod's package adds to a character (Bartz's original Dissidia
// moves, tools/original_moves.py), given to the character as the levels
// pass, whatever save is loaded. MIT licence; see port/README.md.
//
// A character can use an ability once it is in the learned list of its
// record. The game puts abilities there as the character levels up (system.bin
// `chrl`) or when another is mastered (`amas`), which covers what is still to
// come; a save made before the mod has passed those levels already. So
// whenever the game asks for a character's record (main 0x088CB164), the
// abilities of that character whose level the record has reached and that
// the list lacks are put in, with no AP. Only abilities the game's ability
// table has: with package mods off, nothing is given. Off with
// D012_NO_MOD_ABILITIES=1.

#include <cstdint>
#include <string>
#include <vector>

namespace portablekit {
class HleRegistrar;
}

namespace d012::mod_abilities {

struct Grant {
    std::uint16_t id{};  // (character id << 8) | key
    int learn{};         // level; 0 = left to the game
    std::string mod_id;
};

// Called once, by mods::install, with the enabled mods' abilities.
void install(const std::vector<Grant> &grants);
// Keeps the abilities out of the game's save file, in a side file of the
// port's (see d012_mod_abilities.cpp). Called after install, once the save
// utility is bound (d012_hle.cpp).
void install_saves(portablekit::HleRegistrar &hle);

} // namespace d012::mod_abilities
