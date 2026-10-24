// The PortableKit profile for Dissidia 012 Duodecim Final Fantasy, USA
// release ULUS10566 v1.00.
//
// Constants only, as PortableKit's profile guide asks. Every value here is a
// fact about the game's own files (hashes, header words, addresses from the
// executable's program headers) or a choice of this port's names. Nothing in
// this file is derived from the game's code.
//
// MIT licence; see port/README.md.

#include "profile.hpp"

#include "gpu/frame_interpolation.hpp"

#include "d012_hle.hpp"

namespace portablekit {
namespace {

// The game names its saves "ms0:/PSP/SAVEDATA/%s%s": the disc id followed by
// the slot. Slots 00 and 01 were seen in the emulator; whether the game uses
// more than two is not checked yet.
constexpr SaveFolder kSaveFolders[] = {
    {"ULUS10566GameData00", "Save slot 1", true},
    {"ULUS10566GameData01", "Save slot 2", true},
};

// One overlay window. The game loads OVL_MENU_APP.ELF, OVL_BATTLE_APP.ELF or
// OVL_EXTRA_APP.ELF there; each is one PT_LOAD segment at 0x089DD4C0. The
// end is the end of the largest (battle, 1,596,080 bytes) rounded up to the
// next section of main.
constexpr std::uint32_t kOverlaySlots[] = {0x089DD4C0u, 0x08B62F70u};

// Frame interpolation's distances, in the game's world units. Battle's world
// is in metres or close to it: the near fighter is drawn about 6.4 units from
// the eye, the far one 12.7, the arena's edges about 40 (a display list of a
// Quick Battle, frame 15150 of the route in docs/port/bring-up-log.md). The
// framework's defaults were measured on a world about a hundred times finer.
// Measured in battle (<prefix>_TRACE_INTERPOLATION=frames): the camera moves
// up to 2.7 units a game frame while it follows a dash; effect draws paired
// with the wrong instance of their mesh jump 20 to 40 units with the camera
// standing still. The angles and the matched fraction are the defaults.
constexpr gpu::interpolation::CutThresholds kInterpolationThresholds{
    .max_camera_distance = 4.0f,
    .continuous_distance_margin = 0.6f,
    .max_continuous_distance = 8.0f,
    .max_own_motion = 4.0f,
};

} // namespace

const GameProfile &game() {
    static const GameProfile profile{
        .app_name = "D012Native",
        .project_name = "Dissidia 012 Native",
        .env_prefix = "D012",
        .data_organization = "Decomp-Dissidia",
        .data_application = "D012Native",

        .disc_id = "ULUS10566",
        .disc_id_display = "ULUS-10566",
        .game_title = "Dissidia 012 Duodecim Final Fantasy",
        .executable_path_on_disc = "PSP_GAME/SYSDIR/EBOOT.BIN",
        .param_sfo_path_on_disc = "PSP_GAME/PARAM.SFO",
        .release_name = "the North American release (ULUS10566 v1.00)",
        // sha256 of PSP_GAME/SYSDIR/EBOOT.BIN on the disc, and of its
        // decryption (build/ULUS10566/EBOOT.ELF, tools/decrypt_main.sh).
        .encrypted_executable_sha256 = "8fbf3541ab121aff85aae72795760736dfe37706996b2b0f9b81d36fc98f3394",
        .executable_sha256 = "0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d",
        // The word at 0xD0 of EBOOT.BIN and the key that tag selects, the
        // same public value pspdecrypt and the PSP emulators carry; with it
        // --install image.iso decrypts the disc's EBOOT.BIN itself, so a
        // player needs only their own disc image (owner's decision,
        // 24 October 2026).
        .decryption_tag = 0xD91613F0u,
        .decryption_key = {0xEB, 0xFF, 0x40, 0xD8, 0xB4, 0x1A, 0xE1, 0x66,
                           0x91, 0x3B, 0x8F, 0x64, 0xB6, 0xFC, 0xB7, 0x12},

        .load_base = 0x08804000u,
        .guest_ram_bytes = 32u * 1024u * 1024u,
        .boot_path = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN",
        .overlay_slots = kOverlaySlots,

        .save_game_name = "ULUS10566",
        .save_folders = kSaveFolders,

        .adhoc_product_code = "ULUS10566",

        // L2 and R2, which a PSP lacks, switch the loading screen's D-pad
        // between its menu and the battle music (d012_bgm_playlist.cpp); R3
        // recentres the battle camera (d012_camera.cpp).
        .extra_buttons = input::kExtraL2 | input::kExtraR2 | input::kExtraR3,

        .register_extra_hle = &d012::register_extra_hle,
        .patch_loaded_image = &d012::patch_loaded_image,

        // Battles pace their 30 Hz logic on two vblanks (main:0x08883F9C,
        // frame timer interval 2).
        .interpolation_thresholds = &kInterpolationThresholds,
        .frame_vblanks = 2u,

        // A USA console: English, confirm on cross.
        .system_language = 1u,
        .confirm_button = 1u,
    };
    return profile;
}

} // namespace portablekit
