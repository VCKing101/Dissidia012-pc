// Local versus: player 2's Customize, the rules (d012_versus_rules.hpp).
// MIT licence; see port/README.md.

#include "d012_versus_rules.hpp"

namespace d012::versus_rules {
namespace {

// Customize itself and the room's way back from it.
bool in_customize(std::uint32_t state) { return state >= kCustomizeFirst && state <= kCustomizeOpen; }
bool coming_up(std::uint32_t state) { return state <= kRoomComingUpLast; }

} // namespace

bool player2_pick(std::uint32_t state, std::uint32_t next) {
    if (state >= kPlayer2PickFirst && state <= kPlayer2PickLast) return true;
    if (state == kPartyPickWait) return next == kPartyPickBuild;
    return state >= kPartyPickFirst && state <= kPartyPickLeave;
}

Pad room_pad(std::uint32_t state, std::uint32_t next, const Trip &trip) {
    if (player2_pick(state, next)) return Pad::Player2;
    if (trip.active() && (in_customize(state) || coming_up(state))) return Pad::Player2;
    return Pad::Player1;
}

void step(Trip &trip, bool room, std::uint32_t state, std::uint32_t next) {
    if (!trip.active()) return;
    if (!room) {
        trip = {};
        return;
    }
    if (state == kCustomizeOpen) {
        trip.opened = true;
        return;
    }
    if (trip.opened) {
        // Customize closed: the room comes up and goes back where it was.
        if (!in_customize(state) && !coming_up(state)) trip = {};
        return;
    }
    // On the way: the fade out of the screen it started from, then the
    // room taken down.
    (void)next;
    const bool from_here = trip.from == Trip::From::Pick ? state >= kPlayer2PickUpdate && state <= kPlayer2PickLast
                                                         : state >= kSetupUpdate && state <= kSetupLast;
    if (!from_here && !in_customize(state)) trip = {};
}

bool customizable(std::int32_t cell, std::int32_t character) {
    return cell != kLockedCell && cell != kRandomCell && cell != kFeralChaosCell && character != kChaos && character > 0;
}

bool grid_takes_triangle(std::uint32_t grid_state, std::int32_t busy, std::uint32_t grid_flags) {
    return grid_state == kGridInput && busy <= 0 && (grid_flags & kGridNoInput) == 0u;
}

bool pick_customize(std::uint32_t flags_before, std::uint32_t state_after, std::uint32_t result) {
    return (flags_before & kFlagFading) == 0u && state_after == kPlayer2PickUpdate && result == kGridCustomize;
}

bool setup_triangle(std::uint32_t state, std::uint32_t flags, std::uint32_t p2_now, std::uint32_t p2_before,
                    std::uint32_t p1_now) {
    return state == kSetupUpdate && (flags & kFlagFading) == 0u && (p2_now & kTriangle) != 0u &&
           (p2_before & kTriangle) == 0u && (p1_now & kTriangle) == 0u;
}

bool setup_chose_customize(std::uint32_t result_before, std::uint32_t result_now) {
    return result_before != kSetupCustomize && result_now == kSetupCustomize;
}

bool setup_took_customize(std::uint32_t flags_before, std::uint32_t flags_now, std::uint32_t back_to) {
    return (flags_before & kFlagCustomize) == 0u && (flags_now & kFlagCustomize) != 0u &&
           (flags_now & kFlagRuleset) == 0u && back_to == kBackToSetup;
}

} // namespace d012::versus_rules
