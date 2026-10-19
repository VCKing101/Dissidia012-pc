// Dissidia 012's own HLE additions and diagnostics, reached through the
// profile's hooks. MIT licence; see port/README.md.

#include "d012_bgm_playlist.hpp"
#include "d012_hle.hpp"
#include "d012_mods.hpp"

#include "hle/hle_common.hpp"
#include "profile.hpp"

#include "psprecomp/common.hpp"
#include "psprecomp/elf32.hpp"
#include "psprecomp/runtime.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace d012 {
namespace {

using portablekit::arg;
using portablekit::kernel;
using psprecomp::AllegrexContext;
using psprecomp::Runtime;

// The overlay window (the profile's overlay_slots).
constexpr std::uint32_t kOverlayBase = 0x089DD4C0u;
constexpr std::uint32_t kOverlayEnd = 0x08B62F70u;

// D012_DUMP_SLOT=N writes the overlay window to D012_DUMP_SLOT_DIR (default
// the working directory) at the game's Nth vblank wait, as
// overlay_window_<N>.bin. tools/port/overlay_meta.py --check compares it with
// an overlay ELF to find the bytes the game writes into a loaded overlay,
// which the overlay's identity hash must leave out.
void dump_overlay_window(Runtime &rt, unsigned long frame) {
    const char *directory = portablekit::env("DUMP_SLOT_DIR");
    const std::filesystem::path dir = directory != nullptr && *directory != '\0' ? directory : ".";
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    const std::filesystem::path path = dir / ("overlay_window_" + std::to_string(frame) + ".bin");
    std::ofstream out(path, std::ios::binary);
    for (std::uint32_t address = kOverlayBase; address < kOverlayEnd; ++address) {
        const char byte = static_cast<char>(rt.memory().load8(address));
        out.write(&byte, 1);
    }
    std::cout << "[d012] overlay window at vblank wait " << frame << " -> " << path.string() << "\n";
}

void wrap_vblank_waits(portablekit::HleRegistrar &hle) {
    const char *text = portablekit::env("DUMP_SLOT");
    if (text == nullptr) return;
    const unsigned long at = std::strtoul(text, nullptr, 0);
    // sceDisplayWaitVblankStartCB and sceDisplayWaitVblankStart.
    for (const std::uint32_t nid : {0x46F186C3u, 0x984C27E7u}) {
        const portablekit::HleFunction previous = hle.bound_function("sceDisplay", nid);
        if (!previous) continue;
        hle.bind("sceDisplay", nid, [previous, at](Runtime &rt, AllegrexContext &ctx) {
            static unsigned long waits = 0;
            if (++waits == at) dump_overlay_window(rt, waits);
            previous(rt, ctx);
        });
    }
}

} // namespace

void register_extra_hle(portablekit::HleRegistrar &hle) {
    register_suppreacc(hle);

    // The development kit's GPI switches; a retail console reads 0. The game
    // reads them once at boot (main:0x08860D94).
    hle.add("UtilsForUser", "sceKernelGetGPI", [](Runtime &, AllegrexContext &ctx) { kernel().finish(ctx, 0u); });

    // libfont calls PortableKit does not answer, made by the game's font
    // set-up (main:0x08881B18..0x08881BE0). PortableKit serves one font, the
    // host's, at index 0 (sceFontGetNumFontList answers 1), so these answer
    // in the same terms: the alternative character is accepted, the list
    // entry the game asks for is left as the game initialised it, and the
    // font the game looks for is index 0 with no error.
    hle.add("sceLibFont", "sceFontSetAltCharacterCode", [](Runtime &, AllegrexContext &ctx) {
        kernel().finish(ctx, 0u);
    });
    hle.add("sceLibFont", "sceFontGetFontList", [](Runtime &, AllegrexContext &ctx) { kernel().finish(ctx, 0u); });
    hle.add("sceLibFont", "sceFontFindFont", [](Runtime &rt, AllegrexContext &ctx) {
        if (arg(ctx, 2) != 0u) rt.memory().store32(arg(ctx, 2), 0u);
        kernel().finish(ctx, 0u);
    });

    wrap_vblank_waits(hle);

    // Mods: music added to the game (d012_mods.cpp, docs/port/mod-folder.md).
    mods::install(hle);
    // The loading screen's music choice by series (d012_bgm_playlist.cpp).
    playlist::install(hle);
}

void patch_loaded_image(psprecomp::Runtime &runtime, const psprecomp::Elf32Image &elf) {
    (void)runtime;
    (void)elf;
}

} // namespace d012
