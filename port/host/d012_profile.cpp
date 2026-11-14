// The PortableKit profile for Dissidia 012 Duodecim Final Fantasy, USA
// release ULUS10566 v1.00, and the European release ULES01505 v1.00, whose
// disc holds the same executable (docs/port/regions.md).
//
// Constants only, as PortableKit's profile guide asks. Every value here is a
// fact about the game's own files (hashes, header words, addresses from the
// executable's program headers) or a choice of this port's names. Nothing in
// this file is derived from the game's code.
//
// MIT licence; see port/README.md.

#include "profile.hpp"

#include "gpu/frame_interpolation.hpp"

#include "d012_button_prompts.hpp"
#include "d012_cheats.hpp"
#include "d012_hle.hpp"
#include "d012_no_window.hpp"
#include "d012_version.hpp"

#include <algorithm>
#include <iterator>
#include <string>
#include <string_view>

namespace portablekit {
namespace {

// The game's ad hoc groups as the Network page names them
// (docs/port/communication-mode.md, sections 1 to 3): the Online Lobby's
// lobby n joins "LOBBY<n - 1>" and the list names it; Group Battle's rooms are
// listed in "MLOBBY"; a 1-on-1 battle and a filled Group Battle room are the
// host's 8 random digits; Communications exchanges data in "DISCOVER".
constexpr const char *kLobbyNames[] = {
    "Cornelia", "Fynn",    "Palamecia", "Sasune", "Baron",      "Tycoon",    "Bal",    "Figaro",
    "Midgar",   "Balamb",  "Galbadia",  "Esthar", "Alexandria", "Lindblum",  "Besaid", "Zanarkand",
};

std::string adhoc_group_name(std::string_view group) {
    const auto digits = [](std::string_view text) {
        return !text.empty() && text.size() <= 8u &&
               std::all_of(text.begin(), text.end(), [](char c) { return c >= '0' && c <= '9'; });
    };
    if (group.size() > 5u && group.substr(0, 5u) == "LOBBY" && digits(group.substr(5u))) {
        const int number = std::stoi(std::string(group.substr(5u))) + 1;
        std::string name = "Lobby " + std::to_string(number);
        if (number <= static_cast<int>(std::size(kLobbyNames))) name += std::string(" ") + kLobbyNames[number - 1];
        return name;
    }
    if (group == "MLOBBY") return "Group Battle rooms";
    if (group.size() == 8u && digits(group)) return "A battle or party room";
    if (group == "DISCOVER") return "Communications";
    return {};
}

// The game names its saves "ms0:/PSP/SAVEDATA/%s%s": the disc id followed by
// the slot. Slots 00 and 01 were seen in the emulator; whether the game uses
// more than two is not checked yet.
constexpr SaveFolder kSaveFolders[] = {
    {"ULUS10566GameData00", "Save slot 1", true},
    {"ULUS10566GameData01", "Save slot 2", true},
};

// The European disc: the same executable, overlays and data files, byte for
// byte, but for its encrypted EBOOT.BIN and the header of PACKAGE.BIN. The
// game takes its disc id from that header, so on this disc it saves as
// ULES01505 and reads DLC from PSP/GAME/ULES01505.
constexpr SaveFolder kEuropeanSaveFolders[] = {
    {"ULES01505GameData00", "Save slot 1", true},
    {"ULES01505GameData01", "Save slot 2", true},
};

constexpr RegionRelease kRegionReleases[] = {
    {.disc_id = "ULES01505",
     .disc_id_display = "ULES-01505",
     .release_name = "the European release",
     // sha256 of PSP_GAME/SYSDIR/EBOOT.BIN on the European disc; it
     // decrypts to executable_sha256 below.
     .encrypted_executable_sha256 = "0c0c12c0f46e9cc37adb0a649a761975fab92a20c8d2e9252f514ef29d1ca289",
     .save_game_name = "ULES01505",
     .save_folders = kEuropeanSaveFolders},
};

// Releases a player may have instead. The Japanese one is built from other
// code (docs/port/regions.md); the Asian one, sold under two serials, was
// not looked at.
constexpr OtherRelease kOtherReleases[] = {
    {"NPJH50377", "This is the Japanese release, a different build of the game; only the North American and "
                  "European releases work."},
    {"UCAS40329", "This is the Asian release, which has not been checked; only the North American and European "
                  "releases work."},
    {"UCKS45166", "This is the Asian release, which has not been checked; only the North American and European "
                  "releases work."},
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

// Pixel art drawn sharp whatever the Texture filter setting says. The chocobo
// on the white loading screen between battles and menus, with its note,
// carrot, chest and Zzz bubbles: a 64x128 texture whose key is as Dump
// textures names it. Smoothed at a higher resolution, it blurred.
constexpr SharpTexture kSharpTextures[] = {
    {"0000000042af9f8c2c87379b", 64u, 128u},
};


// Battle moves made from two buttons together, one key each (Controls >
// Keyboard and mouse): the dodge, dash, EX Mode, summon and the two assists.
constexpr std::uint32_t kL = 0x0100u, kR = 0x0200u, kTriangle = 0x1000u, kCircle = 0x2000u, kCross = 0x4000u,
                        kSquare = 0x8000u;
constexpr ComboControl kCombos[] = {
    {"dodge", "Dodge (R + ×)", kR | kCross, input::key(6)},                  // C
    {"dash", "Dash (R + △)", kR | kTriangle, input::key(21)},                // R
    {"ex_mode", "EX Mode (R + □)", kR | kSquare, input::key(10)},            // G
    {"summon", "Summon (R + ○)", kR | kCircle, input::key(25)},              // V
    {"brave_assist", "Brave assist (L + ○)", kL | kCircle, input::key(29)},  // Z
    {"hp_assist", "HP assist (L + □)", kL | kSquare, input::key(27)},        // X
};

// The mouse wheel moves through the game's lists, as D-pad up and down.
void keyboard_defaults(input::Bindings &bindings, bool &) {
    bindings[static_cast<std::size_t>(input::Action::Up)][1] = input::kWheelUp;
    bindings[static_cast<std::size_t>(input::Action::Down)][1] = input::kWheelDown;
}

} // namespace

const GameProfile &game() {
    static const GameProfile profile{
        .app_name = "D012Native",
        .project_name = "Dissidia 012 Native",
        .project_version = d012::kVersion,
        .env_prefix = "D012",
        .data_organization = "Decomp-Dissidia",
        .data_application = "D012Native",

        .disc_id = "ULUS10566",
        .disc_id_display = "ULUS-10566",
        .game_title = "Dissidia 012 Duodecim Final Fantasy",
        .executable_path_on_disc = "PSP_GAME/SYSDIR/EBOOT.BIN",
        .param_sfo_path_on_disc = "PSP_GAME/PARAM.SFO",
        .release_name = "the North American release",
        .region_releases = kRegionReleases,
        .other_releases = kOtherReleases,
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

        // Both releases announce the North American code, so a European copy
        // and a North American one can find each other.
        .adhoc_product_code = "ULUS10566",
        .adhoc_group_name = adhoc_group_name,

        // The keyboard: the mouse wheel on the D-pad, and a key each for the
        // battle moves made from two buttons together.
        .keyboard_defaults = &keyboard_defaults,
        .combos = kCombos,

        // L2 and R2, which a PSP lacks, switch the loading screen's D-pad
        // between its menu and the battle music (d012_bgm_playlist.cpp); R3
        // recentres the battle camera (d012_camera.cpp).
        .extra_buttons = input::kExtraL2 | input::kExtraR2 | input::kExtraR3,

        .register_extra_hle = &d012::register_extra_hle,
        .patch_loaded_image = &d012::patch_loaded_image,
        // No window (no working Vulkan driver, issue 24): the setup's build
        // says why and ends (d012_no_window.cpp).
        .renderer_unavailable = &d012::renderer_unavailable,

        // Battles pace their 30 Hz logic on two vblanks (main:0x08883F9C,
        // frame timer interval 2).
        .interpolation_thresholds = &kInterpolationThresholds,
        .frame_vblanks = 2u,

        // A USA console: English, confirm on cross.
        .system_language = 1u,
        .confirm_button = 1u,

        .sharp_textures = kSharpTextures,

        // The game's button symbols drawn as the gamepad's: Controls > Button
        // prompts (d012_button_prompts.cpp).
        .edited_textures = d012::button_prompt_textures(),
        .edit_texture = &d012::draw_button_prompts,
        .button_prompts = true,
        // Local versus: Controls > Player 2 (d012_versus.cpp).
        .second_player = true,

        .cheats = d012::builtin_cheats(),
    };
    return profile;
}

} // namespace portablekit
