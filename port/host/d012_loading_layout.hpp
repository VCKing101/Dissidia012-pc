#pragma once

// The loading screen's layout as the BGM playlist's track wheel needs it: the
// right half's Equipment panel moved down and made shorter, so the wheel fits
// under the BATTLE BGM box (docs/port/bgm-playlist-design.md, section 11).
// MIT licence; see port/README.md.

#include "../decomp/guest.h"

namespace d012::loading_layout {

// Logs the screen's layout elements, their animations' nodes and anchors
// (D012_TRACE_BGM_LAYOUT=1). `screen` is the loading screen (battle
// 0x08AF5C88's object).
void dump(Guest *g, gaddr screen);

// Moves the three pages of the right half (Equipment, Accessories,
// Battlegen: heading, frame, rows, cursor, scroll and page arrows) down and
// makes their frames shorter, by changing the screen's layout as it is in
// memory once it is loaded (from the selector's build). Checks every value
// first: on a layout other than the one expected it changes nothing, logs it
// and returns false.
bool move_pages(Guest *g, gaddr screen);
// After the pages drew their rows (each frame, from the selector's update):
// hides the rows below the shorter frames.
void hide_overflow(Guest *g, gaddr screen);
// Whether the pages were moved on the screen being shown.
[[nodiscard]] bool pages_moved();

// The track list's box, in the PSP's pixels: the band between the BATTLE
// BGM box and the pages' headings (which start at 90).
struct Box {
    float left, top, right, bottom;
};
constexpr Box kWheelBox{247.0f, 27.0f, 460.0f, 86.0f};

} // namespace d012::loading_layout
