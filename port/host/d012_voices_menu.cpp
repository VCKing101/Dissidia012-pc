// Japanese voices in the game's own Options (d012_voices.hpp): two rows
// after Voice, "Japanese Voice Disc" (the image's name; confirm opens a file
// browser over the game) and "Voice Language" (English / Japanese), shown
// once a Japanese disc is chosen.
//
// The Options screen (menu overlay, object *0x08ABB9E4, screen at +0x138)
// lists the game's option table (*0x089D76D8, the `optp` chunk of
// general_archive/main/main.bin, copied into a 2 KB buffer by main
// 0x088C5044): records of 0x1C bytes {ordinal, option id, default in the low
// nibble, value kind (0xFF: a list), shown for Japanese / other languages /
// another mode, -, title text, eight value texts, help text}, the text being
// strings of text/EN/menu/option.bin. The port puts two records of its own
// in the table and its strings in that file (d012_voices.cpp); the screen
// draws them like its own. Option values are bytes of the save at
// *0x089D712C + 0x1DE04 + id, which the screen copies into its cache
// (screen + 0xAB4 + id) on opening (main 0x088F472C) and back on leaving
// (main 0x088F475C, after comparing with main 0x088F4790); the port's two ids
// are kept out of the save there, in settings.ini instead.
// docs/port/japanese-voices.md. MIT licence; see port/README.md.

#include "d012_voices.hpp"

#include "d012_decomp_harness.hpp"

#include "hle/hle_common.hpp"
#include "portablekit/hle_extension.hpp"
#include "profile.hpp"
#include "psprecomp/guest_memory.hpp"
#include "psprecomp/runtime.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "imgui.h"
#include "ui/file_browser.hpp"
#include "ui/layer.hpp"
#include "ui/ui.hpp"
#include "ui/widgets.hpp"
#endif

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

namespace d012::voices {
namespace {

constexpr std::uint32_t kSceCtrlReadBufferPositive = 0x1F803938u;
constexpr std::uint32_t kCross = 0x4000u;
constexpr std::uint32_t kCircle = 0x2000u;
// 1 when the system's confirm button is cross (main 0x08867800).
constexpr std::uint32_t kConfirmIsCross = 0x089D5F1Du;

// The option table.
constexpr std::uint32_t kTablePointer = 0x089D76D8u;
constexpr std::uint32_t kSecondPointer = 0x089D76E0u;
constexpr std::uint32_t kTableBuffer = 0x800u;
constexpr std::uint32_t kRecordSize = 0x1Cu;
constexpr std::uint8_t kVoiceOption = 0x0Eu;
// The port's option ids: free in the game's 0x78 and never stored in its save.
constexpr std::uint8_t kDiscOption = 0x70u;
constexpr std::uint8_t kLanguageOption = 0x71u;
// The game's own strings "English" and "Japanese" (Subtitle Language's).
constexpr std::uint16_t kTextEnglish = 145u;
constexpr std::uint16_t kTextJapanese = 146u;

// The save's options.
constexpr std::uint32_t kSaveRoot = 0x089D712Cu;
constexpr std::uint32_t kSaveOptions = 0x1DE04u;

// The Options screen.
constexpr std::uint32_t kOptionsObject = 0x08ABB9E4u;
constexpr std::uint32_t kScreen = 0x138u;
constexpr std::uint32_t kCursor = 0x8B0u;     // the row the hand is on
constexpr std::uint32_t kLastRow = 0x8B8u;    // the cursor's limit (main 0x088D46E4)
constexpr std::uint32_t kCursorPlace = 0x8D8u;
constexpr std::uint32_t kTextBuffer = 0x8F8u;  // option.bin as loaded, its text in the clear
constexpr std::uint32_t kCache = 0xAB4u;
constexpr std::uint32_t kCacheCopy = 0xB2Cu;
constexpr std::uint32_t kRows = 0xD60u;       // the rows' ordinals
constexpr std::uint32_t kRowCount = 0xE60u;
constexpr std::uint32_t kBuildRows = 0x08AB9AECu;    // (object)
constexpr std::uint32_t kRefreshRows = 0x08AB856Cu;  // (object, rebuild all)

// Main functions the port wraps.
constexpr std::uint32_t kPlaceCursor = 0x088D4B74u;  // each frame of a list screen
constexpr std::uint32_t kOptionsIn = 0x088F472Cu;
constexpr std::uint32_t kOptionsOut = 0x088F475Cu;
constexpr std::uint32_t kOptionsCompare = 0x088F4790u;

bool user_ram(std::uint32_t address) { return address >= 0x08800000u && address < 0x0A000000u; }

struct Menu {
    psprecomp::GuestMemory *memory{};
    bool patched{};
    bool failed{};
    // The Options screen as last seen drawing its cursor.
    std::uint32_t object{};
    std::uint32_t screen{};
    std::chrono::steady_clock::time_point seen{};
    std::uint32_t previous_buttons{};
    // A disc the player chose in the browser, for the emulation thread.
    std::mutex lock;
    std::optional<std::filesystem::path> chosen;
};
Menu &menu() {
    static Menu instance;
    return instance;
}

GuestRegs arguments(const decomp::CallState &in) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    return regs;
}

// The Options screen of the menu overlay, if `screen` is it.
bool options_screen(psprecomp::GuestMemory &memory, std::uint32_t screen, std::uint32_t &object) {
    object = memory.load32(kOptionsObject);
    return user_ram(object) && user_ram(screen) && memory.load32(object + kScreen) == screen;
}

void set_shown(psprecomp::GuestMemory &memory, std::uint32_t record, bool shown) {
    for (std::uint32_t i = 4u; i < 7u; ++i) memory.store8(record + i, shown ? 1u : 0u);
}

// Puts the port's two records after Voice's, once per load of the table,
// and keeps Voice Language shown only with a disc.
void patch_table(psprecomp::GuestMemory &memory) {
    Menu &m = menu();
    const std::uint32_t header = memory.load32(kTablePointer);
    if (!user_ram(header) || m.failed) return;
    const std::uint32_t count = memory.load16(header + 2u);
    const std::uint32_t records = header + 8u;
    std::uint32_t voice = UINT32_MAX;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint8_t id = memory.load8(records + i * kRecordSize + 1u);
        if (id == kLanguageOption) {
            set_shown(memory, records + i * kRecordSize, disc_ready());
            return;
        }
        if (id == kVoiceOption) voice = i;
    }
    const std::uint32_t size = memory.load16(header + 6u);
    const std::uint32_t second = memory.load32(kSecondPointer);
    if (voice == UINT32_MAX || size != count * kRecordSize || second != records + size) {
        m.failed = true;
        std::cerr << "[voices] the option table is not as expected; Options has no Japanese voice rows" << std::endl;
        return;
    }
    // Everything after Voice's record moves down two records, within the
    // table's buffer: the game's 27 records and the section after them take
    // 1,212 of its 2,048 bytes.
    const std::uint32_t insert = records + (voice + 1u) * kRecordSize;
    const std::uint32_t grow = 2u * kRecordSize;
    for (std::uint32_t at = header + kTableBuffer - grow; at-- > insert;) memory.store8(at + grow, memory.load8(at));
    std::array<std::uint8_t, kRecordSize> voice_record{};
    for (std::uint32_t i = 0; i < kRecordSize; ++i) voice_record[i] = memory.load8(records + voice * kRecordSize + i);
    const auto write = [&](std::uint32_t at, std::uint8_t id, std::uint16_t title, std::uint16_t value0,
                           std::uint16_t value1, std::uint16_t help, bool shown) {
        for (std::uint32_t i = 0; i < kRecordSize; ++i) memory.store8(at + i, voice_record[i]);
        memory.store8(at + 1u, id);
        memory.store8(at + 2u, static_cast<std::uint8_t>(voice_record[2] & 0xF0u));  // default: the first value
        memory.store8(at + 3u, 0xFFu);
        set_shown(memory, at, shown);
        memory.store16(at + 8u, title);
        for (std::uint32_t v = 0; v < 8u; ++v) memory.store16(at + 0x0Au + 2u * v, 0xFFFFu);
        memory.store16(at + 0x0Au, value0);
        memory.store16(at + 0x0Cu, value1);
        memory.store16(at + 0x1Au, help);
    };
    write(insert, kDiscOption, kTextDiscTitle, kTextDiscValue, 0xFFFFu, kTextDiscHelp, true);
    write(insert + kRecordSize, kLanguageOption, kTextLanguageTitle, kTextEnglish, kTextJapanese, kTextLanguageHelp,
          disc_ready());
    memory.store16(header + 2u, static_cast<std::uint16_t>(count + 2u));
    memory.store16(header + 6u, static_cast<std::uint16_t>(size + grow));
    memory.store32(kSecondPointer, second + grow);
    for (std::uint32_t i = 0; i < count + 2u; ++i) memory.store8(records + i * kRecordSize, static_cast<std::uint8_t>(i));
    if (!m.patched) std::cout << "[voices] Options has the Japanese voice rows after Voice" << std::endl;
    m.patched = true;
}

// The disc row's text in the screen's loaded option.bin, for a disc chosen
// while the screen shows.
void write_disc_label(psprecomp::GuestMemory &memory, std::uint32_t screen) {
    const std::uint32_t buffer = memory.load32(screen + kTextBuffer);
    if (!user_ram(buffer) || memory.load16(buffer) <= kTextDiscValue) return;
    const std::uint32_t at = buffer + 2u * memory.load16(buffer + 2u + 2u * kTextDiscValue);
    if (memory.load16(at) != 2u * kDiscValueUnits) return;
    std::u16string label = disc_label();
    label.resize(kDiscValueUnits, 0u);
    for (std::size_t j = 0; j < label.size(); ++j) memory.store16(at + 2u + 2u * static_cast<std::uint32_t>(j), label[j]);
}

void run_place_cursor(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs = arguments(in);
    decomp::call_original(g, kPlaceCursor, regs);
    Menu &m = menu();
    if (m.memory == nullptr) return;
    psprecomp::GuestMemory &memory = *m.memory;
    std::uint32_t object = 0;
    const std::uint32_t screen = in.gpr[4] - kCursorPlace;
    if (!options_screen(memory, screen, object)) return;
    m.object = object;
    m.screen = screen;
    m.seen = std::chrono::steady_clock::now();
    std::optional<std::filesystem::path> chosen;
    {
        std::lock_guard guard(m.lock);
        chosen.swap(m.chosen);
    }
    if (!chosen) return;
    if (const auto error = choose_disc(memory, *chosen)) return;
    // The new name, and Voice Language shown: the screen lists its rows
    // again and draws them all, the hand staying where it is (the disc row
    // comes before Voice Language).
    patch_table(memory);
    write_disc_label(memory, screen);
    g_call1(g, kBuildRows, object);
    const std::uint32_t rows = memory.load32(screen + kRowCount);
    if (rows > 0u) memory.store32(screen + kLastRow, rows - 1u);
    g_call2(g, kRefreshRows, object, 1u);
}

// Whether `cache` is the Options screen's, the one it reads and writes the
// options with.
bool screen_cache(psprecomp::GuestMemory &memory, std::uint32_t cache) {
    std::uint32_t object = 0;
    const std::uint32_t screen = memory.load32(memory.load32(kOptionsObject) + kScreen);
    return options_screen(memory, screen, object) && (cache == screen + kCache || cache == screen + kCacheCopy);
}

void run_options_in(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs = arguments(in);
    decomp::call_original(g, kOptionsIn, regs);
    Menu &m = menu();
    const std::uint32_t cache = in.gpr[4];
    if (!m.patched || m.memory == nullptr || !user_ram(cache)) return;
    m.memory->store8(cache + kDiscOption, 0u);
    m.memory->store8(cache + kLanguageOption, japanese() ? 1u : 0u);
}

void run_options_out(Guest *g, const decomp::CallState &in, GuestRegs &) {
    Menu &m = menu();
    const std::uint32_t cache = in.gpr[4];
    if (m.patched && m.memory != nullptr && user_ram(cache) && screen_cache(*m.memory, cache) && disc_ready()) {
        const bool on = m.memory->load8(cache + kLanguageOption) != 0u;
        if (on != japanese()) set_japanese(*m.memory, on);
    }
    GuestRegs regs = arguments(in);
    decomp::call_original(g, kOptionsOut, regs);
    if (m.memory == nullptr) return;
    // The save keeps the game's own options only.
    const std::uint32_t root = m.memory->load32(kSaveRoot);
    if (!user_ram(root)) return;
    m.memory->store8(root + kSaveOptions + kDiscOption, 0u);
    m.memory->store8(root + kSaveOptions + kLanguageOption, 0u);
}

// The port's two values do not count as a change of the game's options
// (which would ask to save): they are compared as the save's.
void run_options_compare(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    Menu &m = menu();
    const std::uint32_t cache = in.gpr[4];
    std::uint8_t kept[2]{};
    const bool hide = m.patched && m.memory != nullptr && user_ram(cache);
    if (hide) {
        kept[0] = m.memory->load8(cache + kDiscOption);
        kept[1] = m.memory->load8(cache + kLanguageOption);
        m.memory->store8(cache + kDiscOption, 0u);
        m.memory->store8(cache + kLanguageOption, 0u);
    }
    GuestRegs regs = arguments(in);
    decomp::call_original(g, kOptionsCompare, regs);
    out.v0 = regs.v0;
    if (hide) {
        m.memory->store8(cache + kDiscOption, kept[0]);
        m.memory->store8(cache + kLanguageOption, kept[1]);
    }
}

struct Install {
    Install() {
        if (portablekit::env("NO_VOICE_OPTIONS") != nullptr) return;
        static const decomp::Override overrides[] = {
            {kPlaceCursor, "voices_place_cursor", decomp::Module::Main, decomp::Returns::Nothing, &run_place_cursor},
            {kOptionsIn, "voices_options_in", decomp::Module::Main, decomp::Returns::Nothing, &run_options_in},
            {kOptionsOut, "voices_options_out", decomp::Module::Main, decomp::Returns::Nothing, &run_options_out},
            {kOptionsCompare, "voices_options_compare", decomp::Module::Main, decomp::Returns::Integer,
             &run_options_compare},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install_overrides;

#if defined(PORTABLEKIT_HAS_RENDERER)
namespace ui = portablekit::ui;

struct Picker {
    std::unique_ptr<ui::FileBrowser> browser;
    std::filesystem::path folder;
    std::string error;
};
Picker &picker() {
    static Picker instance;
    return instance;
}

bool picker_frame() {
    Picker &p = picker();
    ui::Layer &layer = ui::Layer::get();
    if (!p.browser) p.browser = std::make_unique<ui::FileBrowser>(p.folder);
    // An image dropped on the window is chosen as one opened here.
    std::optional<std::filesystem::path> dropped = layer.take_dropped_file();
    const bool back = layer.take_back();
    const ImGuiKey cancel = layer.confirm_south() ? ImGuiKey_GamepadFaceRight : ImGuiKey_GamepadFaceDown;
    const bool pad_back = ImGui::IsKeyPressed(cancel, false);
    ui::begin_panel("##japanese-disc", "Choose the Japanese disc image", "Japanese voices", true);
    ui::begin_content();
    if (!p.error.empty()) ui::paragraph(p.error, ui::colors::kDanger);
    const ui::FileBrowser::Result result = p.browser->frame(back || pad_back);
    layer.set_description("The .iso image of the Japanese release (NPJH50377). Its voices play in place of the "
                          "English ones; the text stays as it is.");
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
    if (const auto error = check_disc(chosen)) {
        p.error = chosen.filename().string() + ": " + *error;
        return true;
    }
    p.error.clear();
    std::lock_guard guard(menu().lock);
    menu().chosen = chosen;
    return false;
}

void open_picker() {
    Picker &p = picker();
    if (p.folder.empty()) p.folder = ui::FileBrowser::home();
    std::cout << "[voices] the Japanese disc browser opened from Options" << std::endl;
    ui::open_panel_over_game(&picker_frame);
}
#endif

// Confirm on the disc row opens the browser; the game, which does nothing
// for confirm on a row of values, never sees that press.
void watch_pad(psprecomp::GuestMemory &memory, std::uint32_t buffer, std::uint32_t count) {
    Menu &m = menu();
    const std::uint32_t buttons = memory.load32(buffer + 4u);
    const std::uint32_t confirm = memory.load8(kConfirmIsCross) == 1u ? kCross : kCircle;
    const bool pressed = (buttons & confirm) != 0u && (m.previous_buttons & confirm) == 0u;
    m.previous_buttons = buttons;
    if (!pressed || !m.patched || std::chrono::steady_clock::now() - m.seen > std::chrono::milliseconds(200)) return;
#if defined(PORTABLEKIT_HAS_RENDERER)
    if (ui::panel_over_game()) return;
    std::uint32_t object = 0;
    if (!options_screen(memory, m.screen, object)) return;
    const std::uint32_t cursor = memory.load32(m.screen + kCursor);
    const std::uint32_t rows = memory.load32(m.screen + kRowCount);
    const std::uint32_t header = memory.load32(kTablePointer);
    if (cursor >= rows || !user_ram(header)) return;
    const std::uint32_t ordinal = memory.load32(m.screen + kRows + 4u * cursor);
    if (ordinal >= memory.load16(header + 2u) || memory.load8(header + 8u + ordinal * kRecordSize + 1u) != kDiscOption)
        return;
    for (std::uint32_t i = 0; i < count; ++i)
        memory.store32(buffer + i * 16u + 4u, memory.load32(buffer + i * 16u + 4u) & ~confirm);
    open_picker();
#else
    (void)count;
#endif
}

} // namespace

void install_menu(portablekit::HleRegistrar &hle) {
    if (portablekit::env("NO_VOICE_OPTIONS") != nullptr) {
        std::cout << "[voices] no Japanese voice rows in Options (" << portablekit::env_name("NO_VOICE_OPTIONS") << ")"
                  << std::endl;
        return;
    }
    const portablekit::HleFunction previous = hle.bound_function("sceCtrl", kSceCtrlReadBufferPositive);
    if (!previous) {
        std::cerr << "[voices] sceCtrlReadBufferPositive is not bound; Options has no Japanese voice rows\n";
        return;
    }
    hle.bind("sceCtrl", kSceCtrlReadBufferPositive,
             [previous](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
                 const std::uint32_t buffer = ctx.gpr[4];
                 const std::uint32_t count = std::min<std::uint32_t>(std::max<std::uint32_t>(ctx.gpr[5], 1u), 64u);
                 previous(rt, ctx);
                 Menu &m = menu();
                 m.memory = &rt.memory();
                 patch_table(rt.memory());
                 if (user_ram(buffer)) watch_pad(rt.memory(), buffer, count);
             });
}

} // namespace d012::voices
