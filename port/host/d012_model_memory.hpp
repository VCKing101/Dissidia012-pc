#pragma once

// Room for models and sound banks bigger than the game's own. The game loads each model file
// (obj/*.gmo, *.objx) through a staging block of a fixed 0x264000 bytes, taken
// from a file heap of 0x275800 bytes, and builds both fighters' models in a
// pool of 0x780000 bytes; all three sit in its main heap, which has no room to
// spare. A package mod's model can be bigger (an edited animation adds to its
// file), so when one is, the port makes the block that size and serves the
// file heap and the pool, grown to match, from guest memory above the 32 MiB
// the game uses. Addresses and how they were found:
// docs/port/mod-folder.md, section 9. MIT licence; see port/README.md.

#include <cstdint>

namespace d012::model_memory {

// The game's sizes.
inline constexpr std::uint32_t kRetailBlock = 0x264000u;     // the staging block
inline constexpr std::uint32_t kRetailFileHeap = 0x275800u;  // the heap it is taken from
inline constexpr std::uint32_t kRetailPool = 0x780000u;      // the battle model pool

// Keeps the guest memory above 0x0A000000 (the profile gives the game 64 MiB,
// as a PSP-2000 has) from the kernel, so the game's own blocks are where they
// would be in 32 MiB; the port takes from it. Called once, before anything is
// allocated (register_extra_hle).
void reserve();

// Makes the staging block big enough for a model file of `largest` bytes, as
// far as the reserved memory allows, and the file heap and pool bigger by as
// much (the pool by twice that, for two fighters), when that is more than the
// game's own. Called once, before the game runs (package_mods::install). Off
// with D012_NO_MODEL_MEMORY=1; D012_TRACE_MODEL_MEMORY=1 logs each use.
void fit(std::uint64_t largest);

// The staging block's size now: the game's, or what fit() made it.
[[nodiscard]] std::uint64_t block_size();

// The battle's sound and effect heaps. The game copies each fighter's sound
// banks into one heap of 0x237000 bytes (main 0x088A0C10), and the battle's
// shared sounds after them (a chase, EX Core, an assist); and each fighter's
// effect package into one heap of 0x3DD000 bytes (main 0x088A5054), and the
// stage's and the battle's shared effects (a rail's sparks and their sound)
// after them. Each fighter's goes in at its own size, so a fighter whose bank
// or package is bigger than the game's own (Bartz's original moves add to
// both) leaves too little room for what is loaded after it, which then does
// not show or play. Makes the heaps `sound_extra` and `effect_extra` bytes
// bigger, served from the reserved memory like the model blocks. Called once,
// before the game runs and before fit(). Off with D012_NO_MODEL_MEMORY=1.
inline constexpr std::uint32_t kRetailSoundHeap = 0x237000u;
void fit_battle_heaps(std::uint32_t sound_extra, std::uint32_t effect_extra);
// D012_TRACE_HEAPS=1 logs every allocation of the game's handle and block
// heaps (sound, effect, model, file and the rest) that finds no room; =2 logs
// every allocation. Called once, before the game runs.
void trace_heaps();

} // namespace d012::model_memory
