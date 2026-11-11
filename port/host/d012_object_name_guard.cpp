// An object's file name read from an empty costume row (issue 19). MIT
// licence; see port/README.md.
//
// The object loader (main 0x08845230) loads a character's model, then the
// costume's parameters. For those it looks up the costume's own row in
// objentry.bin: 0x088470DC(character, costume, 0), the first row of that
// character and costume that is not an assist's, whose name starts "P_" and
// does not end "_W" (0x08846CCC). It uses the row's name, at +0x24, without
// checking that a row was found, to build "obj/<name>.cosx" (0x08844FA4,
// format "obj/%s.%s" at 0x089C88D8). The game's own rows always have one; a
// row put in by DLC or a cheat that fails the lookup itself (an assist's
// flag, a name without "P_") leaves it with none, and the name is read from
// address 0x24. Issue 19 crashed there, choosing Cloud in his Alt 1 look:
// "Guest memory access outside PSP RAM/EDRAM at 0x00000024" in the game's
// sprintf.
//
// PPSSPP reads such an address as zero, so the name is empty: the loader asks
// for "obj/.cosx", finds no such file and goes on without the parameters
// (0x088453D4), and the costume's other lookups are behind the same tests.
// This does the same: a name that is not in guest RAM is read as the empty
// string, and each such object is logged once. Every other call runs the
// original as it is.
//
// The loader calls 0x08844FA4 inside their own 16 KiB unit, which an override
// does not see by itself: port/config/dispatch-calls.txt lists it, so the
// generated code calls it through the runtime's lookup. With a corpus made
// without that row the override is never reached and the crash is as before.
// D012_OBJECT_NAME_GUARD=0 turns it off. D012_TRACE_OBJECT_NAMES=1 logs every
// object file name the game builds, and the object when the loader asks.
#include "d012_decomp_harness.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <string>

namespace d012 {
namespace {

namespace decomp = d012::decomp;

constexpr std::uint32_t kObjectPath = 0x08844FA4u;  // (name, extension) -> "obj/<name>.<extension>"
constexpr std::uint32_t kFormat = 0x089C88D8u;      // "obj/%s.%s"
constexpr std::uint32_t kEmpty = kFormat + 9u;      // the format's own terminating zero: an empty name
// The loader, 1,968 bytes: while it runs, $s1 holds the object, its row at +0.
constexpr std::uint32_t kLoader = 0x08845230u;
constexpr std::uint32_t kLoaderEnd = 0x088459E0u;
constexpr unsigned kMaxLogs = 16u;

bool enabled() {
    const char *text = std::getenv("D012_OBJECT_NAME_GUARD");
    return text == nullptr || std::strcmp(text, "0") != 0;
}

// D012_TRACE_OBJECT_NAMES=1: every object file name, with the object's row
// when the loader asks.
bool tracing() {
    static const bool on = [] {
        const char *text = std::getenv("D012_TRACE_OBJECT_NAMES");
        return text != nullptr && *text != '\0' && std::strcmp(text, "0") != 0;
    }();
    return on;
}

// As g_ptr() takes it: anything else is outside guest RAM.
bool in_ram(const Guest *g, std::uint32_t a, std::uint32_t length) {
    const std::uint32_t offset = (a & 0x1FFFFFFFu) - GUEST_RAM_BASE;
    return offset < g->ram_size && g->ram_size - offset >= length;
}

std::string text_at(Guest *g, std::uint32_t a, std::uint32_t most) {
    std::string text;
    for (std::uint32_t i = 0; i < most && in_ram(g, a + i, 1u); ++i) {
        const char c = static_cast<char>(g_ld8(g, a + i));
        if (c == '\0') break;
        text += c >= 0x20 && c < 0x7F ? c : '?';
    }
    return text;
}

// The row of the object being loaded, when the loader is the caller; else 0.
std::uint32_t loader_row(Guest *g, const decomp::CallState &in) {
    const std::uint32_t ra = in.gpr[31];
    if (ra < kLoader || ra >= kLoaderEnd) return 0u;
    const std::uint32_t object = in.gpr[17];
    const std::uint32_t row = in_ram(g, object, 4u) ? g_ld32(g, object) : 0u;
    return in_ram(g, row, 0x44u) ? row : 0u;
}

std::string describe(Guest *g, std::uint32_t row) {
    char text[96];
    std::snprintf(text, sizeof text, "object %d (%s, character %d, costume %d, flags %02X)", g_ld16s(g, row),
                  text_at(g, row + 0x24u, 16u).c_str(), g_ld8s(g, row + 6u), g_ld8s(g, row + 7u), g_ld8(g, row + 8u));
    return text;
}

std::set<std::int32_t> g_logged_rows;
unsigned g_logs = 0;

void report(Guest *g, const decomp::CallState &in, const std::string &extension) {
    if (const std::uint32_t row = loader_row(g, in); row != 0u) {
        if (!g_logged_rows.insert(g_ld16s(g, row)).second || g_logs >= kMaxLogs) return;
        ++g_logs;
        std::printf("[objects] %s has no costume row of its own; its .%s is asked for as obj/.%s, as on PPSSPP\n",
                    describe(g, row).c_str(), extension.c_str(), extension.c_str());
    } else {
        if (g_logs >= kMaxLogs) return;
        ++g_logs;
        std::printf("[objects] an object file name at %08X (called from %08X) is read as empty, as on PPSSPP: "
                    "obj/.%s\n", in.gpr[4], in.gpr[31], extension.c_str());
    }
    std::fflush(stdout);
}

void run_object_path(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    const bool readable = in_ram(g, regs.a[0], 1u);
    const std::string extension = in_ram(g, regs.a[1], 1u) ? text_at(g, regs.a[1], 8u) : "?";
    if (tracing()) {
        const std::uint32_t row = loader_row(g, in);
        std::printf("[objects] trace: obj/%s.%s from %08X%s%s\n",
                    readable ? text_at(g, regs.a[0], 32u).c_str() : "(none)", extension.c_str(), in.gpr[31],
                    row != 0u ? ", " : "", row != 0u ? describe(g, row).c_str() : "");
        std::fflush(stdout);
    }
    if (!readable) {
        report(g, in, extension);
        regs.a[0] = kEmpty;
    }
    decomp::call_original(g, kObjectPath, regs);
    out.v0 = regs.v0;
}

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kObjectPath, "object_file_name", decomp::Module::Main, decomp::Returns::Integer, &run_object_path},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], enabled());
    }
} install_overrides;

} // namespace
} // namespace d012
