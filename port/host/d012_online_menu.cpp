// How to play, asked from Communications Mode (docs/port/online-matchmaking.md
// section 12): as the player chooses Online Lobby, Group Battle or
// Communications with nothing chosen, over the menu, so that Back leaves them
// on the menu with the row still under the hand.
//
// Communications Mode's rows are handled by the start menu's submenu handler
// (menu 0x089E4A60, submenu state +0xA88 = 2, row +0xA5C): a chosen row plays
// the "chosen" sound, then writes a destination (main 0x09B6D838: 6 Online
// Lobby, 7 Group Battle, 8 Friend Cards, 9 Artifacts, 10 Communications) and
// mode 4 (state +0), and the menu leaves for it from the next frame. All five
// start the network on the way (main 0x0890E61C, sceNetAdhocctlInit), Friend
// Cards and Artifacts too, which need nobody else, so PortableKit's own
// question there is off (GameProfile::adhoc_choice_at_init); and the game
// never reads whether that start worked, so a question there could not take
// the player back.
//
// The handler ends with menu 0x089E1DD8, which calls 0x089E30B8, which calls
// main 0x088D4EC4 with the menu's state + 0x888: the hook, in the same frame
// as the writes. Calls inside the overlay's own generated unit are inline and
// cannot be overridden; this one goes to main. Only the menu overlay calls
// 0x088D4EC4. Backed out of, the mode and the destination go back to what they
// were the call before, and the menu stays as it was.
#include "d012_decomp_harness.hpp"

#include "adhoc/session.hpp"

#include <cstdint>
#include <cstdio>

namespace d012::online_menu {
namespace {

constexpr std::uint32_t kMenuPartsStep = 0x088D4EC4u;
constexpr std::uint32_t kFromMenuEnd = 0x089E30CCu;  // the return from menu 0x089E30C4's call
constexpr std::uint32_t kStateOffset = 0x888u;      // what 0x089E30B8 passes: the menu's state + 0x888
constexpr std::uint32_t kStateMode = 0x000u, kStateSubmenu = 0xA88u;
constexpr std::uint32_t kModeLeaving = 4u, kCommunicationsMode = 2u;
constexpr std::uint32_t kDestination = 0x09B6D838u;
constexpr std::uint32_t kOnlineLobby = 6u, kGroupBattle = 7u, kCommunications = 10u;

// The menu overlay is the one in the overlay window (the battle overlay shares
// its addresses): 0x089E30B8's load of the state and its call.
bool menu_resident(Guest *g) {
    return g_ld32(g, 0x089E30BCu) == 0x8C840140u && g_ld32(g, 0x089E30C4u) == 0x0E2353B1u;
}

bool needs_session(std::uint32_t destination) {
    return destination == kOnlineLobby || destination == kGroupBattle || destination == kCommunications;
}

// The menu's mode and destination at the last call while not leaving.
struct Before {
    bool known = false;
    std::uint32_t mode = 0u, destination = 0u;
} g_before;

void ask_if_leaving_online(Guest *g, gaddr state) {
    const std::uint32_t mode = g_ld32(g, state + kStateMode);
    const std::uint32_t destination = g_ld32(g, kDestination);
    if (mode != kModeLeaving) {
        g_before = {true, mode, destination};
        return;
    }
    if (g_ld32(g, state + kStateSubmenu) != kCommunicationsMode || !needs_session(destination) || !g_before.known)
        return;
    if (portablekit::adhoc_ask_for_choice()) return;
    std::printf("[online] backed out of destination %u: the menu stays (mode %u)\n", destination, g_before.mode);
    std::fflush(stdout);
    g_st32(g, state + kStateMode, g_before.mode);
    g_st32(g, kDestination, g_before.destination);
}

// main 0x088D4EC4 (menu state + 0x888).
void run_menu_parts_step(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    if (in.gpr[31] == kFromMenuEnd && in.gpr[4] > kStateOffset && menu_resident(g))
        ask_if_leaving_online(g, in.gpr[4] - kStateOffset);
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, kMenuPartsStep, regs);
    out.v0 = regs.v0;
    out.v1 = regs.v1;
}

struct Install {
    Install() {
        using decomp::Module;
        using decomp::Returns;
        static const decomp::Override overrides[] = {
            {kMenuPartsStep, "online_menu_parts_step", Module::Main, Returns::Integer, &run_menu_parts_step},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install;

} // namespace
} // namespace d012::online_menu
