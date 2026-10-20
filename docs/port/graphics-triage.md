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
| 4 | Leaf trail in forests flickers | (a)+(b) probably | No forest on the scripted routes; the same glow and blur composites cover the whole world map | Probably fixed with 3; not reproduced | `D012_NO_FB_SNAPSHOTS` |
| 5 | Blobs in front of red gateways jitter sideways | (a)+(b) | `gate_view` scene: replayed frames differ in 8% of pixels without snapshots; the glow and blur round the portal and the dark shapes at its base come from another 120-pixel strip (`item5_gate_replay.png`). With snapshots 0 pixels differ, and at t = 0.5 the portal's bright centre sits between the two frames (995.7, 996.0, 996.1 px) | Fixed by patch 0033 (likely; judged from replays, not motion) | `D012_NO_FB_SNAPSHOTS` |
| 6 | Ground cracks linger in Lightning's EX Burst | open | `lightning_ex` scene (EX Burst, Gestalt Drive, frames 9300-9600): nothing lingers at 30 fps or 60 fps in the captures. Checked and ruled out: a palette rewritten after its first entry (the texture key hashes only the first CLUT word): a trace of every key over the whole battle and the world map found none | Not reproduced. Next: the crack decals' blend mode (factor 7?) and their fade, with the burst's command inputs entered | |
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
