#pragma once

// The BGM playlist: the loading screen's one-line battle music choice made
// two-level (docs/port/bgm-playlist-design.md, "What was built").
//
// L2 or R2 moves the D-pad between the screen's own menu and the music:
//
// - Menu focus, as the screen opens: the game's own controls. Up/down scroll
//   the page, L/R switch pages, left/right step through every track in one
//   list, now ordered series by series.
// - BGM focus: left/right step through the series (the source game: FINAL
//   FANTASY I ... XIII, DISSIDIA, then the series mods add), up/down through
//   that series' tracks. Up/down no longer reach the page.
//
// The BATTLE BGM box shows the series and the track's place in it ("FINAL
// FANTASY X . 3/8", through the game's own name sprite), and a track list
// under it shows the series' tracks as a wheel round the one chosen (an
// overlay, lit in BGM focus, dimmed in menu focus). The screen's right-half
// pages (Equipment, Accessories, Battlegen) move down and get shorter to make
// room (d012_loading_layout.hpp). D012_BGM_PLAYLIST_FLAT=1 keeps the one-line
// selector of before, with the pages where the game has them.
//
// Mod tracks are rows of the game's own BGM table (its DLC mechanism): an id
// in 0x4000..0x7FFE, the file MD<number>.AT3 the mods module serves, weight 1
// so the selector offers them and no stage bits so the random default never
// picks one. Everything else (the field, the replay, playback) is the game's.
//
// D012_NO_BGM_PLAYLIST=1 leaves the game's own selector and layout and adds no
// rows.
// MIT licence; see port/README.md.

#include "d012_decomp_harness.hpp"

namespace portablekit {
class HleRegistrar;
}

namespace d012::playlist {

// False with D012_NO_BGM_PLAYLIST=1.
[[nodiscard]] bool enabled();

// The replacements of battle 0x08ADE4E0 (the choices the selector offers)
// and battle 0x08AF8CA4 (one frame of the selector), in the harness's form.
void run_list(Guest *g, const decomp::CallState &in, GuestRegs &out);
void run_update(Guest *g, const decomp::CallState &in, GuestRegs &out);

// The pad read's wrapper (up/down kept from the page in BGM focus) and the
// overlay that shows the focus. From the profile's HLE hook.
void install(portablekit::HleRegistrar &hle);

} // namespace d012::playlist
