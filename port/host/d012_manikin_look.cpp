// The Manikin look; see d012_manikin_look.hpp. MIT licence; see
// port/README.md.
//
// The three look functions are called from the menu overlay, and the
// character list (0x088D5AC8) also from main 0x0892905C, all from other units,
// so the overrides take every call without a dispatch-calls row. The list's
// own calls of the mask function are inside its unit: the list override adds
// the manikin to its entries itself.
//
// The menu overlay is read from the disc again each time it loads, which
// puts its label tables back. Every screen with a look builds its character
// list or asks for a mask before it shows a label, so each call of these
// functions points costume 0 at the manikin's label again; a table that is
// not the disc's (another overlay in the window, or a disc mod's) is left
// alone.
//
// The battle portrait (battle 0x08B21588, d012_manikin_rules.hpp) makes
// its file's name in a buffer on its stack, then asks for the file's size
// (main 0x08862C40) and reads it (main 0x08862854) by that buffer. Both pass
// the name through main 0x08867984, which puts the player's language in a
// path and, while the battle loads its portraits (the byte at 0x089D5F1C
// set), gives back the name it was given. Then a manikin's portrait name
// there becomes its Normal look's, in that same buffer. 0x08867984 is in
// another generated unit than both, and does no file work itself.

#include "d012_manikin_look.hpp"

#include "d012_decomp_harness.hpp"
#include "d012_manikin_rules.hpp"
#include "d012_package_index.hpp"
#include "d012_package_mods.hpp"

#include "hle/hle_common.hpp"
#include "patched_edition.hpp"
#include "profile.hpp"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace d012::manikin_look {
namespace {

namespace decomp = d012::decomp;
namespace pix = package_index;

constexpr std::uint32_t kLooks = 0x088D5E0Cu;     // (character) -> mask of its looks, 0: not there to choose
constexpr std::uint32_t kList = 0x088D5AC8u;      // (entries, most, with Chaos, with Aerith) -> count
constexpr std::uint32_t kNextLook = 0x088D608Cu;  // (look, mask) -> the next look
constexpr std::uint32_t kCostumeRow = 0x088470DCu;  // (character, costume, assist) -> its objentry row, or 0
constexpr std::uint32_t kLanguagePath = 0x08867984u;  // (path, kind) -> the path in the player's language
constexpr std::uint32_t kKeepPaths = 0x089D5F1Cu;     // byte: the battle is loading its portraits
constexpr std::uint32_t kPathBuffer = 80u;            // the portrait loader's buffer for the name
constexpr std::uint32_t kListEntry = 8u;          // {s32 character, u16, u16 mask}
constexpr std::uint32_t kListMask = 6u;

constexpr const char *kPackage = "disc0:/PSP_GAME/USRDIR/DATA/PACKAGE.BIN";
constexpr const char *kPackageInfo = "disc0:/PSP_GAME/USRDIR/DATA/PACKAGE_INFO.BIN";

// The text packs with a look-label table, as the game opens them on the USA
// and European discs (the menus' "text/JP/..." with the language folder
// rewritten; both discs have only English menus). The sixth one's name is
// not in the executables (the game makes it); its entry is named by CRC.
struct LabelFile {
    const char *path;  // as package_mods takes it
    const char *name;  // for the log
};
constexpr LabelFile kLabelFiles[] = {
    {"text/EN/mess_pk_chara_select_0.bin", "character select"},
    {"text/EN/mess_pk_party_setting_0.bin", "party setting"},
    {"text/EN/mess_pk_customize_resident_0.bin", "customization"},
    {"text/EN/mess_pk_creation_room_0.bin", "creation room"},
    {"text/EN/mess_pk_story_mode_0.bin", "story mode"},
    {"by_crc/AE729F6B.bin", "unnamed customization"},
};
constexpr char16_t kLabel[] = u"Manikin";

struct State {
    std::atomic<bool> active{false};
    bool logged_tables{false};
    bool logged_portrait{false};
};
State &state() {
    static State s;
    return s;
}

bool off_by_switch() { return portablekit::env("NO_MANIKIN_LOOK") != nullptr; }

void left_out(const std::string &why) { std::cout << "[manikin] the Manikin look is left out: " << why << std::endl; }

std::uint32_t entry_crc(std::string_view path) {
    if (path.starts_with("by_crc/")) return static_cast<std::uint32_t>(std::strtoul(std::string(path.substr(7u, 8u)).c_str(), nullptr, 16));
    std::string text(path);
    for (char &c : text)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return pix::path_crc(text);
}

// The disc's own PACKAGE_INFO.BIN, whatever another part of the port serves
// the game in its place.
std::vector<std::uint8_t> read_index() {
    std::vector<std::uint8_t> bytes(4u << 20u);
    const std::int64_t got = portablekit::read_guest_file(kPackageInfo, 0u, bytes.data(), bytes.size());
    bytes.resize(got > 0 ? static_cast<std::size_t>(got) : 0u);
    return bytes;
}

std::vector<package_mods::LateFile> files_for_package(const std::function<bool(std::uint32_t)> &taken) {
    std::vector<package_mods::LateFile> out;
    if (portablekit::patched::active()) {
        left_out("a disc mod is on");
        return out;
    }
    std::string error;
    const auto index = pix::parse(read_index(), error);
    if (!index) {
        left_out("the disc's PACKAGE_INFO.BIN cannot be read (" + error + ")");
        return out;
    }
    for (const LabelFile &file : kLabelFiles) {
        const std::uint32_t crc = entry_crc(file.path);
        const auto entry = std::lower_bound(index->entries.begin(), index->entries.end(), crc,
                                            [](const pix::Entry &e, std::uint32_t wanted) { return e.crc < wanted; });
        if (entry == index->entries.end() || entry->crc != crc) continue;  // never opened on this disc
        if (taken(crc)) {
            left_out(std::string("a mod replaces the ") + file.name + " text (" + file.path + "), which names the looks");
            return {};
        }
        std::vector<std::uint8_t> bytes(entry->size);
        const std::int64_t got =
            portablekit::read_guest_file(kPackage, std::uint64_t(entry->sector) * pix::kSector, bytes.data(), bytes.size());
        if (got != static_cast<std::int64_t>(bytes.size())) {
            left_out(std::string("the ") + file.name + " text cannot be read from the disc");
            return {};
        }
        auto patched = manikin::with_manikin_label(bytes, kLabel);
        if (!patched) {
            left_out(std::string("the ") + file.name + " text (" + file.path + ") is not as the game's release has it");
            return {};
        }
        out.push_back({file.path, std::make_shared<const std::vector<std::uint8_t>>(std::move(*patched))});
    }
    if (out.empty()) {
        left_out("the disc has none of the menus' look names");
        return out;
    }
    state().active = true;
    std::cout << "[manikin] the Manikin look is on: " << out.size() << " menu texts name it" << std::endl;
    return out;
}

// ---- The menus ------------------------------------------------------------------

std::uint16_t ld16(Guest *g, std::uint32_t a) { return static_cast<std::uint16_t>(g_ld16s(g, a)); }

bool ram(const Guest *g, std::uint32_t a, std::uint32_t length) {
    const std::uint32_t offset = (a & 0x1FFFFFFFu) - GUEST_RAM_BASE;
    return offset < g->ram_size && g->ram_size - offset >= length;
}

// Costume 0 of each look table, as the disc has it, shows the manikin's label.
void show_manikin_label(Guest *g) {
    int changed = 0;
    for (const manikin::LabelTable &table : manikin::kLabelTables) {
        if (!ram(g, table.address, 2u * manikin::kCostumes)) continue;
        const std::vector<std::uint16_t> disc = manikin::disc_table(table);
        bool as_disc = true;
        for (int c = 1; c < manikin::kCostumes && as_disc; ++c)
            as_disc = ld16(g, table.address + 2u * static_cast<std::uint32_t>(c)) == disc[static_cast<std::size_t>(c)];
        if (!as_disc || ld16(g, table.address) != disc[0]) continue;
        g_st16(g, table.address, static_cast<std::uint16_t>(manikin::kManikinString));
        ++changed;
    }
    // Once a run, and again whenever not every table is the disc's: a run
    // that changes fewer than three is worth seeing.
    if (changed != 0 && (!state().logged_tables || changed != 3)) {
        state().logged_tables = true;
        std::printf("[manikin] the menu's look labels name costume 0 Manikin (%d of 3 tables)\n", changed);
        std::fflush(stdout);
    }
}

// Whether the game has the manikin of `character` to load: its costume 0
// fighter row.
bool manikin_there(Guest *g, int character) {
    return manikin::has_manikin(character) &&
           g_call3(g, kCostumeRow, static_cast<std::uint32_t>(character), 0u, 0u) != 0u;
}

void run_looks(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    regs.a[0] = in.gpr[4];
    decomp::call_original(g, kLooks, regs);
    out.v0 = regs.v0;
    if (!state().active) return;
    show_manikin_label(g);
    const int character = static_cast<std::int32_t>(in.gpr[4]);
    if (out.v0 != 0u && manikin_there(g, character)) out.v0 = manikin::with_manikin(character, out.v0);
}

void run_list(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 4; ++i) regs.a[i] = in.gpr[4 + i];
    decomp::call_original(g, kList, regs);
    out.v0 = regs.v0;
    if (!state().active) return;
    show_manikin_label(g);
    const std::uint32_t entries = in.gpr[4];
    const std::int32_t count = static_cast<std::int32_t>(out.v0);
    if (count <= 0 || count > 64 || !ram(g, entries, kListEntry * static_cast<std::uint32_t>(count))) return;
    for (std::int32_t i = 0; i < count; ++i) {
        const std::uint32_t entry = entries + kListEntry * static_cast<std::uint32_t>(i);
        const int character = g_ld32s(g, entry);
        const std::uint16_t mask = ld16(g, entry + kListMask);
        if (mask != 0u && manikin_there(g, character))
            g_st16(g, entry + kListMask, static_cast<std::uint16_t>(manikin::with_manikin(character, mask)));
    }
}

void run_next_look(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const auto mask = static_cast<std::uint16_t>(in.gpr[5]);
    if (state().active) show_manikin_label(g);
    if (!state().active || (mask & 1u) == 0u) {
        GuestRegs regs{};
        regs.a[0] = in.gpr[4];
        regs.a[1] = in.gpr[5];
        decomp::call_original(g, kNextLook, regs);
        out.v0 = regs.v0;
        return;
    }
    out.v0 = static_cast<std::uint32_t>(manikin::next_look(static_cast<std::int32_t>(in.gpr[4]), mask));
}

std::string text_at(Guest *g, std::uint32_t a, std::uint32_t most) {
    std::string text;
    for (std::uint32_t i = 0; i < most && ram(g, a + i, 1u); ++i) {
        const char c = static_cast<char>(g_ld8(g, a + i));
        if (c == '\0') return text;
        text += c;
    }
    return {};  // no end within the buffer
}

void write_text(Guest *g, std::uint32_t a, const std::string &text) {
    for (std::size_t i = 0; i <= text.size(); ++i)
        g_st8(g, a + static_cast<std::uint32_t>(i), i < text.size() ? static_cast<std::uint8_t>(text[i]) : 0u);
}

void run_language_path(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    regs.a[0] = in.gpr[4];
    regs.a[1] = in.gpr[5];
    decomp::call_original(g, kLanguagePath, regs);
    out.v0 = regs.v0;
    // Only the name it was given, while the battle loads its portraits.
    if (!state().active || g_ld8(g, kKeepPaths) == 0u || out.v0 != in.gpr[4]) return;
    const std::uint32_t path = out.v0;
    const std::string asked = text_at(g, path, kPathBuffer);
    const auto instead = manikin::portrait_instead(asked);
    if (!instead || instead->size() >= asked.size() || !ram(g, path, kPathBuffer)) return;
    write_text(g, path, *instead);
    if (!state().logged_portrait) {
        state().logged_portrait = true;
        std::printf("[manikin] the battle portrait of a manikin is its Normal look's (%s)\n", instead->c_str());
        std::fflush(stdout);
    }
}

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kLooks, "manikin_looks", decomp::Module::Main, decomp::Returns::Integer, &run_looks},
            {kList, "manikin_character_list", decomp::Module::Main, decomp::Returns::Integer, &run_list},
            {kNextLook, "manikin_next_look", decomp::Module::Main, decomp::Returns::Integer, &run_next_look},
            {kLanguagePath, "manikin_portrait_path", decomp::Module::Main, decomp::Returns::Integer, &run_language_path},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0],
                              std::getenv("D012_NO_MANIKIN_LOOK") == nullptr);
    }
} install_overrides;

} // namespace

void install() {
    static bool installed = false;
    if (installed) return;
    installed = true;
    if (off_by_switch()) {
        std::cout << "[manikin] off (" << portablekit::env_name("NO_MANIKIN_LOOK") << ")\n";
        return;
    }
    if (portablekit::env("NO_PACKAGE_MODS") != nullptr) {
        std::cout << "[manikin] off (" << portablekit::env_name("NO_PACKAGE_MODS") << ")\n";
        return;
    }
    package_mods::add_late_source("the Manikin look", &files_for_package);
}

bool active() { return state().active; }

} // namespace d012::manikin_look
