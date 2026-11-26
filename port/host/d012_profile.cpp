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
#include "d012_disc_mod.hpp"
#include "d012_hle.hpp"
#include "d012_no_window.hpp"
#include "d012_online.hpp"
#include "d012_source_hash.hpp"
#include "d012_version.hpp"

#include <algorithm>
#include <iterator>
#include <string>
#include <string_view>

#if defined(PORTABLEKIT_ANDROID_APP) && defined(D012_INSTALLER_BUILD)
#include <filesystem>
namespace portablekit::install {
class InstallerUi;
}
namespace d012::device_compile {
// d012_device_compile.cpp
bool prepare(portablekit::install::InstallerUi *ui, const std::filesystem::path &executable,
             const std::filesystem::path &disc_image);
}
#elif defined(__linux__) && defined(D012_INSTALLER_BUILD)
#include "d012_update_install.hpp"
#elif defined(_WIN32) && defined(D012_INSTALLER_BUILD)
#include <filesystem>
namespace portablekit::install {
class InstallerUi;
}
namespace d012::game_code {
// d012_game_code.cpp
bool prepare_at_start(portablekit::install::InstallerUi *ui, const std::filesystem::path &executable,
                      const std::filesystem::path &disc_image);
}
#endif

#include <cstddef>

// The menu's faces, compiled in by port/CMakeLists.txt from fonts/.
namespace d012 {
extern const unsigned char kHeadingFont[];
extern const std::size_t kHeadingFontSize;
extern const unsigned char kBodyFont[];
extern const std::size_t kBodyFontSize;
extern const unsigned char kCapsFont[];
extern const std::size_t kCapsFontSize;
} // namespace d012

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

// The tag this copy gives the ad hoc server (GameProfile::adhoc_version): what
// a battle's start needs both players to share (d012_comm_battle.cpp), the
// port's source and the compiler's kind, so that a public server shows each
// player only the players they can battle (docs/port/online-matchmaking.md,
// section 8).
std::string adhoc_version() {
    std::string tag = std::string(d012::source_hash()).substr(0, 16u);
#if defined(_MSC_VER)
    tag += "-msvc" + std::to_string(_MSC_FULL_VER);
#else
    tag += "-std";
#endif
    return tag;
}

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

// The menu's look (docs/port/menu.md): night blue and silver, a crystal-blue
// bar on the focused row and the title's 012 in crimson; Cinzel for titles,
// Barlow for text (port/CMakeLists.txt embeds them).
const InterfaceTheme &menu_theme() {
    static const InterfaceTheme theme{
        .backdrop = 0x060916EEu,
        .panel = 0x0D1430F2u,
        .edge = 0xDFE4F038u,
        .row_hover = 0x8FA6FF1Au,
        .focus = 0x3A5EE8D9u,
        .focus_end = 0x2B4AD214u,
        .accent = 0x4A6CF0FFu,
        .highlight = 0xD4404FFFu,
        .heading = 0xEEF1F8FFu,
        .section = 0x8FA6FFFFu,
        .text = 0xE4E8F3FFu,
        .text_dim = 0xA9B4D0FFu,
        .text_faint = 0x7F8AA8FFu,
        .text_disabled = 0x5D6680FFu,
        .value = 0xC9D1E6FFu,
        .danger = 0xE46A78FFu,
        .good = 0x8FD19EFFu,
        .track = 0xDFE4F029u,
        .heading_font = {d012::kHeadingFont, d012::kHeadingFontSize},
        .body_font = {d012::kBodyFont, d012::kBodyFontSize},
        .caps_font = {d012::kCapsFont, d012::kCapsFontSize},
        .title = "Dissidia",
        .title_accent = "012",
        .subtitle = "Duodecim Final Fantasy",
    };
    return theme;
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
        // A PSP-2000's 64 MiB. The game's own blocks stay in the first 32
        // MiB, a PSP-1000's: the rest is kept from the kernel for the port
        // (d012_model_memory.hpp).
        .guest_ram_bytes = 64u * 1024u * 1024u,
        .boot_path = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN",
        .overlay_slots = kOverlaySlots,

        .save_game_name = "ULUS10566",
        .save_folders = kSaveFolders,

        // Both releases announce the North American code, so a European copy
        // and a North American one can find each other.
        .adhoc_product_code = "ULUS10566",
        .adhoc_group_name = adhoc_group_name,
        .adhoc_version = adhoc_version,
        // Play online (d012_online.hpp, docs/port/online-matchmaking.md).
        .public_server = d012::online::public_server,
        .public_server_players = d012::online::players,
        // Asked from the Communications Mode menu instead, where Back returns
        // to the menu (d012_online_menu.cpp).
        .adhoc_choice_at_init = false,

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
#if defined(PORTABLEKIT_ANDROID_APP) && defined(D012_INSTALLER_BUILD)
        // The player's app makes the game's code on the phone from the disc
        // (d012_device_compile.cpp).
        .prepare_game = &d012::device_compile::prepare,
#elif defined(__linux__) && defined(D012_INSTALLER_BUILD)
        // A copy the Linux setup installed makes the game's code itself after
        // an update from the menu (d012_update_install.cpp).
        .prepare_game = &d012::update_install::prepare_after_update,
#elif defined(_WIN32) && defined(D012_INSTALLER_BUILD)
        // A copy without its game code (the release's zip started by itself,
        // or a setup whose compile failed) makes it on the setup's screens
        // (d012_game_code.cpp).
        .prepare_game = &d012::game_code::prepare_at_start,
#endif
        // A modded disc, switched on on the System page (d012_disc_mod.cpp).
        // After prepare_game, as GameProfile declares them: GCC refuses
        // designators out of order.
        .patched_disc = &d012::disc_mod::disc_for_this_start,

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

        // The menu: its look, and what it says this game does with the
        // right stick, the mouse and L2 and R2 (d012_camera.cpp,
        // d012_bgm_playlist.cpp), and which of its text the font changes.
        .theme = &menu_theme(),
        .right_stick_camera = "the stick turns the camera in battle and on the world map, and up or down moves it "
                              "closer or further. A full push turns it as fast as the game's own camera speed "
                              "option; R3 recentres it in battle.",
        .mouse_camera = "The free camera in battle and the world map turn by these degrees, and moving the mouse up "
                        "or down moves the camera closer or further. Locked on, the mouse turns the camera as the "
                        "right stick does.",
        .extra_triggers = "In this game L2 and R2 switch the loading screen's D-pad between the menu music and the "
                          "battle music.",
        .font_preview_text = "Quick Battle  Cloud Strife  9,999 Gil  装備 アイテム",
        .font_note = "It changes the game's sentence text: descriptions, lists, help lines, the names you type, and "
                     "the music selector. Titles, fighter names, YES and NO, and battle tips are pictures and stay as "
                     "they are.",
    };
    return profile;
}

} // namespace portablekit
