// A disc mod switched on and off (d012_disc_mod.hpp).
//
// The player chooses the mod as its makers hand it out: a patch to the disc
// (.xdelta, VCDIFF) or a disc image already patched (.iso). A patch is applied
// once to the game's own disc image, and an image is compared with it; either
// way only the sectors the mod changes are kept, in
// data/disc-mods/<name>.pkdisc (PortableKit's disc_patch.hpp). The game then
// reads that file as the modded disc, taking every other sector from its own
// image. No second image and no patching tool.
//
// Online, a mod's disc is known by its identity (the sectors it changes,
// whichever way it was prepared): its copies meet only copies playing the same
// mod (PortableKit's adhoc_version_tag()), and fight only them
// (d012_comm_battle.cpp).
//
// settings.ini keeps the mod (disc_mod.image: the .pkdisc or the .iso) and the
// switch (disc_mod.enabled, "1" or "0"). Both are read once, when the game
// starts: the game's code is set up for one disc, so a change takes a restart,
// which the System page offers. D012_PATCHED_DISC and D012_NO_PATCHED_DISC
// (PortableKit) decide over settings.ini for a test run.
// D012_TEST_DISC_PATCH=<patch> prepares that patch as the game starts, logs
// the result and goes on: a test of the preparation without the menu.
//
// A modded disc keeps its saves and downloadable content apart from the
// game's own, in the memory stick folder ms0-patched beside ms0 (PortableKit's
// main.cpp): a mod may store what the game's own code cannot read.
//
// MIT licence; see port/README.md.

#include "d012_disc_mod.hpp"

#include "adhoc/session.hpp"
#include "disc_patch.hpp"
#include "install/user_data.hpp"
#include "patched_edition.hpp"
#include "profile.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "imgui.h"
#include "ui/file_browser.hpp"
#include "ui/layer.hpp"
#include "ui/ui.hpp"
#include "ui/widgets.hpp"

#include "install/installer.hpp"
#endif

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

namespace d012::disc_mod {
namespace {

constexpr const char *kImageKey = "disc_mod.image";
constexpr const char *kEnabledKey = "disc_mod.enabled";
constexpr const char *kLayerExtension = ".pkdisc";

struct State {
    std::mutex lock;
    bool read = false;
    std::filesystem::path image;  // as settings.ini has it now
    bool enabled = false;         // as settings.ini has it now
    std::filesystem::path playing;  // what this start plays; empty: the game's own disc
};
State &state() {
    static State s;
    return s;
}

std::filesystem::path data_directory() { return portablekit::install::user_data_directory(); }

void read_settings(State &s) {
    if (s.read) return;
    s.read = true;
    const auto entries = portablekit::install::read_settings_file(data_directory());
    if (const auto found = entries.find(kImageKey); found != entries.end() && !found->second.empty()) {
        s.image = portablekit::install::path_from_utf8(found->second);
        if (s.image.is_relative()) s.image = data_directory() / s.image;
    }
    const auto enabled = entries.find(kEnabledKey);
    s.enabled = enabled != entries.end() && enabled->second == "1";
}

void save_setting(const char *key, const std::string &value) {
    const auto dir = data_directory();
    auto entries = portablekit::install::read_settings_file(dir);
    entries[key] = value;
    portablekit::install::write_settings_file(dir, entries);
}

// What the player calls the mod: a prepared patch by its patch's name.
std::string mod_name(const std::filesystem::path &path) {
    if (path.empty()) return "Not set";
    if (path.extension() == kLayerExtension) return portablekit::install::path_to_utf8(path.stem());
    return portablekit::install::path_to_utf8(path.filename());
}

std::string megabytes(std::uint64_t bytes) { return std::to_string((bytes + (1u << 19)) >> 20) + " MB"; }

void choose(const std::filesystem::path &image) {
    State &s = state();
    {
        std::lock_guard guard(s.lock);
        s.image = image;
    }
    save_setting(kImageKey, portablekit::install::path_to_utf8(image));
    std::cout << "[disc mod] mod " << portablekit::install::path_to_utf8(image) << std::endl;
}

// A patch applied to the game's own disc, on a thread of its own.
struct Preparation {
    std::mutex lock;
    std::thread worker;
    std::atomic<bool> running{false};
    portablekit::disc_patch::Progress progress;
    std::string name;
    bool image = false;  // a patched disc image, not a patch
    std::string error;   // for the player, once it failed
    std::string done;    // for the player, once it is ready
    ~Preparation() {
        progress.cancel = true;
        if (worker.joinable()) worker.join();
    }
};
Preparation &preparation() {
    static Preparation p;
    return p;
}

// Applies `patch` to the game's own disc, or compares a patched disc image
// with it (synchronously); empty on success, else why not. `summary` says what
// was kept.
std::optional<std::string> prepare_now(const std::filesystem::path &patch, portablekit::disc_patch::Progress &progress,
                                       std::filesystem::path &out, std::string &summary) {
    const std::filesystem::path release = portablekit::patched::release_disc();
    if (release.empty()) return std::string("The game's own disc image is not known.");
    out = data_directory() / "disc-mods" / patch.stem();
    out += kLayerExtension;
    const bool image = !portablekit::disc_patch::is_patch_file(patch);
    const auto started = std::chrono::steady_clock::now();
    const portablekit::disc_patch::Result result =
        image ? portablekit::disc_patch::prepare_image(patch, release, out, progress)
              : portablekit::disc_patch::prepare(patch, release, out, progress);
    const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
    std::cout << "[disc mod] " << portablekit::install::path_to_utf8(patch.filename()) << ": "
              << (result.ok ? "prepared" : "not prepared") << " in " << seconds << " s; " << result.windows
              << " windows, " << result.mismatched << " checksums differ, " << result.changed_sectors
              << " sectors changed (" << result.zero_sectors << " emptied)"
              << (result.message.empty() ? "" : "; " + result.message) << std::endl;
    if (!result.ok) return result.message;
    std::string why;
    const std::optional<portablekit::patched::Edition> edition = portablekit::patched::open(out, why);
    if (!edition) {
        std::error_code ec;
        std::filesystem::remove(out, ec);
        return "The patched disc cannot be played: " + why + ".";
    }
    std::cout << "[disc mod] identity " << edition->identity << std::endl;
    summary = megabytes((result.changed_sectors - result.zero_sectors) * 2048u) +
              " of changes, kept beside your disc." + (image ? " The disc image itself is no longer needed." : "") +
              (result.message.empty() ? "" : " " + result.message);
    return std::nullopt;
}

void start_preparation(const std::filesystem::path &patch) {
    Preparation &p = preparation();
    if (p.running) return;
    if (p.worker.joinable()) p.worker.join();
    p.progress.done = 0u;
    p.progress.total = 0u;
    p.progress.cancel = false;
    {
        std::lock_guard guard(p.lock);
        p.name = portablekit::install::path_to_utf8(patch.filename());
        p.image = !portablekit::disc_patch::is_patch_file(patch);
        p.error.clear();
        p.done.clear();
    }
    p.running = true;
    p.worker = std::thread([patch] {
        Preparation &p = preparation();
        std::filesystem::path out;
        std::string summary;
        const std::optional<std::string> error = prepare_now(patch, p.progress, out, summary);
        if (!error) choose(out);
        {
            std::lock_guard guard(p.lock);
            if (error) p.error = *error == "Cancelled" ? "Cancelled. Nothing was kept." : *error;
            else p.done = summary;
        }
        p.running = false;
    });
}

#if defined(PORTABLEKIT_HAS_RENDERER) && !defined(PORTABLEKIT_ANDROID_APP)
namespace ui = portablekit::ui;

struct Picker {
    std::unique_ptr<ui::FileBrowser> browser;
    std::filesystem::path folder;
    std::string error;
    bool preparing = false;  // the panel shows the preparation
};
Picker &picker() {
    static Picker p;
    return p;
}

ui::FileBrowser::Options browser_options() {
    ui::FileBrowser::Options options;
    options.extensions = {".xdelta", ".vcdiff", ".iso"};
    options.filter_name = ".xdelta and .iso";
    options.listed_name = "patches (.xdelta) and disc images (.iso)";
    options.empty_note = "No folders, patches or disc images here.";
    return options;
}

// The preparation's part of the panel; false once the player closed it.
bool preparation_frame(bool back) {
    Preparation &p = preparation();
    std::string name, error, done;
    bool image = false;
    {
        std::lock_guard guard(p.lock);
        name = p.name;
        image = p.image;
        error = p.error;
        done = p.done;
    }
    ui::begin_panel("##disc-mod", "Preparing the mod", "Disc mod", true);
    ui::begin_content();
    if (p.running) {
        const std::uint64_t made = p.progress.done, total = p.progress.total;
        ui::paragraph(image ? "Comparing " + name + " with your copy of the game's disc. Only what differs is kept; "
                              "this takes a minute or less."
                            : "Applying " + name + " to your copy of the game's disc. Only what it changes is kept; "
                              "this takes a minute or less.");
        ui::progress_bar(total != 0u ? static_cast<float>(made) / static_cast<float>(total) : 0.0f,
                         total != 0u ? megabytes(made) + " of " + megabytes(total) : std::string("Reading the patch…"));
        if (back) p.progress.cancel = true;
    } else if (!error.empty()) {
        ui::paragraph(name + ": " + error, ui::colors::kDanger);
    } else {
        ui::paragraph(name + " is ready: " + done);
        ui::paragraph("Turn on Play the modded disc on the System page, then restart the game.");
    }
    ui::begin_footer();
    ui::hints({{ui::Control::Back, p.running ? "Cancel" : "Close"}});
    ui::end_panel();
    return p.running || !back;
}

bool picker_frame() {
    Picker &p = picker();
    ui::Layer &layer = ui::Layer::get();
    const bool back = layer.take_back();
    const ImGuiKey cancel = layer.confirm_south() ? ImGuiKey_GamepadFaceRight : ImGuiKey_GamepadFaceDown;
    const bool pad_back = ImGui::IsKeyPressed(cancel, false);
    if (p.preparing) {
        if (preparation_frame(back || pad_back)) return true;
        p.preparing = false;
        return false;
    }
    if (!p.browser) p.browser = std::make_unique<ui::FileBrowser>(p.folder, browser_options());
    std::optional<std::filesystem::path> dropped = layer.take_dropped_file();
    ui::begin_panel("##disc-mod", "Choose the mod", "Disc mod", true);
    ui::begin_content();
    if (!p.error.empty()) ui::paragraph(p.error, ui::colors::kDanger);
    const ui::FileBrowser::Result result = p.browser->frame(back || pad_back);
    layer.set_description("The mod as its makers hand it out: its patch to the disc (.xdelta), or a disc image "
                          "already patched (.iso). A patch is applied to your copy of the game's disc.");
    ui::begin_footer();
    ui::hints({{ui::Control::Confirm, "Open"}, {ui::Control::Back, "Up a folder"}});
    ui::end_panel();
    if (result == ui::FileBrowser::Result::Browsing && !dropped) return true;
    p.folder = p.browser->folder();
    if (result == ui::FileBrowser::Result::Cancelled && !dropped) {
        p.browser.reset();
        p.error.clear();
        return false;
    }
    const std::filesystem::path chosen = dropped ? *dropped : p.browser->chosen();
    p.browser.reset();
    // A disc image is checked first (the game's, a plain executable), then
    // kept as a layer as a patch is.
    std::string why;
    const std::string file = portablekit::install::path_to_utf8(chosen.filename());
    if (!portablekit::disc_patch::is_patch_file(chosen)) {
        if (!portablekit::patched::open(chosen, why)) {
            p.error = file + ": " + why + ".";
            return true;
        }
        // Another region's disc would differ in nearly every sector.
        const auto mod_disc = portablekit::install::disc_id_of(chosen);
        const auto own_disc = portablekit::install::disc_id_of(portablekit::patched::release_disc());
        if (mod_disc && own_disc && *mod_disc != *own_disc) {
            p.error = file + ": it is made from another release of the game (" + *mod_disc + "); yours is " +
                      *own_disc + ".";
            return true;
        }
    }
    p.error.clear();
    p.preparing = true;
    start_preparation(chosen);
    return true;
}
#endif

#if defined(PORTABLEKIT_HAS_RENDERER)
void system_rows() {
    namespace ui = portablekit::ui;
    State &s = state();
    std::filesystem::path image, playing;
    bool enabled = false;
    {
        std::lock_guard guard(s.lock);
        read_settings(s);
        image = s.image;
        enabled = s.enabled;
        playing = s.playing;
    }
    ui::section("Disc mod");
    ui::info_row("Playing", playing.empty() ? std::string("The game's own disc") : mod_name(playing));

    ui::RowOptions choose_row;
    choose_row.description = "A mod that patches the game's disc, such as Emendatus: its patch (.xdelta) or a disc "
                             "image already patched (.iso). Only the code the mod changed runs differently; nothing "
                             "is compiled.";
#if defined(PORTABLEKIT_ANDROID_APP)
    choose_row.disabled = true;
    choose_row.note = "Not on Android yet";
#endif
    ui::info_row("Mod", mod_name(image));
    if (ui::button_row("Choose the mod…", choose_row)) {
#if !defined(PORTABLEKIT_ANDROID_APP)
        Picker &p = picker();
        if (p.folder.empty())
            p.folder = image.empty() || image.extension() == kLayerExtension ? ui::FileBrowser::home()
                                                                             : image.parent_path();
        p.preparing = preparation().running.load();
        // The browser shows over the game once the menu is closed.
        ui::open_panel_over_game(&picker_frame);
        ui::request_menu_close();
#endif
    }

    ui::RowOptions on;
    on.description = "Plays the mod from the next start. Online, you meet only players of the same mod. Its saves "
                     "and downloadable content are kept apart from the game's own, in the folder ms0-patched.";
    if (image.empty()) {
        on.disabled = true;
        on.note = "Choose the mod first";
    }
    if (const char *forced = portablekit::env("PATCHED_DISC"); forced != nullptr) {
        on.disabled = true;
        on.note = "Set by D012_PATCHED_DISC";
    }
    if (ui::toggle_row("Play the modded disc", enabled, on)) {
        {
            std::lock_guard guard(s.lock);
            s.enabled = !enabled;
        }
        save_setting(kEnabledKey, enabled ? "0" : "1");
    }
    const bool wanted = enabled && !image.empty();
    if (wanted != !playing.empty() || (wanted && image != playing)) {
        ui::RowOptions restart;
        restart.description = "Closes the game and starts it again with the mod as chosen above. Progress since your "
                              "last save is lost.";
        if (ui::button_row("Restart the game now", restart)) {
            portablekit::install::request_restart_on_exit();
            // The game's next flip quits, and it flips once the menu is closed.
            ui::request_quit();
            ui::request_menu_close();
        }
    }
}
#endif

} // namespace

std::filesystem::path disc_for_this_start() {
    State &s = state();
    std::lock_guard guard(s.lock);
    read_settings(s);
    std::error_code ec;
    s.playing = s.enabled && std::filesystem::is_regular_file(s.image, ec) ? s.image : std::filesystem::path();
    return s.playing;
}

void install(portablekit::HleRegistrar &) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    // Test only: prepares a patch as the game starts.
    if (const char *patch = portablekit::env("TEST_DISC_PATCH"); patch != nullptr && *patch != '\0') {
        portablekit::disc_patch::Progress progress;
        std::filesystem::path out;
        std::string summary;
        const auto error = prepare_now(portablekit::install::path_from_utf8(patch), progress, out, summary);
        std::cout << "[disc mod] test preparation: " << (error ? *error : summary + " " +
                                                                              portablekit::install::path_to_utf8(out))
                  << std::endl;
    }
    if (portablekit::patched::active())
        std::cout << "[disc mod] online with the tag " << portablekit::adhoc_version_tag()
                  << ": only copies playing the same mod meet this one" << std::endl;
#if defined(PORTABLEKIT_HAS_RENDERER)
    portablekit::ui::add_system_rows(&system_rows);
#endif
}

} // namespace d012::disc_mod
