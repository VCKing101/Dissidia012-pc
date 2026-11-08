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
// A spectator's handler (role 1, Group Battle's members who are not
// fighting: main 0x0890DF58) is the port's too: it takes both players' pads
// for the frame from the leading fighter's stream (d012_net.hpp, watch()),
// injects both as the fighters do, and compares its check value with theirs.
// The game's own spectator relay is not used. With no relay to stream through
// (online.battle_link set), a battle others watch stays on the game's own
// exchange, on every copy alike.
//
// Anything but a running battle stays the game's. When the game takes this
// PSP out of a battle (main 0x089072AC: the quit combo, a lost session), the
// link says so to the other copy, whose battle then ends with the game's error
// at once instead of after 10 s.
//
// MIT licence; see port/README.md.

#include "d012_comm_battle.hpp"

#include "d012_camera.hpp"
#include "d012_decomp_harness.hpp"
#include "d012_decomp_pad.hpp"
#include "d012_net.hpp"
#include "d012_source_hash.hpp"

#include "adhoc/session.hpp"
#include "hle/hle_common.hpp"
#include "install/user_data.hpp"
#include "portablekit/hle_extension.hpp"
#include "snapshot.hpp"
#if defined(PORTABLEKIT_HAS_RENDERER)
#include "ui/ui.hpp"
#include "ui/widgets.hpp"
#endif

#include "psprecomp/runtime.hpp"

#include <algorithm>
#include <any>
#include <chrono>
#include <memory>
#include <optional>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>

namespace d012::comm_battle {
namespace {

namespace ext = portablekit::hle_extension;

constexpr std::uint32_t kHandler = 0x0890D664u;   // role 0's per-frame exchange (net)
constexpr std::uint32_t kWatchHandler = 0x0890DF58u;  // role 1's (a spectator's) per-frame handler (net)
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
constexpr std::uint32_t kSpectators = 0x358u;  // the PSPs watching (Group Battle), fed by the game's relay
constexpr std::uint32_t kOpen = 1u;
constexpr std::uint32_t kRunning = 2u;
constexpr std::uint32_t kQuit = 4u;

constexpr std::uint32_t kSelect = 0x0001u;

// Rollback (net::battle::rollback_frames): a state saved as each battle
// frame's handler is about to run, so the game can go back to a frame whose
// guessed input was wrong and run on from it again, showing nothing until it
// is back where it was (ext::set_simulate_only).
struct Rollback {
    struct Saved {
        std::uint32_t frame = ~0u;
        std::shared_ptr<portablekit::snapshot::State> state;
    };
    std::vector<Saved> ring;
    std::optional<std::uint32_t> restore_to;  // asked for by the handler, done after it returns
    std::uint32_t rerun_until = 0;            // the live frame the frames run again lead back to
    std::map<std::uint32_t, std::uint32_t> checks;  // check values by frame, sent once the frame is certain
    std::uint64_t rollbacks = 0, rerun = 0, longest = 0, reported = 0;
    std::chrono::steady_clock::time_point reported_at{};
    std::chrono::steady_clock::duration restore_time{}, save_time{};
};

Rollback &rollback() {
    static Rollback r;
    return r;
}

struct State {
    bool linked = false;
    bool watching = false;  // a spectator on the leading fighter's stream
    bool unseen = false;    // a watcher skipping ahead (net::battle::Pads::unseen)
    bool decided = false;  // this battle's first frame chose the port's link or the game's exchange
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

// The same 24 bits on every copy: the battle group's name, and how many
// battles this copy has begun in that group, fighting or watching (a party
// battle's fighters change from fight to fight, and every member runs each).
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
// A watcher's frames shown and heard again after a skip ahead, from the
// current moment on: the frames it ran unseen went past real time.
void seen_again() {
    State &s = state();
    if (!s.unseen) return;
    s.unseen = false;
    ext::set_simulate_only(false);
    ext::resync_real_time();
}

void quit(Guest *g, gaddr net, std::uint32_t frame, GuestRegs &out) {
    seen_again();
    g_st32(g, net + kFlags, g_ld32(g, net + kFlags) | kQuit);
    g_st32(g, net + kQuitFrame, frame);
    if (state().watching) net::battle::unwatch();
    else net::battle::close(frame);
    state().linked = false;
    state().watching = false;
    out.v0 = 3u;
}

void call_game(Guest *g, std::uint32_t handler, gaddr net, GuestRegs &out) {
    GuestRegs regs{};
    regs.a[0] = net;
    decomp::call_original(g, handler, regs);
    out.v0 = regs.v0;
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

input::Pad sample_mine() {
    const ext::SecondPad now = ext::first_pad_now();
    input::Pad mine;
    mine.connected = now.connected;
    mine.buttons = now.buttons;
    mine.extra = now.extra;
    mine.lx = now.analog_x;
    mine.ly = now.analog_y;
    mine.rx = now.right_x;
    mine.ry = now.right_y;
    camera::send_mouse(mine);
    if ((mine.extra & ext::kExtraR3) != 0u) mine.buttons |= kSelect;
    return mine;
}

// A battle frame with rollback: both pads at once, the other's guessed when it
// has not come, and back to the earliest frame whose guess turned out wrong.
void run_ahead(Guest *g, gaddr net, std::uint32_t frame, GuestRegs &out) {
    State &s = state();
    Rollback &r = rollback();
    const bool rerun = r.rerun_until != 0u && frame < r.rerun_until;
    if (!rerun && r.rerun_until != 0u) {
        // Back at the frame the game had reached: shown and heard again.
        r.rerun_until = 0u;
        ext::set_simulate_only(false);
    }
    std::uint64_t certain = 0;
    if (!rerun) {
        if (const std::optional<std::uint64_t> wrong = net::battle::take_wrong_guess(); wrong && *wrong < frame) {
            // Back to it once this call returns (after_dispatch): nothing of
            // this frame has run yet.
            r.restore_to = static_cast<std::uint32_t>(*wrong);
            r.rerun_until = frame;
            out.v0 = 0u;
            return;
        }
        certain = net::battle::confirmed();
    }
    bool guessed = false;
    const net::battle::Pads pads = net::battle::exchange_ahead(frame, rerun ? input::Pad{} : sample_mine(), rerun, guessed);
    if (pads.lost) {
        if (pads.left) std::cout << "[comm-battle] this copy's player left the battle at frame " << frame << std::endl;
        else std::cerr << "[comm-battle] the other fighter's copy is gone at frame " << frame << std::endl;
        ext::set_simulate_only(false);
        return quit(g, net, frame, out);
    }
    if (frame > 0u && frame % 10u == 0u) {
        g_call1(g, kCheckStep, net);
        const std::uint32_t count = g_ld32(g, net + kCheckCount);
        const gaddr newest = net + kChecks + (count - 1u) * 8u;
        if (count != 0u && g_ld32(g, newest) == frame) r.checks[frame] = g_ld32(g, newest + 4u);
    }
    // The check values of frames whose inputs are all certain: run with the
    // real inputs, so the two copies' must agree.
    for (auto it = r.checks.begin(); !rerun && it != r.checks.end() && it->first < certain;) {
        if (!net::battle::check(it->first, it->second)) {
            std::cerr << "[comm-battle] the two battles differ; it ends at frame " << frame << std::endl;
            ext::set_simulate_only(false);
            return quit(g, net, frame, out);
        }
        it = r.checks.erase(it);
    }
    for (int player = 0; player < 2; ++player) {
        inject(g, player, pads.player[player]);
        camera::linked_mouse(player, pads.player[player]);
    }
    if (!rerun) note_frame_step(g, frame);
    s.frame = frame + 1u;
    g_st32(g, net + kFrame, frame + 1u);
    g_st32(g, net + kSampled, frame + 1u);
    g_st32(g, net + kPeerConfirmed, frame);
    if (!rerun && frame >= r.reported + 900u) {
        const auto ms = [](std::chrono::steady_clock::duration d, std::uint64_t n) {
            return n == 0u ? 0.0 : std::chrono::duration<double, std::milli>(d).count() / static_cast<double>(n);
        };
        const auto now = std::chrono::steady_clock::now();
        if (r.reported_at != std::chrono::steady_clock::time_point{})
            std::cout << "[comm-battle] frames " << r.reported << "-" << frame << " in "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(now - r.reported_at).count() << " ms"
                      << std::endl;
        r.reported_at = now;
        std::cout << "[comm-battle] rollback to frame " << frame << ": went back " << r.rollbacks << " times, "
                  << r.rerun << " frames run again, the most at once " << r.longest << "; a save "
                  << ms(r.save_time, frame) << " ms, a restore " << ms(r.restore_time, r.rollbacks) << " ms"
                  << std::endl;
        r.reported = frame;
    }
    out.v0 = 0u;
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
    if (frame != 0u) s.decided = false;
    if (ours && frame == 0u && !s.decided) {
        s.decided = true;
        s.watching = false;
        const std::uint32_t tag = tag_for(g);
        // A battle others watch (Group Battle's members who are not fighting)
        // goes to them on the leading fighter's stream, through the session
        // host's relay. Without one (online.battle_link) it stays on the game's
        // own exchange, which relays the fighters' pads to the watchers. Every
        // copy sees the same watchers and settings, so all choose alike.
        if (const std::uint32_t watching = g_ld32(g, net + kSpectators); watching != 0u) {
            std::cout << "[comm-battle] " << watching << (watching == 1u ? " copy watches" : " copies watch")
                      << " this battle"
                      << (net::battle::can_stream() ? ": they get the fighters' stream"
                                                     : ": it runs on the game's own exchange (no relay to stream through)")
                      << std::endl;
            if (!net::battle::can_stream()) {
                s.linked = false;
                return call_game(g, kHandler, net, out);
            }
        }
        // A new battle: its own link.
        rollback() = Rollback{};
        const int player = static_cast<int>(g_ld32(g, net + kLocal));
        s.linked = net::battle::open(player, tag);
        s.frame = 0u;
        camera::clear_linked_mouse();
        if (!s.linked) {
            std::cerr << "[comm-battle] no link to the other fighter's copy; the battle ends" << std::endl;
            // Why, before the game's own error box: the other copy differs, or
            // never answered.
            if (const std::string why = net::battle::why_not(); !why.empty()) net::battle::notice(why);
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
    if (net::battle::rollback_frames() != 0u) return run_ahead(g, net, frame, out);
    const ext::SecondPad now = ext::first_pad_now();
    input::Pad mine;
    mine.connected = now.connected;
    mine.buttons = now.buttons;
    mine.extra = now.extra;
    mine.lx = now.analog_x;
    mine.ly = now.analog_y;
    mine.rx = now.right_x;
    mine.ry = now.right_y;
    // The mouse in the pad sent, so both copies turn the camera by it.
    camera::send_mouse(mine);
    // R3 is the camera's recentre, SELECT, as on the port's own pad reads.
    if ((mine.extra & ext::kExtraR3) != 0u) mine.buttons |= kSelect;
    const net::battle::Pads pads = net::battle::exchange(frame, mine);
    if (pads.lost) {
        if (pads.left) std::cout << "[comm-battle] this copy's player left the battle at frame " << frame << std::endl;
        else std::cerr << "[comm-battle] the other fighter's copy is gone at frame " << frame << std::endl;
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
    for (int player = 0; player < 2; ++player) {
        inject(g, player, pads.player[player]);
        camera::linked_mouse(player, pads.player[player]);
    }
    note_frame_step(g, frame);
    s.frame = frame + 1u;
    g_st32(g, net + kFrame, frame + 1u);
    g_st32(g, net + kSampled, frame + 1u);
    g_st32(g, net + kPeerConfirmed, frame);
    out.v0 = 0u;
}

// A spectator's frame on the port's stream: both pads as the leading fighter
// used them, the check value every tenth frame compared with theirs, and never
// the game's wait. The stream ending before this frame (a fighter left, or
// the stream stopped) ends the battle with the game's error, as the fighters'.
void run_watch(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const gaddr net = in.gpr[4];
    const std::uint32_t flags = g_ld32(g, net + kFlags);
    State &s = state();
    const bool ours = (flags & (kOpen | kRunning)) == (kOpen | kRunning) && (flags & kQuit) == 0u &&
                      g_ld32(g, net + kRole) == 1u;
    const std::uint32_t frame = g_ld32(g, net + kFrame);
    if (!enabled()) return call_game(g, kWatchHandler, net, out);
    if (frame != 0u) s.decided = false;
    if (ours && frame == 0u && !s.decided) {
        s.decided = true;
        const std::uint32_t tag = tag_for(g);
        s.watching = false;
        s.linked = false;
        if (net::battle::can_stream()) {
            s.watching = net::battle::watch(tag);
            s.frame = 0u;
            camera::clear_linked_mouse();
            if (!s.watching) {
                std::cerr << "[comm-battle] cannot watch this battle; it ends" << std::endl;
                return quit(g, net, 0u, out);
            }
            std::cout << "[comm-battle] this copy watches the battle on the fighters' stream" << std::endl;
        } else {
            std::cout << "[comm-battle] this copy watches the battle on the game's own exchange" << std::endl;
        }
    }
    if (!ours || !s.watching || frame != s.frame) return call_game(g, kWatchHandler, net, out);
    const net::battle::Pads pads = net::battle::watch_frame(frame);
    if (pads.lost) {
        if (pads.left) std::cout << "[comm-battle] this copy's player stopped watching at frame " << frame << std::endl;
        else std::cout << "[comm-battle] the watched battle ends at frame " << frame << std::endl;
        if (const std::string why = net::battle::why_not(); !why.empty()) net::battle::notice(why);
        return quit(g, net, frame, out);
    }
    if (frame > 0u && frame % 10u == 0u) {
        g_call1(g, kCheckStep, net);
        const std::uint32_t count = g_ld32(g, net + kCheckCount);
        const gaddr newest = net + kChecks + (count - 1u) * 8u;
        if (count != 0u && g_ld32(g, newest) == frame && !net::battle::watch_check(frame, g_ld32(g, newest + 4u))) {
            std::cerr << "[comm-battle] this copy's battle differs from the fighters'; it ends at frame " << frame
                      << std::endl;
            return quit(g, net, frame, out);
        }
    }
    // Skipping ahead to the buffer: this frame run as fast as it can, not
    // shown or heard; the first frame after it shown again.
    if (pads.unseen && !s.unseen) {
        s.unseen = true;
        ext::set_simulate_only(true);
    } else if (!pads.unseen) {
        seen_again();
    }
    for (int player = 0; player < 2; ++player) {
        inject(g, player, pads.player[player]);
        camera::linked_mouse(player, pads.player[player]);
    }
    s.frame = frame + 1u;
    g_st32(g, net + kFrame, frame + 1u);
    out.v0 = 0u;
}

void run_leave_battle(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    State &s = state();
    seen_again();
    if (s.linked) {
        std::cout << "[comm-battle] this copy leaves the battle at frame " << s.frame << std::endl;
        net::battle::close(s.frame);
        s.linked = false;
    }
    if (s.watching) {
        std::cout << "[comm-battle] this copy leaves the watched battle at frame " << s.frame << std::endl;
        net::battle::unwatch();
        s.watching = false;
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

#if defined(PORTABLEKIT_HAS_RENDERER)
constexpr const char *kDelayKey = "online.input_delay";

// The Network page: the input delay of the battles this copy hosts (the
// listening copy's counts), kept in settings.ini as PortableKit keeps its
// own; it leaves this key alone. 0 is Auto, else 2 to 10 frames.
void network_rows() {
    namespace ui = portablekit::ui;
    static int frames = [] {
        const std::string text = net::setting("D012_NET_DELAY", kDelayKey);
        if (text.empty() || text == "auto") return 0;
        return std::clamp(std::atoi(text.c_str()), 2, 10);
    }();
    ui::section("Online battles");
    ui::RowOptions options;
    options.description = "How many frames pass between a button and the fighter acting, in online battles. Auto "
                          "sets it from the connection as the battle starts: 2 up to a round trip of about 100 ms, "
                          "more beyond. More smooths a poor connection; fewer responds sooner. When the two players' "
                          "settings differ, the higher counts.";
    if (const char *text = std::getenv("D012_NET_DELAY"); text != nullptr && *text != '\0') {
        options.disabled = true;
        options.note = "Set by D012_NET_DELAY";
    }
    const std::string shown = frames == 0 ? "Auto" : std::to_string(frames) + " frames";
    const int step = ui::choice_row("Input delay", shown, options);
    if (step == 0) return;
    // Auto, then 2 to 10: ten choices.
    const int index = ((frames == 0 ? 0 : frames - 1) + step + 10) % 10;
    frames = index == 0 ? 0 : index + 1;
    try {
        const std::filesystem::path dir = portablekit::install::user_data_directory();
        portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
        entries[kDelayKey] = frames == 0 ? "auto" : std::to_string(frames);
        portablekit::install::write_settings_file(dir, entries);
    } catch (const std::exception &e) {
        std::cerr << "[comm-battle] settings.ini not written for " << kDelayKey << ": " << e.what() << std::endl;
    }
}

// The port's lines at the head of a saved network log: what both fighters'
// copies must share, the battle's port and input delay, and why the last
// battle could not start.
void network_log_lines(std::vector<std::string> &lines) {
    for (const auto &[what, value] : net::battle::must_match_items())
        lines.push_back("must match: " + what + " = " + (value.empty() ? "-" : value));
    const std::string delay = net::setting("D012_NET_DELAY", kDelayKey);
    lines.push_back("battle: UDP " + std::to_string(net::battle::port()) + ", input delay " +
                    (delay.empty() ? "auto" : delay));
    if (const std::string why = net::battle::why_not(); !why.empty())
        lines.push_back("last battle that could not start: " + why);
}
#endif

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kHandler, "comm_battle_exchange", decomp::Module::Main, decomp::Returns::Integer, &run_handler},
            {kWatchHandler, "comm_battle_watch", decomp::Module::Main, decomp::Returns::Integer, &run_watch},
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
    // The handler's own count of the battle's frames goes back with the game.
    portablekit::snapshot::add(
        "comm battle", [] { return std::any(state().frame); },
        [](const std::any &any) { state().frame = std::any_cast<std::uint32_t>(any); });
    psprecomp::set_runtime_pre_dispatch_hook(
        [](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx, std::uint32_t pc, std::int32_t) {
            before_dispatch(rt, pc, ctx);
        });
    for (const ClockImport &clock : kClocks) wrap_clock(hle, clock);
    if (clock_offset_us() != 0u) wrap_peer_times(hle);
    // The release build's log is a file under Proton and Windows alike, and
    // a game closed from the system loses what is still buffered: unbuffered
    // whenever this copy is set up for network battles.
    if (net::battle::configured()) std::setvbuf(stdout, nullptr, _IONBF, 0);
    // A copy that hosts the session relays its battles: its router forwards
    // the battle port along with the server's.
    portablekit::adhoc_host_forward_also({true, net::battle::port()});
    net::battle::serve();
    // What both fighters' copies must share, or their battles part: the
    // program, mods that change the game (none yet: the mods folder holds music
    // only, which may differ; a kind of mod that changes the game sets this
    // item), and the test switches that change how a fighter or a camera
    // moves. The right-stick camera's own settings may differ: each copy sends
    // its player's (d012_camera.cpp). The compiler counts too, but only where
    // it changes the game's maths: clang and GCC builds (Windows, and Linux's
    // GCC program with its clang game code) use the port's own sinf and the
    // rest (d012_math.cpp) and play each other, while MSVC cannot replace the C
    // runtime's (communication-mode.md, section 16).
    const auto env = [](const char *name) {
        const char *value = std::getenv(name);
        return std::string(value != nullptr ? value : "");
    };
    net::battle::must_match("the port's version", source_hash());
#if defined(_MSC_VER)
    net::battle::must_match("the compiler the port was built with", "MSVC " + std::to_string(_MSC_FULL_VER));
#else
    net::battle::must_match("the compiler the port was built with", "clang or GCC, with the port's own maths");
#endif
    net::battle::must_match("mods that change the game", "");
    net::battle::must_match("the right-stick camera (D012_NO_RIGHT_STICK_CAMERA)", camera::enabled() ? "on" : "off");
    net::battle::must_match("the smooth stick (D012_NO_SMOOTH_STICK)", decomp::smooth_stick_on() ? "on" : "off");
    net::battle::must_match("analog walking in battle (D012_SMOOTH_STICK_BATTLE)", env("D012_SMOOTH_STICK_BATTLE"));
    net::battle::must_match("the camera follow (D012_CAMERA_FOLLOW_DEADZONE)", env("D012_CAMERA_FOLLOW_DEADZONE"));
#if defined(PORTABLEKIT_HAS_RENDERER)
    portablekit::ui::add_network_rows(&network_rows);
    portablekit::ui::add_network_log_lines(&network_log_lines);
#endif
}

void before_dispatch(psprecomp::Runtime &rt, std::uint32_t pc, const psprecomp::AllegrexContext &ctx) {
    if (pc != kHandler || !state().linked || net::battle::rollback_frames() == 0u) return;
    const std::uint32_t frame = rt.memory().load32(ctx.gpr[4] + kFrame);
    if (frame != state().frame) return;
    Rollback &r = rollback();
    const std::size_t size = net::battle::rollback_frames() + 4u;
    if (r.ring.size() != size) r.ring.resize(size);
    Rollback::Saved &slot = r.ring[frame % size];
    const auto began = std::chrono::steady_clock::now();
    portablekit::snapshot::save_into(rt, slot.state);
    r.save_time += std::chrono::steady_clock::now() - began;
    slot.frame = frame;
}

void after_dispatch(psprecomp::Runtime &rt) {
    Rollback &r = rollback();
    if (!r.restore_to) return;
    const std::uint32_t to = *r.restore_to;
    r.restore_to.reset();
    const auto slot = std::find_if(r.ring.begin(), r.ring.end(), [&](const Rollback::Saved &x) { return x.frame == to; });
    if (slot == r.ring.end() || !slot->state) {
        std::cerr << "[comm-battle] frame " << to << " is too far back to run again; the battles may part"
                  << std::endl;
        r.rerun_until = 0u;
        return;
    }
    const auto began = std::chrono::steady_clock::now();
    portablekit::snapshot::restore(rt, *slot->state, true);
    r.restore_time += std::chrono::steady_clock::now() - began;
    ext::set_simulate_only(true);
    ++r.rollbacks;
    r.rerun += r.rerun_until - to;
    r.longest = std::max<std::uint64_t>(r.longest, r.rerun_until - to);
}

bool active(Guest *g) {
    const State &s = state();
    if (!(s.linked && net::battle::open_now()) && !(s.watching && net::battle::watching())) return false;
    const gaddr session = g_ld32(g, kSession);
    if (session == 0u) return false;
    const gaddr net = session + kSessionNet;
    const std::uint32_t flags = g_ld32(g, net + kFlags);
    return (flags & (kOpen | kRunning)) == (kOpen | kRunning) && (flags & kQuit) == 0u &&
           g_ld32(g, net + kRole) == (s.watching ? 1u : 0u);
}

int local_player(Guest *g) {
    if (state().watching || !active(g)) return -1;
    return static_cast<int>(g_ld32(g, g_ld32(g, kSession) + kSessionNet + kLocal));
}

} // namespace d012::comm_battle
