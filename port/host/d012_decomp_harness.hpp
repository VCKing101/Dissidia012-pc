#pragma once

// Decompiled functions as native overrides, and the differential test that
// checks them against the recompiled originals. MIT licence; see
// port/README.md.
//
// A decompiled function (port/decomp/) is registered at its guest address
// with Runtime::register_function, so a call that reaches it through
// dispatch runs the C instead of the recompiled code. Calls that a generated
// unit makes to a function in the same 16 KiB unit are inline gotos and still
// run the original: an override only takes the calls from other units (see
// docs/port/bgm-playlist-design.md, "Hook points").
//
// The differential test (D012_DECOMP_VERIFY=1) runs, at a call, the original
// and the decompiled function from the same guest state in a second,
// isolated Runtime (the reference): the whole of guest RAM and VRAM copied
// from the game, the caller's registers, the main corpus and the overlay
// library resident in the game. System calls there are recorded, answer 0 and
// touch nothing. It then compares the results ($v0/$v1 or $f0, the
// callee-saved registers, $sp), every byte of RAM except the dead stack below
// the caller's $sp, and the list of system calls with their arguments. The
// game itself is never touched by the test: it goes on with the decompiled
// function (or with the original, with D012_DECOMP_VERIFY_ONLY=1).

#include "../decomp/guest.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace d012::decomp {

// The caller's registers at the call.
struct CallState {
    std::array<std::uint32_t, 32> gpr{};
    std::array<float, 32> fpr{};
};

enum class Returns { Nothing, Integer, Float };
enum class Module { Main, BattleOverlay };

// Runs the decompiled function on `g` with the arguments in `in`; puts what
// it returns in `out` (v0/v1/f0).
using DecompFunction = void (*)(Guest *g, const CallState &in, GuestRegs &out);
// Called before an override runs while the test is on: further checks of
// decompiled helpers from the same state (verify_probe).
using ProbeHook = void (*)(const CallState &in);

struct Override {
    std::uint32_t address;
    const char *name;
    Module module;
    Returns returns;
    DecompFunction run;
    // false: the function is only checked (through probes), never put in
    // place of the original. For functions whose callees may block the
    // calling thread, which a native override cannot allow.
    bool live = true;
    ProbeHook probes = nullptr;
    // Set when `run` is a port replacement that does not behave as the
    // original (the BGM playlist's list and selector): the game runs `run`,
    // and the differential test compares this, the equivalent decompiled
    // function, with the original instead.
    DecompFunction check = nullptr;
};

// Called once, before the game runs (static initialisation of the
// registering file), with `enable` false when none of them is wanted; the
// overrides go in when the game first misses a function, which is when the
// first overlay loads. D012_DECOMP_VERIFY=1 tests them.
void add_overrides(const Override *overrides, std::size_t count, bool enable);

// Checks the decompiled `run` against the original at `address`, called with
// the argument registers in `arguments` (a0-a3, t0-t3, f12-f15) and the rest
// of the state of the current call. Only while the test is on.
void verify_probe(const char *name, std::uint32_t address, Returns returns, DecompFunction run,
                  const CallState &caller, const GuestRegs &arguments);
[[nodiscard]] bool verifying();

// A guest function that the reference runtime replaces with a record of the
// call and a fixed answer, for both the original and the decompiled run: the
// file layer, whose synchronous reads wait frames for the I/O the reference
// does not do.
struct ReferenceStub {
    std::uint32_t address;
    const char *name;
    std::uint32_t v0;
    int arguments;            // how many of $a0-$a3 are recorded
    bool path_first = false;  // $a0 is a path: record the text, not where it is
};
void add_reference_stubs(const ReferenceStub *stubs, std::size_t count);
// A word of the game's memory, for probes that need to know where to look.
[[nodiscard]] std::uint32_t live_load32(std::uint32_t address);

} // namespace d012::decomp
