# The crash on quitting

State, 19 October 2026: **fixed** by framework patch 0030. Quitting no longer crashes, and a quit now exits with
0 (patch 0031).

V = seen in a run or a crash dump, I = inferred.

## 1. The symptom

- Some runs crashed after the input script's `quit`, once the game had stopped.
  - The earlier reports: about 1 in 15 runs at first, then 5 in 17 for one agent.
  - All five of those crashes had the right-stick camera on, and five runs with it off did not crash.
- Every run exited with code 4, clean or not.

## 2. The cause

`VulkanRenderer::shutdown` (PortableKit, `host/gpu/vulkan_renderer.cpp`) destroyed the descriptor pool, then the
render targets. `destroy_target` frees each sampled target's two copy descriptor sets with
`vkFreeDescriptorSets(device, descriptor_pool, ...)`. So every quit called the driver with a pool it had just
destroyed: a use-after-free.

- On the AMD driver (`amdvlk64.dll`), the first thing `vkFreeDescriptorSets` does is read the pool object.
  - Usually that memory is still mapped, and nothing visible happens.
  - Sometimes the driver has already returned it to the system, and the read is an access violation.
- It is in the renderer's destructor, which the C runtime runs among the atexit destructors after `main` returns.
- The order has been the same since PortableKit's upstream commits of 18 and 19 September. It is not one of this
  port's patches.

It is **not the camera**:

- Crashes happened with `D012_NO_RIGHT_STICK_CAMERA=1` (3 of the 4 measured here).
- No crash stack touches port code.
- The camera's statics (`Options`, `Pad` in `port/host/d012_camera.cpp`) are plain values with no destructors.
- Its hooks run only from guest code, which runs on the main thread inside `runtime.run()`, and that has returned
  before any teardown.

The earlier all-camera pattern was chance: 5 crashes against 5 clean camera-off runs, at a rate of about 8%.

## 3. Evidence

### The tools

**`tools/port/check_quit_crash.py`** is the loop. It runs the recompiled build N times through
`tools/port/run_d012.py`, at most 90 s each.

- Data is in `build/port/data-quit-test`, a copy of `build/port/data`.
- There are two routes:
  - `battle`: the camera doc's Quick Battle route. Stick pushes in battle, then a quit at frame 8820.
  - `worldmap`: Story Mode on the world map, a quit at frame 4560. This is where the earlier crashes were seen.
- `--camera on|off|both` sets the camera.
- There are three ways to quit:
  - `--quit script`: the script's `quit`.
  - `--quit window`: a `WM_CLOSE` to the started process's own window, found by its PID.
  - `--quit menu`: Esc, System, Quit game, Quit.

**`D012_CRASH_DUMP=<dir>`** (`port/host/d012_crash_report.cpp`) handles an unhandled exception.

- It writes `crash_<pid>.dmp`, a minidump of every thread, with DbgHelp, which is part of Windows.
- It logs the faulting stack with names.
- The recompiled build now links with `/DEBUG:FULL` (`CMAKE_EXE_LINKER_FLAGS_RELEASE`), so `D012Native.pdb` sits
  next to the executable.

Logs, dumps and summaries are in `build/port/quit-test/` (`before.txt`, `wm_before.txt`, `wmdiag_before.txt`,
`wm_after.txt`, `after.txt`, `logs/`, `dumps/`).

### The fault (V)

Run `before16_nocam`, with the camera off:

```
[crash] unhandled exception 0xc0000005 at 00007FFFF6AAC3DC in thread 57056 (read 0x19c54f47d40)
  #00 amdvlk64.dll!...+0x5eeac
  #01 vulkan-1.dll!vkResetEvent+0x59fdd        (the loader's trampoline; no symbols)
  #02 D012Native.exe!portablekit::gpu::VulkanRenderer::Impl::destroy_target+0x4f
  #03 D012Native.exe!portablekit::gpu::VulkanRenderer::shutdown+0x76c
  #04 D012Native.exe!portablekit::gpu::VulkanRenderer::~VulkanRenderer+0xe
  #05 D012Native.exe!`dynamic atexit destructor for ...'
  #06..#11 ucrtbase.dll (execute_onexit_table: exit())
  #12 D012Native.exe!__scrt_common_main_seh+0x173
```

The minidump (`dumps/crash_40664.dmp`) was read with `cdb -z`, from the Windows Kits already on the machine, with
only the local PDB on the symbol path.

- `destroy_target+0x4f` is the return address of its **only `vkFreeDescriptorSets` call**. It is called with
  `rdx = [this+0x8F68]`, the pool.
- The faulting instruction is `cmp qword ptr [rdx],rbp`, and `rdx = 0x19c54f47d40`. That is the pool handle, a
  pointer to the driver's own object, and it is unmapped (`????`).
- The other three crashes measured here (`wmbefore02_nocam`, `wmbefore10_nocam`, `wmdiag_before08_cam`) have the
  same exception and the same frames at the same offsets (`destroy_target+0x4f`, `shutdown+0x76c`). Each read a
  different pool address.
- None of the four logged a stall.
- The earlier `crash6` (`build/port/camera/logs/`) has the same functions.

### The use-after-free happens on every quit (V)

A temporary count was printed where the pool used to be destroyed: the copy descriptor sets still allocated in
render targets. It was 8 sets in 4 of the 5 targets in all six runs: world map and battle, camera on and off.

So every quit freed 8 sets from the destroyed pool. Whether that faulted depended only on the state of the driver's
heap.

### The shutdown order (V, from `host/main.cpp` and the dump)

1. **The scripted quit.**
   - It is an `SDL_EVENT_QUIT`, the same event as the window's close button.
   - `pump_events()` returns false in `present_frame`, which calls `rt.stop("window closed")`.
2. **`runtime.run()` returns on the main thread.**
   - Guest threads are contexts the kernel switches on that one host thread (`Kernel::switch_to`). They are not host
     threads, so no guest code runs after this.
   - No hook runs after this either: not the camera, the pad wrapper or the playlist.
3. **`main` finishes up.**
   - It calls `adhoc_shutdown()`, the thread report and the HLE histogram, then returns.
   - The C runtime then runs the static destructors.
   - The renderer is `MediaState::renderer`, a function-local static `unique_ptr`. `~VulkanRenderer` calls
     `shutdown()`:
     - `vkDeviceWaitIdle`;
     - the ImGui backend;
     - textures;
     - the prewarm thread, joined;
     - pipelines;
     - samplers;
     - **the descriptor pool**;
     - then, before the fix, **the targets**, where it faulted.
4. **The other threads at the fault, from the dump's `~*kn`.** None of them was in PortableKit's renderer or the
   port's code, except the stall watchdog.
   - The stall watchdog, asleep in `sleep_for`. It reads only atomics and is not involved.
   - SDL's WASAPI management, waiting. No audio callback thread was running (I: the `AudioSink` stream had already
     been destroyed with its static).
   - SDL's joystick and hotplug threads.
   - D3D12 and DXGI background threads, from the loader's probe of the second, Intel ICD.
   - OBS's `graphics-hook64` threads (an injected capture hook on this machine).
   - Thread-pool workers.

### Candidates checked and ruled out

| Candidate | Finding |
| --- | --- |
| Camera statics or hooks after teardown | Ruled out: crashes with the camera off; no port frame in any stack; trivially destructible statics; hooks only run inside `runtime.run()` |
| Guest thread running during renderer destruction | Ruled out: guest threads are not host threads; `runtime.run()` has returned |
| The quit path | It is the same SDL quit event as the window's close button; the crash is later, in exit-time destructors. The quit path explains exit code 4 (section 5) |
| Audio thread | Not running at the fault; no audio frame in any stack |
| Use-after-free in the Vulkan teardown | **The cause** |
| A stall | Not needed. The crashing runs here had no stall at the quit. A stall only delays the quit |

## 4. The fix

Patch 0030, PortableKit commit `ca541b3` on `dissidia` (`port/portablekit-patches/0030-*.patch`):

- In `VulkanRenderer::shutdown`, the targets (`targets`, `held`, the interpolation pictures and blend targets)
  are destroyed **before** `vkDestroyDescriptorPool`. Each target's descriptor sets are freed while the pool
  exists.
- The pool's handle is set to `VK_NULL_HANDLE` once it is destroyed.

This is game-agnostic. Any game whose render targets get sampled hit it.

## 5. Exit code 4

`main` returned `runtime.stop_reason().empty() ? 0 : 4`, and every way of ending a run sets a reason:

- `"window closed"`: the close button, the script's `quit`, or SIGTERM;
- `"quit from the menu"`;
- `"guest called sceKernelExitGame"`.

So every normal quit exited with 4, the code of an error stop. The desktop app reads it as "the game stopped with
the port's own exit code 4".

Patch 0031 (commit `0e99f01`) fixes it:

- The three reasons are now named constants in `host/system.hpp`.
- `is_requested_stop()` tells them from the other reasons.
- Those three exit with **0**. Any other stop (no runnable thread, an unmapped address, a missing import) still
  exits with 4.

## 6. Before and after

All runs below used the recompiled build, `PSPRECOMP_NO_INTERPRETER=1` and `D012_UNTHROTTLED=1`.

**Before** (the build at `ddf4a6f` plus the crash reporter):

| Runs | Camera on | Camera off |
| --- | --- | --- |
| Battle route, script quit | 0 of 10 crashed | 1 of 10 |
| World map route, script quit | 0 of 10 | 2 of 10 |
| World map, `D012_TRACE_CAMERA`, `D012_TRACE_STICK_CAMERA`, `D012_DECOMP_VERIFY` | 1 of 12 | |
| **All** | **1 of 32** | **3 of 20** |

That is 4 crashes in 52 runs, 7.7%. Every clean run exited with 4.

**After** (patches 0030 and 0031):

| Runs | Camera on | Camera off |
| --- | --- | --- |
| World map, script quit | **0 of 40 crashed**, all exit 0 | |
| Battle route, script quit | 0 of 10, all exit 0 | 0 of 10, all exit 0 |
| World map, window close button (`WM_CLOSE`) | 0 of 3, exit 0, "window closed" | |
| World map, in-game menu Quit game, Quit | 0 of 3, exit 0, "quit from the menu" | |

- That is 0 crashes in 66 runs, 56 of them with the camera on. At the measured 7.7%, 66 clean runs would happen by chance with probability about 0.5% (0.923^66).
- The cause no longer occurs at all: the descriptor sets are freed while the pool exists.

## 7. Reproduce

```powershell
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-recomp -j4
.\.venv\Scripts\python.exe tools/port/check_quit_crash.py --runs 20 --camera both --route worldmap
.\.venv\Scripts\python.exe tools/port/check_quit_crash.py --runs 20 --camera on --route battle
.\.venv\Scripts\python.exe tools/port/check_quit_crash.py --runs 3 --camera on --route worldmap --quit window
.\.venv\Scripts\python.exe tools/port/check_quit_crash.py --runs 3 --camera on --route worldmap --quit menu
```

Each run takes about 35 s on the world map and 55 s to the battle. Read a dump with
`cdb -z build\port\quit-test\dumps\crash_<pid>.dmp -y tools\local\port\build-d012-recomp\bin -c ".ecxr; kn; ~*kn; q"`.

There is another way to get dumps of any crash without the port's handler: Windows Error Reporting's LocalDumps
(`HKLM\SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps\D012Native.exe`, `DumpType=2`). It is a
registry change, so it was **not** made.
