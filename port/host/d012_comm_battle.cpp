// Communication Mode battles on the port's lockstep (d012_comm_battle.hpp).
//
// The game's role-0 handler (main 0x0890D664, net = *(0x089D8364) + 0x10D8 in
// a0; docs/port/communication-mode.md 4.2) runs once a game frame from the
// network manager's task, just before the pad manager polls the raw pads.
// Each call it samples this PSP's pad, trades 16-bit words with the other
// fighter's PSP, and once both words for frame net+0x48 are in, writes both
// into the raw pads' injected samples (pad manager + i * 0x60 + 0x40, player i
// in slot i on both PSPs) and returns 0; otherwise it returns 1, and the
// battle waits.
//
// The override does the same with the port's link: this copy's whole pad for
// the frame goes to the other copy, and the call returns only once the other
// copy's is in, so it never returns 1 and the game's own wait (its pause
// and its 900-frame timeout) never runs. Both copies inject the same bytes,
// the sticks at full resolution and the right stick in the spare bytes 10
// and 11, as PortableKit's sceCtrl fills them. Every tenth frame it runs the
// game's own check value (main 0x0890CD90 -> the battle's callback), as the
// handler does, and compares it with the other copy's; a difference, or the
// other copy gone, ends the battle with the game's own error (quit flag,
// return 3).
//
// Anything but a running battle, and a spectator (role 1, Group Battle), stays
// the game's. When the game takes this PSP out of a battle (main 0x089072AC:
// the quit combo, a lost session), the link says so to the other copy, whose
// battle then ends with the game's error at once instead of after 10 s.
//
// MIT licence; see port/README.md.

#include "d012_comm_battle.hpp"

#include "d012_decomp_harness.hpp"
#include "d012_net.hpp"

#include "hle/hle_common.hpp"
#include "portablekit/hle_extension.hpp"

#include "psprecomp/runtime.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>

namespace d012::comm_battle {
namespace {

namespace ext = portablekit::hle_extension;

constexpr std::uint32_t kHandler = 0x0890D664u;   // role 0's per-frame exchange (net)
constexpr std::uint32_t kCheckStep = 0x0890CD90u; // the check value of every tenth frame (net)
constexpr std::uint32_t kLeaveBattle = 0x089072ACu; // (session, flag): the raw pads back to this PSP's
constexpr std::uint32_t kSession = 0x089D8364u;   // -> the lobby session; net is at +0x10D8
constexpr std::uint32_t kSessionGroup = 0x1664u;  // the battle group's name, 8 digits
constexpr std::uint32_t kSessionNet = 0x10D8u;    // the battle's pad exchange (net)
constexpr std::uint32_t kPadManager = 0x089D6008u;
constexpr std::uint32_t kRawPadSize = 0x60u;
constexpr std::uint32_t kInjected = 0x40u;  // the injected SceCtrlData of a raw pad

// net fields
constexpr std::uint32_t kQuitFrame = 0x0Cu;
constexpr std::uint32_t kFlags = 0x10u;
constexpr std::uint32_t kChecks = 0x1Cu;     // {frame, value} x 4, oldest first
constexpr std::uint32_t kCheckCount = 0x3Cu;
constexpr std::uint32_t kRole = 0x40u;
constexpr std::uint32_t kLocal = 0x44u;      // this PSP's player
constexpr std::uint32_t kFrame = 0x48u;      // the frame applied next
constexpr std::uint32_t kPeerConfirmed = 0x4Cu + 0x50u;
constexpr std::uint32_t kSampled = 0x568u;
constexpr std::uint32_t kOpen = 1u;
constexpr std::uint32_t kRunning = 2u;
constexpr std::uint32_t kQuit = 4u;

constexpr std::uint32_t kSelect = 0x0001u;

struct State {
    bool linked = false;
    std::uint32_t frame = 0;  // the frame this link reached
    std::map<std::string, std::uint32_t> battles;  // battles per group, for the link's tag
};

State &state() {
    static State s;
    return s;
}

bool flag(const char *name) {
    const char *value = std::getenv(name);
    return value != nullptr && *value != '\0' && std::strcmp(value, "0") != 0;
}

// The same 24 bits on both copies: the battle group's name, and how many
// battles this copy has begun in that group.
std::uint32_t tag_for(Guest *g) {
    const gaddr session = g_ld32(g, kSession);
    std::string group;
    for (std::uint32_t i = 0; session != 0u && i < 8u; ++i) {
        const char c = static_cast<char>(g_ld8(g, session + kSessionGroup + i));
        if (c == '\0') break;
        group += c;
    }
    const std::uint32_t ordinal = state().battles[group]++;
    std::uint32_t hash = 2166136261u;
    for (const char c : group) hash = (hash ^ static_cast<std::uint8_t>(c)) * 16777619u;
    return (hash + ordinal * 0x9E3779B1u) & 0xFFFFFFu;
}

// Ends the battle as the game ends one on a desync or a quit notice: its own
// error box on this PSP.
void quit(Guest *g, gaddr net, std::uint32_t frame, GuestRegs &out) {
    g_st32(g, net + kFlags, g_ld32(g, net + kFlags) | kQuit);
    g_st32(g, net + kQuitFrame, frame);
    net::battle::close();
    state().linked = false;
    out.v0 = 3u;
}

void inject(Guest *g, int player, const input::Pad &pad) {
    const gaddr sample = g_ld32(g, kPadManager) + static_cast<std::uint32_t>(player) * kRawPadSize + kInjected;
    g_st32(g, sample, 0u);
    g_st32(g, sample + 4u, pad.buttons);
    g_st8(g, sample + 8u, pad.lx);
    g_st8(g, sample + 9u, pad.ly);
    g_st8(g, sample + 10u, pad.rx);
    g_st8(g, sample + 11u, pad.ry);
    for (std::uint32_t i = 12u; i < 16u; ++i) g_st8(g, sample + i, 0u);
}

// The main loop's frame pacing (main 0x08883F9C, its object at
// *0x089D69E0): +0x04 the pacing mode (1: the step is the vblanks that really
// passed; else a fixed one), +0x10 the vblanks a frame should take, +0x14
// those it took, +0x18 the step as a float. Logged when they change: a step
// that differs between two copies parts their battles.
void note_frame_step(Guest *g, std::uint32_t frame) {
    static std::uint32_t last_mode = ~0u, last_target = ~0u, last_taken = ~0u, lines = 0u;
    const gaddr pacing = g_ld32(g, 0x089D69E0u);
    if (pacing == 0u || lines >= 400u) return;
    const std::uint32_t mode = g_ld32(g, pacing + 0x04u);
    const std::uint32_t target = g_ld32(g, pacing + 0x10u);
    const std::uint32_t taken = g_ld32(g, pacing + 0x14u);
    if (mode == last_mode && target == last_target && taken == last_taken) return;
    last_mode = mode;
    last_target = target;
    last_taken = taken;
    ++lines;
    float step = 0.0f;
    std::memcpy(&step, g_ptr(g, pacing + 0x18u, 4u), 4u);
    std::cout << "[comm-battle] frame " << frame << ": pacing mode " << static_cast<std::int32_t>(mode) << ", "
              << taken << " vblanks taken of " << target << ", step " << step << std::endl;
}

void run_handler(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const gaddr net = in.gpr[4];
    const std::uint32_t flags = g_ld32(g, net + kFlags);
    State &s = state();
    const bool ours = (flags & (kOpen | kRunning)) == (kOpen | kRunning) && (flags & kQuit) == 0u &&
                      g_ld32(g, net + kRole) == 0u;
    const std::uint32_t frame = g_ld32(g, net + kFrame);
    if (!enabled()) {
        GuestRegs regs{};
        regs.a[0] = net;
        decomp::call_original(g, kHandler, regs);
        out.v0 = regs.v0;
        return;
    }
    if (ours && frame == 0u) {
        // A new battle: its own link.
        const int player = static_cast<int>(g_ld32(g, net + kLocal));
        s.linked = net::battle::open(player, tag_for(g));
        s.frame = 0u;
        if (!s.linked) {
            std::cerr << "[comm-battle] no link to the other fighter's copy; the battle ends" << std::endl;
            return quit(g, net, 0u, out);
        }
    }
    if (!ours || !s.linked || frame != s.frame) {
        GuestRegs regs{};
        regs.a[0] = net;
        decomp::call_original(g, kHandler, regs);
        out.v0 = regs.v0;
        return;
    }
    const ext::SecondPad now = ext::first_pad_now();
    input::Pad mine;
    mine.connected = now.connected;
    mine.buttons = now.buttons;
    mine.extra = now.extra;
    mine.lx = now.analog_x;
    mine.ly = now.analog_y;
    mine.rx = now.right_x;
    mine.ry = now.right_y;
    // R3 is the camera's recentre, SELECT, as on the port's own pad reads.
    if ((mine.extra & ext::kExtraR3) != 0u) mine.buttons |= kSelect;
    const net::battle::Pads pads = net::battle::exchange(frame, mine);
    if (pads.lost) {
        std::cerr << "[comm-battle] the other fighter's copy is gone at frame " << frame << std::endl;
        return quit(g, net, frame, out);
    }
    // The game's check value for this frame, before its input goes in, as the
    // handler takes it.
    if (frame > 0u && frame % 10u == 0u) {
        g_call1(g, kCheckStep, net);
        const std::uint32_t count = g_ld32(g, net + kCheckCount);
        const gaddr newest = net + kChecks + (count - 1u) * 8u;
        if (count != 0u && g_ld32(g, newest) == frame && !net::battle::check(frame, g_ld32(g, newest + 4u))) {
            std::cerr << "[comm-battle] the two battles differ; it ends at frame " << frame << std::endl;
            return quit(g, net, frame, out);
        }
    }
    for (int player = 0; player < 2; ++player) inject(g, player, pads.player[player]);
    note_frame_step(g, frame);
    s.frame = frame + 1u;
    g_st32(g, net + kFrame, frame + 1u);
    g_st32(g, net + kSampled, frame + 1u);
    g_st32(g, net + kPeerConfirmed, frame);
    out.v0 = 0u;
}

void run_leave_battle(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    State &s = state();
    if (s.linked) {
        std::cout << "[comm-battle] this copy leaves the battle at frame " << s.frame << std::endl;
        net::battle::close();
        s.linked = false;
    }
    GuestRegs regs{};
    regs.a[0] = in.gpr[4];
    regs.a[1] = in.gpr[5];
    decomp::call_original(g, kLeaveBattle, regs);
    out.v0 = regs.v0;
}

// The PSP clocks a game can read. During a battle on the link, the first read
// from each place in the game's code is logged with the battle frame: a read
// the battle's outcome depends on would part two copies whose clocks differ.
struct ClockImport {
    const char *library;
    std::uint32_t nid;
    const char *name;
};
constexpr ClockImport kClocks[] = {
    {"ThreadManForUser", 0x369ED59Du, "sceKernelGetSystemTimeLow"},
    {"ThreadManForUser", 0x82BC5777u, "sceKernelGetSystemTimeWide"},
    {"ThreadManForUser", 0xDB738F35u, "sceKernelGetSystemTime"},
    {"sceRtc", 0x3F7AD767u, "sceRtcGetCurrentTick"},
    {"sceRtc", 0xE7C27D1Bu, "sceRtcGetCurrentClockLocalTime"},
    {"sceRtc", 0x4CFA57B0u, "sceRtcGetCurrentClock"},
    {"sceDisplay", 0x9C6EAAD7u, "sceDisplayGetVcount"},
    {"UtilsForUser", 0x27CC57F0u, "sceKernelLibcTime"},
    {"UtilsForUser", 0x71EC4271u, "sceKernelLibcGettimeofday"},
    {"UtilsForUser", 0x91E4F6A7u, "sceKernelLibcClock"},
};

// D012_CLOCK_OFFSET_S (a test switch): this copy's system time (the
// sceKernelGetSystemTime* reads) runs this many seconds ahead, as a copy
// started that much earlier would see it.
std::uint64_t clock_offset_us() {
    static const std::uint64_t offset = [] {
        const char *text = std::getenv("D012_CLOCK_OFFSET_S");
        return text != nullptr ? std::strtoull(text, nullptr, 10) * 1000000ull : 0ull;
    }();
    return offset;
}

void wrap_clock(portablekit::HleRegistrar &hle, const ClockImport &clock) {
    const portablekit::HleFunction previous = hle.bound_function(clock.library, clock.nid);
    if (!previous) return;
    const std::string name = clock.name;
    hle.bind(clock.library, clock.nid,
             [previous, name](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
                 const std::uint32_t caller = ctx.gpr[31];
                 const std::uint32_t argument = ctx.gpr[4];
                 previous(rt, ctx);
                 if (const std::uint64_t offset = clock_offset_us(); offset != 0u) {
                     if (name == "sceKernelGetSystemTimeLow") {
                         ctx.gpr[2] += static_cast<std::uint32_t>(offset);
                     } else if (name == "sceKernelGetSystemTimeWide") {
                         const std::uint64_t time =
                             (static_cast<std::uint64_t>(ctx.gpr[3]) << 32u | ctx.gpr[2]) + offset;
                         ctx.gpr[2] = static_cast<std::uint32_t>(time);
                         ctx.gpr[3] = static_cast<std::uint32_t>(time >> 32u);
                     } else if (name == "sceKernelGetSystemTime" && argument != 0u) {
                         auto &memory = rt.memory();
                         const std::uint64_t time =
                             (static_cast<std::uint64_t>(memory.load32(argument + 4u)) << 32u |
                              memory.load32(argument)) + offset;
                         memory.store32(argument, static_cast<std::uint32_t>(time));
                         memory.store32(argument + 4u, static_cast<std::uint32_t>(time >> 32u));
                     }
                 }
                 State &s = state();
                 if (!s.linked) return;
                 static std::map<std::uint32_t, bool> seen;
                 if (seen[caller]) return;
                 seen[caller] = true;
                 std::cout << "[comm-battle] " << name << " read by the game at " << std::hex << "0x" << caller
                           << std::dec << " (battle frame " << s.frame << ")" << std::endl;
             });
}

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kHandler, "comm_battle_exchange", decomp::Module::Main, decomp::Returns::Integer, &run_handler},
            {kLeaveBattle, "comm_battle_leave", decomp::Module::Main, decomp::Returns::Nothing, &run_leave_battle},
        };
        // Always in place: whether to use them is read when the game first
        // calls them (settings.ini cannot be read this early).
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install_overrides;

} // namespace

bool enabled() {
    static const bool on = [] {
        const bool value = net::battle::configured() && !flag("D012_NO_NET_BATTLE");
        if (value) std::cout << "[comm-battle] Communication Mode battles run on the port's link" << std::endl;
        return value;
    }();
    return on;
}

// With D012_CLOCK_OFFSET_S, the peers' last-seen times move on with the
// system time, or the game finds every peer stale and drops it.
void wrap_peer_times(portablekit::HleRegistrar &hle) {
    constexpr std::uint32_t kPeerInfoSize = 152u;
    constexpr std::uint32_t kLastSeen = 144u;
    const auto shift = [](psprecomp::GuestMemory &memory, std::uint32_t entry) {
        const std::uint64_t seen =
            (static_cast<std::uint64_t>(memory.load32(entry + kLastSeen + 4u)) << 32u | memory.load32(entry + kLastSeen)) +
            clock_offset_us();
        memory.store32(entry + kLastSeen, static_cast<std::uint32_t>(seen));
        memory.store32(entry + kLastSeen + 4u, static_cast<std::uint32_t>(seen >> 32u));
    };
    // sceNetAdhocctlGetPeerInfo(mac*, size, info*)
    if (const portablekit::HleFunction info = hle.bound_function("sceNetAdhocctl", 0x8DB83FDCu)) {
        hle.bind("sceNetAdhocctl", 0x8DB83FDCu, [info, shift](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
            const std::uint32_t entry = ctx.gpr[6];
            info(rt, ctx);
            if (ctx.gpr[2] == 0u && entry != 0u) shift(rt.memory(), entry);
        });
    }
    // sceNetAdhocctlGetPeerList(length*, buffer*)
    if (const portablekit::HleFunction list = hle.bound_function("sceNetAdhocctl", 0xE162CB14u)) {
        hle.bind("sceNetAdhocctl", 0xE162CB14u, [list, shift](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
            const std::uint32_t length = ctx.gpr[4];
            const std::uint32_t buffer = ctx.gpr[5];
            list(rt, ctx);
            if (ctx.gpr[2] != 0u || length == 0u || buffer == 0u) return;
            const std::uint32_t count = rt.memory().load32(length) / kPeerInfoSize;
            for (std::uint32_t i = 0; i < count; ++i) shift(rt.memory(), buffer + i * kPeerInfoSize);
        });
    }
}

void install(portablekit::HleRegistrar &hle) {
    for (const ClockImport &clock : kClocks) wrap_clock(hle, clock);
    if (clock_offset_us() != 0u) wrap_peer_times(hle);
    // The release build's log is a file under Proton and Windows alike, and
    // a game closed from the system loses what is still buffered: unbuffered
    // whenever this copy is set up for network battles.
    if (net::battle::configured()) std::setvbuf(stdout, nullptr, _IONBF, 0);
}

bool active(Guest *g) {
    if (!state().linked || !net::battle::open_now()) return false;
    const gaddr session = g_ld32(g, kSession);
    if (session == 0u) return false;
    const gaddr net = session + kSessionNet;
    const std::uint32_t flags = g_ld32(g, net + kFlags);
    return (flags & (kOpen | kRunning)) == (kOpen | kRunning) && (flags & kQuit) == 0u && g_ld32(g, net + kRole) == 0u;
}

} // namespace d012::comm_battle
