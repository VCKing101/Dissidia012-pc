#pragma once

// The save's summonstone slots, and the All summons cheat's code for them
// (d012_cheats.cpp, d012_summon_slot_guard.cpp): no game here, so d012_tests
// checks them. MIT licence; see port/README.md.

#include <cstdint>

namespace d012::summon_slots {

// 64 slots of four bytes in the save's inventory: the game's 61 summons, then
// three that stay empty.
inline constexpr std::uint32_t kFirstSlot = 0x08BBF89Cu;
inline constexpr std::uint32_t kSlotCount = 64u;
inline constexpr std::uint32_t kSummonCount = 61u;
inline constexpr std::uint8_t kEmptyId = 0xFFu;
inline constexpr std::uint8_t kOwned = 0x02u;

struct Slot {
    std::uint8_t id;
    std::uint8_t uses;
    std::uint8_t recharge;
    std::uint8_t flags;
};

// An empty slot marked as owned: the end of a battle stops the game on it.
[[nodiscard]] constexpr bool marked_but_empty(const Slot &slot) {
    return slot.id == kEmptyId && (slot.flags & kOwned) != 0u;
}

// The slot as a new game leaves an empty one, its other flags kept.
[[nodiscard]] constexpr Slot emptied(const Slot &slot) {
    return Slot{slot.id, 0u, 0u, static_cast<std::uint8_t>(slot.flags & ~kOwned)};
}

// All summons: flags 3 and 10 uses in each of the 61 summons' slots.
inline constexpr const char *kAllSummonsCode =
    "0x803BF89F 0x003D0004\n0x00000003 0x00000000\n"
    "0x803BF89D 0x003D0004\n0x0000000A 0x00000000";

} // namespace d012::summon_slots
