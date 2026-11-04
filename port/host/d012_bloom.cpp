// Bloom, and the Video page's switch that leaves it out.
//
// The scene's draw (0x08892760, in the main executable) ends with up to two
// calls to the bloom, 0x0885E150: the frame shrunk into the scratch buffer
// at 0x0CC000 (a quarter of the screen in battle, 30x17 on the world map) and
// up to four layers of it stretched back over the screen, blended. The bloom
// only adds GE commands: it changes no game state, and it puts the draw buffer
// back as it found it. So with bloom off the override returns at once, and
// with it on it runs the original.
//
// MIT licence; see port/README.md.

#include "d012_bloom.hpp"
#include "d012_decomp_harness.hpp"

#include "install/user_data.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "ui/ui.hpp"
#include "ui/widgets.hpp"
#endif

#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

namespace d012::bloom {
namespace {

constexpr std::uint32_t kBloom = 0x0885E150u;
constexpr const char *kBloomKey = "game.bloom";

// D012_BLOOM=0 or 1, or null when the player's setting decides.
const char *forced() {
    const char *text = std::getenv("D012_BLOOM");
    return text != nullptr && *text != '\0' ? text : nullptr;
}

std::atomic<bool> &on() {
    static std::atomic<bool> value{[] {
        if (const char *text = forced(); text != nullptr) return std::strcmp(text, "0") != 0;
        // settings.ini's game.bloom, which PortableKit's own settings leave
        // alone (they rewrite only their keys).
        try {
            const std::filesystem::path dir = portablekit::install::user_data_directory();
            if (!std::filesystem::exists(dir / portablekit::install::kSettingsFile)) return true;
            const portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
            if (const auto found = entries.find(kBloomKey); found != entries.end()) {
                const std::string &text = found->second;
                return !(text == "0" || text == "off" || text == "false");
            }
        } catch (const std::exception &e) {
            std::cerr << "[bloom] settings.ini not read for " << kBloomKey << ": " << e.what() << "\n";
        }
        return true;
    }()};
    return value;
}

void run_bloom(Guest *g, const decomp::CallState &in, GuestRegs &) {
    if (!on().load(std::memory_order_relaxed)) return;
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    decomp::call_original(g, kBloom, regs);
}

#if defined(PORTABLEKIT_HAS_RENDERER)
// The menu's Video page, kept in settings.ini as PortableKit keeps its own.
void menu_rows() {
    namespace ui = portablekit::ui;
    ui::section("Effects");
    ui::RowOptions options;
    options.description = "The soft glow over battles and the world map.";
    if (forced() != nullptr) {
        options.disabled = true;
        options.note = "Set by D012_BLOOM";
    }
    const bool bloom = on().load();
    if (!ui::toggle_row("Bloom", bloom, options)) return;
    on().store(!bloom);
    try {
        const std::filesystem::path dir = portablekit::install::user_data_directory();
        portablekit::install::SettingsEntries entries = portablekit::install::read_settings_file(dir);
        entries[kBloomKey] = bloom ? "0" : "1";
        portablekit::install::write_settings_file(dir, entries);
    } catch (const std::exception &e) {
        std::cerr << "[bloom] settings.ini not written for " << kBloomKey << ": " << e.what() << "\n";
    }
}
#endif

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kBloom, "bloom", decomp::Module::Main, decomp::Returns::Nothing, &run_bloom},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install_override;

} // namespace

bool enabled() { return on().load(); }

void install(portablekit::HleRegistrar &) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    if (!enabled()) std::cout << "[bloom] off" << (forced() != nullptr ? " (D012_BLOOM=0)" : "") << std::endl;
#if defined(PORTABLEKIT_HAS_RENDERER)
    portablekit::ui::add_menu_rows(&menu_rows);
#endif
}

} // namespace d012::bloom
