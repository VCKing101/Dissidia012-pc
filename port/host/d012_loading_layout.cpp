// The loading screen's layout: see d012_loading_layout.hpp.
// MIT licence; see port/README.md.

#include "d012_loading_layout.hpp"

#include "../decomp/bgm_types.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace d012::loading_layout {
namespace {

float ldf(Guest *g, gaddr a) {
    const std::uint32_t bits = g_ld32(g, a);
    float f;
    std::memcpy(&f, &bits, sizeof f);
    return f;
}

void dump_animation(Guest *g, gaddr data, int part_base, int anim, int depth) {
    const std::int32_t anims = g_ld32s(g, data + 0x2Cu);
    if (anim < 0 || anim >= anims || depth > 3) return;
    const gaddr a = g_ld32(g, data + 0x0Cu) + static_cast<std::uint32_t>(anim) * 0x1Cu;
    const int nodes = g_ld8(g, a);
    char line[512];
    std::snprintf(line, sizeof line, "%*sanim %d: %d nodes, s16 %d %d %d", depth * 2, "", anim, nodes,
                  g_ld16s(g, a + 2), g_ld16s(g, a + 4), g_ld16s(g, a + 6));
    std::cout << "[layout] " << line << std::endl;
    const gaddr node_array = g_ld32(g, a + 0x0Cu);
    for (int n = 0; n < nodes; ++n) {
        const gaddr node = node_array + static_cast<std::uint32_t>(n) * 8u;
        const int part = g_ld8(g, node);
        const int keys = g_ld8(g, node + 1);
        const gaddr table = g_ld32(g, node + 4);
        std::string text;
        for (int k = 0; k < keys && k < 3; ++k) {
            const gaddr key = table + static_cast<std::uint32_t>(g_ld16s(g, table + static_cast<std::uint32_t>(k) * 2u));
            char kt[256];
            std::snprintf(kt, sizeof kt,
                          " [k%d %d..%d t%d f%04X pos %d,%d sc %.2f,%.2f | %d,%d->%d,%d sc %.2f,%.2f->%.2f,%.2f]", k,
                          g_ld16s(g, key), g_ld16s(g, key + 2), g_ld8(g, key + 5), g_ld16s(g, key + 6) & 0xFFFF,
                          g_ld16s(g, key + 0xC), g_ld16s(g, key + 0xE), ldf(g, key + 0x10), ldf(g, key + 0x14),
                          g_ld16s(g, key + 0x18), g_ld16s(g, key + 0x1A), g_ld16s(g, key + 0x20),
                          g_ld16s(g, key + 0x22), ldf(g, key + 0x28), ldf(g, key + 0x2C), ldf(g, key + 0x30),
                          ldf(g, key + 0x34));
            text += kt;
        }
        std::string part_text;
        if (part != 0xFF && part < part_base) {
            const std::int32_t parts = g_ld32s(g, data + 0x24u);
            if (part < parts) {
                const gaddr p = g_ld32(g, data + 4u) + static_cast<std::uint32_t>(part) * 8u;
                const std::int32_t count = g_ld32s(g, p);
                const gaddr entries = g_ld32(g, p + 4u);
                for (std::int32_t e = 0; e < count && e < 4; ++e) {
                    const gaddr en = entries + static_cast<std::uint32_t>(e) * 10u;
                    char pt[128];
                    std::snprintf(pt, sizeof pt, " {b%d pic%d %d,%d,%d,%d}", g_ld8(g, en), g_ld8(g, en + 1),
                                  g_ld16s(g, en + 2), g_ld16s(g, en + 4), g_ld16s(g, en + 6), g_ld16s(g, en + 8));
                    part_text += pt;
                }
                if (count > 4) part_text += " ...(" + std::to_string(count) + ")";
            }
        }
        std::snprintf(line, sizeof line, "%*s node %d (anchor %d): part %d%s keys %d", depth * 2, "", n,
                      nodes - 1 - n, part, part != 0xFF && part >= part_base ? " (sub-animation)" : "", keys);
        std::cout << "[layout] " << line << part_text << text << std::endl;
        if (part != 0xFF && part >= part_base) dump_animation(g, data, part_base, part - part_base, depth + 1);
    }
}

void dump_element(Guest *g, const char *label, gaddr e, bool whole) {
    if (e == 0) {
        std::cout << "[layout] " << label << ": none" << std::endl;
        return;
    }
    const gaddr holder = g_ld32(g, e + 0x24u);
    char line[256];
    std::snprintf(line, sizeof line,
                  "%s @%08X: anim %d next %d frame %.1f at %.1f,%.1f scale %.2f,%.2f flags %04X colour %08X holder "
                  "%08X",
                  label, e, g_ld16s(g, e + 0x0C), g_ld16s(g, e + 0x0E), ldf(g, e), ldf(g, e + 0x10), ldf(g, e + 0x14),
                  ldf(g, e + 0x18), ldf(g, e + 0x1C), g_ld16s(g, e + 0x20) & 0xFFFF, g_ld32(g, e + 0x28u), holder);
    std::cout << "[layout] " << line << std::endl;
    if (holder == 0) return;
    const gaddr data = g_ld32(g, holder + 0x0Cu);
    const int part_base = g_ld16s(g, holder + 0x18);
    if (whole) {
        std::snprintf(line, sizeof line, "  data %08X: pictures %d parts %d anims %d part base %d", data,
                      g_ld32s(g, data + 0x20u), g_ld32s(g, data + 0x24u), g_ld32s(g, data + 0x2Cu), part_base);
        std::cout << "[layout] " << line << std::endl;
        for (int a = 0; a < g_ld32s(g, data + 0x2Cu); ++a) dump_animation(g, data, part_base, a, 1);
        for (int part = 0; part < g_ld32s(g, data + 0x24u); ++part) {
            const gaddr p = g_ld32(g, data + 4u) + static_cast<std::uint32_t>(part) * 8u;
            const std::int32_t count = g_ld32s(g, p);
            const gaddr entries = g_ld32(g, p + 4u);
            std::string text;
            for (std::int32_t en = 0; en < count; ++en) {
                const gaddr at = entries + static_cast<std::uint32_t>(en) * 10u;
                char pt[96];
                std::snprintf(pt, sizeof pt, " {b%d pic%d %d,%d,%d,%d}", g_ld8(g, at), g_ld8(g, at + 1),
                              g_ld16s(g, at + 2), g_ld16s(g, at + 4), g_ld16s(g, at + 6), g_ld16s(g, at + 8));
                text += pt;
            }
            std::cout << "[layout]   part " << part << " @" << std::hex << entries << std::dec << ":" << text
                      << std::endl;
        }
        for (int pic = 0; pic < g_ld32s(g, data + 0x20u); ++pic) {
            const gaddr t = g_ld32(g, data) + static_cast<std::uint32_t>(pic) * 4u;
            const int type = g_ld16s(g, t);
            const gaddr at = g_ld32(g, data) + static_cast<std::uint32_t>(g_ld16s(g, t + 2));
            std::string text;
            for (int w = 0; w < 10; ++w) {
                char wt[16];
                std::snprintf(wt, sizeof wt, " %08X", g_ld32(g, at + static_cast<std::uint32_t>(w) * 4u));
                text += wt;
            }
            std::cout << "[layout]   picture " << pic << " type " << type << ":" << text << " floats "
                      << ldf(g, at + 0x18u) << "," << ldf(g, at + 0x1Cu) << std::endl;
        }
    }
    // The anchors of the current animation, as the game reads them.
    const int anim = g_ld16s(g, e + 0x0C);
    if (anim < 0 || anim >= g_ld32s(g, data + 0x2Cu)) return;
    const int nodes = g_ld8(g, g_ld32(g, data + 0x0Cu) + static_cast<std::uint32_t>(anim) * 0x1Cu);
    const gaddr frame = g_push(g, 0x50u);
    std::string text;
    for (int n = 0; n < nodes; ++n) {
        g_call4(g, F_LAYOUT_ANCHOR, e, static_cast<std::uint32_t>(n), frame, frame + 0x40u);
        char at[96];
        std::snprintf(at, sizeof at, " a%d=(%.0f,%.0f s%.2f,%.2f al%d)", n, ldf(g, frame + 0x30u), ldf(g, frame + 0x34u),
                      ldf(g, frame), ldf(g, frame + 0x14u), g_ld8(g, frame + 0x43u));
        text += at;
    }
    g_pop(g, 0x50u);
    std::cout << "[layout]   anchors:" << text << std::endl;
}

// ---- Moving the right half's pages ---------------------------------------------
//
// The screen's layout (one file, shared by its nine elements and the three
// pages' own elements) is a set of animations: each node of an animation is a
// part (pictures, each a quad with a rectangle relative to the node) or an
// anchor (part 255: a point the game places its own sprites at), moved by
// keyframes. The pages draw their heading, the heading's line, the frame and
// the rows' anchors from their animations: Equipment from 12 to 16 (16 at
// rest), Accessories 2 to 6, Battlegen 7 to 11. Moving those nodes' keyframes
// moves the heading, the frame, the rows the game places at the anchors and
// the hand cursor it places at a row's anchor; the frame is made of gradient
// quads, so it is made shorter by moving its lower quads up.

// Where things go, in the PSP's pixels. The BATTLE BGM box keeps its place;
// the track list takes the band under it; the headings start at kHeadingTop.
constexpr int kHeadingTop = 90;
constexpr int kAccessoriesShift = kHeadingTop - 23;  // its heading's top was at 23, its frame 41..248
constexpr int kEquipmentShift = kHeadingTop - 48;    // heading at 48, frame 64..242
constexpr std::int32_t kVisibleRows = 6;             // Accessories and Battlegen showed 10
// Equipment does not scroll: it has seven anchors for at most six rows, 25
// pixels apart from 80 (the extras a little further). They are drawn closer,
// from 122, so that six fit the shorter frame.
int equipment_row(int y) { return 122 + static_cast<int>(std::lround((y - 80) * 0.82)); }

constexpr int kPartFrameTall = 7;   // Accessories' and Battlegen's frame
constexpr int kPartFrameShort = 8;  // Equipment's
constexpr int kAnchor = 255;

// One s16 of the layout and the value it is given, with the value it must
// have before: nothing is written unless every value is as expected (or
// already as wanted, when the layout was moved before and kept).
struct Write {
    gaddr at;
    std::int16_t from;
    std::int16_t to;
};

struct Plan {
    std::vector<Write> writes;
    std::vector<gaddr> caches;  // interpolated keyframes: their cached place is dropped
    std::string error;
};

gaddr animation_of(Guest *g, gaddr data, int anim) {
    return g_ld32(g, data + 0x0Cu) + static_cast<std::uint32_t>(anim) * 0x1Cu;
}

// Every keyframe of a node: `y` maps its vertical position.
void plan_node(Guest *g, Plan &plan, gaddr node, const std::function<int(int)> &y) {
    const int keys = g_ld8(g, node + 1);
    const gaddr table = g_ld32(g, node + 4);
    for (int k = 0; k < keys; ++k) {
        const gaddr key = table + static_cast<std::uint32_t>(g_ld16s(g, table + static_cast<std::uint32_t>(k) * 2u));
        const bool fixed = (g_ld16s(g, key + 6) & 0x40) != 0;
        // A fixed keyframe holds its place at +0x0C; one that moves goes
        // from +0x18 to +0x20, and keeps the place it last computed at
        // +0x0C with the time it was for at +0x08.
        const std::uint32_t offsets[2] = {fixed ? 0x0Eu : 0x1Au, 0x22u};
        for (int i = 0; i < (fixed ? 1 : 2); ++i) {
            const std::int16_t from = g_ld16s(g, key + offsets[i]);
            const auto to = static_cast<std::int16_t>(y(from));
            if (to != from) plan.writes.push_back({key + offsets[i], from, to});
        }
        if (!fixed) plan.caches.push_back(key + 0x08u);
    }
}

// The quads of a part: the edges at or below `cut` go up by `by`.
void plan_part(Guest *g, Plan &plan, gaddr data, int part, int cut, int by) {
    const gaddr p = g_ld32(g, data + 4u) + static_cast<std::uint32_t>(part) * 8u;
    const std::int32_t count = g_ld32s(g, p);
    const gaddr quads = g_ld32(g, p + 4u);
    for (std::int32_t q = 0; q < count; ++q) {
        const gaddr at = quads + static_cast<std::uint32_t>(q) * 10u;
        for (const std::uint32_t edge : {4u, 8u}) {
            const std::int16_t y = g_ld16s(g, at + edge);
            if (y >= cut) plan.writes.push_back({at + edge, y, static_cast<std::int16_t>(y - by)});
        }
    }
}

void each_node(Guest *g, gaddr data, int anim, const std::function<void(gaddr node, int part, int anchor)> &f) {
    const gaddr a = animation_of(g, data, anim);
    const int nodes = g_ld8(g, a);
    const gaddr array = g_ld32(g, a + 0x0Cu);
    for (int n = 0; n < nodes; ++n) {
        const gaddr node = array + static_cast<std::uint32_t>(n) * 8u;
        f(node, g_ld8(g, node), nodes - 1 - n);
    }
}

Plan make_plan(Guest *g, gaddr data) {
    Plan plan;
    if (g_ld32s(g, data + 0x2Cu) != 28 || g_ld32s(g, data + 0x24u) != 17) {
        plan.error = "it has " + std::to_string(g_ld32s(g, data + 0x2Cu)) + " animations and " +
                     std::to_string(g_ld32s(g, data + 0x24u)) + " parts, not 28 and 17";
        return plan;
    }
    const auto down = [](int by) { return [by](int y) { return y + by; }; };
    // Accessories (2..6) and Battlegen (7..11): everything down.
    for (int anim = 2; anim <= 11; ++anim)
        each_node(g, data, anim, [&](gaddr node, int, int) { plan_node(g, plan, node, down(kAccessoriesShift)); });
    // Equipment (12..16): heading, line and frame down; the rows closer.
    for (int anim = 12; anim <= 16; ++anim)
        each_node(g, data, anim, [&](gaddr node, int part, int) {
            if (part == kAnchor) plan_node(g, plan, node, equipment_row);
            else plan_node(g, plan, node, down(kEquipmentShift));
        });
    // The scroll arrows: up (26, 27) above the frames' new tops; down (20,
    // 21) stays under their bottoms, which do not move.
    for (int anim = 26; anim <= 27; ++anim)
        each_node(g, data, anim, [&](gaddr node, int, int) { plan_node(g, plan, node, down(kAccessoriesShift)); });
    // The page arrows (L, R) are placed at anchors 2 and 3 of the screen's
    // own animation (17 coming in, 18 at rest), at y 130 beside the frames:
    // down with them.
    for (int anim = 17; anim <= 18; ++anim)
        each_node(g, data, anim, [&](gaddr node, int part, int anchor) {
            if (part == kAnchor && (anchor == 2 || anchor == 3)) plan_node(g, plan, node, down(kEquipmentShift));
        });
    // The frames: shorter by as much as their tops moved; their bottoms stay.
    plan_part(g, plan, data, kPartFrameTall, 100, kAccessoriesShift);
    plan_part(g, plan, data, kPartFrameShort, 100, kEquipmentShift);
    return plan;
}

bool &pages_moved_flag() {
    static bool value = false;
    return value;
}

} // namespace

bool move_pages(Guest *g, gaddr screen) {
    pages_moved_flag() = false;
    const gaddr element = g_ld32(g, screen);
    const gaddr holder = element != 0 ? g_ld32(g, element + 0x24u) : 0u;
    if (holder == 0) {
        std::cerr << "[playlist] error: the loading screen has no layout yet; its pages stay where they are\n";
        return false;
    }
    const gaddr data = g_ld32(g, holder + 0x0Cu);
    Plan plan = make_plan(g, data);
    std::size_t already = 0;
    for (const Write &w : plan.writes) {
        const std::int16_t now = g_ld16s(g, w.at);
        if (now == w.to) ++already;
        else if (now != w.from && plan.error.empty()) {
            char text[128];
            std::snprintf(text, sizeof text, "the value at 0x%08X is %d, not %d", w.at, now, w.from);
            plan.error = text;
        }
    }
    if (!plan.error.empty()) {
        std::cerr << "[playlist] error: the loading screen's layout is not the one expected (" << plan.error
                  << "); its pages stay where they are\n";
        return false;
    }
    std::size_t done = 0;
    for (const Write &w : plan.writes)
        if (g_ld16s(g, w.at) == w.from) {
            g_st16(g, w.at, static_cast<std::uint16_t>(w.to));
            ++done;
        }
    for (const gaddr cache : plan.caches) g_stf(g, cache, std::numeric_limits<float>::quiet_NaN());
    // The pages that scroll show fewer rows at once.
    for (const gaddr page : {screen + 0xB0u, screen + 0x144u})
        if (g_ld32s(g, page) > kVisibleRows) g_st32(g, page, static_cast<std::uint32_t>(kVisibleRows));
    pages_moved_flag() = true;
    std::cout << "[playlist] the loading screen's pages moved down for the track list: " << done << " of "
              << plan.writes.size() << " layout values changed"
              << (already != 0u ? ", " + std::to_string(already) + " already were" : std::string()) << std::endl;
    return true;
}

void hide_overflow(Guest *g, gaddr screen) {
    if (!pages_moved_flag()) return;
    // Accessories (a sprite a row, at +8) and Battlegen (two, +4 and +8):
    // the game draws ten rows from the top one; the frames now hold six.
    const struct {
        gaddr page;
        bool two;
    } pages[] = {{screen + 0xB0u, false}, {screen + 0x144u, true}};
    for (const auto &p : pages) {
        const std::int32_t count = g_ld32s(g, p.page + 4u);
        const std::int32_t top = g_ld32s(g, p.page + 12u);
        for (std::int32_t i = top + kVisibleRows; i < count && i < top + 10; ++i) {
            const gaddr row = p.page + 0x18u + static_cast<std::uint32_t>(i) * 0x0Cu;
            if (p.two && g_ld32(g, row + 4u) != 0) g_call1(g, F_SPRITE_FINISH, g_ld32(g, row + 4u));
            if (g_ld32(g, row + 8u) != 0) g_call1(g, F_SPRITE_FINISH, g_ld32(g, row + 8u));
        }
    }
}

bool pages_moved() { return pages_moved_flag(); }

void dump(Guest *g, gaddr screen) {
    std::cout << "[layout] loading screen " << std::hex << screen << std::dec << ", pages "
              << g_ld32s(g, screen + 0xF38u) << ", current " << g_ld32s(g, screen + 0xF3Cu) << std::endl;
    for (int i = 0; i < 9; ++i) {
        const std::string label = "element " + std::to_string(i);
        dump_element(g, label.c_str(), g_ld32(g, screen + static_cast<std::uint32_t>(i) * 4u), i == 0);
    }
    const gaddr pages[3] = {screen + 0x50u, screen + 0xB0u, screen + 0x144u};
    for (int p = 0; p < 3; ++p) {
        char line[160];
        std::snprintf(line, sizeof line, "page %d @%08X: visible %d count %d cursor %d top %d", p, pages[p],
                      g_ld32s(g, pages[p]), g_ld32s(g, pages[p] + 4u), g_ld32s(g, pages[p] + 8u),
                      g_ld32s(g, pages[p] + 12u));
        std::cout << "[layout] " << line << std::endl;
        const std::string label = "page " + std::to_string(p) + " element";
        dump_element(g, label.c_str(), g_ld32(g, pages[p] + 0x14u), false);
    }
    const gaddr sel = screen + 0x6ACu;
    dump_element(g, "bgm box", g_ld32(g, sel), true);
}

} // namespace d012::loading_layout
