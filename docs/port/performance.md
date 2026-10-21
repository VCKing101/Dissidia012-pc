# Performance: the 45 and 30 fps dips with frame interpolation

> 21 October 2026. The owner reported that with 60 fps frame interpolation the game "drops to 45 and 30 frames quite
> frequently", on the PC (i7-12700K, 32 GB, Radeon RX 9070 XT, a 280 Hz monitor) and on the Steam Deck (the Windows
> build through Proton); later, that the world map's "power of the dead" orbs slow the Deck down and make the music
> choppy. This is the measurement, the causes, the fixes (each with an off switch) and before/after numbers on the PC.
> The Deck was not touched; section 7 gives recommendations for it.
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
both, presents on a thread of their own, is a larger change to the renderer (section 8).

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

- Presents on a thread of their own (record the frame's two presents at the flip; acquire, submit and present on a
  present thread with the queue behind a lock): no latency cost, and it would hold 60 even where the game's code leaves
  less than 16.7 ms of idle time, which is the Deck's case. A larger change to `vulkan_renderer.cpp`.
- A first overlay install still copies 390,000 names (100-137 ms): keeping the corpus's static names as `const char*`
  in `FunctionEntry`, or registering both overlays at start-up on a worker, would remove it.
- The audio device's 46 ms lead (`kPrebufferFrames`): a larger lead hides stalls at the cost of audio latency; not
  changed.
- The world map's continue load (1% low 18-19: 165-205 ms frames in the game's own loading) and the immediate-mode
  gate transition were not looked into.
- Patches 0044-0047 for `port/portablekit-patches/` and the README's framework table are left for the merge (another
  branch, `water-fix`, may take 0044).
