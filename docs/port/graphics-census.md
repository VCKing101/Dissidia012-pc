# Graphics census: what the game asks of the GE that the renderer approximates

> 20 October 2026. A systematic sweep alongside the triage of the owner's reported glitches
> ([graphics-triage.md](graphics-triage.md)): a census of GE features in the renderer, counted per scene, and a
> re-runnable tour of the game's effect-heavy scenes. PortableKit `dissidia` with patches 0036 to 0039 (below);
> recompiled build in `tools/local/port/build-d012-tour` (the options of `build-d012-recomp`, built from the worktree
> `tools/local/port/PortableKit-census`). Data: `build/port/data-gfx-tour`, a copy of `build/port/data`, its `ms0/`
> restored before every run; `build/port/data` was only copied. Captures, logs and census files:
> `build/port/gfx-tour/` (never committed). Numbers below are from the third full tour (`tour_full3.txt`, 30 runs,
> 387 captures, the routes but `qb_ex_revenge`, which was split off `qb_ex_results` afterwards and run on its own,
> `tour_ex2.txt`) unless a section says otherwise.

## 0. Summary

- **Census.** `D012_GE_CENSUS=<file>` (PortableKit `host/gpu/ge_census.*`, game-agnostic, off by default) checks
  every draw against 76 features the renderer does not reproduce exactly, read off the renderer's code, and writes
  one JSON per run: each feature's count, per scene, the first frame and scene, and an example draw (target, texture,
  state words). The input script's new `scene NAME` step names the scenes.
- **Tour.** `tools/port/gfx_tour.py`: 12 scripted routes (title and menus; Story Mode world map, a gateway, the gate
  transition and the gateway board; Quick Battles on five stages incl. Lunar Subterrane and Order's Sanctuary; EX Mode
  and EX Burst; EX Revenge; Kuja; the results screen), each in three variants (owner's settings with a frame-replay check, 30 fps,
  30 fps at 1x), compared with a local baseline.
- **Fixed during the sweep** (all with off switches): blend factors of twice the source alpha (395,000 draws in the
  tour, every 3D scene; triage patch 0034), colour-only textures (TCC, 45,000; patch 0035), render targets read by
  interpolated frames (every battle's replay check went from 48-79% of pixels differing to 0; patch 0033), and here,
  palette textures keyed by their whole palette and index mode (patch 0038).
- **Top open findings**, by likely visual impact:
  1. **Palette views of the framebuffer** (`fbtex.miss_clut_view`, 1,340 draws): EX Revenge's greyscale reads the
     5551 frame as CLUT16 with index shifts 0, 5, 10. The renderer decodes guest memory instead of the render: the
     whole scene turns flat grey for the effect's length (`evidence/ex_revenge_grey.png`). **Fixed 21 October**
     (section 9): decoded on the GPU at the internal scale; the stage now shows as the game's high-contrast
     greyscale negative (`evidence/ex_revenge_fixed.png`).
  2. **Lines are not drawn** (`prim.lines`, 25,063 draws, every battle from the intro on): additive (factor 6/FIX)
     orange line segments, two vertices each.
  3. **Environment mapping** (`texmap.env_map`, 24,696 draws, Orphan's Cradle only): drawn with the vertex UVs.
     **Implemented 21 October** (section 11): U and V from the normal and the lights `0xC1` names; on this stage it
     dims the violet strips on Orphan's wheels a little (`evidence/orphans_cradle_env_map.png`).
  4. **Depth range clip** (`depth.range_minz_maxz`, 7,501 draws, Order's Sanctuary and Empyreal Paradox): additive
     64x64 sprites the GE would cut at MINZ/MAXZ are drawn whole. **Implemented 21 October** (section 12), with the
     GE's near/far clipping and culling: no visible change on the tour, since the game's MINZ/MAXZ equal its
     viewport's depth range.
  5. **EX Burst's captured frame** (`transfer.from_target`, `tex.large_change_missed_by_key`): the frame is copied to
     RAM at 480x272 and drawn as a 512x512 texture whose cache key samples one word per 256 bytes. **Fixed 21
     October** (section 10): the copy is also made on the GPU at the internal scale and drawn from there
     (`evidence/ex_burst_capture_before_after.png`); the stale picture was a false alarm of the census on this route.
  6. **Effects past the far plane** (`clip.depth_clamp_far`, 875 draws): clipped by Vulkan, clamped by the GE.
     **Implemented 21 October** (section 12): the GE clamps, then MINZ cuts them at exactly the far plane in this
     game, so the cut was right; the renderer now does it the GE's way.
- **Scaling and interpolation.** No capture pair at 4x and 1x differed beyond what scale explains (the largest
  matched-timeline mean difference: 7.4 of 255, a walk). After patch 0033 every replay check is pixel-exact in colour;
  Empyreal Paradox's checks still count 50-69% of pixels, but only in the alpha bits (the stencil), which do not
  show.

## 1. The census

`<prefix>_GE_CENSUS=<file>` (documented in PortableKit's `docs/BRINGING_UP_A_GAME.md`). Hooks of one line each:
`GeState::draw_primitive` and `draw_patch` (every draw, with the raw command registers), `kTransferStart`,
`trace_unhandled`, `note_vram_copy` (`ge_state.cpp`); the renderer's texture-source choice, a draw dropped for vertex
space, a texture drawn white, each present (`vulkan_renderer.cpp`); the write at exit (`main.cpp`); the input
script's `scene NAME` (`input_script.cpp`). `<prefix>_GE_CENSUS_SCENE` names the first scene. With the variable unset
every hook returns at its first test. Cost with it on: describing a draw only for a feature's first hit keeps a
battle at about 2.5 times real time unthrottled (the first version, formatting every draw, ran it at real time).

JSON: `frames`, `draws`; `scenes[]` (frames, draws, per-feature counts, ignored commands); `features[]` sorted by
count (category, impact, description, count, first frame and scene, per-scene counts, `variants` such as the factor
pair or the CLUT shift, and `example`: frame, scene, a one-line description of the draw and its state words,
`0x12` vertex type to `0x50` shade mode); `unhandled_commands`.

The features (`kFeatures` in `ge_census.cpp`), each with what the renderer does instead:

| Group | Features |
| --- | --- |
| Primitives, vertices | points and lines not drawn; morph targets (first only); flat shading |
| Fragment | alpha test NEVER/ALWAYS (the shader takes 1 for NEVER: a latent bug, not used by this game), mask, EQUAL tolerance; colour test; logic op; dither into 16-bit targets; antialias |
| Depth, viewport | MINZ/MAXZ narrower than the viewport; depth clamp with vertices past the far or near plane; z scale 0; z range clamped; draws past 480x272 |
| Blend | ABS; doubled alpha factors (6/7 now handled, 8/9 not); two fixed colours that are not complements; destination alpha on 5551/4444/5650 targets |
| Texture | BLEND (env colour ignored), ADD (as REPLACE), TCC (now handled); map modes 1 (projection) and 2 (environment); mipmaps and level modes; per-draw filter ignored; size over 512; buffer width below width; VRAM mirrors; textures over depth buffers; large textures whose sampled key misses a change; CLUT shift/mask/start and palette changes (now keyed) |
| Render to texture | sampled from a target; self-sampling; format reinterpreted; alpha from 16-bit targets; samples past 480x272; REPEAT on a target; misses that decode guest memory: palette view, swizzled, other pixel size, other row length, DXT, memory changed; target format or stride changed; a target inside another; colour over a depth buffer; depth stride mismatch; odd strides |
| Transfers | block transfer out of or into a target, overlapping, pixel size against the target; DMA copies out of or into a target |
| Lists, renderer | bounding box; immediate vertices `0xF0`-`0xF9`; SIGNAL behaviours; vertex space full; texture decode failed |
| Interpolation | perspective draws into a target never shown (not blended between frames) |

A target counts for render-to-texture misses only if it was drawn this frame or the last, over the rows its draws
reached (the scissor, and within it the vertices or the viewport): VRAM under an old target, or below a half-size
one, holds ordinary textures (the world map's swizzled CLUT4 textures at `0x0413A600` were counted 226,538 times
before this rule).

## 2. The graphics tour

```powershell
.\.venv\Scripts\python.exe tools/port/gfx_tour.py --list
.\.venv\Scripts\python.exe tools/port/gfx_tour.py                                          # every route, base variant
.\.venv\Scripts\python.exe tools/port/gfx_tour.py --variants base,nointerp,1x --save-baseline  # about 35 minutes
.\.venv\Scripts\python.exe tools/port/gfx_tour.py --routes qb_kuja,qb_ex_burst --variants nointerp
.\.venv\Scripts\python.exe tools/port/gfx_tour.py --name probe --env D012_SCREENSHOT_EVERY=60 --script "460:pad a;..."
```

Build first (`tools/local/port/build-d012-tour`, or `--build-dir`). Each run: `run_d012.py`, at most 90 s,
`PSPRECOMP_NO_INTERPRETER=1`, `D012_UNTHROTTLED=1` to the scene (battles switch to `speed normal` for the fight),
the census on, captures at fixed script frames. A run that logs `No recompiled function registered` is a problem;
none did. Every route runs from the owner's save (Chapter 12-1-2, Lightning; 15 Quick Battle stages, the 16 heroes).

| Route | Scenes (census) | Shows | Switches |
| --- | --- | --- | --- |
| `menus` | boot, title, main_menu | Notice, title, LOAD GAME and its list, main menu, Battle Mode, Quick Battle player and CPU select and setup | |
| `story_gateway` | menus, worldmap, gate_transition, gateway_board | Story Mode, Continue; walking on the world map; the Gateway of Fools and Hope's portal glow, its card; the white swirl into it and the title card; the board with its beam and Lightning's lines | |
| `qb_chaos_shrine` | menus, battle_intro, battle | Quick Battle, Old Chaos Shrine (either form): the intro, brave and HP attacks, a dash, an assist | |
| `qb_lunar_subterrane` | as above | Lunar Subterrane (FFIV) | |
| `qb_orders_sanctuary` | as above | Order's Sanctuary | |
| `qb_empyreal_paradox` | as above | Empyreal Paradox | |
| `qb_orphans_cradle` | as above | Orphan's Cradle | |
| `qb_ex_burst` | battle_intro, battle, ex_mode, ex_burst | Lightning enters EX Mode at once (R + square) and lands an EX Burst: Gestalt Drive, Zantetsuken (Odin) | `D012_TEST_EX_FULL` |
| `qb_kuja` | as above | Kuja (picked from the third grid row) in EX Mode (Trance) and his EX Burst | `D012_TEST_UNLOCK_FIGHTERS`, `D012_TEST_EX_FULL` |
| `qb_ex_revenge` | battle | EX-Centric rules, fought unthrottled; R + square under attack is EX Revenge (its greyscale from frame 8721, 335 frames, the same on every run seen), EX Mode, Bravery Break | `D012_TEST_EX_FULL` |
| `qb_ex_results` | battle, results | EX-Centric rules, fought unthrottled to the win: Bravery Break, the stage's change, the results screen (win pose, AP, gil, PP, the rematch menu) on most timelines | |

The test switches are the profile's (`port/host/d012_test_switches.cpp`, added by the triage); the tour reuses them.
Not covered: summons (no summon fired on these routes; Zantetsuken is an EX Burst's Odin), a forest on the world map,
absorbing a dead orb (the triage's `orb` scene has one), the other ten stages.

Variants: `base` (the owner's settings: 4x, 60 fps with interpolation, plus `D012_CHECK_REPLAY`), `nointerp`
(`D012_FRAME_RATE=30`), `1x` (30 fps and `D012_INTERNAL_SCALE=1`). Output under `build/port/gfx-tour/`:
`runs/<route>/<variant>/` (captures, `png/`, `sheet.png`, `census.json`, `run.log`), `baseline/`, `report.json`,
`census_summary.json` (the censuses of the invocation's base runs merged), `replay_checks.json`; `evidence/` keeps the images this
document cites.

**Regression check.** Menus, title, setup and VS screens repeat exactly (difference 0.0). Battles and walks do not:
the guest clock follows the wall clock, so a run falls into one of a few timelines (the shrine's ruined or restored
form, the CPU's moves, a moogle met on the way to the gateway), at 30 fps as at 60. The baseline therefore keeps one
picture per timeline seen (`NAME.png`, `NAME.alt1.png`, ...): `--save-baseline` adds a capture that matches none, and a
capture is compared with the nearest. After two or three saving runs a flag means a real change. `nointerp` is also
compared with the nearest `1x` picture, which differs by the scale alone when the timelines agree (2 to 4 of 255 in
battle). Interpolation is checked with the frame-replay check instead of pixels: every 150 flips the older frame is
drawn again from its recording and compared with the frame as drawn; the tool recomputes the share from the
check's RGB images, since the check's own count includes the alpha bits (the stencil), which the replay does not
rebuild. More than 1% is a problem.

`story_gateway` is the most timeline-dependent route: on some runs the confirm near the gateway starts the moogle's
lines instead, and the board captures then show the world map.

## 3. Findings

Counts are draws over the tour's base runs (one of each route). "Before" is the second tour, built before the
triage's patches 0033 to 0035 were merged.

| # | Feature | Draws | Where | What the renderer does | Likely visible as | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `fbtex.miss_clut_view` | 1,340 | EX Revenge, 335 frames (`qb_ex_revenge`; counted under `qb_ex_results` in the third tour, which then had the gauge switch) | The 5551 framebuffer (`0x04000000`/`0x04044000`) read as a 512x512 CLUT16 texture into `0x040CC000`, three passes with index shift 0, 5 and 10 and mask `0x1F` (one channel each), then composited. A palette texture is never matched to a render target, so it is decoded from guest memory, which holds at best the 1x write-back of an earlier shown frame | The greyscale of EX Revenge comes out flat grey: the stage disappears behind fighters, particles and HUD (`evidence/ex_revenge_grey.png`, `ex_revenge_grey_ex_fight2.png`, `_fight3.png`) | Fixed 21 October (section 9): palette views decoded on the GPU; `D012_NO_FB_CLUT` |
| 2 | `prim.lines` | 25,063 | Every battle, intro and fight (1,100-6,800 a route) | Line and line-strip primitives are skipped (`expand()` returns false) | Missing streaks: additive (2x alpha, FIX white) orange segments of two vertices, colour `0x80105FE4` | Open |
| 3 | `texmap.env_map` | 24,696 | Orphan's Cradle | Texture map mode 2 (shade mapping from the lights in `0xC1`) ignored; the vertex UVs are used | A 128x128 CLUT4 texture on quads (vertex type `0x99`, no normals) blended additively; the GE would take the coordinates from the lights and normals | Implemented 21 October (section 11); `D012_NO_ENV_MAP` |
| 4 | `depth.range_minz_maxz` | 7,501 | Order's Sanctuary (6,454), Empyreal Paradox | MINZ/MAXZ (`0x2710`..`0xC350`) narrower than the viewport's depth range on these draws; not applied | Additive 64x64 CLUT8 glow sprites (vertex alpha `0x7D`) drawn where the GE would cut them: a stage overlay too much | Not a visible problem: the range is the viewport's own but for 6 units, and the strips sit at the near plane Vulkan already clips (section 12; branch `depth-clip`, not merged) |
| 5 | `transfer.from_target`, `tex.large_change_missed_by_key` | 1 and 1 a burst | EX Burst | The shown frame is block-copied to RAM (`0x095CD140`, 480 wide) and drawn as a 512x512 5551 texture: read back at 480x272 (1x), and the cache key samples one word per 256 bytes; once the copy changed where no sampled word did | The burst's captured background is 1x at any scale, and can be an earlier capture | Fixed 21 October (section 10): copied on the GPU at the internal scale; no stale picture on this route; `D012_NO_TRANSFER_TARGETS`, `D012_TRANSFER_SAMPLED_KEY` |
| 6 | `clip.depth_clamp_far` | 875 | World map, EX Burst, Kuja, Empyreal Paradox | `0x1C` on, vertices past the far plane: Vulkan clips them | Parts of large additive effects (the 128x64 CLUT8 2x-alpha draws) cut at the far plane | Not a visible problem: the GE cuts them at the same place here (MINZ is the far plane) (section 12; branch `depth-clip`, not merged) |
| 7 | `blend.doubled_source_alpha` | 395,449 (487,304 before) | Every 3D scene | Factors 6/7 were read as plain alpha | Haze, glows, EX Force crystals, hit flashes at half strength (`evidence/double_alpha_before_after.png`: the shrine's mist and a crystal) | Fixed, patch 0034 (triage); `D012_NO_DOUBLE_ALPHA` |
| 8 | `tfx.tcc_rgb_handled` | 44,989 (59,355 before) | Every scene with a scene copy | TCC was ignored: the texel's alpha (the stencil bit) was used | The gate swirl (triage item 2); the half-size scene copies | Fixed, patch 0035 (triage); `D012_NO_TEXTURE_RGB` |
| 9 | Replay check | 48-79% of pixels before | Every battle | In-between frames composited a later pass of a reused scratch target | Miniatures top-left, strobing glow (triage items 3, 5, 7-9) | Fixed, patch 0033 (triage): 0.00-0.06% in colour after |
| 10 | `clut.variant`, `clut.contents_changed` | 670 and 17 | EX Revenge's three shifts; battle palettes in VRAM `0x0419xxxx` | The cache key took the first palette word and no index mode: the three channel passes shared one decode; a palette changed after entry 0 kept its old decode | Wrong colours in palette effects | Fixed, patch 0038; `D012_CLUT_FIRST_WORD_KEY` |
| 11 | `color_test` | 41,272 | Every 3D scene | Ignored | Only on the shadow composite, where the rejected texels subtract nothing (bring-up log, section 20) | Harmless here |
| 12 | `target.format_changed`, `.stride_changed` | 16,790 each | Every 3D scene | `0x040CC000` alternates 5551/512 (shadows) and 8888/64 or 128 (blur, glow) each frame; one image reused | Nothing: each use clears or covers it first | Harmless here |
| 13 | `interp.offscreen_perspective` | 25,025 (second tour), 0 (third) | Kuja in Trance | Perspective draws into `0x040CC000` (a CLUT8 256x128 texture, 51 triangles), composited onto the screen | Kuja's trance layer moves at 30 Hz between interpolated frames; timeline-dependent | Open, design question |
| 14 | `fbtex.format_reinterpreted` | 1 | First world-map frame | `0x04122000` drawn as 5650, sampled as 5551 | One frame | Ignore |
| 15 | `dither.16bit_target`, `clip.depth_clamp_near`, `tex.bufw_below_width`, `fbtex.alpha_from_16bit_target`, `tex.filter_nearest_ignored` | 48.9M, 7.2M, 1.7M, 41k, 1.7k | Everywhere | Not dithered (targets keep 8 bits); Vulkan's near clip; row length below the texture width (decoded as the GE reads it); 1-bit alpha kept at 8 bits; the player's filter setting over the draw's | Nothing seen | Low |

Seen in earlier tours, not the third: `target.inside_other_target` (7 draws: half-size copies at `0x041A6000` inside
`0x04196000`'s rows; harmless with the row rule above).

Checked on every draw and **never seen**: points, morph targets, flat shading, every alpha test case (so the shader's
NEVER/ALWAYS swap does not bite this game), logic op, antialias, z-scale 0, clamped viewport z, draws past 480x272,
ABS, two fixed colours, destination-alpha blends on 16-bit targets, texture functions BLEND and ADD, projection
mapping, mipmaps and level modes, textures over 512, VRAM mirrors, textures over depth buffers, sampling a target
past 480x272 or with REPEAT, swizzled/other-size/other-stride/DXT views of targets, memory changed under a target,
colour over a depth buffer, transfers or DMA into targets, bounding boxes, immediate vertices, SIGNAL behaviours,
vertex space full, textures drawn white.

Ignored commands at exit (`GeState::report_ignored_commands`, the census's `unhandled_commands`): `0x15`/`0x16`
region, `0x1C`, `0x20`, `0x25`-`0x28`, `0x2C`-`0x33` (set once), `0x36`-`0x38` patch state, `0x50`, `0xA1`-`0xBF`
(set once), `0xC0`, `0xC1`, `0xC8`, `0xCA`, `0xCC`, `0xD0`, `0xD8`-`0xDA`, `0xE2`-`0xE6`, `0xFF`. Each is covered by a
feature above where its value matters.

## 4. Recommended fix order

| Order | Fix | Effort | Why first |
| --- | --- | --- | --- |
| 1 | Palette views of render targets: when a CLUT4/8/16 texture lies over a target drawn with the same row length and pixel size, sample the target's copy on the GPU, rebuild the 16-bit pixel from the RGBA8 texel (5551, 5650, 4444), apply shift, mask and start, and look the index up in the palette (a small palette texture) in the shader | 2 days | A whole-screen effect lost (EX Revenge); the same mechanism serves other colour-grading effects. **Done** (section 9) |
| 2 | Lines (and points, same path): expand each segment into a screen-aligned quad one PSP pixel wide times the internal scale, as transformed sprites are expanded, so the replay and the pipelines need no new topology | 1 day | Every battle, from the intro |
| 3 | MINZ/MAXZ: discard in the fragment shader outside the range, mapped like the viewport's z (a uniform; the push constants are full) | 0.5 day | Order's Sanctuary's extra glows; cheap. **Not needed here** (section 12): no visible change; branch `depth-clip`, not merged |
| 4 | EX Burst capture: read back at the internal scale into a texture kept for the RAM copy (or treat the copy as a render target), and key large textures written by a transfer by a full hash once after the transfer | 1 day | The burst's backdrop: 1x and possibly stale. **Done** (section 10) |
| 5 | Environment mapping (mode 2): texture coordinates from the light vectors named in `0xC1` and the normal, in the vertex shader; check first what the GE does without normals (vertex type `0x99`) | 1-2 days | One stage, but every frame of it. **Done** (section 11) |
| 6 | Depth clamp: Vulkan `depthClamp` while `0x1C` is on, after a check on hardware or a reference of what the GE does past the far plane | 0.5 day + the check | Few draws. **Not needed here** (section 12): no visible change; branch `depth-clip`, not merged |
| 7 | Off-screen perspective passes under interpolation (Kuja's trance): blend or re-render them per present | 2+ days, a design | Timeline-dependent; motion only |
| 8 | Latent, not used here: the alpha test NEVER/ALWAYS swap in `ge.frag`, blend factors 8/9, BLEND's environment colour, ADD | 0.5 day together | Other games |

## 5. Changes

PortableKit, branch `dissidia` (patches regenerated in `port/portablekit-patches/`):

| Patch | Change | Off switch |
| --- | --- | --- |
| 0036 | GE census (`host/gpu/ge_census.*`, hooks in `ge_state.cpp`, `vulkan_renderer.cpp`, `main.cpp`, `input_script.*`; `cmake/PortableKit.cmake`; `docs/BRINGING_UP_A_GAME.md`); input script step `scene NAME` | Off unless `<prefix>_GE_CENSUS` |
| 0037 | Census: describe a draw only at a feature's first hit; write every 600 frames | |
| 0038 | Texture cache: a palette texture keyed by the bytes its last CLUT load read and by its index shift, mask and start, in the cache key and the per-list lookup (`texture_decode.cpp`, `vulkan_renderer.cpp`) | `<prefix>_CLUT_FIRST_WORD_KEY` |
| 0039 | Census: doubled source alpha and colour-only textures counted as handled (approximations only with their off switches) | |

A prototype of the doubled-alpha blend made before the triage's patch 0034 landed is kept on the clone's local
branch `proto-double-alpha` (not merged); its before/after (`evidence/double_alpha_before_after.png`) agrees with
patch 0034.

In this repository: `tools/port/gfx_tour.py`, this document, `port/README.md`.

## 6. Not verified

- Hardware: what the GE does past the far plane with `0x1C` on; environment mapping without normals; how EX Revenge
  looks on a PSP (the colour passes say greyscale; the port showed flat grey; since section 9 it shows what the
  traced passes compute, not compared with a handheld).
- The line segments' look: they were counted, never drawn.
- Summons, forests, dead orbs, ten of the fifteen stages, other fighters' EX Bursts, the Story Mode cutscenes and
  movies; captures were looked at as stills, not in motion.
- The CLUT key change against palette-animated textures on screen (the census counted 17 changes and the three
  shared decodes; no capture isolates them).

## 7. Reproduce

```powershell
.\tools\port\pk_cmake.cmd -S port -B tools/local/port/build-d012-tour -G Ninja `
    "-DD012_PORTABLEKIT_DIR=$PWD/tools/local/port/PortableKit" "-DD012_WORK_DIR=$PWD/build/port/recomp" `
    -DPSPRECOMP_GENERATED_JOBS=4
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-tour -j4
.\.venv\Scripts\python.exe tools/port/gfx_tour.py --variants base,nointerp,1x --save-baseline
# One route with the census of another switch's old behaviour:
.\.venv\Scripts\python.exe tools/port/gfx_tour.py --routes qb_ex_results --env D012_CLUT_FIRST_WORD_KEY=1
# The census alone, on any run: --env D012_GE_CENSUS=build/port/census.json, and "N:scene NAME" steps in the script
```

## 8. Lines: fixed (finding 2)

> 21 October 2026. Finding 2 (`prim.lines`) is fixed on the PortableKit branch `ge-lines` (worktree
> `tools/local/port/PortableKit-lines`, commit `6e5f8ba` on `dissidia` at `b701e14`; not merged, no patch
> regenerated). Build `tools/local/port/build-d012-lines` (the options of `build-d012-recomp`), data
> `build/port/data-lines` (a copy of `build/port/data`, with `video.present_mode=immediate`: with vsync every run
> stalled before its first frame on this session's unattended desktop, the unchanged tour build too). Captures, logs
> and censuses: `build/port/gfx-lines/` (never committed); the tour was run through a wrapper that points
> `gfx_tour.py` at those directories, so `build/port/gfx-tour` was not touched.

**What the lines are.** Hit sparks. `D012_TRACE_LINES` (new) on every battle route: all are transformed (none in
through mode), untextured, vertex type `0x19C` (8888 colour, float position), additive (factors 6/10: twice the
source alpha, FIX `0xFFFFFF`), depth test GEQUAL without depth writes, alpha test on, no stencil, lighting, fog or
culling, into the shown framebuffer (`0x04000000`/`0x04044000`). Almost every draw is a line strip of two
vertices, one segment: a bright head (alpha `0x80`) fading to a tail of half its colour and alpha, 2 to 30 PSP
pixels long, flying out from where an attack lands. Colour by stage: orange `0x105FE4` (Old Chaos Shrine, Empyreal
Paradox, the EX routes), yellow-orange `0x0E7FE4` (Lunar Subterrane, Order's Sanctuary), grey `0x808080` (Orphan's
Cradle). EX Revenge also draws red ones (`0xFF8080` to `0x990000`) as line lists of 14 to 20 segments in one draw.
They come in bursts: the intro's clash, each brave hit, HP attacks and EX Bursts, up to 45-90 segments a game
frame and none in between; 25,880 draws over the tour's base runs, none in menus or Story Mode. Nothing else in
the game draws lines on these routes (no speed lines, trails or debug lines). Without them every hit lost its
sparks: `evidence/l_hit_b_*_before_after.png` (a brave combo: a fan of orange streaks around Lightning),
`l_hp_*` (an HP attack's white streaks), `l_intro_a_*` (the intro's clash).

**The fix** (`host/gpu/line_quads.hpp`, `vulkan_renderer.cpp`): each segment becomes a quad of two triangles drawn
through the draw's own pipeline, so blending, depth, stencil, the pixel mask, colour doubling and the alpha test
are those of any triangle, and frame interpolation records and replays it like one.

- Width: one PSP pixel, measured along the minor axis (a mostly horizontal segment is one pixel tall in every
  column), which is the coverage of the GE's one-pixel line: at 1x it lights the same pixels, a diagonal is not
  1.41 times as bright, and at scale N it is N target pixels wide, the same share of the screen. A
  one-target-pixel line (Vulkan's own) would be a quarter of the width at 4x and, these lines being additive, a
  quarter of the light.
- Ends: no caps; a quad runs from end to end, so a strip's shared vertex is not drawn twice (half-open, as raster
  lines are). Colour, texture coordinates and depth per end, interpolated along the line (Gouraud).
- Transformed: cut at the near plane, the corners offset on the screen at each end's own depth and handed back in
  object space through the inverse transform, as transformed sprites are (so the vertex shader, lighting and fog
  are unchanged, and an interpolated frame blends the transform as for any draw); centred on the projected line.
  Through mode: centred half a pixel in, as the pixel a through coordinate names. Culling is off for lines.
- Interpolation: lines take part like every other draw. They pair by the same key (vertex address, type, count);
  their vertices move by new data each game frame, not by matrices, so in-between frames show them where the older
  frame drew them, moved with the camera, as the game's other particles.
- Switches: `<prefix>_NO_LINES` skips lines as before; `<prefix>_TRACE_LINES[=N]` logs the first N line draws and
  each frame's count. The census counts `prim.lines_drawn` (info) instead of `prim.lines` unless `NO_LINES` is set.
  `portablekit_render_tests` checks segment pairing, widths, ends, colours, the near-plane cut.

**Verified.** Before/after at the same frame (`NO_LINES` against the build) on `qb_chaos_shrine` at 4x and 1x,
30 fps: the timelines matched and only the line pixels differ (2,000 to 12,000 pixels at hit frames, 0 where there
are none); at 60 fps with interpolation lines show (`l_hit_c_base_60fps_after.png`) and every replay check stayed
at 0.00-0.03% (Kuja 0.18%, under the 1% limit). The whole tour (33 runs, 432 captures, `tour_after.txt`): no run
with problems and no interpreter fallback; its 184 flags against the census tour's baseline are other timelines
(the battles run on a different clock with immediate presents: other stage forms, moments and camera angles,
`evidence/tour_flags_are_timelines.png`), not rendering changes. Cost (`D012_PERF=log`, the shrine at 4x/60 fps):
at most about 17 line draws a game frame, of about 2,800; battle seconds averaged render 4.2 / GPU 1.9 ms without
lines and 4.0 / 1.2 ms with them, the seconds with the most sparks 4.7 / 1.3 ms: within run-to-run noise.
`portablekit_host_check` builds and the 21 framework tests pass.

**Not verified.** The GE's exact pixel choice for a line (which pixel a step lands in, whether the last pixel is
drawn) and flat-shaded lines (the census saw none); line antialiasing (`0x25`, never set). The sparks were
compared with the port's own frames, not with a handheld.

## 9. EX Revenge: fixed (finding 1)

> 21 October 2026. Finding 1 (`fbtex.miss_clut_view`) is fixed on the PortableKit branch `ex-revenge` (worktree
> `tools/local/port/PortableKit-exrevenge`, commit `79adede` on `dissidia` at `b701e14`; not merged, no patch
> regenerated). Build `tools/local/port/build-d012-exrevenge` (the options of `build-d012-recomp`), data
> `build/port/data-exrevenge` (a copy of `build/port/data` with `video.present_mode=immediate`: the monitor was
> asleep, and with vsync every run presented at 4 fps and never reached the title). Runs through a wrapper that
> points `gfx_tour.py` at that data and at `build/port/gfx-tour-exrevenge*/` (never committed); `build/port/gfx-tour`
> got only the two evidence images below.

**What the draws do.** Traced with `D012_TRACE_FB_CLUT=1` (new) and `D012_TRACE_GE_LIST=8722:2:400000` on
`qb_ex_revenge` at 30 fps (list run 8722, game frame 8721, the effect's first; it lasts 299 to 303 frames). Every
frame of the effect, once the stage is drawn into the shown framebuffer F (`0x04000000` or `0x04044000`, 5551, 512
wide), five passes follow, each of full-screen through-mode triangles, texture 512x512 with row length 512, no
vertex colour (so the colour is the material ambient colour, `0x55`/`0x58`), dither off:

| Pass | Into | Texture | Index (`0xC5`) | CLUT (5551, 2 blocks = 32 entries) | Colour, function | Blend | Leaves in red |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | `0x040CC000` | F as CLUT16 | shift 0, mask `0x1F`: F's red | `0x089D6050`: entry v = red v/31, no green or blue | `0x000098` (0.596), modulate, TCC off | source alpha / 1 - alpha, alpha 1: replace | 0.596 R |
| 2 | `0x040CC000` | F as CLUT16 | shift 5: F's green | the same | `0x0000FF` (1.0) | source alpha / FIX `0xFFFFFF`: add | + 1.0 G |
| 3 | `0x040CC000` | F as CLUT16 | shift 10: F's blue | the same | `0x00003A` (0.227) | add | + 0.227 B |
| 4 | `0x040CC000` | `0x040CC000` itself as CLUT16 | shift 0: the sum | `0x089D60D0`: entry v = grey (31 - v)/31 | replace (`0xC9000003`), TCC off | replace | grey |
| 5 | F | `0x040CC000` as 5551 | | | `0xFFFFFF`, modulate | replace | |

The GE reads a CLUT16 texel as the 16-bit word in VRAM, so over a 5551 framebuffer the index is the pixel itself
(red in bits 0-4, green 5-9, blue 10-14, the stencil bit 15), and the entry is ((index >> shift) & mask) |
(start << 4); passes 1-3 therefore copy one 5-bit channel each into the scratch buffer's red channel, weighted by
the material colour: twice the Rec. 601 luma (0.299, 0.587, 0.114, doubled to 0.598, 1.174, 0.228; green capped at
1.0 by the 8-bit colour), clamped at 31. Pass 4 maps that through an inverted ramp, pass 5 copies it over the
frame, and the fighters, effects and HUD drawn afterwards keep their colour. Both CLUTs are in main RAM, loaded by
`0xC4` from the game's data; none comes from a render target. So the intended look is not a plain desaturation: the
stage becomes a high-contrast greyscale negative in 32 levels, white where it was black, black wherever its luma
reaches one half (a bright sky), with the fighters in colour.

**Cause.** `find_framebuffer_texture()` matched only direct-colour textures to render targets. A palette texture
over F or `0x040CC000` was decoded from guest memory, which holds no render (at best the 1x write-back of an older
shown frame): the composite came out flat grey (175 of 255) wherever the stage was.

**Fix** (`host/gpu/vulkan_renderer.cpp` `palette_view()`, `host/gpu/shaders/fb_clut.frag`, game-agnostic). A
CLUT4/8/16/32 texture is matched to a render target when its rows are the target's rows (row length times index
bits equals the target's row in bits), its index fits a whole number of times in a pixel, and its first texel lies
inside the rectangle the target's draws reached (their scissors), guest memory under it unchanged as for
any texture over a target. A pass at the internal scale then reads the target's copy, turns each pixel back into the
guest word the GE would hold (5650, 5551, 4444 or 8888, the top bits of each channel, as the write-back stores
them), takes the texel's index from its bits (low bits first for indices smaller than a pixel), applies shift, mask
and start, and looks the entry up: in a 512x1 texture of the palette decoded from guest memory (kept per distinct
palette), or, when the CLUT address lies inside what a render target drew (entry size equal to its pixel size), in
that target, read at the middle of each guest pixel. The draw samples the resulting picture like any render target,
so filtering comes after the lookup, as on the GE. A picture made for a draw recorded for frame interpolation is
kept until no recorded frame can name it, as the render-target snapshots of patch 0033 are; others are reused in
command order. Off switch `D012_NO_FB_CLUT` (guest memory, as before); `D012_TRACE_FB_CLUT` logs the first 400
views; the census counts `fbtex.clut_view` (info) and `clut.in_target`. A sync of the target down to guest memory
was not needed: every step runs on the GPU at the internal scale.

**Verified.**

- Before and after, same timeline, same capture (`revenge4`), at 4x and 1x, 60 fps (interpolated) and 30 fps:
  `build/port/gfx-tour/evidence/ex_revenge_fixed.png` (before: `D012_NO_FB_CLUT=1` on the same build, or the build
  before the change). Before: flat grey with the fighters and some debris on it. After: the restored Old Chaos
  Shrine's walls, pillars and doors as a light greyscale negative, the sky black with grey clouds, Lightning and the
  HUD in colour; the 1x and 4x pictures agree, the 4x one at full detail (`ex_revenge_fixed_crop.png`: 1x before,
  1x after, 4x after). It is believed right because every value comes from the traced state: the game's own two
  palettes, its weights and blends; the rest of each frame is unchanged.
- Census: `fbtex.miss_clut_view` 1,196-1,212 a run before, 0 after; `fbtex.clut_view` 1,196-1,212, in the
  `qb_ex_revenge` battle only (four views a frame); no other route of the tour makes a palette view, and
  `clut.in_target` never occurs, so every other draw takes the unchanged path.
- Interpolation: the route fights unthrottled, which does not interpolate, so a variant at real time with
  `D012_CHECK_REPLAY` and `D012_CAPTURE_BLENDED` (`build/port/gfx-tour-exrevenge-rt*`): EX Revenge drawn in the
  blended frames; of 8 replay checks, two inside the effect differ in 1.9-2.1% of pixels, the same run with
  `D012_NO_FB_CLUT=1` 4.5% and 2.7%, both in the same box around the player in EX Mode (a pre-existing replay
  mismatch, not the palette views); the others 0-0.1%.
- Whole tour on the fixed build (`build/port/gfx-tour-exrevenge/`, 33 runs, 432 captures): no run with problems,
  no interpreter fallback, battle replay checks 0.00-0.23%. Its 185 flags against the census tour's baseline are
  other timelines (the owner's save has moved on since the baseline was made, and immediate presents change the
  clock), as for the lines fix in section 8.
- The 21 framework tests pass and `portablekit_host_check` builds (`tools/local/port/build-framework-exrevenge`).

**Not verified.** The look on a PSP. Palette views with indices smaller than a pixel (CLUT4/8 over a framebuffer),
over 5650, 4444 or 8888 targets, and CLUTs read from a render target: implemented, but this game uses none of them
on the toured routes. The hardware rounds each pass to 5 bits before the next blend reads it; the port adds in 8
bits and rounds once, which can move a pixel by a level or two. The extra pass's cost was not measured (four passes
a frame at the internal scale, only during EX Revenge).

## 10. EX Burst's captured background: fixed (fix order 4)

> 21 October 2026. On the PortableKit branch `exburst-envmap` (worktree `tools/local/port/PortableKit-exbg`, commit
> `a6bd1ad` on `dissidia` at `1a27208`; not merged, no patch regenerated), with section 11's commit before it.
> Build `tools/local/port/build-d012-exbg` (the options of `build-d012-recomp`), framework build
> `tools/local/port/build-framework-exbg`, data `build/port/data-exbg` (a copy of `build/port/data` with
> `video.present_mode=immediate`). Runs through a wrapper that points `gfx_tour.py` at that data and at
> `build/port/gfx-tour-exbg*/` (never committed); `build/port/gfx-tour` got only the evidence images.

**What the game does.** Traced with `D012_TRACE_TRANSFERS` (new) on `qb_ex_burst`. One GE block transfer
(`0xB2`-`0xB5`, `0xEB`/`0xEC`/`0xEE`, start `0xEA`), not `sceGuCopyImage` from the CPU and not a render to texture:
the shown framebuffer `0x04044000` (5551, 512 wide), rectangle 0,0 480x272, 2 bytes a pixel, into main memory at
`0x095CD140` with rows of 480 pixels, at Gestalt Drive's command prompt (frame 8802 on most timelines, 8722 on
others). For the next 56 frames each frame draws it as a 512x512 5551 texture, row length 480, two transformed
triangles into the shown framebuffer, source-alpha blended: the frozen scene behind the prompt. Every other
transfer on the route goes from main memory to main memory (a 64-wide 16-bit atlas patched into a 128-wide one, a
few times a second) or from main memory into VRAM (32-wide 8888 textures and 256x1 palettes); none reads a target.

**Cause.** `read_back_framebuffer()` waited for the GPU and stored the target at 480x272 in guest memory; the copy
then ran in guest memory, and the texture was decoded from it: 1x at any internal scale. The stale picture: the
census's check hashes the texture's whole 512 rows (491 KB), while the copy is 272 rows (261 KB). The memory past
row 272 changes at frame 8835 (game data, never sampled); the 480x272 the draws read did not change in any of the
56 frames (the new per-list hash of it, below). So nothing stale was drawn on this route; but a later capture that
differed from an earlier one only between sampled words would have been, so the key is fixed too.

**Fix** (`host/gpu/vulkan_renderer.cpp` `copy_block_transfer()`, called from `block_transfer()` in
`host/hle/hle_media.cpp` after the guest copy; game-agnostic). When a transfer's source lies in a render target
with the same row length and pixel size, the same rectangle is blitted on the GPU, at the internal scale, in command
order, into a render target standing for the destination (made the first time; at most four). Guest memory still
gets the 1x copy for the game's own reads. A texture over the destination is matched to that target as to any
render target, but only inside the rectangle copied and only while every byte of guest memory under it is what the
transfer left (a hash, checked once a display list); otherwise it is decoded from memory as before. Draws recorded
for frame interpolation sample a snapshot of it, as of any target (patch 0033). A transfer into a target the game
draws to is left as before. Textures over the latest 16 transfer destinations are keyed by every byte instead of
one word in 256. Switches: `D012_NO_TRANSFER_TARGETS` (guest memory only, as before), `D012_TRANSFER_SAMPLED_KEY`
(sampled key, as before), `D012_TRACE_TRANSFERS` (each transfer and what became of it, each lookup over a copy).
The census counts `transfer.gpu_copy` (info).

**Verified.**

- Before (`D012_NO_TRANSFER_TARGETS=1 D012_TRANSFER_SAMPLED_KEY=1`) and after, same timeline, capture `ex_burst3`
  (frame 8820), 4x 30 fps: only the frame changes (1,635,368 pixels, nearly all by a few levels), from the 1x copy
  scaled up to the scene at full detail; `evidence/ex_burst_capture_before_after.png` (4x 30 fps before and after,
  4x 60 fps after) and `ex_burst_capture_full.png`. Every lookup in the 56 frames sampled the copy, at 30 and 60 fps
  and at 1x.
- 1x: the GPU copy is the same size as the read-back; captures before and after match where the timelines did
  (`ex_burst_capture_1x.png` shows each with the capture, at different frames).
- Interpolation (4x and 1x, 60 fps, `D012_CHECK_REPLAY`): replay checks 0.00-0.04% on `qb_ex_burst`; the check at
  frame 8850 falls inside the 56 frames and was 0 pixels.
- Other routes: no other transfer reads a target, so nothing else takes the new path; the full-byte key applies to
  the atlas patches and VRAM uploads above (small textures, keyed whole before already).
- Whole tour on the build with both fixes (`build/port/gfx-tour-exbg-full/`, 33 runs, 432 captures, base, nointerp
  and 1x): no run with problems, no interpreter fallback, no stall; replay checks 0.00-0.03% in colour; the menus
  differ from the census tour's baseline exactly as the section 9 tour's did (main menu and Battle Mode, mean 1 of
  255); the 187 flags are other timelines, as in sections 8 and 9. One earlier unattended run (`qb_ex_burst`, 60
  fps) stalled at the player select (no frame for 66 s, every guest thread waiting); not seen again in 40 runs.
- The 20 framework tests (`tools/port/pk_ctest.cmd`, ad hoc tests excluded) pass and `portablekit_host_check`
  builds (`tools/local/port/build-framework-exbg`).

**Not verified.** A transfer whose rectangle is not at 0,0 or not 480x272, chains of copies, and a copy whose memory
the game then edits with the CPU (implemented: the hash then rejects the GPU copy); the look on a PSP.

## 11. Environment mapping: implemented (fix order 5)

> 21 October 2026. Same branch, worktree, builds and data as section 10; commit `be8560c`.

**What the draws do.** Traced with `D012_TRACE_GE_LIST=9000:2:400000` and `D012_TRACE_ENV_MAP` (new) on
`qb_orphans_cradle`: 84 draws a frame from the battle on, all of texture `0x04118580` (128x128 CLUT4, swizzled, one
level: `0xC2` = 1, so `0xC8` = 0, level mode auto with no bias, changes nothing), triangle strips of four vertices
of type `0x99` (8-bit UV, 4444 colour, 8-bit position, **no normal**), lighting on with every light off, additive
(source alpha / FIX white), depth writes off, the stencil test on: the violet glowing strips around the wheels of
Orphan's shell. `0xC0` = `0x102`: map mode 2 (environment), projection source UV (unused in mode 2); `0xC1` = 0:
light 0 for both U and V. Light 0's vector was never written (0,0,0; lights 2 and 3 hold (1,0,0) and (0,-1,0)). The
texture scale is (0.502, 0.498) and the offset scrolls in U (6.67, -4.93, ...), set for the vertex UVs, which run
0-2 in U and 0.5-1.0 in V. So the game draws these strips in mode 2 with the state of a UV-mapped draw: the mode
looks left over, and what the GE makes of it decides their look.

**PSP behaviour.** From public GE documentation: the PSPSDK's `sceGuTexMapMode(GU_ENVIRONMENT_MAP, a1, a2)` and its
`envmap` sample, which sets lights 0 and 1 to the two vectors the normal is projected on; the GE's shade mapping takes s = (1 + N.L1) / 2 and t = (1 + N.L2) / 2 with N the normal in world space (after
the world matrix, normalised, reversed by `0x51`) and L the lights' position registers normalised, whatever the
lights' types and enables; the vertex UVs are not read. Less certain, and not testable here: the texture scale
applies to the result and the offset does not; a zero light vector counts as (0, 0, 1); a vertex without a normal
counts as (0, 0, 1) in world space (the renderer's lighting already does so). Under those, every vertex of these
strips gets the same coordinate (0.502, 0.498): one texel near the texture's centre, instead of the scrolling band.

**Cause.** `0xC0`/`0xC1` were ignored commands; every transformed draw used its vertex UVs.

**Fix** (`environment_map_uv()` in `host/gpu/shaders/ge.vert`; `submit()` in `vulkan_renderer.cpp`;
`DrawCall::texture_map`, `texture_env_lights` from `GeState`). In mode 2 the vertex shader makes U and V as above
from the object block's world matrix, the environment block's light vectors and two new fields (the lights for U
and V, reverse normals); env-mapped draws write both blocks and are recorded with their object block for frame
interpolation, as lit draws are. Switches: `D012_NO_ENV_MAP` (vertex UVs, as before), `D012_ENV_MAP_OFFSET` (add
the texture offset too), `D012_TRACE_ENV_MAP[=N]`. The census counts `texmap.env_map_drawn` (info, with whether the
vertices have normals) instead of `texmap.env_map`. Projection mapping (mode 1) is still not done (never seen).

**Verified.** Before (`D012_NO_ENV_MAP=1`) and after, same timeline, `fight4`-`fight6`, at 4x and 1x, 30 and 60 fps
(`evidence/orphans_cradle_env_map.png`: `fight5` cropped, before, after and the difference): only the strips change,
840 to 2,600 pixels at 4x and about 1,500 at 1x (10,000 at 1x 60 fps, most by a level or two), a little dimmer and without the band's pattern; everything else is
identical. Replay checks at 60 fps: 0.00% on the stage. The census: 24,696 draws a run, `lights 0,0, no normals`.

**Not verified.** Mode 2 on hardware, and in particular these three details, on which the strips' look depends:
the missing normal, the zero light vector and the offset. If the PSP draws these strips with their vertex UVs
(that is, if the mode is ignored without normals), `D012_NO_ENV_MAP` is the right picture; a capture from a handheld
of Orphan's Cradle would decide it. Env mapping with normals (none on the tour).

## 12. Depth range and the far plane: investigated, not merged (fix order 3 and 6)

**Decision (21 October 2026):** not merged into `dissidia`. On every traced draw the before and after captures are
identical, so neither finding is a visible problem in this game, and the change is about 600 lines that had already
caused one crash (fixed on the branch). Its rules were also taken from PPSSPP's software renderer as a reference,
which needs a careful clean-room rewrite before it could go into an MIT framework. It stays on the branch `depth-clip`
for a game that needs it. The notes below describe that branch.

> 21 October 2026. Findings 4 and 6 of section 3 (`depth.range_minz_maxz`, `clip.depth_clamp_far`), fix order 3 and 6,
> on the PortableKit branch `depth-clip` (worktree `tools/local/port/PortableKit-depthclip`, commits `d29810a` and
> `61256fb` on `dissidia` at `1a27208`; not merged, no patch regenerated). Build `tools/local/port/build-d012-depthclip`
> (the options of `build-d012-recomp`), data `build/port/data-depthclip` (a copy of `build/port/data` with
> `video.present_mode=immediate`), runs through a wrapper that points `gfx_tour.py` at them and at
> `build/port/gfx-depthclip-*/` (never committed); `build/port/gfx-tour` got only the evidence images below.

**What the GE does** (public reference: the hardware-tested clipper and pixel pipeline of PPSSPP's software
renderer, `GPU/Software/Clipper.cpp`, `TransformUnit.cpp`, `DrawPixel.cpp`; not checked on a handheld here). For
transformed draws only; through mode takes z as given and skips all of it.

- Screen depth is S = z / w * z scale + z centre. The viewport's z range clips nothing: there is no far plane.
- MINZ/MAXZ: a pixel with S outside [MINZ, MAXZ] is discarded before every other test (no colour, depth or stencil
  write), in clear mode too.
- `0x1C` on (`GU_CLIP_PLANES`): triangles and lines are cut at the near plane (z = -w); sprites are never cut. S is
  clamped to 0..65535, so past the far plane the GE draws at the end of the range unless MINZ/MAXZ discards it.
- `0x1C` off: nothing is cut at the near plane either, but a primitive with a vertex whose S leaves 0..65535, or
  with w <= 0, is dropped whole.
- Always: a primitive with a vertex outside the guard band (screen x or y before the offset outside 0..4096) is
  dropped whole, except that with `0x1C` on a vertex the near plane cuts away does not count; one whose vertices all
  lie past the far plane, or all past the near plane, is dropped; a sprite with a corner at w <= 0 is dropped.

Ambiguous or unverified: whether the clamp applies per vertex (the reference) or per pixel (PPSSPP's hardware
renderers); the exact rounding of S against MINZ/MAXZ (integer depths); hardware tests are cited by the reference,
not repeated here.

**What this game sets** (`D012_TRACE_DEPTH_RANGE`, new; every battle stage, the world map, the EX Burst): every 3D
draw has viewport z scale -20000 and centre 30000 (device z -1..1 to S 50000..10000: near 50000, far 10000, the depth
test GEQUAL) and MINZ/MAXZ `0x2710`..`0xC350`, 10000..50000: exactly the viewport's range. The census's 7,925
"narrower" draws are glow strips with centre 30006 (near plane at 50006, MAXZ 6 units in front of it): Order's
Sanctuary's 64x64 CLUT8 glow (`0x090CE7D0`), Empyreal Paradox's (`0x0909FA10`), vertex type `0x19F`, `0x1C` off,
additive. The far-plane draws (`0x1C` on, 1,477 on the tour) are the large 2x-alpha effects; past the far plane their
S drops below 10000, the GE clamps it (still below 10000) and MINZ discards it: the GE cuts them at the far plane,
where Vulkan cut them.

**The fix** (`host/gpu/depth_clip.hpp`, `vulkan_renderer.cpp`, `shaders/ge.vert`, game-agnostic). The vertex shader
maps a transformed draw's depth so that Vulkan's clip volume (0 <= z <= w) ends where the GE stops drawing: the
window of S a pixel may have (MINZ/MAXZ, and the near plane with `0x1C` on), with the Vulkan viewport's depth range
set to the same window so the depth written is S / 65535 as before. For flat primitives clipping there covers
exactly the pixels the GE's test passes, so there is no discard and no loss of early depth testing. With `0x1C` on
and no MINZ/MAXZ on the far side, a vertex past the far plane has its depth clamped to the end of the range. The two
mapping constants ride in the push constants' viewport x and y, unused by transformed draws. Primitives the GE drops
whole are left out on the CPU (per-vertex classification, then per triangle, sprite or line segment, in the
expansion and the index lists alike); GPU-decoded (raw) draws are not culled. Frame interpolation replays the
recorded push constants and viewport, so in-between frames clip alike; lines (section 8) go through the same
culling; a skinned draw recorded for interpolation keeps its vertex numbering (no compaction, no culling when
expanded), and the vertex count recorded for replay is now the count written (an earlier version crashed at 60 fps
on Empyreal Paradox when culling made a skinned draw compact). Switches: `D012_NO_DEPTH_RANGE_CLIP` (MINZ/MAXZ
ignored), `D012_NO_GE_DEPTH_CLIP` (Vulkan's near and far planes, nothing dropped), both: the old mapping exactly;
`D012_NO_GUARD_BAND`; `D012_TRACE_DEPTH_RANGE[=N]`; `D012_DEPTH_CLIP_ALTERNATE` (odd frames the old way, and the
frame pairs around a dropped glow primitive captured to `D012_SCREENSHOT_DIR`). Census: `*_applied` and
`clip.out_of_range_dropped` (7,403 draws on the tour) are info unless a switch is set.

**Verified.**

- What is dropped: about 4,000-7,000 primitives a frame in battle, nearly all stage quads wholly behind the eye or
  with a vertex off the guard band, which Vulkan clipped to nothing on screen; of the glow strips, a few primitives a
  frame while the camera passes through them (`0x1C` off, a vertex past the near plane and out of 0..65535).
- Before/after on the same build (both switches against none), 30 fps 4x, matched timelines: Order's Sanctuary's
  intro and first seconds (39 captures) and a fight with the glow route's inputs (17): every capture pixel-identical
  (`evidence/depthclip_orders_before_after.png`, `depthclip_orders_intro_before_after.png`). At 1x and at 60 fps the
  runs fell on other timelines (intro lines, CPU moves); the alternate mode compares on one timeline instead: 40
  pairs on Order's Sanctuary and 9-11 on Empyreal Paradox, at 4x and 1x, around frames that dropped glow primitives show only the motion between two frames,
  no glow (`evidence/depthclip_alternate_orders_8576.png`, `depthclip_alternate_paradox_9014.png`: the old frame,
  the GE frame, their difference).
- 60 fps (`D012_FRAME_RATE_AUTO=0`): both stages run, replay checks 0.00% in colour; render and GPU time within the
  noise of the before runs (both captured every 10 frames).
- Whole tour (33 runs, 432 captures, `build/port/gfx-depthclip-tour.txt`): no run with problems, no interpreter
  fallback, every run exited 0, replay checks 0.00-0.03% in colour; its flags are `nointerp` against `1x` on other
  timelines (the directory has no baseline). Menus against the census baseline: identical but for the save's play
  time.
- `portablekit_render_tests` (depth window and culling cases) and the other framework tests pass (20, the ad hoc
  ones skipped), `portablekit_host_check` builds (`tools/local/port/build-framework-depthclip`).

**Not verified.** Any of it on a handheld. A visible case: none of the toured scenes changes, so the code paths that
matter for other games (a narrower MINZ/MAXZ, depth past the far plane inside 0..65535, `0x1C` off with vertices out
of range on screen) are checked by the unit tests only. Per-vertex against per-pixel clamping; the near plane with
`0x1C` on when it lies beyond 0..65535 (the window stops at the end of the range instead of clamping).
