# Graphics triage: ten glitches from a Story Mode play

> 20 October 2026. The owner played Story Mode on the recompiled build at 60 fps (frame interpolation) and internal
> scale 4 and listed ten differences from the handheld. Each was taken to a scripted scene on the recompiled build
> (`tools/local/port/build-d012-gfx`, the same options as `build-d012-recomp`), A/B'd with switches at the same
> scripted frame, and given a class: (a) interpolation pairing or blending, (b) render-to-texture / framebuffer as
> texture, (c) an unhandled GE state, (d) something else. Data: a copy of the owner's save,
> `build/port/data-gfx-triage` (Chapter 12-1, Lightning on the world map); `build/port/data` was only copied.
> Captures and logs: `build/port/gfx-triage/`. PortableKit: patches 0032 to 0035 on `dissidia`.

## Summary

| # | Item | Class | Evidence | Fix or status | Switch |
| --- | --- | --- | --- | --- | --- |
| 1 | Flicker absorbing a "power of the dead" orb | (a)+(b), as 3 | `orb` scene: the in-between frames' glow and haze around the orb differ from 30 fps without snapshots and match it with them (`item1_orb.png`). The world map's in-between frames differ from the frame they replay in 9% of pixels without snapshots, 0 with | Fixed by patch 0033 (likely; the flicker itself is motion, judged from stills) | `D012_NO_FB_SNAPSHOTS` |
| 2 | Swirl into a battle or gate missing | (c) TCC bit of the texture function | `gate_enter` scene, GE trace of runs 4155-4178: from the gateway prompt on, each frame draws the other framebuffer back over the screen, 17 strips of 62 vertices as they are, then 17 twisted strips blended by source alpha, texture function `0x000000` (TCC clear: colour only, alpha from the vertex). The renderer took the texel's alpha, the 5551 framebuffer's stencil bit, 0: the twisted layer was invisible and only the white fade showed | Fixed, patch 0035: the swirl turns as on the handheld (`fix3_gate_swirl.png`) | `D012_NO_TEXTURE_RGB` |
| 3 | Glow around mountain tips strobing / low-res | (a)+(b) | World map: the frame is downsampled to a 30x17 8888 glow buffer and blurred through 120-pixel strips, all in one scratch buffer `0x040CC000`. In-between frames are replays of the older frame, and every composite read that buffer's last picture (the last strip): hard white rims on the ridge line every other present (`item3_glow_compare.png`). `D012_CHECK_REPLAY`: 187,706 and 191,164 of 2,088,960 pixels differ, 0 with snapshots (`fix1_snapshots_worldmap.png`). The 30x17 glow is the game's own resolution | Fixed, patch 0033 | `D012_NO_FB_SNAPSHOTS` |
| 4 | Leaf trail in forests flickers | (a)+(b) probably | No forest on the scripted routes; the same glow and blur composites cover the whole world map | Fixed (owner confirmed on 21 October 2026, after the render-target snapshots, 0033) | `D012_NO_FB_SNAPSHOTS` |
| 5 | Blobs in front of red gateways jitter sideways | (a)+(b) | `gate_view` scene: replayed frames differ in 8% of pixels without snapshots; the glow and blur round the portal and the dark shapes at its base come from another 120-pixel strip (`item5_gate_replay.png`). With snapshots 0 pixels differ, and at t = 0.5 the portal's bright centre sits between the two frames (995.7, 996.0, 996.1 px) | Fixed by patch 0033 (likely; judged from replays, not motion) | `D012_NO_FB_SNAPSHOTS` |
| 6 | Ground cracks linger in Lightning's EX Burst | (b) | `lightning_ex` scene (EX Burst, Gestalt Drive, frames 9300-9600): nothing lingers at 30 fps or 60 fps in the captures. Checked and ruled out: a palette rewritten after its first entry (the texture key hashes only the first CLUT word): a trace of every key over the whole battle and the world map found none | Fixed (owner confirmed on 21 October 2026): an intended overlay of cracks and holes in the screen, now drawn right; most likely the snapshot or palette/transfer fixes | |
| 7 | Flickering overlay over the top-left quarter in the EX Burst | (a)+(b) | `lightning_ex`: in-between frames show a ghost quarter-size copy of the scene in the top-left quarter (small figures, `item7_exburst_topleft.png`); replay checks at 9300, 9450, 9600: 0 pixels differ with snapshots | Fixed, patch 0033 | `D012_NO_FB_SNAPSHOTS` |
| 8 | Small overlay top-left on Lunar Subterrane | (a)+(b) | `kuja_lunar`: in-between frames show a 60x34 miniature of the scene in the top-left corner: the shadow composite reading the blur buffer (8888, 128 wide) that later reuses `0x0CC000` (`item8_lunar_topleft.png`). Replay checks: 898,687 to 1,802,392 pixels differ (43-86%), 0 with snapshots (`fix1_snapshots_lunar.png`) | Fixed, patch 0033 | `D012_NO_FB_SNAPSHOTS` |
| 9 | Larger overlay top-left on Order's Sanctuary | (a)+(b) | `kuja_order`: the same, larger (about three quarters of the width, half the height); 863,499 to 1,030,467 pixels differ, 0 with snapshots (`fix1_snapshots_order.png`) | Fixed, patch 0033 | `D012_NO_FB_SNAPSHOTS` |
| 10 | Kuja's orbs and attacks miss a layer | (c) blend factors 6/7 | Battle list (`kuja_lunar`, run 8900): additive effects use blend `0x0A6` (source factor 6 = twice the source alpha, destination one) and `0x076` (2As, 1-2As) with alpha 0x80. The renderer read factor 6 as plain alpha: half strength. Same frame at 30 fps, A/B (`fix2_double_alpha_kuja.png`): Kuja's orbs, hit flashes and impact cracks at full strength | Fixed, patch 0034 | `D012_NO_DOUBLE_ALPHA` |

Not interpolation pairing in the narrow sense: no item traced to a wrong pair of draws. The world map's matcher
pairs 99-100% of draws with no pair rejected while standing, walking and turning (`D012_TRACE_INTERPOLATION=1`), and
the replay of a frame at t = 0 is pixel-exact once composites read snapshots.

## The causes

**Render targets read by in-between frames (patch 0033).** An in-between present draws the older frame's recorded
draws into the shown framebuffer again. A draw that samples another render target recorded the target's one sampled
copy, and by the time of the present that copy held the target's last picture. Dissidia reuses one scratch buffer
for several passes a frame: in battle `0x0CC000` is the shadow buffer (5551, 512 wide), then the blur buffer (8888, 128
wide, a quarter-size copy of the scene); on the world map `0x040CC000` is the 30x17 glow buffer and then each of four
120-pixel blur strips. So every in-between frame composited the wrong picture. While frames are recorded, a draw
into a shown framebuffer that samples a target now takes a snapshot of the rectangle it reads (plus a pixel of
margin), kept until no recorded frame can name it (three flips); identical snapshots are shared, images reused, at
most 48. Draws into other targets and every draw at 30 fps are unchanged.

**Twice the source alpha (patch 0034).** GE blend factors 6 and 7 are 2As and 1-2As, clamped. The shader now doubles
the alpha it hands the blender (after the alpha test) and the pipeline halves the alpha written through the blend
constant's alpha, so the alpha bits (the stencil) keep the fragment's alpha. Factors 8 and 9 (twice the destination
alpha) still read the plain alpha; not seen.

**Colour-only textures (patch 0035).** TCC (texture function bit 8) was parsed and never used. With it clear the
texel's alpha is taken as one (modulate, decal, blend, add) and replace keeps the fragment's alpha.

## Test switches and scenes

Profile (`port/host/d012_test_switches.cpp`, off by default): `D012_TEST_UNLOCK_FIGHTERS=1` sets the 31 unlock
flags in the running game's flag field (`*(0x089D726C)`, flags `0x2BC` + save slot), so Kuja can be picked;
`D012_TEST_EX_FULL=1` keeps the player's EX gauge full (`*(*(*(0x089D6A10)+0x38)+0x52C)+0x18`, a float 0 to 10000,
written only when it reads as one). Both act once a vblank wait. Stages and fighters are then picked through the
menus by the route.

Framework, for tests: `D012_CAPTURE_BLENDED=1` (patch 0032) makes a `shot` wait for an interpolated present; with
`D012_FRAME_RATE_AUTO=0` the rate does not step down while captures stall frames. `D012_CHECK_REPLAY=1` (existing)
replays the older frame every 150 flips and counts the pixels that differ.

`tools/port/gfx_triage.py SCENE LABEL [--env ...]` runs a scene on `build-d012-gfx` with the triage data copy:

| Scene | What |
| --- | --- |
| `worldmap`, `worldmap_turn`, `worldmap_walk` | Standing, turning the camera, walking (Chapter 12-1) |
| `gate_view`, `gate_enter` | The red gateway left of the start; walking up to it and entering (the swirl) |
| `orb` | Walking to the glowing orb and taking it with cross (about frame 4400) |
| `kuja_lunar`, `kuja_order` | Quick Battle, Kuja against a CPU Lightning, Lunar Subterrane / Order's Sanctuary (needs `D012_TEST_UNLOCK_FIGHTERS=1`) |
| `lightning_ex` | Lightning against Lightning, Old Chaos Shrine, EX Mode at 8775, EX Burst 9300-9600 (needs `D012_TEST_EX_FULL=1`) |

```powershell
Copy-Item -Recurse build/port/data build/port/data-gfx-triage   # once; never run against build/port/data
.\.venv\Scripts\python.exe tools/port/gfx_triage.py kuja_lunar check --env D012_TEST_UNLOCK_FIGHTERS=1 `
    --env D012_FRAME_RATE_AUTO=0 --env D012_CHECK_REPLAY=1                      # add D012_NO_FB_SNAPSHOTS=1 for before
.\.venv\Scripts\python.exe tools/port/gfx_triage.py lightning_ex blended --env D012_TEST_EX_FULL=1 `
    --env D012_FRAME_RATE_AUTO=0 --env D012_CAPTURE_BLENDED=1
.\.venv\Scripts\python.exe tools/port/gfx_triage.py gate_enter fps30 --env D012_FRAME_RATE=30   # D012_NO_TEXTURE_RGB=1
```

At 30 fps the scenes are deterministic (the same frame shows the same moment run after run); at 60 fps a battle's
timeline sometimes drifts, so before/after pairs there use `D012_CHECK_REPLAY`'s older/again images of one run.

## Verification

- Title, main menu, Battle Mode, Quick Battle setup and character select: pixel-identical with all three fixes on
  and off at 30 fps (`build/port/gfx-triage/regress/`); the VS and Opponent Info screens and the battle differ only
  in their random BGM, assist and fight (`regression_menus_battle.png`). World map, gateway, orb walk and three
  battle stages rendered at 30 and 60 fps through these runs.
- No `No recompiled function registered` in any run (`PSPRECOMP_NO_INTERPRETER=1`).
- PortableKit's 21 framework tests pass and `portablekit_host_check` builds (`tools/local/port/build-framework-gfx`).
- Cost: one image copy per composite into the shown framebuffer per frame while interpolating (about six on the
  world map), of the sampled rectangle only.

## Notes

- Builds: `tools/local/port/build-d012-gfx` and `build-framework-gfx` build from a worktree of the clone,
  `tools/local/port/PortableKit-gfx` (branch `gfx-triage`, fast-forwarded into `dissidia`), so another agent's
  uncommitted work in the clone did not mix in.
- Still ignored in battle and on the world map (`GE commands ignored` at exit): colour test (`0x27`, `0xD8`-`0xDA`;
  in the traced battle frame only on the shadow composite, where it changes nothing), logic op `0x28`, texture map
  mode `0xC0` (UV mode in the traced frames), dither, clip planes, the region, patch state, shade model `0x50`. Nothing seen traced to
  them.

## Lightning's Water: a dark negative instead of a clear sphere

> 21 October 2026. Reported by the owner on the Steam Deck (the Windows build through Proton): Lightning's Water,
> cast in her RAV (Ravager) paradigm, shows as a colour negative instead of a transparent sphere. Not Proton: it
> reproduces on the PC at 1x and 4x, at 30 and 60 fps. Fixed on the PortableKit branch `water-fix` (worktree
> `tools/local/port/PortableKit-water`, commit `402619f` on `dissidia` at `cb3f376`; not merged, no patch
> regenerated). Build `tools/local/port/build-d012-water` (the options of `build-d012-recomp`), framework build
> `tools/local/port/build-framework-water`, data `build/port/data-water-test` (a copy of `build/port/data` with
> `video.present_mode=immediate`). Captures and logs: `build/port/gfx-water*/` (never committed).

**Scene.** New tour route `qb_water` (`tools/port/gfx_tour.py`): Quick Battle on Old Chaos Shrine, R while
holding L (the tutorial's paradigm change) switches Lightning to RAV, she runs away from the CPU (at point-blank
range the sphere bursts at once), jumps, and circle in the air casts Water; three casts, captures every 6 frames.
No test switch was needed: the first cast lands on every timeline seen.

**What the draws do.** `D012_TRACE_GE_LIST` of the frame with the sphere (list run 8874 of an exploration run;
`build/port/gfx-water/list8874.txt`, `state_at.py`):

- Every frame, after the stage and the fighters, the shown framebuffer F (`0x04000000`, 5551) is drawn into
  `0x04196000` (5551, row 256, scissor 256x128): a half-size copy.
- The sphere: a fan of 18 and a strip of 34 vertices into F, textured with `0x041A6000` (5551, 256x128, row 256),
  blend 2As / 1-2As (`0xDF` = `0x76`), material colour `0x808080` alpha `0x50`, modulate with colour doubling and
  TCC clear (`0xC9` = `0x010000`), alpha test off, no colour test (`0x27` = 0), **no logic op** (`0x28` = 0, `0xE6` =
  COPY). Its other layers (clouds, bubbles, a rainbow sheen: 128x128 CLUT textures) are blended on top as usual.
- At the end of the frame F is drawn into `0x041A6000` (5551, row 256, scissor 256x128, culling on), after the
  sphere. So the sphere refracts a half-size copy of the **previous** frame, which `0x041A6000` holds until then.

`0x041A6000` is exactly 256 x 128 x 2 bytes past `0x04196000`: the two copies sit back to back in VRAM.

**PSP behaviour** (public GE documentation: the PSPSDK's `sceGuDrawBuffer`, `sceGuTexImage`, `sceGuTexFunc`,
`sceGuBlendFunc`, `sceGuLogicalOp`): VRAM has no buffers, only addresses. A texture at `0x041A6000` reads the
bytes there, which the last draw into `0x041A6000` wrote; the draw into `0x04196000` stops at row 127 (its
scissor), the byte before. With TCC clear the texel's alpha is ignored (the vertex's is used), so the copy shows
through at 2 x 0x50 = 63% over the frame: a clear, distorted ball of water.

**Cause** (`find_framebuffer_texture()` in PortableKit's `vulkan_renderer.cpp`, code older than this month's fixes).
A texture is matched to the render target whose memory holds its first texel, a target taken as 272 rows of its
row length. `0x04196000`'s 272 rows of 256 run to `0x041B8000` and take in `0x041A6000`; of two candidates the
renderer picked the one drawn last. At the sphere `0x04196000` had been drawn this frame and `0x041A6000` the frame
before, so the sphere read rows 128-255 of `0x04196000`'s image, which nothing draws: dark. That dark disc at 63%,
with the clouds, bubbles and sheen on top, is the "negative".

**Did a recent fix cause it?** It made it visible. A/B on the old choice (`D012_NO_TARGET_DRAWN_AREA=1`), same
scripted frames, 30 fps (`build/port/gfx-tour/evidence/water_ab_switches.png`): with `D012_NO_TEXTURE_RGB=1` (no TCC, patch 0035
off) the dark disc is gone, since the texel's alpha (0 in the undrawn rows) then hides the layer, and with it the
refraction; with every other recent switch off together (`NO_FB_CLUT`, `NO_DOUBLE_ALPHA`, `NO_FB_SNAPSHOTS`,
`NO_TRANSFER_TARGETS`, `NO_ENV_MAP`, `NO_LINES`, `CLUT_FIRST_WORD_KEY`) it stays (half as dark without the doubled
alpha). So the wrong target choice is old, and patch 0035 (colour-only textures, a correct fix) exposed it on 20
October. The logic op was ruled out by the trace (never enabled here, as in the census).

**Fix** (PortableKit `find_framebuffer_texture()`, game-agnostic). Among the targets that take in the texture's
first texel, one whose drawn rectangle (the scissors of its draws since it was last first drawn in a frame, as the
palette views use) holds that texel wins over one that takes it in only with rows it never drew; among equals the
newest draw wins, as before. The guest-memory check that follows is unchanged. Off switch
`D012_NO_TARGET_DRAWN_AREA` (the newest target only, as before); `D012_TRACE_FB_TEXTURES` logs each texture where
the two choices differ; the census counts `fbtex.target_by_drawn_area` (info), or
`fbtex.undrawn_rows_of_newer_target` (high) with the switch set.

**Verified.**

- Before (`D012_NO_TARGET_DRAWN_AREA=1`) and after on `qb_water` at 4x and 1x, 30 and 60 fps (60 with
  `D012_CAPTURE_BLENDED`): `build/port/gfx-tour/evidence/water_before_after.png` (capture `water1_5` and
  `water1_7` of each) and `water_before_after_crop.png` (4x 30 fps, same timeline). Before: a dark ball with white
  cloud streaks and a teal centre. After: a clear ball of water bending the floor and wall behind it, with its
  bubbles and sheen. The 4x 30 fps pair is one timeline; the others fell on other timelines but show the same
  difference. Replay checks at 60 fps: 0.00-0.06% (5 checks a run).
- Census: `fbtex.target_by_drawn_area` 362 draws on `qb_water`, all in the Water scene (two a frame while a sphere is
  up), example texture `0x041A6000` into F.
- Whole tour on the fixed build, the new route included (`build/port/gfx-water-tour/`, 36 runs, 522 captures,
  base, nointerp and 1x; `build/port/gfx-water/tour_after.txt`): no run with problems, no interpreter fallback,
  replay checks 0.00-0.23% in colour; its flags are other timelines, as in the census document's sections 8 to 10
  (the menus repeat exactly; `qb_water` has no baseline yet). The census counts `fbtex.target_by_drawn_area` on every
  battle route, 4,148 draws over the base runs: the same pattern, a draw reading a buffer drawn the frame before
  right after one drawn this frame (`0x041A6000` on most stages and in EX Mode, EX Burst and EX Revenge; `0x041AC000`
  Lunar Subterrane, `0x041A2000` Orphan's Cradle, `0x0418C000` Empyreal Paradox, `0x04178000` Order's Sanctuary;
  `0x04122000` 13 draws entering Story Mode).
- The ten other routes again at 30 fps with the old choice (`build/port/gfx-water-tour-before/`, compared by
  `build/port/gfx-water/compare_ab.py`, pairs in `build/port/gfx-water/ab_tour/`): on the routes that kept their
  timeline the only changes are such effects. Razor Gale's dome (a COM attack, `qb_ex_burst` capture
  `ex_burst2`) now bends the previous frame, ghost of the blade and petals included, instead of a flat haze
  (`build/port/gfx-tour/evidence/water_fix_razor_gale.png`); a faint dome on Empyreal Paradox likewise
  (`water_fix_empyreal_dome.png`); `story_gateway` and Order's Sanctuary are pixel-identical; Orphan's Cradle differs only in
  the Opponent Info page's rotating BGM line (menu timing). The other battles fell
  on other timelines (their versus screens already differ), so they were not compared pixel for pixel.
- The 20 framework tests (`tools/port/pk_ctest.cmd`, ad hoc tests excluded) pass and `portablekit_host_check`
  builds (`tools/local/port/build-framework-water`).

**Not verified.** The sphere on a handheld: the look is the port's frames read from the traced state, not compared
with a PSP. Most of the other effects that take the new choice (the census found them on every
stage; only the ones above were compared pixel for pixel). The Proton build on the Deck (not touched; the cause is not platform-specific).

## World of Darkness pillars flicker between frames

> 30 October 2026. Reported by the owner on the Steam Deck (the clang build): the pillars of World of Darkness
> flicker at 60 and 90 fps and not at 30, so it is frame interpolation. Fixed in patch 0065 (PortableKit branch
> `interp-pairing`, worktree `tools/local/port/PortableKit-interp`, fast-forwarded into `dissidia` on 31 October).
> Build `tools/local/port/build-d012-interp-clang` (clang release, `-O1`), framework build
> `tools/local/port/build-framework-interp`. Captures and logs: `build/port/gfx-wod/` (never committed).

**Cause.** The stage is a hall of copies: about 2,600 draws a frame, most of them a few meshes drawn again and
again. The matcher paired the copies of one mesh in drawing order. When a copy at the front of the list leaves the
view, every later copy pairs with its neighbour; a neighbour farther than the own-motion guard (4 units) followed
the camera, but a closer one was kept and drawn halfway towards it, where neither real frame has anything. The
camera estimate (the median by turn angle) suffered too: a mispaired copy turns exactly as the camera does and
slides by the gap between the two, so the median could pick it, which gave 33-unit "camera moves" cut as camera
jumps, 19 in a minute of battle.

**Fix** (`host/gpu/frame_interpolation.cpp`, game-agnostic). The camera is the motion most pairs of meshes drawn
once agree on (everything that stands still moves by the same matrix), falling back to all pairs and then to the
median. A copy whose drawing-order partner is where that camera carries it keeps it; the rest of each mesh pair up
nearest first over all candidates, so a copy that left the view does not take its neighbour's place. Off switch
`D012_INTERPOLATION_ORDER_PAIRS` (drawing order and the median, as before); the trace and the per-second guard line
count the pairs taken out of drawing order. `D012_CHECK_REPLAY` now also draws the halfway frame and counts its
pixels far from both frames (a colour channel more than 40 off each), and `D012_CHECK_REPLAY=N` checks every N
flips.

**Verified** (Windows, clang release build, 60 fps, internal scale 4, scripted Quick Battles; the guest clock
follows the wall clock, but this route repeats its timeline exactly):

- Halfway pixels far from both frames at four checks, before and after: 15.93 / 2.07 / 3.92 / 0.73% became
  11.96 / 1.37 / 3.13 / 0.64%; the distant pillar drawn halfway between two copies no longer is. Camera cuts in a
  minute of World of Darkness: 19 became 1.
- Old Chaos Shrine on the same timeline: 1.401% and 1.399% (no change). Story Mode world map, orb pickup: identical.
- Render time within noise (5.20-5.46 ms before, 5.37-5.49 after).
- The 21 framework tests pass, three of them new (swapped copies, a row that loses its first copy, a hall of
  copies' camera).

**Not verified.** The Steam Deck and the native Linux and macOS builds. The owner's other report from the same
play, the world map's "Power of the dead" light trails flickering at 60 and 90 fps, is not this cause (the fix
changes nothing on the world map, and blended stills there look like the 30 fps frames); it is still open.
