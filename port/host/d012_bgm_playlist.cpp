// The BGM playlist: series and tracks on the loading screen's battle music
// choice, mod tracks as rows of the game's BGM table, and the focus switch on
// L2/R2. See d012_bgm_playlist.hpp and docs/port/bgm-playlist-design.md.
// MIT licence; see port/README.md.

#include "d012_bgm_playlist.hpp"

#include "d012_loading_layout.hpp"
#include "d012_mods.hpp"

#include "../decomp/bgm_types.h"

#include "hle/hle_common.hpp"
#include "portablekit/hle_extension.hpp"
#include "profile.hpp"

#include "psprecomp/runtime.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "imgui.h"
#include "ui/ui.hpp"
#endif

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace d012::playlist {
namespace {

using decomp::CallState;
namespace ext = portablekit::hle_extension;

// Ids of mod rows (design, section 4): never the game's 0..130 or a DLC's.
constexpr std::uint32_t kModIdFirst = 0x4000u;
constexpr std::uint32_t kModIdLast = 0x7FFEu;
// The DLC name slot of a mod row: not a DLC slot (0..19), not free (-1).
constexpr std::int16_t kModNameSlot = -2;
// A name slot holds 0x80 bytes: 63 units and the end.
constexpr std::uint32_t kNameUnits = 63u;
constexpr std::uint32_t kSceCtrlReadBufferPositive = 0x1F803938u;
constexpr std::uint32_t kToggleButtons = ext::kExtraL2 | ext::kExtraR2;

bool flag(const char *name) {
    const char *value = std::getenv(name);
    return value != nullptr && *value != '\0' && std::strcmp(value, "0") != 0;
}

bool tracing() {
    static const bool on = flag("D012_TRACE_BGM_PLAYLIST");
    return on;
}

// D012_BGM_PLAYLIST_FLAT=1: the one-line selector of before (the track in
// the box, the series on a line under it, the pages where the game has
// them) instead of the series in the box and the track list under it.
bool flat() {
    static const bool on = flag("D012_BGM_PLAYLIST_FLAT");
    return on;
}

std::string hex(std::uint32_t value) {
    char text[16];
    std::snprintf(text, sizeof text, "0x%X", value);
    return text;
}

std::string trim(std::string text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) text.pop_back();
    std::size_t start = 0;
    while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start]))) ++start;
    return text.substr(start);
}

std::string upper(std::string text) {
    for (char &c : text) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return text;
}

// The game's text is 16-bit units, ASCII where it is ASCII: main 0x0887DF14
// makes them from bytes that way. Anything else shows as '?' here (logs and
// series names only).
std::string text_of(Guest *g, gaddr text) {
    std::string out;
    if (text == 0) return out;
    for (std::uint32_t i = 0; i < 128u; ++i) {
        const auto unit = static_cast<std::uint16_t>(g_ld16s(g, text + i * 2u));
        if (unit == 0u) break;
        out += unit >= 0x20u && unit < 0x7Fu ? static_cast<char>(unit) : '?';
    }
    return out;
}

// `text` (UTF-8) in the game's units, at most `max_units` and the end; a
// character outside ASCII becomes '?', which every font has.
void write_text(Guest *g, gaddr out, const std::string &text, std::uint32_t max_units) {
    std::uint32_t n = 0;
    for (std::size_t i = 0; i < text.size() && n < max_units;) {
        const auto c = static_cast<unsigned char>(text[i]);
        std::uint16_t unit = '?';
        std::size_t length = 1;
        if (c < 0x80u) unit = c >= 0x20u ? c : '?';
        else if ((c & 0xE0u) == 0xC0u) length = 2;
        else if ((c & 0xF0u) == 0xE0u) length = 3;
        else if ((c & 0xF8u) == 0xF0u) length = 4;
        g_st16(g, out + n * 2u, unit);
        ++n;
        i += length;
    }
    g_st16(g, out + n * 2u, 0);
}

// ---- Mod tracks as rows of the BGM table -----------------------------------

struct ModRow {
    std::int32_t id{};
    const mods::Track *track{};
    std::size_t series{};  // into mods::catalogue().series
};

struct ModRows {
    bool done = false;
    gaddr table = 0;
    std::int32_t first_row = -1;
    std::vector<ModRow> rows;  // in row order
    std::map<std::int32_t, std::size_t> by_id;
};

ModRows &mod_rows() {
    static ModRows rows;
    return rows;
}

const ModRow *mod_row_of(std::int32_t id) {
    const ModRows &m = mod_rows();
    const auto found = m.by_id.find(id);
    return found == m.by_id.end() ? nullptr : &m.rows[found->second];
}

// Appends the enabled mods' tracks to the table, after the disc's rows and
// any DLC's, once (and again if the table was rebuilt). Called from the list,
// so that DLC rows, appended at boot, are in first.
void insert_mod_rows(Guest *g) {
    ModRows &m = mod_rows();
    const gaddr table = g_ld32(g, G_BGM_TABLE);
    if (table == 0) return;
    const BgmTable *t = G_STRUCT(g, BgmTable, table);
    BgmFile *file = G_STRUCT(g, BgmFile, t->file);
    if (m.done && m.table == table) {
        if (m.rows.empty()) return;
        const gaddr first = t->entries + static_cast<std::uint32_t>(m.first_row) * sizeof(BgmEntry);
        if (m.first_row + static_cast<std::int32_t>(m.rows.size()) <= file->count &&
            G_STRUCT(g, BgmEntry, first)->id == m.rows.front().id)
            return;
        std::cout << "[playlist] the BGM table no longer holds the mod rows; adding them again" << std::endl;
    }
    m = ModRows{};
    m.done = true;
    m.table = table;
    const mods::Catalogue &catalogue = mods::catalogue();
    if (catalogue.tracks.empty()) return;

    // Ids: 0x4000 + (number % 0x3FFF), the next free one upwards (wrapping
    // in the range) past ids in use, the tracks taken in id-text order, so an
    // id depends only on which tracks there are.
    std::set<std::int32_t> used;
    for (std::int32_t row = 0; row < file->count; ++row)
        used.insert(G_STRUCT(g, BgmEntry, t->entries + static_cast<std::uint32_t>(row) * sizeof(BgmEntry))->id);
    std::vector<const mods::Track *> by_text;
    for (const mods::Track &track : catalogue.tracks) by_text.push_back(&track);
    std::sort(by_text.begin(), by_text.end(), [](const mods::Track *a, const mods::Track *b) { return a->id < b->id; });
    std::map<const mods::Track *, std::int32_t> id_of;
    constexpr std::uint32_t kRange = kModIdLast - kModIdFirst + 1u;
    for (const mods::Track *track : by_text) {
        std::uint32_t id = kModIdFirst + track->number % kRange;
        for (std::uint32_t tries = 0; used.count(static_cast<std::int32_t>(id)) != 0u && tries < kRange; ++tries)
            id = id == kModIdLast ? kModIdFirst : id + 1u;
        used.insert(static_cast<std::int32_t>(id));
        id_of[track] = static_cast<std::int32_t>(id);
    }

    // Their volume: the median of the game's own battle music (the rows it
    // offers; 37 to 80 percent, mostly about 50), so a track mastered as
    // loud as the game's own plays about as loud. 100 without any.
    std::vector<int> volumes;
    for (std::int32_t row = 0; row < t->disc_count && row < file->count; ++row) {
        const BgmEntry *e = G_STRUCT(g, BgmEntry, t->entries + static_cast<std::uint32_t>(row) * sizeof(BgmEntry));
        if (e->weight != 0 && e->volume > 0) volumes.push_back(e->volume);
    }
    std::sort(volumes.begin(), volumes.end());
    const std::int8_t volume = volumes.empty() ? std::int8_t{100} : static_cast<std::int8_t>(volumes[volumes.size() / 2]);

    // Rows in display order, as many as the table has room for.
    m.first_row = file->count;
    std::int32_t row = file->count;
    std::size_t left_out = 0;
    for (std::size_t s = 0; s < catalogue.series.size(); ++s) {
        for (const std::size_t index : catalogue.series[s].tracks) {
            const mods::Track &track = catalogue.tracks[index];
            if (row >= BGM_TABLE_CAPACITY) {
                std::cerr << "[playlist] error: no room in the BGM table (" << BGM_TABLE_CAPACITY
                          << " rows) for " << track.id << "; it is left out\n";
                ++left_out;
                continue;
            }
            const gaddr at = t->entries + static_cast<std::uint32_t>(row) * sizeof(BgmEntry);
            BgmEntry *e = G_STRUCT(g, BgmEntry, at);
            std::memset(e, 0, sizeof *e);
            e->id = static_cast<std::int16_t>(id_of.at(&track));
            e->name_text = 0;
            for (std::int16_t &flag : e->unlock) flag = -1;
            e->flags = 0;      // not a DLC: streamed from the disc folder, which the mods module serves
            e->volume = volume;
            e->weight = 1;     // offered by the selector; no stage bits, so never the random default
            std::snprintf(e->file, sizeof e->file, "MD%08X.AT3", track.number);
            const gaddr slot = t->dlc_names + static_cast<std::uint32_t>(row - t->disc_count) * sizeof(BgmDlcName);
            BgmDlcName *name = G_STRUCT(g, BgmDlcName, slot);
            name->dlc_slot = kModNameSlot;
            name->row = static_cast<std::int16_t>(row);
            write_text(g, slot + 4u, track.title, kNameUnits);
            m.by_id[e->id] = m.rows.size();
            m.rows.push_back(ModRow{e->id, &track, s});
            if (tracing())
                std::cout << "[playlist] row " << row << ": id " << hex(static_cast<std::uint32_t>(e->id)) << " "
                          << e->file << " " << track.id << " \"" << track.title << "\"" << std::endl;
            ++row;
        }
    }
    file->count = row;
    std::cout << "[playlist] " << m.rows.size() << " mod track(s) added to the BGM table as rows " << m.first_row
              << ".." << row - 1 << " at volume " << int(volume) << "%"
              << (left_out != 0u ? ", " + std::to_string(left_out) + " left out" : "") << std::endl;
}

// ---- Series ------------------------------------------------------------------

struct Choice {
    gaddr entry{};
    gaddr name{};
    bool dlc_twin = false;  // a DLC track named as a disc track of its series: "(DLC)" in the list
};

struct Series {
    std::string label;   // what the overlay shows
    bool mod = false;
    std::vector<Choice> choices;
    std::size_t first = 0;  // its first index in the selector
    std::size_t last = 0;   // the position last shown, for coming back to it
};

// A disc row's series, from its name: "FFXIII - Saber's Edge" is FINAL
// FANTASY XIII's. A name without an FF prefix (Cosmos, Chaos's themes, the
// opening) is one of Dissidia's own.
std::string game_series(const std::string &name) {
    const std::size_t dash = name.find(" - ");
    if (dash != std::string::npos) {
        const std::string prefix = upper(trim(name.substr(0, dash)));
        std::string rest;
        if (prefix.rfind("FINAL FANTASY", 0) == 0) rest = trim(prefix.substr(13));
        else if (prefix.rfind("FF", 0) == 0) rest = trim(prefix.substr(2));
        if (!rest.empty()) return "FINAL FANTASY " + rest;
    }
    return "DISSIDIA";
}

// A game track's name without its series: "FFXIII - Saber's Edge" is
// "Saber's Edge" under FINAL FANTASY XIII. A name without an FF prefix is
// kept whole.
std::string title_in_series(const std::string &name) {
    const std::size_t dash = name.find(" - ");
    if (dash != std::string::npos && game_series(name).rfind("FINAL FANTASY", 0) == 0) return trim(name.substr(dash + 3));
    return name;
}

// The offered choices (in table order) grouped: the game's series in the
// order the table has them, DLC rows in their game's series after its disc
// rows (by name, as the disc's: "FFI - Chaos Shrine" is FINAL FANTASY I's)
// and under DOWNLOADED without an FF prefix, then the mods' series in the
// catalogue's order, then "No BGM" (the row with no file) last.
std::vector<Series> group(Guest *g, const std::vector<Choice> &offered) {
    const gaddr table = g_ld32(g, G_BGM_TABLE);
    const BgmTable *t = G_STRUCT(g, BgmTable, table);
    const mods::Catalogue &catalogue = mods::catalogue();
    std::vector<Series> game;
    std::map<std::string, std::size_t> game_index;
    Series downloaded{"DOWNLOADED"};
    Series none{"NO MUSIC"};
    std::vector<Series> added(catalogue.series.size());
    for (std::size_t i = 0; i < added.size(); ++i) {
        added[i].label = catalogue.series[i].name;
        added[i].mod = true;
    }
    // DLC rows (and the mods' rows) come after the disc's.
    const auto from_dlc = [&](const Choice &choice) {
        return static_cast<std::int32_t>((choice.entry - t->entries) / sizeof(BgmEntry)) >= t->disc_count;
    };
    for (const Choice &choice : offered) {
        const BgmEntry *e = G_STRUCT(g, BgmEntry, choice.entry);
        const bool dlc = from_dlc(choice);
        if (const ModRow *mod = mod_row_of(e->id); mod != nullptr && dlc) {
            added[mod->series].choices.push_back(choice);
        } else if (dlc && game_series(text_of(g, choice.name)).rfind("FINAL FANTASY", 0) != 0) {
            downloaded.choices.push_back(choice);
        } else if (e->file[0] == '\0') {
            none.choices.push_back(choice);
        } else {
            const std::string label = game_series(text_of(g, choice.name));
            const auto [at, inserted] = game_index.emplace(label, game.size());
            if (inserted) game.push_back(Series{label});
            game[at->second].choices.push_back(choice);
        }
    }
    // A DLC track with a disc track's title in its series ("Battle" under
    // FINAL FANTASY I, on the disc and in a DLC pack) is told apart in the list.
    for (Series &series : game) {
        std::set<std::string> disc_titles;
        for (const Choice &choice : series.choices)
            if (!from_dlc(choice)) disc_titles.insert(upper(title_in_series(text_of(g, choice.name))));
        for (Choice &choice : series.choices)
            choice.dlc_twin = from_dlc(choice) && disc_titles.count(upper(title_in_series(text_of(g, choice.name)))) != 0u;
    }
    std::vector<Series> all = std::move(game);
    if (!downloaded.choices.empty()) all.push_back(std::move(downloaded));
    for (Series &series : added)
        if (!series.choices.empty()) all.push_back(std::move(series));
    if (!none.choices.empty()) all.push_back(std::move(none));
    return all;
}

// ---- The screen being shown --------------------------------------------------

struct Screen {
    gaddr selector = 0;
    std::vector<Series> series;
    std::vector<std::size_t> series_of;  // per selector index
    bool bgm_focus = false;
    std::uint32_t extra_held = 0;        // extra buttons held at the last frame
    int repeat_direction = 0;            // up/down held in BGM focus: -1, 0, 1
    float repeat_timer = 0.0f;
    std::uint64_t update_reads = 0;      // pad reads when the selector last took input
    bool anchors_logged = false;
    bool started = false;
    std::uint32_t frames = 0;
    // The box shows the series (and the track's place in it) instead of
    // the track: every name the selector holds points at this text, in its
    // own memory (0: the box shows the track, as the game's).
    gaddr box_text = 0;
    std::uint64_t serial = 0;  // counts the screens
};

// Where the box's text is kept: the end of the selector's entry[] array,
// past the choices (which need count <= 256 - kBoxWords). 49 units and the end.
constexpr std::uint32_t kBoxWords = 25u;
constexpr std::uint32_t kBoxUnits = kBoxWords * 2u - 1u;

gaddr box_text_at(gaddr selector, std::int32_t count) {
    if (flat() || count > BGM_SELECTOR_MAX - static_cast<std::int32_t>(kBoxWords)) return 0;
    return selector + 0x18u + (BGM_SELECTOR_MAX - kBoxWords) * 4u;
}

Screen &screen() {
    static Screen value;
    return value;
}

std::uint64_t next_serial() {
    static std::uint64_t serial = 0;
    return ++serial;
}

// The name of the choice at `index`, from the series (the selector's own
// names may all point at the box's text).
gaddr name_of(const Screen &sc, std::int32_t index) {
    if (index < 0 || static_cast<std::size_t>(index) >= sc.series_of.size()) return 0;
    const Series &series = sc.series[sc.series_of[static_cast<std::size_t>(index)]];
    return series.choices[static_cast<std::size_t>(index) - series.first].name;
}

// "FINAL FANTASY X . 3/8" (a middle dot, U+00B7) in the game's units: the
// series and the place of the choice at `index` in it.
void write_box_text(Guest *g, const Screen &sc, std::int32_t index) {
    if (sc.box_text == 0 || index < 0 || static_cast<std::size_t>(index) >= sc.series_of.size()) return;
    const Series &series = sc.series[sc.series_of[static_cast<std::size_t>(index)]];
    const std::string place = std::to_string(static_cast<std::size_t>(index) - series.first + 1) + "/" +
                              std::to_string(series.choices.size());
    std::string label = series.label;
    // Long series names (a mod's) are cut so that the place still shows.
    const std::size_t room = kBoxUnits - 3u - place.size();
    if (label.size() > room) label = label.substr(0, room - 3u) + "...";
    std::uint32_t n = 0;
    for (const char c : label) {
        const auto unit = static_cast<unsigned char>(c);
        g_st16(g, sc.box_text + n++ * 2u, unit >= 0x20u && unit < 0x7Fu ? unit : '?');
    }
    g_st16(g, sc.box_text + n++ * 2u, ' ');
    g_st16(g, sc.box_text + n++ * 2u, 0x00B7u);
    g_st16(g, sc.box_text + n++ * 2u, ' ');
    for (const char c : place) g_st16(g, sc.box_text + n++ * 2u, static_cast<unsigned char>(c));
    g_st16(g, sc.box_text + n * 2u, 0);
}

// Points every name of the selector's (or the build's) array at the box's
// text, which then holds the choice at `index`.
void use_box_text(Guest *g, Screen &sc, gaddr names, std::int32_t count, std::int32_t index) {
    if (sc.box_text == 0) return;
    for (std::int32_t i = 0; i < count; ++i) g_st32(g, names + static_cast<std::uint32_t>(i) * 4u, sc.box_text);
    write_box_text(g, sc, index);
}

// ---- The box's name sprite, swapped without a gap ---------------------------
//
// The game retires the old name sprite and makes the new one from the old
// one's end callback (battle 0x08AF5C6C). That callback belongs to the old
// sprite's text block (its glyph buffer, sprite +0x00), which the text
// system's frame (main 0x0887DBD4) frees two passes after the block's last
// sprite went: reference count 1 -> 0 at the delete, 0 -> -1 at the next pass,
// and at the pass after that (the GPU no longer reads it) the buffer is freed
// (main 0x0887D828), the block goes back to its pool and the callback runs.
// So the box had no name for two game frames at every change (V:
// D012_TRACE_BGM_NAME, design doc 11.7). A
// new sprite draws in the frame it is made (main 0x0887D868 builds its
// glyphs at once), so the playlist makes the new sprite first and retires the
// old one in the same frame, without the callback: one text sprite made and
// one deleted per change, as the game's way, and the old block is still
// freed by the text system two frames later, as always.

// Takes `sprite` out of layout element `owner`'s sprite list (as the game
// does inline at battle 0x08AF8DF8).
void unlink_name(Guest *g, gaddr owner, gaddr sprite) {
    g_st32(g, sprite + SPRITE_FLAGS, g_ld32(g, sprite + SPRITE_FLAGS) & ~SPRITE_IN_LIST);
    const gaddr prev = g_ld32(g, sprite + SPRITE_PREV);
    const gaddr next = g_ld32(g, sprite + SPRITE_NEXT);
    if (prev == 0) g_st32(g, owner + LAYOUT_SPRITE_HEAD, next);
    else g_st32(g, prev + SPRITE_NEXT, next);
    if (next == 0) g_st32(g, owner + LAYOUT_SPRITE_TAIL, prev);
    else g_st32(g, next + SPRITE_PREV, prev);
    g_st32(g, sprite + SPRITE_PREV, 0);
    g_st32(g, sprite + SPRITE_NEXT, 0);
    g_st32(g, owner + LAYOUT_SPRITE_COUNT, g_ld32(g, owner + LAYOUT_SPRITE_COUNT) - 1u);
}

// D012_TRACE_BGM_NAME=1: the name sprite and the text system's counts (live
// sprites +0x170, live text blocks +0x17C) at each change and for a few
// frames after it.
bool tracing_name() {
    static const bool on = flag("D012_TRACE_BGM_NAME");
    return on;
}

struct NameTrace {
    int frames_left = 0;
    std::uint64_t updates = 0;
    std::uint64_t changes = 0;
};

NameTrace &name_trace() {
    static NameTrace value;
    return value;
}

void trace_name(Guest *g, gaddr selector, const char *where) {
    if (!tracing_name()) return;
    const gaddr text_system = g_ld32(g, G_TEXT_SYSTEM);
    const gaddr text = G_STRUCT(g, BgmSelector, selector)->text;
    const gaddr block = text != 0 ? g_ld32(g, text) : 0u;
    std::printf("[playlist] name %s: update %llu, change %llu, sprite %08X (block %08X), text system: %u sprites, "
                "%u blocks\n",
                where, static_cast<unsigned long long>(name_trace().updates),
                static_cast<unsigned long long>(name_trace().changes), text, block,
                text_system != 0 ? g_ld32(g, text_system + 0x170u) : 0u,
                text_system != 0 ? g_ld32(g, text_system + 0x17Cu) : 0u);
    std::fflush(stdout);
}

// The name sprite of the current choice made at once, the old one retired
// in the same frame. Falls back to the game's way (the old sprite's end
// callback makes the new one) when there is no old sprite or the text system
// could not make a new one now.
void swap_name(Guest *g, gaddr selector) {
    BgmSelector *s = G_STRUCT(g, BgmSelector, selector);
    ++name_trace().changes;
    const gaddr old = s->text;
    if (old == 0 || s->name[s->index] == 0 || s->layout == 0) {
        bgm_selector_name_changed(g, selector);
        return;
    }
    s->text = 0;
    bgm_selector_make_name(g, selector);  // the game's: makes, shows and lists the new sprite
    if (s->text == 0) {
        std::cerr << "[playlist] the text system could not make the name now; it follows the game's way\n";
        s->text = old;
        bgm_selector_name_changed(g, selector);
        return;
    }
    unlink_name(g, s->layout, old);
    g_call1(g, F_SPRITE_FINISH, old);
    g_call1(g, F_SPRITE_DELETE, old);
}

// After the choice changed: the box's text, then the name sprite made again.
void show_choice(Guest *g, const Screen &sc, gaddr selector) {
    write_box_text(g, sc, G_STRUCT(g, BgmSelector, selector)->index);
    swap_name(g, selector);
    if (tracing_name()) {
        name_trace().frames_left = 3;
        trace_name(g, selector, "changed");
    }
}

// The pad as the game read it last, before the wrapper took anything out.
struct PadSeen {
    std::uint64_t reads = 0;
    std::uint32_t raw = 0;
};

PadSeen &pad_seen() {
    static PadSeen value;
    return value;
}

// What the overlay shows; written by the selector each frame.
struct Overlay {
    std::uint64_t updates = 0;
    std::uint64_t updates_seen = 0;
    int idle_frames = 1000;
    bool input = false;
    bool bgm_focus = false;
    float x = 0.0f, y = 0.0f;              // anchor 0 of the BGM element: the track name's centre
    float alpha = 0.0f;
    std::string series;
    std::size_t position = 0, count = 0;   // in the series
    // The track list: drawn at all, the titles of the series shown, which
    // series it is and on which screen.
    bool wheel = false;
    std::vector<std::string> titles;
    std::size_t series_index = 0;
    std::uint64_t screen_serial = 0;
};

Overlay &overlay() {
    static Overlay value;
    return value;
}

// Lays the grouped choices out in the selector's two arrays; returns how many.
std::int32_t write_choices(Guest *g, Screen &sc, gaddr entries, gaddr names, std::int32_t max) {
    sc.series_of.clear();
    std::int32_t index = 0;
    for (std::size_t s = 0; s < sc.series.size(); ++s) {
        Series &series = sc.series[s];
        series.first = static_cast<std::size_t>(index);
        std::size_t kept = 0;
        for (const Choice &choice : series.choices) {
            if (index >= max) break;
            g_st32(g, entries + static_cast<std::uint32_t>(index) * 4u, choice.entry);
            g_st32(g, names + static_cast<std::uint32_t>(index) * 4u, choice.name);
            sc.series_of.push_back(s);
            ++index;
            ++kept;
        }
        series.choices.resize(kept);
    }
    sc.series.erase(std::remove_if(sc.series.begin(), sc.series.end(), [](const Series &s) { return s.choices.empty(); }),
                    sc.series.end());
    return index;
}

void log_choices(Guest *g, const Screen &sc) {
    std::size_t total = 0;
    for (const Series &series : sc.series) total += series.choices.size();
    std::cout << "[playlist] " << total << " tracks in " << sc.series.size() << " series:";
    for (const Series &series : sc.series) std::cout << " " << series.label << " (" << series.choices.size() << ")";
    std::cout << std::endl;
    if (!tracing()) return;
    for (const Series &series : sc.series)
        for (std::size_t i = 0; i < series.choices.size(); ++i) {
            const BgmEntry *e = G_STRUCT(g, BgmEntry, series.choices[i].entry);
            std::cout << "[playlist]   " << series.first + i << ": " << series.label << " " << i + 1 << "/"
                      << series.choices.size() << " id " << e->id << " volume " << int(e->volume) << " " << e->file
                      << " \""
                      << text_of(g, series.choices[i].name) << "\"" << std::endl;
        }
}

// A selector the list was not seen building (or one of another screen):
// regroups the choices it holds, keeping the one shown.
void adopt(Guest *g, gaddr selector) {
    Screen &sc = screen();
    BgmSelector *s = G_STRUCT(g, BgmSelector, selector);
    std::vector<Choice> offered;
    const gaddr box = box_text_at(selector, s->count);
    const gaddr table = g_ld32(g, G_BGM_TABLE);
    for (std::int32_t i = 0; i < s->count && i < BGM_SELECTOR_MAX; ++i) {
        // Names that point at the box's text (a selector of the playlist's
        // own, seen again) are the table's names of their rows.
        gaddr name = s->name[i];
        if (name == box && box != 0)
            name = bgm_table_name_of(g, table, static_cast<std::uint32_t>(G_STRUCT(g, BgmEntry, s->entry[i])->id));
        offered.push_back({s->entry[i], name});
    }
    const gaddr shown = s->index >= 0 && s->index < s->count ? s->entry[s->index] : 0u;
    sc = Screen{};
    sc.serial = next_serial();
    sc.selector = selector;
    sc.series = group(g, offered);
    s->count = write_choices(g, sc, selector + 0x18u, selector + 0x418u, s->count);
    for (std::int32_t i = 0; i < s->count; ++i)
        if (s->entry[i] == shown) s->index = i;
    sc.box_text = box_text_at(selector, s->count);
    use_box_text(g, sc, selector + 0x418u, s->count, s->index);
    if (sc.box_text != 0) swap_name(g, selector);
    if (!flat()) loading_layout::move_pages(g, selector - 0x6ACu);
    std::cout << "[playlist] took over a selector at " << hex(selector) << std::endl;
    log_choices(g, sc);
}

std::string describe(Guest *g, gaddr selector) {
    const Screen &sc = screen();
    const BgmSelector *s = G_STRUCT(g, BgmSelector, selector);
    if (s->index < 0 || s->index >= s->count || static_cast<std::size_t>(s->index) >= sc.series_of.size()) return "?";
    const Series &series = sc.series[sc.series_of[static_cast<std::size_t>(s->index)]];
    const BgmEntry *e = G_STRUCT(g, BgmEntry, s->entry[s->index]);
    return series.label + " " + std::to_string(static_cast<std::size_t>(s->index) - series.first + 1) + "/" +
           std::to_string(series.choices.size()) + ": id " + std::to_string(e->id) + " " + e->file + " \"" +
           text_of(g, name_of(sc, s->index)) + "\"";
}

bool pad_repeats(Guest *g, gaddr pad, gaddr mask) {
    return g_vcall_u64(g, pad, PAD_VTABLE, PAD_SLOT_IS_REPEAT, g_ld32(g, mask), g_ld32(g, mask + 4u)) != 0;
}

void cursor_sound(Guest *g) { g_call4(g, F_SE_PLAY, 0, 10000, 1, 0); }

void press_arrow(Guest *g, gaddr arrow, std::uint32_t pressed, std::uint16_t rest) {
    g_call3(g, F_LAYOUT_PLAY, arrow, pressed, 1);
    g_st16(g, arrow + LAYOUT_IDLE_ANIMATION, rest);
    cursor_sound(g);
}

// Up (-1), down (1) or neither in the pad as read (the game no longer sees
// them in BGM focus).
int vertical_direction() {
    const std::uint32_t raw = pad_seen().raw;
    const bool up = (raw & RAW_DPAD_UP) != 0u;
    const bool down = (raw & RAW_DPAD_DOWN) != 0u;
    return up == down ? 0 : (up ? -1 : 1);
}

bool sideways_held() { return (pad_seen().raw & (RAW_DPAD_LEFT | RAW_DPAD_RIGHT)) != 0u; }

// Up or down seen while left or right is held belong to that press (a D-pad
// rocked on the way to or from a side, a thumb on a diagonal): they count as
// held from now, so they step only if still held when the key repeat's delay
// has passed. Without this a left/right press that touched up or down also
// stepped the track (design doc 11.8).
void hold_vertical(Screen &sc) {
    sc.repeat_direction = vertical_direction();
    sc.repeat_timer = PAD_REPEAT_DELAY;
}

// Up or down in BGM focus, from the pad as read, with the pad module's own
// key repeat.
int vertical_step(Guest *g, Screen &sc) {
    const int direction = vertical_direction();
    if (direction != sc.repeat_direction) {
        sc.repeat_direction = direction;
        sc.repeat_timer = PAD_REPEAT_DELAY;
        return direction;
    }
    if (direction == 0) return 0;
    float frame_step = 0.0f;
    const std::uint32_t bits = g_ld32(g, G_PAD_FRAME_STEP);
    std::memcpy(&frame_step, &bits, sizeof frame_step);
    sc.repeat_timer -= std::max(frame_step, 2.0f);
    if (sc.repeat_timer > 0.0f) return 0;
    sc.repeat_timer = PAD_REPEAT_INTERVAL;
    return direction;
}

// D012_TEST_BGM_START=<BGM id, or a mod track's id text>: the selector starts
// on that track instead of the game's default, which is random. A test switch
// for repeatable runs.
void test_start(Guest *g, gaddr selector) {
    const char *value = std::getenv("D012_TEST_BGM_START");
    if (value == nullptr || *value == '\0') return;
    BgmSelector *s = G_STRUCT(g, BgmSelector, selector);
    std::int32_t id = -1;
    for (const ModRow &row : mod_rows().rows)
        if (row.track->id == value) id = row.id;
    if (id < 0) {
        char *end = nullptr;
        const long number = std::strtol(value, &end, 0);
        if (end != value && *end == '\0') id = static_cast<std::int32_t>(number);
    }
    for (std::int32_t i = 0; i < s->count; ++i) {
        if (G_STRUCT(g, BgmEntry, s->entry[i])->id != id) continue;
        if (i != s->index) {
            s->index = i;
            show_choice(g, screen(), selector);
        }
        std::cout << "[playlist] test switch D012_TEST_BGM_START: starts on " << describe(g, selector) << std::endl;
        return;
    }
    std::cout << "[playlist] test switch D012_TEST_BGM_START=" << value << ": not offered; the game's default stays"
              << std::endl;
}

// A track's line in the list: a mod track's own title (any character the
// font has); a game track's name without its series (which the box shows),
// with "(DLC)" after a DLC track that has a disc track's title.
std::string list_title(Guest *g, const Choice &choice) {
    const BgmEntry *e = G_STRUCT(g, BgmEntry, choice.entry);
    if (const ModRow *mod = mod_row_of(e->id)) return mod->track->title;
    const std::string title = title_in_series(text_of(g, choice.name));
    return choice.dlc_twin ? title + " (DLC)" : title;
}

void note_overlay(Guest *g, gaddr selector, gaddr position, gaddr colour, bool input) {
    Overlay &o = overlay();
    const Screen &sc = screen();
    const BgmSelector *s = G_STRUCT(g, BgmSelector, selector);
    float matrix[16];
    std::memcpy(matrix, g_ptr(g, position, sizeof matrix), sizeof matrix);
    o.x = matrix[12];
    o.y = matrix[13];
    o.alpha = static_cast<float>(g_ld8(g, colour + 3u)) / 255.0f;
    ++o.updates;
    o.input = input;
    o.bgm_focus = sc.bgm_focus;
    // The track list is drawn whenever the box shows the series; over the
    // pages if they could not be moved (a layout other than the one known).
    o.wheel = sc.box_text != 0;
    o.series.clear();
    o.position = o.count = 0;
    if (s->index >= 0 && s->index < s->count && static_cast<std::size_t>(s->index) < sc.series_of.size()) {
        const std::size_t index = sc.series_of[static_cast<std::size_t>(s->index)];
        const Series &series = sc.series[index];
        o.series = series.label;
        o.position = static_cast<std::size_t>(s->index) - series.first + 1u;
        o.count = series.choices.size();
        if (o.titles.size() != series.choices.size() || o.series_index != index || o.screen_serial != sc.serial) {
            o.series_index = index;
            o.titles.clear();
            for (const Choice &choice : series.choices) o.titles.push_back(list_title(g, choice));
        }
    }
    o.screen_serial = sc.serial;
}

// D012_TEST_BGM_UNLOCK_ALL=1: the selector offers every row with a weight,
// unlocked or not (the test save has three tracks a series at most; the
// game has five). Test only: the save is not touched.
bool test_unlock_all() {
    static const bool on = flag("D012_TEST_BGM_UNLOCK_ALL");
    return on;
}

std::int32_t list_every_row(Guest *g, gaddr entries, gaddr names, std::int32_t max) {
    const gaddr table = g_ld32(g, G_BGM_TABLE);
    if (table == 0) return 0;
    const BgmTable *t = G_STRUCT(g, BgmTable, table);
    const std::int32_t rows = G_STRUCT(g, BgmFile, t->file)->count;
    std::int32_t n = 0;
    for (std::int32_t row = 0; row < rows && n < max; ++row) {
        const gaddr entry = t->entries + static_cast<std::uint32_t>(row) * sizeof(BgmEntry);
        if (G_STRUCT(g, BgmEntry, entry)->weight == 0) continue;
        g_st32(g, entries + static_cast<std::uint32_t>(n) * 4u, entry);
        g_st32(g, names + static_cast<std::uint32_t>(n) * 4u, bgm_table_name(g, table, row));
        ++n;
    }
    std::cout << "[playlist] test switch D012_TEST_BGM_UNLOCK_ALL: " << n << " rows offered, locked or not" << std::endl;
    return n;
}

} // namespace

bool enabled() {
    static const bool on = !flag("D012_NO_BGM_PLAYLIST");
    return on;
}

// battle 0x08ADE4E0, from the selector's build (battle 0x08AF590C), whose $s0
// is the selector: the offered rows as the game computes them (unlocked, a
// weight), grouped by series. The build then finds the default's index.
void run_list(Guest *g, const CallState &in, GuestRegs &out) {
    const gaddr entries = in.gpr[5];
    const gaddr names = in.gpr[6];
    const auto max = static_cast<std::int32_t>(in.gpr[7]);
    insert_mod_rows(g);
    const std::int32_t offered_count = test_unlock_all() ? list_every_row(g, entries, names, max)
                                                         : bgm_list_offered(g, in.gpr[4], entries, names, max);
    std::vector<Choice> offered;
    for (std::int32_t i = 0; i < offered_count; ++i)
        offered.push_back({g_ld32(g, entries + static_cast<std::uint32_t>(i) * 4u),
                           g_ld32(g, names + static_cast<std::uint32_t>(i) * 4u)});
    Screen &sc = screen();
    const gaddr selector = in.gpr[16];
    sc = Screen{};
    sc.serial = next_serial();
    sc.selector = selector;
    sc.extra_held = ext::extra_buttons();
    sc.series = group(g, offered);
    const std::int32_t count = write_choices(g, sc, entries, names, max);
    log_choices(g, sc);
    // The series in the box and the track list under it: the pages of the
    // screen's right half move down to make room. The build (battle
    // 0x08AF590C) shows the choice whose id is the screen's default (the
    // last one, else the first) right after this returns, so the box's
    // text is that choice's already.
    if (!flat()) {
        sc.box_text = box_text_at(selector, count);
        const std::int32_t start_id = g_ld32s(g, selector - 0x6ACu + 0xF60u);
        std::int32_t start = 0;
        for (std::int32_t i = 0; i < count; ++i)
            if (G_STRUCT(g, BgmEntry, g_ld32(g, entries + static_cast<std::uint32_t>(i) * 4u))->id == start_id) start = i;
        use_box_text(g, sc, names, count, start);
        if (sc.box_text == 0)
            std::cerr << "[playlist] " << count << " choices leave no room for the series text; the box shows the track\n";
        loading_layout::move_pages(g, selector - 0x6ACu);
    }
    bool same = count == offered_count;
    for (std::int32_t i = 0; same && i < count; ++i)
        same = g_ld32(g, entries + static_cast<std::uint32_t>(i) * 4u) == offered[static_cast<std::size_t>(i)].entry;
    std::cout << "[playlist] the game offers " << offered_count << "; the flat list (left/right in menu focus) is "
              << (same ? "in the game's own order" : "regrouped by series") << std::endl;
    out.v0 = static_cast<std::uint32_t>(count);
}

// battle 0x08AF8CA4: one frame of the selector.
void run_update(Guest *g, const CallState &in, GuestRegs &) {
    const gaddr selector = in.gpr[4];
    const bool take_input = (in.gpr[5] & 0xFFu) == 1u;
    Screen &sc = screen();
    if (sc.selector != selector || sc.series_of.size() != static_cast<std::size_t>(G_STRUCT(g, BgmSelector, selector)->count))
        adopt(g, selector);
    const gaddr frame = g_push(g, 0x60u);
    const gaddr position = frame;          /* a 4x4 matrix */
    const gaddr colour = frame + 0x40u;    /* RGBA */
    BgmSelector *s = G_STRUCT(g, BgmSelector, selector);
    ++name_trace().updates;
    if (name_trace().frames_left > 0) {
        --name_trace().frames_left;
        trace_name(g, selector, "frame");
    }
    if (++sc.frames == 60u && flag("D012_TRACE_BGM_LAYOUT")) loading_layout::dump(g, selector - 0x6ACu);
    // The pages drew their rows earlier in this frame (battle 0x08AF94D8).
    loading_layout::hide_overflow(g, selector - 0x6ACu);
    if (!sc.started) {
        sc.started = true;
        std::cout << "[playlist] starts on " << describe(g, selector) << std::endl;
        test_start(g, selector);
    }

    if (take_input && s->count > 0) {
        sc.update_reads = pad_seen().reads;
        const std::uint32_t extra = ext::extra_buttons();
        const std::uint32_t pressed = extra & ~sc.extra_held;
        sc.extra_held = extra;
        if ((pressed & kToggleButtons) != 0u) {
            sc.bgm_focus = !sc.bgm_focus;
            sc.repeat_direction = 0;
            cursor_sound(g);
            std::cout << "[playlist] focus: " << (sc.bgm_focus ? "BGM (left/right series, up/down track)" : "menu")
                      << std::endl;
        }
        const gaddr pad = g_call1(g, F_PAD_LOGICAL, 0);
        const std::int32_t before = s->index;
        const std::size_t series_index = sc.series_of[static_cast<std::size_t>(s->index)];
        if (!sc.bgm_focus) {
            // The game's own: one list, wrapping both ways.
            std::int32_t step = 0;
            if (pad_repeats(g, pad, G_MASK_DPAD_RIGHT)) {
                press_arrow(g, s->arrow_right, 4, 3);
                step = 1;
            } else if (pad_repeats(g, pad, G_MASK_DPAD_LEFT)) {
                press_arrow(g, s->arrow_left, 2, 1);
                step = -1;
            }
            s->index = g_rem(s->index + s->count + step, s->count);
        } else {
            const auto series_count = static_cast<std::int32_t>(sc.series.size());
            std::int32_t series_step = 0;
            if (pad_repeats(g, pad, G_MASK_DPAD_RIGHT)) {
                press_arrow(g, s->arrow_right, 4, 3);
                series_step = 1;
            } else if (pad_repeats(g, pad, G_MASK_DPAD_LEFT)) {
                press_arrow(g, s->arrow_left, 2, 1);
                series_step = -1;
            }
            // Left/right always change the series, and up/down never step
            // while left or right is down.
            if (series_step != 0) {
                const auto next = static_cast<std::size_t>(
                    g_rem(static_cast<std::int32_t>(series_index) + series_count + series_step, series_count));
                const Series &to = sc.series[next];
                s->index = static_cast<std::int32_t>(to.first + std::min(to.last, to.choices.size() - 1u));
                hold_vertical(sc);
            } else if (sideways_held()) {
                hold_vertical(sc);
            } else if (const int step = vertical_step(g, sc); step != 0) {
                const Series &in_series = sc.series[series_index];
                const auto n = static_cast<std::int32_t>(in_series.choices.size());
                const auto at = static_cast<std::int32_t>(static_cast<std::size_t>(s->index) - in_series.first);
                s->index = static_cast<std::int32_t>(in_series.first) + g_rem(at + n + step, n);
                cursor_sound(g);
            }
        }
        Series &now = sc.series[sc.series_of[static_cast<std::size_t>(s->index)]];
        now.last = static_cast<std::size_t>(s->index) - now.first;
        if (s->index != before) {
            show_choice(g, sc, selector);
            std::cout << "[playlist] " << describe(g, selector) << std::endl;
            const BgmEntry *e = G_STRUCT(g, BgmEntry, s->entry[s->index]);
            if (const ModRow *mod = mod_row_of(e->id)) mods::prefetch(*mod->track);
        }
    } else if (!take_input) {
        // Confirmed: the screen stops taking input, and the D-pad is the
        // game's again.
        if (sc.bgm_focus) std::cout << "[playlist] focus: menu (the screen stopped taking input)" << std::endl;
        sc.bgm_focus = false;
        sc.extra_held = ext::extra_buttons();
    }

    // Placing, as the game does: the name at anchor 0, the arrows at 1 and 2.
    g_call4(g, F_LAYOUT_ANCHOR, s->layout, 0, position, colour);
    if (s->text != 0) {
        g_call2(g, F_SPRITE_SET_POS, s->text, position);
        // The anchor's colour: the text's own (a dark blue) and the screen's
        // fade. In BGM focus the name turns a dark rust red (the GE doubles
        // it), its fade kept.
        g_call2(g, F_SPRITE_SET_SIZE, s->text, colour);
        if (sc.bgm_focus && take_input) {
            g_st8(g, s->text + 0x0Cu, 0x48);
            g_st8(g, s->text + 0x0Du, 0x18);
            g_st8(g, s->text + 0x0Eu, 0x00);
        }
        g_call1(g, F_SPRITE_UPDATE, s->text);
        g_call1(g, F_SPRITE_DRAW, s->text);
    }
    note_overlay(g, selector, position, colour, take_input);
    if (tracing() && !sc.anchors_logged) {
        sc.anchors_logged = true;
        float m[16];
        std::memcpy(m, g_ptr(g, position, sizeof m), sizeof m);
        std::cout << "[playlist] anchor 0: scale " << m[0] << "," << m[5] << " at " << m[12] << "," << m[13]
                  << " colour " << hex(g_ld32(g, colour)) << std::endl;
    }
    g_call4(g, F_LAYOUT_ANCHOR, s->layout, 1, position, colour);
    g_call2(g, F_LAYOUT_SET_POS, s->arrow_left, position);
    g_call2(g, F_LAYOUT_SET_SIZE, s->arrow_left, colour);
    g_call4(g, F_LAYOUT_ANCHOR, s->layout, 2, position, colour);
    g_call2(g, F_LAYOUT_SET_POS, s->arrow_right, position);
    g_call2(g, F_LAYOUT_SET_SIZE, s->arrow_right, colour);
    g_pop(g, 0x60u);
}

namespace {

// In BGM focus up/down pick a track; the page beside the selector must not
// scroll with them, so the game's pad read loses them. Only while the
// selector is taking input, seen within the last few reads.
bool swallowing() {
    const Screen &sc = screen();
    return sc.bgm_focus && pad_seen().reads - sc.update_reads <= 4u;
}

void wrap_pad_read(portablekit::HleRegistrar &hle) {
    const portablekit::HleFunction previous = hle.bound_function("sceCtrl", kSceCtrlReadBufferPositive);
    if (!previous) {
        std::cerr << "[playlist] sceCtrlReadBufferPositive is not bound; up/down also scroll the page in BGM focus\n";
        return;
    }
    hle.bind("sceCtrl", kSceCtrlReadBufferPositive,
             [previous](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
                 const std::uint32_t buffer = ctx.gpr[4];
                 const std::uint32_t count = std::clamp<std::uint32_t>(ctx.gpr[5], 1u, 64u);
                 // The buffer is written before the read waits, if it does.
                 previous(rt, ctx);
                 PadSeen &seen = pad_seen();
                 ++seen.reads;
                 if (buffer == 0u) return;
                 seen.raw = rt.memory().load32(buffer + 4u);
                 if (!swallowing()) return;
                 for (std::uint32_t i = 0; i < count; ++i) {
                     const std::uint32_t at = buffer + i * 16u + 4u;
                     rt.memory().store32(at, rt.memory().load32(at) & ~(RAW_DPAD_UP | RAW_DPAD_DOWN));
                 }
             });
}

#if defined(PORTABLEKIT_HAS_RENDERER)
// Drawn over the game with the box's fade (its anchor's alpha), so it fades
// with the screen. With the track list (the default) it stays while the
// screen shows, input or not; the flat selector's hints only while the
// screen takes input.

bool overlay_visible() {
    Overlay &o = overlay();
    if (o.updates != o.updates_seen) {
        o.updates_seen = o.updates;
        o.idle_frames = 0;
    } else if (o.idle_frames < 1000) {
        ++o.idle_frames;
    }
    return o.idle_frames <= 4 && (o.input || o.wheel) && o.alpha > 0.02f;
}

ImU32 rgba(int r, int gr, int b, float a) {
    return IM_COL32(r, gr, b, static_cast<int>(std::clamp(a, 0.0f, 1.0f) * 255.0f));
}

// The track list (the wheel): a box under the BATTLE BGM box, where the
// pages were, in the style of the game's own frames (lavender, darker to the
// left, a light line round it). The track chosen sits on a highlight bar in
// the middle row with the series' tracks before and after it above and
// below, dimmer and smaller further out; it rolls when the choice moves by
// one. Lit (a gold outline, a bright bar) in BGM focus, dimmed in menu
// focus. Every position and size is in the PSP's pixels, scaled to where the
// game's picture is, so it follows any window or internal resolution.
//
// Long titles are cut with an ellipsis rather than scrolled sideways: the
// list rolls vertically already, a second motion in the same box would pull
// the eye on a screen that is mostly read at a glance, and a cut title stays
// readable in place. Game titles lose their series prefix (the box above
// shows it), so only long mod titles are ever cut.

constexpr float kRowPitch = 10.0f;
constexpr float kCentreRow = 53.0f;
constexpr float kHintRow = 81.5f;

struct Roll {
    std::uint64_t serial = 0;
    std::size_t series = static_cast<std::size_t>(-1);
    std::size_t position = 0;
    float offset = 0.0f;  // in rows: where the centre track is drawn, relative to the middle row
};

Roll &roll() {
    static Roll value;
    return value;
}

// `text` cut to `width` with an ellipsis (whole UTF-8 characters).
std::string fit(ImFont *font, float size, const std::string &text, float width) {
    if (font->CalcTextSizeA(size, 1.0e9f, 0.0f, text.c_str()).x <= width) return text;
    const std::string ellipsis = "\xE2\x80\xA6";
    std::string cut = text;
    while (!cut.empty()) {
        std::size_t end = cut.size() - 1;
        while (end > 0 && (static_cast<unsigned char>(cut[end]) & 0xC0u) == 0x80u) --end;
        cut.erase(end);
        while (!cut.empty() && cut.back() == ' ') cut.pop_back();
        if (font->CalcTextSizeA(size, 1.0e9f, 0.0f, (cut + ellipsis).c_str()).x <= width) break;
    }
    return cut + ellipsis;
}

void wheel_draw() {
    const Overlay &o = overlay();
    const portablekit::ui::GamePicture picture = portablekit::ui::game_picture();
    const float sx = picture.width / 480.0f;
    const float sy = picture.height / 272.0f;
    const auto at = [&](float x, float y) { return ImVec2(picture.left + x * sx, picture.top + y * sy); };
    ImDrawList *draw = ImGui::GetForegroundDrawList();
    ImFont *font = portablekit::ui::game_text_font();
    const bool lit = o.bgm_focus && o.input;
    const float a = o.alpha;
    const loading_layout::Box box = loading_layout::kWheelBox;
    const float line = std::max(1.0f, 0.5f * sy);

    // The roll: a step of one within the series slides the rows from where
    // the new track was drawn; any other change shows it at once.
    Roll &r = roll();
    const std::size_t n = o.titles.size();
    const std::size_t position = o.position > 0 ? o.position - 1 : 0;
    if (r.serial != o.screen_serial || r.series != o.series_index || n < 5) {
        r.offset = 0.0f;
    } else if (position != r.position) {
        const std::size_t ahead = (position + n - r.position) % n;
        if (ahead == 1) r.offset += 1.0f;
        else if (ahead == n - 1) r.offset -= 1.0f;
        else r.offset = 0.0f;
        r.offset = std::clamp(r.offset, -2.0f, 2.0f);
    }
    r.serial = o.screen_serial;
    r.series = o.series_index;
    r.position = position;
    // D012_TEST_OVERLAY_FIXED_STEP=1 advances the roll by 1/60 s per drawn frame instead of by the clock, so frame
    // captures (which pause the game while they save) show every step of it.
    static const bool fixed_step = std::getenv("D012_TEST_OVERLAY_FIXED_STEP") != nullptr;
    const float dt = fixed_step ? 1.0f / 60.0f : ImGui::GetIO().DeltaTime;
    r.offset *= std::exp(-dt * 16.0f);
    if (std::fabs(r.offset) < 0.01f) r.offset = 0.0f;

    // The frame: lavender, darker to the left, as the game's gradient quads.
    const ImU32 fill_left = rgba(117, 115, 180, 0.42f * a);
    const ImU32 fill_right = rgba(117, 115, 180, 0.13f * a);
    draw->AddRectFilledMultiColor(at(box.left, box.top), at(box.right, box.bottom), fill_left, fill_right, fill_right,
                                  fill_left);
    draw->AddRect(at(box.left - 0.5f, box.top - 0.5f), at(box.right + 0.5f, box.bottom + 0.5f),
                  rgba(80, 80, 140, 0.35f * a), 3.0f * sy, 0, line);
    draw->AddRect(at(box.left, box.top), at(box.right, box.bottom), rgba(255, 255, 255, 0.80f * a), 3.0f * sy, 0,
                  line);
    if (lit) {
        for (int i = 3; i >= 1; --i)
            draw->AddRect(at(box.left - static_cast<float>(i), box.top - static_cast<float>(i)),
                          at(box.right + static_cast<float>(i), box.bottom + static_cast<float>(i)),
                          rgba(255, 196, 64, 0.16f * a), 4.0f * sy, 0, line);
        draw->AddRect(at(box.left - 1.0f, box.top - 1.0f), at(box.right + 1.0f, box.bottom + 1.0f),
                      rgba(255, 196, 64, a), 4.0f * sy, 0, 1.4f * sy);
    }

    // The highlight bar of the middle row: white to pale blue, as the BGM box.
    const float bar_top = kCentreRow - 5.5f;
    const float bar_bottom = kCentreRow + 5.5f;
    const float bar_a = (lit ? 0.94f : 0.55f) * a;
    draw->AddRectFilledMultiColor(at(box.left + 4.0f, bar_top), at(box.right - 4.0f, bar_bottom),
                                  rgba(252, 253, 255, bar_a), rgba(252, 253, 255, bar_a), rgba(190, 205, 245, bar_a),
                                  rgba(190, 205, 245, bar_a));
    draw->AddRect(at(box.left + 4.0f, bar_top), at(box.right - 4.0f, bar_bottom),
                  rgba(120, 140, 210, (lit ? 0.80f : 0.45f) * a), 0.0f, 0, line);

    // The rows: the series' tracks round the one chosen, each once.
    if (n > 0) {
        int first = -2, last = 2;
        if (n < 5) {
            first = -static_cast<int>((n - 1) / 2);
            last = static_cast<int>(n / 2);
        } else if (r.offset != 0.0f) {
            first = -3;
            last = 3;
        }
        const float text_left = box.left + 11.0f;
        const float text_width = box.right - 8.0f - text_left;
        draw->PushClipRect(at(box.left + 1.0f, box.top + 1.0f), at(box.right - 1.0f, box.bottom - 1.0f), true);
        for (int k = first; k <= last; ++k) {
            const float place = static_cast<float>(k) + r.offset;
            const float distance = std::fabs(place);
            if (distance > 2.6f) continue;
            const std::size_t index = (position + n + static_cast<std::size_t>(k + static_cast<int>(n) * 3)) % n;
            const float size = (8.5f - std::min(distance, 1.0f)) * sy;
            float alpha = distance <= 1.0f ? 1.0f - 0.15f * distance
                                           : (distance <= 2.0f ? 0.85f - 0.35f * (distance - 1.0f)
                                                               : 0.5f * (2.6f - distance) / 0.6f);
            if (!lit) alpha *= 0.7f;
            const bool centre = distance < 0.5f;
            const ImU32 colour = centre ? (lit ? rgba(10, 30, 210, a) : rgba(40, 50, 140, 0.85f * a))
                                        : rgba(25, 25, 55, alpha * a);
            const std::string title = fit(font, size, o.titles[index], text_width * sx);
            const float y = kCentreRow + place * kRowPitch;
            const ImVec2 extent = font->CalcTextSizeA(size, 1.0e9f, 0.0f, title.c_str());
            draw->AddText(font, size, ImVec2(at(text_left, y).x, at(text_left, y).y - extent.y * 0.5f), colour,
                          title.c_str());
        }
        draw->PopClipRect();
    }

    // The keys, small, at the bottom right.
    const std::string hint = lit ? "\xE2\x97\x80\xE2\x96\xB6 Series  \xE2\x96\xB2\xE2\x96\xBC Track  L2/R2 Done"
                                 : "L2/R2  BGM";
    const float hint_size = 6.0f * sy;
    const ImVec2 extent = font->CalcTextSizeA(hint_size, 1.0e9f, 0.0f, hint.c_str());
    draw->AddText(font, hint_size, ImVec2(at(box.right - 4.0f, kHintRow).x - extent.x, at(0.0f, kHintRow).y - extent.y * 0.5f),
                  rgba(40, 50, 120, (lit ? 0.85f : 0.6f) * a), hint.c_str());
}

// The flat selector (D012_BGM_PLAYLIST_FLAT=1), beside the game's BGM box: a
// faint L2/R2 hint in menu focus; in BGM focus a gold outline round the box
// and the series on a line of its own under it. Positions in the PSP's
// pixels from the box's own anchors.

void overlay_draw() {
    const Overlay &o = overlay();
    if (o.wheel) {
        wheel_draw();
        return;
    }
    const portablekit::ui::GamePicture picture = portablekit::ui::game_picture();
    const float sx = picture.width / 480.0f;
    const float sy = picture.height / 272.0f;
    const auto at = [&](float x, float y) { return ImVec2(picture.left + x * sx, picture.top + y * sy); };
    ImDrawList *draw = ImGui::GetForegroundDrawList();
    ImFont *font = portablekit::ui::game_text_font();
    const float a = o.alpha;
    // The box, around the name's anchor (its centre): the light part of the
    // box is about 216 by 18 of the PSP's pixels (captures of the screen).
    const float centre = o.x;
    const float half = 104.0f;
    const float left = centre - half;
    const float right = centre + half;
    const float top = o.y - 7.0f;
    const float bottom = o.y + 8.5f;
    const auto text = [&](float size, float x, float y, ImU32 colour, const std::string &line, float align) {
        const ImVec2 extent = font->CalcTextSizeA(size * sy, 1.0e9f, 0.0f, line.c_str());
        draw->AddText(font, size * sy, ImVec2(at(x, y).x - extent.x * align, at(x, y).y - extent.y * 0.5f), colour,
                      line.c_str());
    };
    if (!o.bgm_focus) {
        const std::string hint = "L2/R2  BGM by series";
        const float size = 6.5f;
        const float width = font->CalcTextSizeA(size * sy, 1.0e9f, 0.0f, hint.c_str()).x / sx;
        draw->AddRectFilled(at(right - width - 6.0f, bottom + 2.0f), at(right, bottom + 10.0f),
                            rgba(20, 30, 80, 0.30f * a), 2.0f * sy);
        text(size, right - 3.0f, bottom + 6.0f, rgba(255, 255, 255, 0.92f * a), hint, 1.0f);
        return;
    }
    // A glow, then the outline, round the box.
    for (int i = 3; i >= 1; --i)
        draw->AddRect(at(left, top), at(right, bottom), rgba(255, 196, 64, 0.12f * a), 4.0f * sy, 0,
                      (1.2f + static_cast<float>(i) * 1.1f) * sy);
    draw->AddRect(at(left, top), at(right, bottom), rgba(255, 196, 64, a), 4.0f * sy, 0, 1.4f * sy);
    // The series, on a band of its own under the box, with the track's place
    // in it; the keys under that.
    const float line_top = bottom + 2.5f;
    const float line_bottom = line_top + 11.0f;
    const float line_half = 82.0f;
    draw->AddRectFilled(at(centre - line_half, line_top), at(centre + line_half, line_bottom),
                        rgba(20, 30, 80, 0.78f * a), 2.0f * sy);
    draw->AddRect(at(centre - line_half, line_top), at(centre + line_half, line_bottom), rgba(255, 196, 64, 0.8f * a),
                  2.0f * sy, 0, 0.6f * sy);
    const float mid = (line_top + line_bottom) * 0.5f;
    text(8.0f, centre, mid, rgba(255, 255, 255, a), "\xE2\x97\x80 " + o.series + " \xE2\x96\xB6", 0.5f);
    text(6.0f, centre + line_half - 4.0f, mid, rgba(255, 220, 140, a),
         std::to_string(o.position) + "/" + std::to_string(o.count), 1.0f);
    text(6.0f, right - 3.0f, line_bottom + 5.0f, rgba(30, 40, 110, a), "\xE2\x96\xB2\xE2\x96\xBC Track   L2/R2 Done",
         1.0f);
}
#endif

} // namespace

void install(portablekit::HleRegistrar &hle) {
    static bool installed = false;
    if (installed || !enabled()) return;
    installed = true;
    wrap_pad_read(hle);
#if defined(PORTABLEKIT_HAS_RENDERER)
    portablekit::ui::add_game_overlay(&overlay_visible, &overlay_draw);
#endif
}

} // namespace d012::playlist
