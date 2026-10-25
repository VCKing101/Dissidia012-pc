# Performance: the 45 and 30 fps dips with frame interpolation

> 21 October 2026. The owner reported that with 60 fps frame interpolation the game "drops to 45 and 30 frames quite
> frequently", on the PC (i7-12700K, 32 GB, Radeon RX 9070 XT, a 280 Hz monitor) and on the Steam Deck (the Windows
> build through Proton); later, that the world map's "power of the dead" orbs slow the Deck down and make the music
> choppy. This is the measurement, the causes, the fixes (each with an off switch) and before/after numbers on the PC.
> The Deck was not touched; section 7 gives recommendations for it.
>
> 22 October 2026: section 9 adds the real fix, presents on a thread of their own: the 99th percentile of the time
> between presents is 17.3-17.5 ms in every 3D scene and the delay back at 19.2 ms.
>
> PortableKit: four commits on branch `perf` in the worktree `tools/local/port/PortableKit-perf` (from `dissidia` at
> `cb3f376`), not merged. Recompiled build: `tools/local/port/build-d012-perf` (the options of `build-d012-recomp`, plus
> `/DEBUG:FULL` at link time for the stall sampler's names). Data: `build/port/data-perf`, a copy of `build/port/data`
> whose `ms0/` and `settings.ini` are restored before every run. Logs, CSVs, statistics: `build/port/perf/` (not
> committed). The framework's 20 tests (ad hoc excluded) pass and `portablekit_host_check` builds
> (`tools/local/port/build-framework-perf`).

## 1. Summary

- **What "45" and "30" are on the PC.** Not a lower frame rate: the rate governor never stepped down in any PC run,
  and the fps meter (presents per second) read 59.5 to 60 in every scene. With interpolation, the present between two
  game frames came up to 7 ms late in about one frame in ten: presents 11 and 22 ms apart. 22 ms is "45 fps" and the
  overlay's `FRAME ... MAX` showed about 22 every second; a present skipped is 33 ms, "30". The meter itself dips (to
  36-49 for one second) only at loads.
- **Causes, ranked** (section 4): (1) presents made on the emulation thread fall in the game's code; (2) a quarter of a
  second of function registration at every overlay swap (menus to battle or world map), with the sound running dry; (3)
  byte-at-a-time HLE memory copies (0.7 ms of every 7 ms frame); (4) log writes to the repository's hard drive (a
  measurement artefact here, a real risk for anyone running with a console or a log on a slow disk). Not causes:
  pipeline compilation, texture hashing and decoding, snapshot copies, the GPU (sections 4.5-4.8).
- **After the fixes**, at the owner's settings (4x, 60 fps, vsync, warm cache): the 1% low went from 44.6-45.5 to
  53.6-54.6 fps in every 3D scene, the 99th percentile of the time between presents from 22.0-22.4 ms to 18.1-18.4 ms,
  presents more than 20 ms apart in the orb route from 147 to 2 of 3,420. Mean fps unchanged (60). The cost: 9 ms more
  delay between a game frame and its display (19.6 to 29 ms), as section 5 explains.
- **The orbs** (the new priority): nothing in them is expensive on the PC. Standing at, walking to and absorbing an
  orb, at 4x and 2x: GPU 1.2-1.3 ms a frame, render 2.8 ms, guest 6.5-7.3 ms, 2 snapshots a frame, no pipelines, no
  audio gaps, no stall over 44 ms. The orb scene shows exactly the general jitter (1% low 45.4 to 54.5 fixed). What would
  stop the whole process on the Deck is in section 7.

## 2. Method

**Instrumentation** (PortableKit, game-agnostic, off by default; `docs/BRINGING_UP_A_GAME.md` lists them):

- `D012_PERF_CSV=<file>` (`host/perf/frame_stats.*`): one row per present (interval since the last, what it showed:
  `flip` (no interpolation), `blended`, `newest` (a frame as it is), `older`, `cut` (pairing refused), `textures`; blend
  fraction, grid moments skipped, lateness against its due time, the governor's rate) and one per game frame (guest,
  render, GPU wait, pacing, GPU time, the emulation thread's CPU time, pipelines created and their time, pipelines the
  prewarm had ready, textures decoded and their CPU time, texture-key hashing time (whole-palette and every-byte
  transfer keys included), render-target snapshots and their size, interpolation's cut reason and matched/eligible draws,
  presents dropped for a busy display or over budget). The input script's `scene NAME` is a column.
- `D012_TRACE_STALL_SAMPLES[=ms]` (Windows): while a frame runs past the threshold, samples the emulation thread's host
  stack every 4 ms and prints the commonest stacks at the next flip; `=present` samples while a present is more than
  2 ms overdue. With the PDB beside the program the stacks have names. This found causes 2, 3 and 4.
- Existing: `D012_PERF=log`, `D012_TRACE_INTERPOLATION=1` (the `[interp]` lines, governor steps), `D012_TRACE_STALLS`,
  `D012_TRACE_AUDIO` (per second: frames the device took that the game had not written, i.e. underruns).

**Runner**: `tools/port/perf_tour.py` (uses `gfx_tour.py`'s menu paths; each run through `run_d012.py`, at most 90 s,
`PSPRECOMP_NO_INTERPRETER=1`). Each route is unthrottled to its scene (scene `fast`, left out) and real time after:

| Route | Real-time scenes |
| --- | --- |
| `menus` | loading the save, main menu, Battle Mode, character select |
| `worldmap` | Story Mode Continue (its load), the world map walk, a gateway, the gate transition, the gateway board |
| `orb` | the world map walk to a "power of the dead" orb, absorbing it (cross), standing after (gfx_triage's `orb` scene) |
| `qb_ex_burst` | versus screen, loading screen, intro, attacks, EX Mode, EX Burst (Lightning, Old Chaos Shrine) |
| `qb_kuja` | the same as Kuja (`D012_TEST_UNLOCK_FIGHTERS`), his orbs and effects |

```powershell
.\.venv\Scripts\python.exe tools/port/perf_tour.py run --label after_vsync_warm            # all routes
.\.venv\Scripts\python.exe tools/port/perf_tour.py run --label before_vsync_warm --env D012_NO_OVERLAY_PARKING=1 `
    --env D012_NO_CODE_PRESENT_POLL=1 --env D012_PRESENT_AFTER_FLIP=1 --env D012_NO_FAST_HLE_COPIES=1
.\.venv\Scripts\python.exe tools/port/perf_tour.py run --label x --cold --present immediate --env D012_INTERNAL_SCALE=2
.\.venv\Scripts\python.exe tools/port/perf_tour.py table before_vsync_warm after_vsync_warm  # Markdown, as below
```

Statistics per scene from the present rows: mean presents per second; the 1% low (1000 over the 99th percentile of
the time between presents); seconds whose present count, as the fps meter counts them, is under 55; presents more than
25 ms after the one before; the share blended. "Before" is the same build with the four off switches, so both sides
run the same binary, data and routes.

**Conditions and pitfalls**:

- Vsync runs: FIFO on the 280 Hz monitor (awake throughout; a sleeping monitor would stall FIFO presents).
- *Cold* deletes the data copy's `pipeline_cache.bin` and `pipeline_keys.bin`; the AMD driver's own shader cache
  could not be cleared (no system changes), so "cold" pipelines still compiled in 0.1 ms each.
- The repository's `G:` is a 4 TB SMR hard drive. The game's own log writes there blocked for 0.4 to 2.1 s
  (`NtWriteFile` under `fflush` in `load_trace::tick` and `SasCore::render`, from the sampler), which showed as "guest"
  or "render" stalls of 120-550 ms in the first runs. The runner now writes logs and CSVs on the NVMe drive and copies
  them afterwards; the disc image and the data copy stay on `G:`.
- Another agent was running its own build of the game at the same time. The runner waits until no other
  `D012Native.exe` runs, watches during the run, and repeats a run that overlapped (twice at most); every table below
  is from runs without an overlap.
- A battle's timeline differs from run to run (the guest clock follows the wall clock); on `qb_ex_burst` the frames'
  moments fell between grid moments, so both presents of a frame were blended (blended share 0.93-1.0) and its "before"
  was already better than the others'.

## 3. How the fps meter counts

The perf overlay's `FPS` and the `[perf]` line's `fps` are presents per real second over the last whole second: every
present counts, interpolated or not, so a refused interpolation (a camera cut: the older or newer frame shown as it is)
does not lower it. `GAME` is the game's own flips per emulated second (30). `FRAME avg MAX` are the mean and the longest
time between presents in that second; MAX is what read 22 ms ("45 fps") every second before. The graph shows each
present's interval. A reading of 45 in `FPS` itself means 45 presents that second: the governor at 45 (it steps 60 →
45 → 30 when presents are late or the game falls behind real time, and then blocks 60 for 30 s), or a stall.

## 4. Causes

### 4.1 Presents in the game's code (the PC's "45"; fixed)

With interpolation at 60 each game frame gets two presents: the frame as it is, and one blended halfway to the next.
They are made on the emulation thread, in the kernel's idle time or when the game's code reaches a thread switch or a
draw. The present clock delayed them by half a frame plus the time to the flip (19.6 ms): the in-between present fell
about 2.7 ms after the next flip, while the game, which flips early in its frame, was still running its next frame's
code (6.5-10 ms of it; the guest is 6.5-7.3 ms a frame in 3D scenes, not the 1-2 ms of the title screen). One in ten
of those presents waited for the code (orb route: blended presents' lateness p50 0.06 ms, p99 6.4 ms; intervals
p1 11.4 ms, p99 22.0 ms). The `=present` sampler put the overdue time in recompiled units (loops inside one unit, no
dispatch), in present calls themselves and in SDL's event pump.

Fixes: (a) the runtime's starvation boundaries (every 20,000 dispatches) poll for due presents
(`D012_NO_CODE_PRESENT_POLL` turns it off): in a test run p90 lateness went from 5.5 to 0.2 ms, p99 stayed (the
loops have no boundary);
(b) `PresentClock::note_idle`: the first idle time after a flip ends the frame's work, and the delay allows for it, so
both presents fall in the idle time (`D012_PRESENT_AFTER_FLIP` turns it off). If the code leaves less idle time than
the presents' spacing (16.7 ms at 60), a longer delay cannot help and the flip's time is kept. Together (orb route):
lateness p99 2.1 ms (the sleep's wake-up, about the same for every present), intervals p1 15.1, p99 18.4 ms.

### 4.2 Overlay swaps (a quarter-second stall and an audio gap at every load; reduced)

The menus run in `OVL_MENU_APP`, the world map and battles in `OVL_BATTLE_APP`, loaded into one window. A swap
unregistered the old overlay's 390,000 registrations (a walk over the whole 850,000-entry table, an erase and a freed
name each) and registered the new one's: 210-305 ms frames, every time, sampled as `unregister_functions` and hash-table
inserts. The audio device is kept only 46 ms ahead (`audio_sink.cpp`, 2,048 frames), so a stall that long is a gap.

Fix (`Runtime::park_functions`/`restore_functions`, `D012_NO_OVERLAY_PARKING`): the replaced overlay's registrations
are moved out as node handles and back when it returns; ranges are cleared address by address; the table is reserved
before a corpus registers. Measured (`[overlay]` lines): menu replaced, dropped 60-70 ms → parked 29-33 ms; a parked
overlay coming back 37 ms instead of a first install's 100-137 ms; so a battle → menus swap is 70 ms instead of about
180 (and 250-300 before the reserve and the sweep). A first install still costs 100-137 ms (the names are copied
strings); with the game's own loading work after it the longest present gap at a battle load is still 165-172 ms (one
or two seconds under 55, before and after).

### 4.3 HLE copies a byte at a time (0.7 ms a frame; fixed)

`sceKernelMemcpy`, `sceKernelMemset` and `sceDmacMemcpy` went through `load8`/`store8` per byte. One host
`memmove`/`memset` when both runs are contiguous (`D012_NO_FAST_HLE_COPIES`): guest time 7.2-7.3 → 6.5-6.6 ms a frame
in 3D scenes, p99 9.6-9.9 → 7.8-8.2 ms. On a CPU half as fast (the Deck) that is 1.5 ms a frame.

### 4.4 Log writes and slow disks (not a code fix)

Every `[perf]`/trace line is flushed from the emulation thread; on the busy SMR drive one write blocked for up to 2.1 s
(the meter read 10 that second). A normal run prints little, but the program is a console application: on Windows a
console window that has text selected (QuickEdit) blocks every write, and the whole game with it.

### 4.5 Pipelines (not a cause on the PC)

Cold: 45-50 pipelines created per route, 2.3-2.9 ms in all, 0.1 ms the longest (39 of them in the first battle
frame). Warm: none created; the prewarm made the 151 known keys in the background in 5.5 ms, and 42-57 of them were
taken from it. The cache is saved 3 s after a burst and the keys go to `pipeline_keys.bin`. Cold vs warm made no
difference to any statistic. On the Deck the first run compiles for real (section 7).

### 4.6 Texture cache and hashing (not a cause)

Texture-key hashing, whole-palette (0038) and every-byte transfer keys (0042) included: 0.03-0.10 ms a frame. Decodes:
113 at a battle load (36 ms of CPU in all, decoded in the background where possible), 146 over the menus, under 2 ms in
any one frame.

### 4.7 Snapshot copies and the GPU (not a cause)

1.8-2.0 snapshots a frame while interpolating, about 7.5-8.2 MB a frame at 4x (2 MB at 2x). GPU time a game frame,
replays included: 1.2-3.5 ms mean, 4.6 ms p99 at 4x on the 9070 XT.

### 4.8 Interpolation refusals, the governor, missed presents (not a cause)

Pairing matched 99-100% of draws; cuts were only at scene changes (`no frame before`, `not recorded`). In every PC run:
no governor step, no present dropped for a busy display or over budget, no `skipped` presents outside loads.

## 5. Fixes and switches

| Commit (branch `perf`) | Change | Off switch |
| --- | --- | --- |
| `c46230d` Perf: a per-frame CSV and a stall sampler | `D012_PERF_CSV`, `D012_TRACE_STALL_SAMPLES` | off unless set |
| `943a3e2` Runtime: an overlay swapped out keeps its registrations | park/restore, sweep, reserve, timed `[overlay]` lines | `D012_NO_OVERLAY_PARKING` |
| `c0c7ab3` HLE: memcpy, memset and DMA copies in one host copy | | `D012_NO_FAST_HLE_COPIES` |
| `1fce2fa` Frame pacing: presents between flips fall in the idle time | `note_idle`; presents polled at starvation boundaries | `D012_PRESENT_AFTER_FLIP`, `D012_NO_CODE_PRESENT_POLL` |

The cost of 4.1(b) is latency: the interpolation delay went from 19.6 ms to 29.0 ms (`[interp] delay 29.0 ms (code
12.4 ms)`), on top of the frame itself. Who prefers the old pacing sets `D012_PRESENT_AFTER_FLIP=1`. The real fix for
both, presents on a thread of their own, is section 9.

## 6. Before and after (PC)

Owner's settings: internal scale 4, 60 fps, auto rate on, vsync (FIFO, 280 Hz), warm cache. Each cell: mean fps /
1% low fps / seconds under 55 / presents over 25 ms apart / p99 interval ms / blended share.

| Route | Scene | Before | After |
| --- | --- | --- | --- |
| menus | load_save | 59.9 / 50.9 / 0 / 3 / 19.6 / 0.00 | 60.0 / 53.8 / 0 / 3 / 18.6 / 0.00 |
| menus | main_menu | 60.1 / 51.1 / 0 / 0 / 19.6 / 0.00 | 60.1 / 55.1 / 0 / 0 / 18.1 / 0.00 |
| menus | battle_mode | 60.0 / 53.3 / 0 / 0 / 18.8 / 0.00 | 60.0 / 54.6 / 0 / 0 / 18.3 / 0.00 |
| menus | character_select | 59.9 / 51.8 / 0 / 1 / 19.3 / 0.00 | 59.9 / 54.9 / 0 / 1 / 18.2 / 0.00 |
| orb | orb_walk | 60.0 / 45.1 / 0 / 1 / 22.2 / 0.50 | 59.9 / 54.5 / 0 / 1 / 18.3 / 0.50 |
| orb | orb_absorb | 60.0 / 45.4 / 0 / 0 / 22.0 / 0.50 | 60.0 / 54.5 / 0 / 0 / 18.3 / 0.50 |
| orb | orb_after | 60.0 / 45.4 / 0 / 0 / 22.0 / 0.50 | 60.0 / 54.5 / 0 / 0 / 18.3 / 0.50 |
| qb_ex_burst | versus | 60.0 / 54.7 / 0 / 1 / 18.3 / 0.00 | 60.1 / 54.8 / 0 / 1 / 18.2 / 0.00 |
| qb_ex_burst | loading | 59.0 / 51.4 / 2 / 9 / 19.5 / 0.96 | 59.0 / 54.2 / 2 / 10 / 18.5 / 0.96 |
| qb_ex_burst | battle | 60.2 / 52.8 / 0 / 0 / 18.9 / 1.00 | 60.2 / 55.2 / 0 / 0 / 18.1 / 1.00 |
| qb_ex_burst | ex_burst | 60.0 / 48.1 / 0 / 0 / 20.8 / 1.00 | 60.0 / 53.3 / 0 / 0 / 18.8 / 1.00 |
| qb_kuja | loading | 59.0 / 44.6 / 2 / 9 / 22.4 / 0.48 | 59.0 / 53.1 / 2 / 9 / 18.8 / 0.48 |
| qb_kuja | battle | 60.2 / 45.0 / 0 / 0 / 22.2 / 0.50 | 60.2 / 54.7 / 0 / 0 / 18.3 / 0.50 |
| qb_kuja | ex_mode | 60.1 / 44.6 / 0 / 0 / 22.4 / 0.50 | 60.2 / 54.6 / 0 / 0 / 18.3 / 0.50 |
| qb_kuja | ex_burst | 60.0 / 45.1 / 0 / 0 / 22.2 / 0.50 | 60.0 / 54.3 / 0 / 0 / 18.4 / 0.50 |
| worldmap | continue_load | 53.9 / 17.5 / 1 / 10 / 57.1 / 0.05 | 54.4 / 19.5 / 1 / 10 / 51.3 / 0.05 |
| worldmap | worldmap | 59.8 / 45.5 / 1 / 2 / 22.0 / 0.50 | 60.0 / 54.3 / 0 / 0 / 18.4 / 0.50 |
| worldmap | gate_transition | 60.1 / 46.0 / 0 / 0 / 21.7 / 0.50 | 60.2 / 54.9 / 0 / 0 / 18.2 / 0.50 |
| worldmap | gateway_board | 60.5 / 44.9 / 0 / 0 / 22.3 / 0.51 | 60.5 / 55.2 / 0 / 0 / 18.1 / 0.51 |

Whole routes in the other conditions (same cells):

| Route | Cold before | Cold after | Immediate before | Immediate after | 2x before | 2x after |
| --- | --- | --- | --- | --- | --- | --- |
| menus | 59.9 / 53.0 / 0 / 5 / 18.9 | 59.9 / 54.5 / 0 / 4 / 18.3 | 59.9 / 52.4 / 0 / 4 / 19.1 | 59.9 / 54.2 / 0 / 3 / 18.5 | | |
| orb | 59.0 / 45.4 / 2 / 4 / 22.0 | 59.9 / 54.6 / 0 / 1 / 18.3 | 60.0 / 45.3 / 0 / 0 / 22.1 | 59.9 / 54.7 / 0 / 1 / 18.3 | 59.9 / 46.0 / 0 / 1 / 21.7 | 59.9 / 54.1 / 0 / 2 / 18.5 |
| qb_ex_burst | 59.5 / 52.8 / 2 / 9 / 18.9 | 59.5 / 53.9 / 2 / 10 / 18.6 | 59.5 / 52.6 / 2 / 10 / 19.0 | 59.5 / 54.1 / 2 / 12 / 18.5 | 59.5 / 53.2 / 2 / 11 / 18.8 | 59.5 / 54.2 / 2 / 10 / 18.5 |
| qb_kuja | 59.5 / 45.0 / 2 / 10 / 22.2 | 59.5 / 54.1 / 2 / 10 / 18.5 | 59.5 / 45.1 / 2 / 11 / 22.2 | 59.5 / 54.0 / 2 / 10 / 18.5 | | |
| worldmap | 59.6 / 45.3 / 2 / 11 / 22.1 | 59.6 / 50.0 / 2 / 14 / 20.0 | 59.4 / 45.1 / 3 / 16 / 22.2 | 59.5 / 52.6 / 3 / 18 / 19.0 | | |

The remaining seconds under 55 are the loads (the first overlay install and the game's own loading code, 130-210 ms
frames), the same before and after. Audio: no seconds with a gap in any real-time scene but the loads (one or two
seconds each, before and after). In immediate mode the gate transition has a 1% low of about 30 both before and after
(three presents over 25 ms at the cut into the gateway), not looked into.

## 7. The Steam Deck (not measured: recommendations)

The Deck has a 60 Hz panel, a CPU about half as fast per thread, a GPU about a twentieth of the 9070 XT, Proton, and
a fresh pipeline cache on its first run. Nothing above was measured there. What follows from the PC measurements:

- **What the orbs most likely do there.** The world map is the heaviest CPU scene (the orb walk draws 1,700 GE draws a
  frame against 950 elsewhere; guest 6.5 + render 2.8 ms on the PC, so perhaps 18-22 ms on the Deck), and with
  interpolation's presents (0.5 ms each on the PC) the 33 ms frame has almost no spare time. Then the governor steps
  down ("presents were late" or "the game had no time to spare": 60 → 45, and 30 if it persists, the reading the owner
  sees), and any frame over 33 ms puts the game behind real time; the audio, mixed on the same thread and kept only 46 ms
  ahead, then runs dry: choppy music. Every stall in section 4 is about twice as long there: an overlay swap was 0.5 s
  and is now about 0.15 s (first install 0.2-0.3 s), and the HLE copy fix saves about 1.5 ms of every frame.
  To confirm on the Deck: `D012_PERF=log` plus `D012_TRACE_INTERPOLATION=1` (look for `[interp] frame rate 60 -> 45:`
  and its reason) and `D012_TRACE_AUDIO=1` (`silent=` counts underruns), at the orbs.
- **Settings.** Internal scale 2 (the panel is 1280x800; 4x is 1920x1088 of work on a GPU a twentieth the size);
  frame rate 60 with "Lower the frame rate when the game falls behind" on, so it drops to 45 rather than slowing the
  game and the music; vsync. If the in-game meter keeps showing 45 at the orbs with the new build, 30 (no
  interpolation) is the honest choice there: the game itself runs at 30. Steam's own frame limiter off (or 60) and the
  refresh rate at 60, so the two do not fight.
- **First run.** RADV compiles each pipeline in roughly 1-5 ms; the 39 of the first battle frame could then cost
  0.1-0.2 s once. `pipeline_cache.bin` is per device and driver (it is dropped on the Deck), but `pipeline_keys.bin`
  is not: copying the PC's `data/pipeline_keys.bin` into the Deck package's `data/` would let the first run make all
  151 known pipelines in the background at start-up (`prewarm_pipelines`). `package_deck.py` does not do that today.
- **Logs.** Start it without a console window or log capture, or at least never with text selected in a console.

## 8. Not done, and next steps

- Presents on a thread of their own: done, section 9.
- A first overlay install still copies 390,000 names (100-137 ms): keeping the corpus's static names as `const char*`
  in `FunctionEntry`, or registering both overlays at start-up on a worker, would remove it.
- The audio device's 46 ms lead (`kPrebufferFrames`): a larger lead hides stalls at the cost of audio latency; not
  changed.
- The world map's continue load (1% low 18-19: 165-205 ms frames in the game's own loading) and the immediate-mode
  gate transition were not looked into.
- Patches 0044-0047 for `port/portablekit-patches/` and the README's framework table are left for the merge (another
  branch, `water-fix`, may take 0044).

## 9. Presents on a thread of their own (22 October 2026)

> The "real fix" of section 5. PortableKit: branch `present-thread` in the worktree
> `tools/local/port/PortableKit-present` (from `dissidia` at `2b83ebe`, patches 0045-0048 included), not merged.
> Build: `tools/local/port/build-d012-present` (the options of `build-d012-recomp`); the unchanged `dissidia`
> build for the comparison was kept as `tools/local/port/build-d012-present-base`. Data: `build/port/data-present`
> (a copy of `build/port/data`; `ms0/` and `settings.ini` restored before every run). Runs, logs and statistics:
> `build/port/perf/runs/pt_*`, `build/port/present-robust`, `build/port/gfx-tour-present` (not committed).

### 9.1 Design

**Threads.** The emulation thread does what it did: it walks the display lists, records each game frame into its
frame slot's command buffer (with the texture cache, uploads, render-target snapshots, palette views, block
transfers and the write-back) and submits it at the flip. With frame interpolation the flip also copies the
frame's picture, keeps its recording (the draw summaries and replay groups) and matches it against the frame
before, as before. What changed is who presents. A thread of the renderer's own (`present_loop` in
`host/gpu/vulkan_renderer.cpp`) makes every present:

- with interpolation, the presents between flips, each at its due time on the grid of the present clock;
- without it (30 fps, a held keyboard frame, unthrottled, fast loading), the frame of a flip, as soon as the flip
  hands it over (thinned to the display's refresh rate; a fast load to one every 33 ms, as before);
- the interface's presents while the game is paused or not started (`present_ui`), which the emulation thread
  waits for (a quarter of a second at most), so the menu is paced by its presents as before.

The hand-off is a mailbox (`host/gpu/present_queue.hpp`): the flip posts, the present thread wakes. What it shows
is the renderer's state behind its lock: the two newest frames (their recordings, `older_frame`/`newer_frame`,
and pictures), replaced as a pair at each flip, so the present thread always pairs the two newest.

**Locks.** All of the renderer's state (`VulkanRenderer::Impl`) is behind one recursive mutex. The emulation
thread holds it for each call into the renderer (`Impl::Guard` in every entry point: a draw, a display list, a
transfer, the flip, the settings, the pump); the present thread while it records and submits one present. Between
two calls the emulation thread lets a waiting present thread in first (an atomic flag), so a present waits at most
for the one call in progress, never for the game's code. Presents interleaved with the game's draws before too
(`poll_presents` every 16 draws), so the renderer already kept a consistent state between calls. The queue has a
mutex of its own, always taken after the renderer's: every submit, `vkQueuePresentKHR`, queue and device idle wait,
and ImGui's font uploads (it submits from `RenderDrawData`), and the texture pack's uploads.

**What moved, and why:**

| Work | Where | Why |
| --- | --- | --- |
| Recording a game frame (draws, textures, uploads, snapshots, transfers, write-back) | emulation thread | It reads guest memory as the game's code leaves it at each draw; moving it would mean copying the GE state of every draw |
| Interpolation's replay (an in-between frame drawn again from the recording) | present thread, holding the renderer | It must run at the present's time with that time's blend fraction. It reads the two newest recordings (unchanged after their flip), writes its own blend target and its slot's scratch area of the vertex buffer, binds descriptor sets and records from the shared command pool: the lock covers the pool and the sets |
| Queue submission | both threads, one queue behind the queue mutex | One graphics queue (AMD on Windows, RADV): a second queue of the family is not guaranteed. Submission order then orders the GPU work as before: a frame's picture copy is submitted before any present that shows it, and a present before any frame recorded after it |
| Swapchain acquire, present, rebuild | present thread only, without the renderer's lock | These are where vsync blocks; a display that keeps its images holds only the present thread. Acquire waits up to 100 ms (3 ms before, which dropped a present); a minimised window or a sleeping monitor gives the present up and the game runs on |
| Render-target snapshots (0033) | made by the emulation thread, read by replays | Unchanged lifetimes: a snapshot is reused only three flips after the last frame that drew with it, and the present thread replays only the older of the two newest frames. GPU order is submission order |
| Texture cache | emulation thread | A replay refuses an older frame that drew with a texture evicted since (`destroyed_texture_clock`, as before); evicted textures are destroyed after the fence of a frame submitted after every present that could draw them |
| Descriptor pools, pipelines | emulation thread (pipelines also the prewarm thread, as before) | The present thread creates neither; its blend targets and pictures are made under the lock |
| The interface (ImGui) | built by the emulation thread, drawn by the present thread | Its frame is built holding the renderer (`begin_ui_frame` to `set_ui_draw_data`), so a present draws the frame before or the next one, never one half built; the font atlas's uploads happen while drawing, under the queue mutex |
| SDL's window and display queries | emulation thread | The present thread uses the window size and refresh rate the last pump saw |

**Lifetimes and the rest.** A present slot's command buffer, scratch area and overlay staging are reused after
its fence. A resize, a scale or aspect change, or `pause_interpolation` drops the frames under the lock; a present
that finds them gone shows the newest picture (or the target shown) instead, since its image is already acquired.
Captures (`shot`, `<prefix>_CAPTURE_BLENDED`) are recorded into the next present the present thread makes and
written after it; the input script still runs at the flips. The stall sampler's `=present` mode samples the
emulation thread and so applies only without the present thread. Shutdown: the program stops the present thread
when the game has stopped (`stop_presenting()` at the end of `main`, also on an error), an exit handler stops it on
any other exit before the statics it uses go, and `shutdown()` stops it before anything else; the order of patch
0030 (render targets before the descriptor pool) is unchanged. Android keeps presents on the emulation thread
(its surface comes and goes with the app, which only that path handles).

### 9.2 Timing and latency

The present clock is the same (`frame_pacing.hpp`): a grid of moments at the presents' spacing aligned with the
frames' moments (the real time each flip's emulated time stands for), each shown a fixed delay later, blending
the two frames around it. The **delay** is the time from a frame's moment to the present that shows it as it is;
the in-between present for a moment halfway between two frames is made the same delay after that moment. It is
the frame time less the shortest step to the next grid moment (half a game frame at 60) plus the game's work from
a frame's moment to its flip (the 9th longest of the last 60) and 1 ms. On the emulation thread, patch 0048 added
the game's code after the flip to the work so that both presents fell in its idle time (29.0 ms); the present
thread needs no idle time, so the work ends at the flip again: 16.7 + 1.9 + 1.0 = 19.6 ms on the PC. That is less
than a game frame (33.4 ms), the least frame interpolation can do (the in-between frame needs the newer frame,
which exists only from its flip), plus scan-out. Camera-cut detection, pairing and the other rules are unchanged.

The present thread waits on the mailbox (a flip may move the grid) until 2 ms before a present is due, then sleeps
the rest and spins the last 0.15 ms without yielding: it wakes 0.2-0.3 ms late at most (unit test). On Windows both
waits use a high-resolution waitable timer (with the mailbox's event): a condition variable's timed wait follows the
system tick, up to 15.6 ms coarse; elsewhere they are the condition variable and the steady clock's sleep. It runs at SDL's high thread priority. With vsync (FIFO) each
present then waits in the display's queue for the next refresh; with vsync off (immediate) it is shown at once.
The governor (section 3) counts what presents cost the game: with the thread, only the emulation thread's waits
for the lock the present thread held.

### 9.3 Switches

| Switch | Presents | Pacing |
| --- | --- | --- |
| (none) | the present thread | delay to the flip's work (19.6 ms on the PC) |
| `D012_NO_PRESENT_THREAD=1` | the emulation thread, exactly as patch 0048 | idle time after the flip, polled in code (29 ms) |
| `D012_PRESENT_AFTER_FLIP=1` (with `D012_NO_CODE_PRESENT_POLL=1`: the whole old pacing) | the emulation thread, as before patch 0048 | the flip's work only (19.6 ms), presents up to 7 ms late |

Patch 0048's idle-time logic (`note_idle`) and the code poll do nothing while the thread is on. Test switches:
`D012_TEST_GUEST_SPIN_MS=N` (the emulation thread spins N ms after every flip, polling for due presents every
millisecond as the code poll would) and the input script's `window fullscreen|windowed|minimize|restore|scale N`.
With `D012_PERF=log` or `D012_TRACE_INTERPOLATION` the present thread prints a `[present-thread]` line a second.

### 9.4 Measurements (PC)

Method as in section 2 (`perf_tour.py`, now with `--data-dir`, `--affinity` and `--tag`; it also reads the
`[present-thread]` lines, the `[interp]` delay and the emulation thread's CPU time from the CSV). Owner's settings:
60 fps, auto rate on, warm cache; vsync is FIFO on the 280 Hz monitor (awake throughout: no run logged a display that
took no image, and no present interval over a second). Four configurations, each the same routes and scenes:

- **0048**: the `dissidia` build as it was (patches 0045-0048), presents on the emulation thread in its idle time;
- **old**: the same build with `D012_PRESENT_AFTER_FLIP=1 D012_NO_CODE_PRESENT_POLL=1`, the pacing before 0048;
- **thread**: this branch, vsync;
- **thread, immediate**: this branch, vsync off (`--present immediate`).

`D012_NO_PRESENT_THREAD=1` on this branch measured as 0048 does (orb: p99 18.2 ms, 1% low 54.8, delay 28.4 ms;
battle route: 18.4 ms, 54.5, 28.2 ms). The thread columns are from the build before `81bfc74`, which removed one
early present in a hundred (section 9.8); the orb and battle routes measured again after it are unchanged but for
that (orb: mean 16.68 ms, p99 17.3 ms, 0 presents over 20 ms, 1% low 57.8; battle route 16.68 ms, 17.4 ms, 57.4).

Each cell: mean time between presents ms / 99th percentile ms / presents more than 20 ms after the one before /
1% low fps. Internal scale 4:

| Route | Scene | 0048 | old | thread | thread, immediate |
| --- | --- | --- | --- | --- | --- |
| orb | orb_walk | 16.71 / 18.2 / 1 / 54.8 | 16.69 / 21.7 / 39 / 46.1 | 16.63 / 17.4 / 1 / 57.5 | 16.58 / 17.3 / 1 / 57.7 |
| orb | orb_absorb | 16.68 / 18.3 / 0 / 54.6 | 16.68 / 21.6 / 25 / 46.3 | 16.63 / 17.4 / 0 / 57.5 | 16.63 / 17.4 / 0 / 57.4 |
| orb | orb_after | 16.68 / 18.3 / 0 / 54.7 | 16.68 / 21.3 / 80 / 47.0 | 16.53 / 17.4 / 0 / 57.5 | 16.63 / 17.4 / 0 / 57.3 |
| menus | main_menu | 16.68 / 18.4 / 0 / 54.4 | 16.68 / 18.7 / 0 / 53.5 | 16.68 / 17.3 / 0 / 57.7 | 16.48 / 17.2 / 0 / 58.1 |
| menus | character_select | 16.72 / 18.2 / 5 / 55.0 | 16.71 / 19.0 / 6 / 52.7 | 16.61 / 17.3 / 0 / 57.8 | 16.62 / 17.4 / 0 / 57.5 |
| qb_ex_burst | battle | 16.69 / 18.8 / 0 / 53.3 | 16.69 / 21.8 / 9 / 45.9 | 16.55 / 17.4 / 0 / 57.4 | 16.55 / 17.3 / 0 / 57.7 |
| qb_ex_burst | ex_burst | 16.68 / 18.3 / 0 / 54.6 | 16.68 / 22.2 / 23 / 45.1 | 16.54 / 17.3 / 0 / 57.7 | 16.61 / 17.4 / 0 / 57.4 |
| worldmap | worldmap | 16.71 / 18.3 / 2 / 54.6 | 16.70 / 21.5 / 191 / 46.5 | 16.57 / 17.4 / 0 / 57.4 | 16.62 / 17.4 / 0 / 57.5 |
| worldmap | gate_transition | 17.30 / 40.3 / 4 / 24.8 | 17.00 / 34.5 / 3 / 29.0 | 16.93 / 35.4 / 3 / 28.2 | 16.86 / 37.8 / 3 / 26.5 |

The gate transition (3.6 s) has three or four intervals of 40-70 ms at the cut into the gateway, the game's own frames,
in every configuration. Internal scale 2, whole routes (real-time scenes):

| Route | 0048 | old | thread | thread, immediate |
| --- | --- | --- | --- | --- |
| orb | 16.69 / 18.3 / 1 / 54.7 | 16.68 / 21.1 / 119 / 47.3 | 16.68 / 17.3 / 0 / 57.8 | 16.64 / 17.5 / 1 / 57.2 |
| menus | 16.70 / 18.3 / 5 / 54.6 | 16.70 / 18.7 / 6 / 53.5 | 16.59 / 17.3 / 1 / 57.9 | 16.59 / 17.3 / 1 / 57.7 |
| qb_ex_burst | 16.82 / 18.6 / 18 / 53.8 | 16.81 / 21.7 / 175 / 46.2 | 16.71 / 17.5 / 11 / 57.2 | 16.70 / 17.5 / 11 / 57.3 |
| worldmap | 16.78 / 18.5 / 16 / 54.0 | 16.81 / 21.7 / 221 / 46.1 | 16.69 / 17.4 / 21 / 57.3 | 16.70 / 17.5 / 15 / 57.2 |

(The routes' presents over 20 ms are their loads: the versus and loading screens, the continue load, the gate.)

**Latency.** The delay as defined in section 9.2, the median of the `[interp]` lines over the real-time scenes, the
same at 4x and 2x: 0048 28.0-28.6 ms (22.4-22.9 in the menus, where little code follows the flip); old 19.4 ms; thread
19.2-19.3 ms, vsync or not. How late each present is against its due time (99th percentile): 0048 2.0-2.2 ms, old
5.5-6.4 ms, thread 0.6-0.7 ms (at most 1.0-2.0 ms).

**CPU per thread** (ms of CPU per second of real time, 4x; the clock is Windows' 15.6 ms thread-time tick, averaged
over 30-80 s):

| | orb | battle route | world map | menus |
| --- | --- | --- | --- | --- |
| emulation thread, 0048 | 302 | 295 | 286 | 127 |
| emulation thread, old | 308 | 313 | 273 | 132 |
| emulation thread, thread | 289 | 281 | 266 | 113 |
| present thread | 22.5 | 20.5 | 22.0 | 18.2 |
| emulation thread waiting for the present thread's lock | 7.5 | 6.6 | 6.5 | 0.5 |

The present thread uses about 2% of a core (its waits for the renderer's lock: 0.3-0.9 ms a second, at most 0.5-1.1
ms at once; for an image: 5-6 ms a second, at most 0.3 ms under FIFO at 280 Hz). The emulation thread saves its
presents (about 13 ms a second) and waits about 7 ms a second for the lock, which is its new cost of interpolation and
what the governor now counts.

### 9.5 A slower CPU

The Deck's world map is CPU-bound. Simulated here with `D012_TEST_GUEST_SPIN_MS=N` (the emulation thread spins N ms
after every flip at real time, polling for due presents every millisecond, which is generous to 0048: real code
polls only at its starvation boundaries) and the process limited to two logical CPUs on two cores
(`perf_tour.py --affinity 0x5`), internal scale 2, the orb route (orb walk, absorb, after; whole route, cells as
above, plus how late the presents were, 99th percentile):

| Guest time a frame | 0048 | old | thread |
| --- | --- | --- | --- |
| 18.5 ms (spin 12) | 16.69 / 19.9 / 25 / 50.4, 3.7 ms late | 16.68 / 26.1 / 1706 / 38.4, 11.9 ms late | 16.53 / 17.3 / 1 / 57.8, 0.6 ms late |
| 25 ms (spin 18) | 16.69 / 17.9 / 1 / 55.7, 1.0 ms late | 22.41 / 35.2 / 1693 / 28.4, 17.7 ms late, governor 60 → 45 twice | 16.54 / 17.3 / 3 / 57.7, 0.5 ms late |

With 18.5 ms of guest time 0048's idle-time pacing cannot place both presents in the idle time (less than 16.7 ms of
it), keeps the flip's time and makes the presents in the code: one interval in a hundred is 20 ms. At 25 ms the
idle time is too short for anything to wait for, and 0048's 1 ms polls carry it; the old pacing falls apart and the
governor steps down. The present thread keeps 17.3 ms at the 99th percentile in both, with the game's code taking
three quarters of the frame and two CPUs for everything. A first version of the thread yielded the CPU in the last
quarter millisecond of its wait; with two CPUs to share that handed a time slice to the game's thread and made
presents 3-5 ms late (99th percentile 19.9 ms at spin 12); it now spins those 150 µs (`a1d1c22`).

### 9.6 Robustness

All on the final build (`81bfc74`), `PSPRECOMP_NO_INTERPRETER=1`: no run logged an interpreter fallback or a missing
recompiled function, and every log says `[render] presents on a thread of their own`.

- **Quitting**: `check_quit_crash.py --build-dir tools/local/port/build-d012-present` (data
  `build/port/data-present-quit`): 20 runs in a row to a Quick Battle and the script's quit, camera on and off
  alternately: 20 clean (exit 0). Four quits from the port's menu (Esc, System, Quit game: the paused menu's presents
  are the interface's, made by the thread while the emulation thread waits) and four by `WM_CLOSE` to the window: all
  clean.
- **Window changes** (input script `window` steps on the world map at real time, then Esc into the paused menu and
  out): fullscreen (2560x1440) and back, minimise and restore, window scale 2 and 4. Captures after each are the same
  as those of the same script with `D012_NO_PRESENT_THREAD=1`, the swapchain is rebuilt once per change, and the game
  ran on while minimised. Without the thread the minimised window rebuilt the swapchain 300 times (every present
  between flips; as before, untouched): the present thread keeps the old swapchain while the surface has no area.
- **Graphics tour** (`gfx_tour.py --routes all --variants base,nointerp --data-dir build/port/data-present-gfx --out
  build/port/gfx-tour-present`, 24 runs, 348 captures): the replay checks pass (colour differs in at most 0.19% of
  pixels, Old Chaos Shrine EX Burst; 0.00% elsewhere; the earlier tours measured 0.02-0.23% there and the same
  Empyreal Paradox alpha-only difference, the stencil's). Every capture of the routes with a fixed timeline (menus,
  the story gateway, Lightning's RAV Water) matches the baseline (0 of 128 flagged); the battles' flags (138 of 196)
  are another timeline of the fight (the guest clock follows the wall clock), equally many with and without
  interpolation, and the flagged pictures are whole and correct when looked at.
- **Framework**: `tools/port/pk_ctest.cmd tools/local/port/build-framework-present`: 21 tests pass (the new
  `portablekit_present_queue_tests` included; ad hoc excluded); `portablekit_host_check` builds.
- **Unattended vsync**: the monitor stayed awake. A display that keeps every image (a sleeping monitor under FIFO)
  now holds only the present thread: its acquire gives up after 100 ms, the log says `[present] the display has
  taken no image for 1.0 s (vsync with the monitor asleep?); the game runs on without presents` and the runner's
  statistics would show presents missing rather than a slow game.

Not verified: the Steam Deck (Proton, RADV, a 60 Hz panel; nothing was deployed), Linux and macOS natively, Android
(which keeps presents on the emulation thread), a monitor actually asleep, `<prefix>_GPU_DECODE`, texture packs (their
uploads take the queue lock; no pack was loaded), and ad hoc play.

### 9.7 Risks

- **One lock for the renderer.** The emulation thread holds it for each renderer call, so a present waits for the call
  in progress: a pipeline compiled in a frame (1-5 ms each on RADV's first run, section 7), a texture decode waited
  for, a framebuffer read back for a block transfer (a GPU wait), a resize. On the PC the longest wait in the real-time
  scenes was 1.1 ms (once 63 ms, in a world-map route's load); a Deck's first run may show longer ones at a new
  effect. Releasing the lock around those waits is the next step if
  it does.
- **One queue.** The present's `vkQueuePresentKHR` holds the queue lock; a driver that blocks there under FIFO (not
  seen on AMD's Windows driver) would make the emulation thread's next submit wait. A second queue, where a device
  has one, would remove it.
- **Data races.** Every Impl field is touched under the lock; the review and about 90 runs found none, but no
  thread sanitizer was run (MSVC has none). The interface's frame is
  built holding the renderer (`begin_ui_frame` to `set_ui_draw_data`): a UI frame that called `begin_ui_frame` and
  never `set_ui_draw_data` would hold it; every caller pairs them.
- **Captures and CAPTURE_BLENDED** are taken from the next present the thread makes, so a `shot` lands up to a
  present later than before (the input script's steps still run at the flips). The stall sampler's `=present` mode
  samples the emulation thread and has nothing to find with the thread on.
- **Unthrottled** (Game speed: unlimited): vsync no longer paces the game (the present thread thins the flips to the
  refresh rate instead), so the game runs as fast as its code allows, as with vsync off before.

### 9.8 Commits (branch `present-thread`, not merged)

| Commit | Change |
| --- | --- |
| `3b47ed8` Perf: statistics safe to note from a second thread | the frame statistics, CSV and overlay behind one lock; the present thread's waits kept out of the game's frame time |
| `6fa810b` Renderer: presents on a thread of their own | the present thread, the renderer and queue locks, the mailbox, the switches, `stop_presenting`, the input script's `window` steps, `D012_TEST_GUEST_SPIN_MS` |
| `cc125c7` Tests: the present thread's hand-off and timing | `tests/present_queue_tests.cpp` |
| `a1d1c22` Present thread: the last stretch of a wait spun without yielding | 3-5 ms late presents with two CPUs |
| `81bfc74` Present thread: no present before its time when a flip moves the grid | one early present in a hundred |

The repository's tools changed with it (not committed): `run_d012.py --affinity`, `perf_tour.py --data-dir
--affinity --tag` and its new statistics, `gfx_tour.py --data-dir --out`, `check_quit_crash.py --build-dir`. The
patch files for `port/portablekit-patches/` and the README's framework table are left for the merge.

## 10. Hitches the first time after a boot: the disc image (26 October 2026)

The owner remembered the game hanging for a moment while things loaded, the last time they played (a while before),
and wondered whether it was shaders caching for both cameras. Measured on the PC (RX 9070 XT, disc image on G:, a 5400 rpm SMR hard
drive), with the versus route of `docs/port/local-versus.md` 7.9 at 60 fps, `D012_PERF_CSV` and `D012_TRACE_IO`, which
now gives each read's time and whether it was asynchronous.

- **The battle reads almost nothing from the disc.** The game loads a battle on its loading screen (`PACKAGE.BIN`,
  mostly asynchronous reads). In the fight only the music streams, about 48 KiB at a time.
- **The one stall with a cold file cache** is the battle's first music block: 96 KiB read synchronously as the battle
  starts, 85 ms from the cold drive, which made a 116 ms frame. A cold cache without a reboot: the image copied with
  unbuffered I/O (`robocopy /J`) and the copy's folder given as `D012_GAME_DIR`.
- **Shaders: not a cause here.** Without the port's pipeline cache a versus battle made 68 pipelines in 4 ms (section
  4.5). The driver's own shader cache was warm. AMD's driver has no switch to turn it off for one process, so a first
  run on a new machine or driver is not measured.
- **Longer stalls (0.3 to 2.5 s)** came only while the drive was still writing a fresh 881 MB copy of a data folder
  (section 4.4): writes from the port itself (its logs) are held back then. Not something a player would meet.

**The fix** (PortableKit `IsoImage::read_through_in_background`): from launch, a thread reads the whole image once and
throws the bytes away, so the system's file cache holds it by the time the game needs it. On Windows the thread runs
in background mode, which lowers its I/O priority below the game's own reads. It is not started for an image larger
than half the machine's memory. `D012_NO_DISC_READ_THROUGH` turns it off.

| Cold file cache | Without | With |
| --- | --- | --- |
| The battle's first music block | 85 ms | 11.5 ms |
| The first battle frame | 116 ms | 47 ms |
| Reads over 10 ms, the whole run | 19, 1.4 s in all | 17, 0.6 s in all |
| The read-through | | 1,673 MiB in 33.6 s |

The scripted route rushes through the menus, so the read-through ended just after the battle began. At normal speed,
the title screen and the menus take longer than that. Not checked: the Steam Deck (no lower I/O priority there; a
microSD card reads 1.7 GB in about 20 s), and how much it slows the first loading screens on a hard drive (the whole
run took 1.2 s longer).
