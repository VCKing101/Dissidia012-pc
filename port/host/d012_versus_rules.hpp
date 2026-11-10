#pragma once

// Local versus: player 2's Customize (docs/port/local-versus.md, 7.16), the
// rules d012_versus.cpp follows, with no guest memory here so that d012_tests
// checks them. MIT licence; see port/README.md.
//
// The character room (menu overlay, work area *(*0x08AA3F44 + 0x154)) runs a
// state machine of 53 states (+0x910, table menu 0x08AA3F48). The states
// these rules know:
//
//   4 to 7    player 1's pick (5 the update, menu 0x08A9E0E8)
//   8 to 11   player 2's pick (9 the update, menu 0x08A984BC)
//   12 to 15  the battle setup (13 the update, menu 0x08A9E5E0)
//   50 to 52  Customize: 50 and 51 take the room down, 52 starts the game's
//             Customize (menu 0x08AF3130) for the character at +0x8048 and
//             waits, paused, until it closes; its callback (menu 0x08A9A52C)
//             puts the room in state 0, which goes straight to the state at
//             +0x918 (4, 8 or 12)
//   0 to 3    the room coming up

#include <cstdint>

namespace d012::versus_rules {

inline constexpr std::uint32_t kPlayer1PickUpdate = 5u;
inline constexpr std::uint32_t kPlayer2PickFirst = 8u;
inline constexpr std::uint32_t kPlayer2PickUpdate = 9u;
inline constexpr std::uint32_t kPlayer2PickLast = 11u;
inline constexpr std::uint32_t kSetupUpdate = 13u;
inline constexpr std::uint32_t kSetupLast = 15u;
inline constexpr std::uint32_t kCustomizeFirst = 50u;
inline constexpr std::uint32_t kCustomizeOpen = 52u;
inline constexpr std::uint32_t kRoomComingUpLast = 3u;
// The state the room comes back to after Customize (+0x918).
inline constexpr std::uint32_t kBackToPlayer2Pick = 8u;
inline constexpr std::uint32_t kBackToSetup = 12u;

// The room's flags (+0x91C): fading out (set with the fade), faded out (the
// fade's callback, menu 0x08A9CEC4), going to Customize, going to the
// ruleset (the setup's L).
inline constexpr std::uint32_t kFlagFading = 0x1u;
inline constexpr std::uint32_t kFlagFaded = 0x4u;
inline constexpr std::uint32_t kFlagCustomize = 0x8u;
inline constexpr std::uint32_t kFlagRuleset = 0x20u;

// The pick grid's result (grid +0x28 -> +0x5F8): 2 a character picked,
// 4 back, 5 Customize.
inline constexpr std::uint32_t kGridCustomize = 5u;
// The grid's own state (+0x5E4): 100 takes input; its flags (+0x5E8) with
// 0x1000 or 0x4000 take none.
inline constexpr std::uint32_t kGridInput = 100u;
inline constexpr std::uint32_t kGridNoInput = 0x1000u | 0x4000u;

// Triangle, the game's Customize on the picks and the setup screen: the PSP
// button (SceCtrlData) and the menus' logical bit (pad module table A).
inline constexpr std::uint32_t kTriangle = 0x1000u;
inline constexpr std::uint32_t kLogicalTriangle = 0x1000u;

// Characters the game's own pick refuses Customize for (menu 0x08A8255C,
// the player's grid): Chaos, and the cell values for Feral Chaos, a locked
// cell and the random cell.
inline constexpr std::int32_t kChaos = 0x17;
inline constexpr std::int32_t kFeralChaosCell = 0x20;
inline constexpr std::int32_t kLockedCell = -2;
inline constexpr std::int32_t kRandomCell = -3;

enum class Pad { Player1, Player2 };

// Player 2's way to Customize and back, from their pick or the setup screen.
struct Trip {
    enum class From : std::uint8_t { None, Pick, Setup };
    From from = From::None;
    bool opened = false;  // Customize has been up (the room in state 52)
    [[nodiscard]] bool active() const { return from != From::None; }
};

// Whose pad the game's own pad read gets while the character room is in
// `state`: player 2's on their pick (states 8 to 11) and on their way to
// Customize, in it and back; player 1's otherwise.
[[nodiscard]] Pad room_pad(std::uint32_t state, const Trip &trip);

// The trip after a pad read with the room in `state` (`room` false: the
// room is gone). It ends when the room is somewhere it does not pass
// through on the way: back at a pick or the setup once Customize was up, or
// anywhere else before.
void step(Trip &trip, bool room, std::uint32_t state);

// Whether the character under player 2's cursor can be customised, from the
// grid's cell (menu 0x08A7F104(grid, 2, -1)) and character (menu
// 0x08A81188), as the player's own pick decides.
[[nodiscard]] bool customizable(std::int32_t cell, std::int32_t character);

// Player 2's pick update: whether triangle, pressed this frame, reaches a
// grid that takes it (its state, busy count +0x620 and flags).
[[nodiscard]] bool grid_takes_triangle(std::uint32_t grid_state, std::int32_t busy, std::uint32_t grid_flags);

// Player 2's pick update ran with the room's flags `flags_before`, and left
// the room in `state_after` and the grid's result at `result`: whether that
// is player 2's Customize, which the port then starts as player 1's pick
// starts its own (the game's update has no case for it, and the room would
// wait for ever).
[[nodiscard]] bool pick_customize(std::uint32_t flags_before, std::uint32_t state_after, std::uint32_t result);

// The setup screen: whether player 2's triangle goes into this pad read, as
// a press on the game's pad. `p2_now` and `p2_before` are player 2's buttons
// at this read and the last; `p1_now` the game's pad as read.
[[nodiscard]] bool setup_triangle(std::uint32_t state, std::uint32_t flags, std::uint32_t p2_now, std::uint32_t p2_before,
                                  std::uint32_t p1_now);

// The setup's own result (its state +0x558): 3 Customize. Its update records
// it at the press and hands it to the room a frame later (its closing state,
// menu 0x08AA4158), so the room's Customize flag comes one read after.
inline constexpr std::uint32_t kSetupCustomize = 3u;
// How many reads the port waits for the room to take the setup's Customize.
inline constexpr std::uint32_t kSetupWaitReads = 30u;

// The read after one that gave the setup player 2's triangle: whether the
// setup chose its Customize from it (`result_before` at that read).
[[nodiscard]] bool setup_chose_customize(std::uint32_t result_before, std::uint32_t result_now);

// While player 2's Customize from the setup screen is on its way: whether
// the room now goes to Customize (the flag new since `flags_before`, not the
// ruleset, back to the setup after), so that it can be given player 2's
// character before it opens.
[[nodiscard]] bool setup_took_customize(std::uint32_t flags_before, std::uint32_t flags_now, std::uint32_t back_to);

} // namespace d012::versus_rules
