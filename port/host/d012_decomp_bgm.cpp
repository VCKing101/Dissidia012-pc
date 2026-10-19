// The decompiled BGM functions (port/decomp/bgm_*.c) as native overrides,
// behind D012_DECOMP_BGM=1, and the extra checks the differential test
// (D012_DECOMP_VERIFY=1) makes of them. See docs/port/bgm-playlist-design.md.
// MIT licence; see port/README.md.

#include "d012_bgm_playlist.hpp"
#include "d012_decomp_harness.hpp"

#include "../decomp/bgm_types.h"

#include <cstdlib>
#include <cstring>
#include <vector>

namespace d012::decomp {
namespace {

std::uint32_t a(const CallState &in, int i) { return in.gpr[4 + i]; }

void run_find_row(Guest *g, const CallState &in, GuestRegs &out) {
    out.v0 = static_cast<std::uint32_t>(bgm_table_find_row(g, a(in, 0), a(in, 1)));
}
void run_find(Guest *g, const CallState &in, GuestRegs &out) { out.v0 = bgm_table_find(g, a(in, 0), a(in, 1)); }
void run_name(Guest *g, const CallState &in, GuestRegs &out) {
    out.v0 = bgm_table_name(g, a(in, 0), static_cast<std::int32_t>(a(in, 1)));
}
void run_name_of(Guest *g, const CallState &in, GuestRegs &out) { out.v0 = bgm_table_name_of(g, a(in, 0), a(in, 1)); }
void run_play_id(Guest *g, const CallState &in, GuestRegs &out) { out.v0 = bgm_play_id(g, a(in, 0), a(in, 1)); }
void run_play_entry(Guest *g, const CallState &in, GuestRegs &out) { out.v0 = bgm_play_entry(g, a(in, 0), a(in, 1)); }
void run_stream_path(Guest *g, const CallState &in, GuestRegs &out) {
    out.v0 = bgm_stream_path(g, a(in, 0), a(in, 1), a(in, 2));
}
void run_stream_start(Guest *g, const CallState &in, GuestRegs &out) {
    out.v0 = bgm_stream_start(g, in.fpr[12], a(in, 0), a(in, 1), a(in, 2), a(in, 3));
}
void run_list(Guest *g, const CallState &in, GuestRegs &out) {
    out.v0 = static_cast<std::uint32_t>(
        bgm_list_offered(g, a(in, 0), a(in, 1), a(in, 2), static_cast<std::int32_t>(a(in, 3))));
}
void run_update(Guest *g, const CallState &in, GuestRegs &) { bgm_selector_update(g, a(in, 0), a(in, 1)); }
void run_make_name(Guest *g, const CallState &in, GuestRegs &) { bgm_selector_make_name(g, a(in, 0)); }
void run_chosen(Guest *g, const CallState &in, GuestRegs &out) {
    out.v0 = static_cast<std::uint32_t>(bgm_selector_chosen(g, a(in, 0)));
}

GuestRegs args(std::uint32_t a0, std::uint32_t a1 = 0, std::uint32_t a2 = 0, std::uint32_t a3 = 0) {
    GuestRegs r{};
    r.a[0] = a0;
    r.a[1] = a1;
    r.a[2] = a2;
    r.a[3] = a3;
    return r;
}

// From the selector's update: the chosen id and a remade name sprite.
void probe_selector(const CallState &in) {
    verify_probe("selector_chosen", 0x08AF5AC0u, Returns::Integer, run_chosen, in, args(a(in, 0)));
    verify_probe("selector_make_name", 0x08AF5AE8u, Returns::Nothing, run_make_name, in, args(a(in, 0)));
}

// From the list of choices: the table lookups for ids that are there, one
// that is a DLC-style id and one that is not there, and the stream path of
// rows from the disc.
void probe_table(const CallState &in) {
    const std::uint32_t table = live_load32(G_BGM_TABLE);
    if (table == 0) return;
    const std::uint32_t entries = live_load32(table + 8u);
    for (std::uint32_t id : {0u, 18u, 70u, 130u, 0x7FFFu, 0xFFFFFFFFu}) {
        verify_probe("table_find_row", 0x0889F5E8u, Returns::Integer, run_find_row, in, args(table, id));
        verify_probe("table_find", 0x0889F3E4u, Returns::Integer, run_find, in, args(table, id));
        verify_probe("table_name_of", 0x0889F4ACu, Returns::Integer, run_name_of, in, args(table, id));
    }
    // The request buffer of the stream player holds the path in the
    // reference only; the game's copy is untouched.
    for (std::uint32_t row : {1u, 33u, 66u}) {
        const std::uint32_t name = entries + row * sizeof(BgmEntry) + 0x1Cu;
        verify_probe("stream_path", 0x088A04A8u, Returns::Integer, run_stream_path, in,
                     args(G_STREAM_REQUEST, name, 0u));
        verify_probe("stream_path", 0x088A04A8u, Returns::Integer, run_stream_path, in,
                     args(G_STREAM_REQUEST, name, 1u));
    }
}

// From the start of a battle's music: the streaming start with the
// arguments main 0x0889F534 gives it.
void probe_stream(const CallState &in) {
    const std::uint32_t entry = a(in, 1);
    if (entry == 0) return;
    const std::uint32_t word = live_load32(entry + 0x0Cu);
    const std::int8_t volume = static_cast<std::int8_t>((word >> 8) & 0xFFu);
    const std::uint32_t from_ms0 = (word & 4u) != 0u;
    std::uint32_t hundredth_bits = 0x3C23D70Au;
    float hundredth;
    std::memcpy(&hundredth, &hundredth_bits, sizeof hundredth);
    GuestRegs r = args(0u, entry + 0x1Cu, 0u, from_ms0);
    r.f[0] = static_cast<float>(volume) * hundredth;
    verify_probe("stream_start", 0x0889FD38u, Returns::Integer, run_stream_start, in, r);
}

void probe_play_id(const CallState &in) {
    // The entry play_entry will get.
    CallState next = in;
    const std::uint32_t table = a(in, 0);
    const std::uint32_t count = live_load32(live_load32(table + 4u) + 8u);
    const std::uint32_t entries = live_load32(table + 8u);
    next.gpr[5] = 0;
    for (std::uint32_t row = 0; row < count; ++row) {
        const std::uint32_t word = live_load32(entries + row * sizeof(BgmEntry));
        if (static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(word & 0xFFFFu))) ==
            a(in, 1)) {
            next.gpr[5] = entries + row * sizeof(BgmEntry);
            break;
        }
    }
    verify_probe("play_entry", 0x0889F534u, Returns::Integer, run_play_entry, next, args(table, next.gpr[5]));
    probe_stream(next);
}

const Override kOverrides[] = {
    // main: the table and playback
    {0x0889F5E8u, "table_find_row", Module::Main, Returns::Integer, run_find_row},
    {0x0889F3E4u, "table_find", Module::Main, Returns::Integer, run_find},
    {0x0889F434u, "table_name", Module::Main, Returns::Integer, run_name},
    {0x0889F4ACu, "table_name_of", Module::Main, Returns::Integer, run_name_of},
    {0x0889F234u, "play_id", Module::Main, Returns::Integer, run_play_id, true, probe_play_id},
    {0x0889F534u, "play_entry", Module::Main, Returns::Integer, run_play_entry, true, probe_stream},
    {0x088A04A8u, "stream_path", Module::Main, Returns::Integer, run_stream_path},
    // Its callees stop the previous stream and may wait for a vblank on the
    // calling thread, which a native override cannot nest; checked through
    // probe_stream, and play_entry's tail call reaches the original.
    {0x0889FD38u, "stream_start", Module::Main, Returns::Integer, run_stream_start, false},
    // battle overlay: the loading screen's selector
    {0x08ADE4E0u, "list_offered", Module::BattleOverlay, Returns::Integer, run_list, true, probe_table},
    {0x08AF8CA4u, "selector_update", Module::BattleOverlay, Returns::Nothing, run_update, true, probe_selector},
    {0x08AF5AC0u, "selector_chosen", Module::BattleOverlay, Returns::Integer, run_chosen},
    // Called only from its own unit (inline), so an override would never run;
    // checked through probe_selector.
    {0x08AF5AE8u, "selector_make_name", Module::BattleOverlay, Returns::Nothing, run_make_name, false},
};

// The file layer, in the reference only: an open that answers handle 5 and
// a read that succeeds, with no I/O. The open records the path's text: the
// tail call in play_entry runs the stream start 0x10 bytes higher on the
// stack than the original does, so the path buffer is elsewhere.
const ReferenceStub kStubs[] = {
    {F_FILE_OPEN, "file_open", 5u, 2, true},
    {F_FILE_READ, "file_read", 1u, 4},
    {F_FILE_CLOSE, "file_close", 0u, 1},
};

bool flag(const char *name) {
    const char *value = std::getenv(name);
    return value != nullptr && *value != '\0' && std::strcmp(value, "0") != 0;
}

// D012_DECOMP_BGM=1 puts the twelve decompiled functions in (ten of them in
// the game). The BGM playlist, on unless D012_NO_BGM_PLAYLIST=1, replaces two
// of them: the list and the selector's frame run the playlist's code, and the
// differential test still checks the decompiled originals against the game's.
struct Install {
    Install() {
        const bool decompiled = flag("D012_DECOMP_BGM");
        const bool playlist = playlist::enabled();
        std::vector<Override> wanted;
        for (const Override &o : kOverrides) {
            Override entry = o;
            if (playlist && o.address == 0x08ADE4E0u) {
                entry.check = o.run;
                entry.run = &playlist::run_list;
            } else if (playlist && o.address == 0x08AF8CA4u) {
                entry.check = o.run;
                entry.run = &playlist::run_update;
            } else if (!decompiled) {
                continue;
            }
            wanted.push_back(entry);
        }
        add_overrides(wanted.data(), wanted.size(), decompiled || playlist);
        add_reference_stubs(kStubs, sizeof kStubs / sizeof kStubs[0]);
    }
} install;

} // namespace
} // namespace d012::decomp
