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
     whole scene turns flat grey for the effect's length (`evidence/ex_revenge_grey.png`).
  2. **Lines are not drawn** (`prim.lines`, 25,063 draws, every battle from the intro on): additive (factor 6/FIX)
     orange line segments, two vertices each.
  3. **Environment mapping** (`texmap.env_map`, 24,696 draws, Orphan's Cradle only): drawn with the vertex UVs.
  4. **Depth range clip** (`depth.range_minz_maxz`, 7,501 draws, Order's Sanctuary and Empyreal Paradox): additive
     64x64 sprites the GE would cut at MINZ/MAXZ are drawn whole.
  5. **EX Burst's captured frame** (`transfer.from_target`, `tex.large_change_missed_by_key`): the frame is copied to
     RAM at 480x272 and drawn as a 512x512 texture whose cache key samples one word per 256 bytes; one capture was
     drawn from a stale decode.
  6. **Effects past the far plane** (`clip.depth_clamp_far`, 875 draws): clipped by Vulkan, clamped by the GE.
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
| 1 | `fbtex.miss_clut_view` | 1,340 | EX Revenge, 335 frames (`qb_ex_revenge`; counted under `qb_ex_results` in the third tour, which then had the gauge switch) | The 5551 framebuffer (`0x04000000`/`0x04044000`) read as a 512x512 CLUT16 texture into `0x040CC000`, three passes with index shift 0, 5 and 10 and mask `0x1F` (one channel each), then composited. A palette texture is never matched to a render target, so it is decoded from guest memory, which holds at best the 1x write-back of an earlier shown frame | The greyscale of EX Revenge comes out flat grey: the stage disappears behind fighters, particles and HUD (`evidence/ex_revenge_grey.png`, `ex_revenge_grey_ex_fight2.png`, `_fight3.png`) | Open |
| 2 | `prim.lines` | 25,063 | Every battle, intro and fight (1,100-6,800 a route) | Line and line-strip primitives are skipped (`expand()` returns false) | Missing streaks: additive (2x alpha, FIX white) orange segments of two vertices, colour `0x80105FE4` | Open |
| 3 | `texmap.env_map` | 24,696 | Orphan's Cradle | Texture map mode 2 (shade mapping from the lights in `0xC1`) ignored; the vertex UVs are used | A 128x128 CLUT4 texture on quads (vertex type `0x99`, no normals) blended additively; the GE would take the coordinates from the lights and normals | Open |
| 4 | `depth.range_minz_maxz` | 7,501 | Order's Sanctuary (6,454), Empyreal Paradox | MINZ/MAXZ (`0x2710`..`0xC350`) narrower than the viewport's depth range on these draws; not applied | Additive 64x64 CLUT8 glow sprites (vertex alpha `0x7D`) drawn where the GE would cut them: a stage overlay too much | Open |
| 5 | `transfer.from_target`, `tex.large_change_missed_by_key` | 1 and 1 a burst | EX Burst | The shown frame is block-copied to RAM (`0x095CD140`, 480 wide) and drawn as a 512x512 5551 texture: read back at 480x272 (1x), and the cache key samples one word per 256 bytes; once the copy changed where no sampled word did | The burst's captured background is 1x at any scale, and can be an earlier capture | Open |
| 6 | `clip.depth_clamp_far` | 875 | World map, EX Burst, Kuja, Empyreal Paradox | `0x1C` on, vertices past the far plane: Vulkan clips them | Parts of large additive effects (the 128x64 CLUT8 2x-alpha draws) cut at the far plane | Open; GE behaviour not verified |
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
| 1 | Palette views of render targets: when a CLUT4/8/16 texture lies over a target drawn with the same row length and pixel size, sample the target's copy on the GPU, rebuild the 16-bit pixel from the RGBA8 texel (5551, 5650, 4444), apply shift, mask and start, and look the index up in the palette (a small palette texture) in the shader | 2 days | A whole-screen effect lost (EX Revenge); the same mechanism serves other colour-grading effects |
| 2 | Lines (and points, same path): expand each segment into a screen-aligned quad one PSP pixel wide times the internal scale, as transformed sprites are expanded, so the replay and the pipelines need no new topology | 1 day | Every battle, from the intro |
| 3 | MINZ/MAXZ: discard in the fragment shader outside the range, mapped like the viewport's z (a uniform; the push constants are full) | 0.5 day | Order's Sanctuary's extra glows; cheap |
| 4 | EX Burst capture: read back at the internal scale into a texture kept for the RAM copy (or treat the copy as a render target), and key large textures written by a transfer by a full hash once after the transfer | 1 day | The burst's backdrop: 1x and possibly stale |
| 5 | Environment mapping (mode 2): texture coordinates from the light vectors named in `0xC1` and the normal, in the vertex shader; check first what the GE does without normals (vertex type `0x99`) | 1-2 days | One stage, but every frame of it |
| 6 | Depth clamp: Vulkan `depthClamp` while `0x1C` is on, after a check on hardware or a reference of what the GE does past the far plane | 0.5 day + the check | Few draws |
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
  looks on a PSP (the colour passes say greyscale; the port shows flat grey).
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
