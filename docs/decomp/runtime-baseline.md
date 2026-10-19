# Local packaging and first-screen runtime milestone

11 October 2026. **The rebuilt decrypted test ISO boots in PPSSPP and reaches
the game's first autosave notice.** The retail USA ISO and independently packaged
original-decrypted ISO reach the same checkpoint. Their final lossless RGB frames
are identical. Title, menus, battle, in-game save/load and data installation remain
unverified; the broader runtime milestone is incomplete.

## Evidence

All cases use PPSSPP **1.20.4 Windows x64**, Direct3D 11, fresh isolated memory
sticks, disabled remote services and no controller input. Each process remained
alive for its 30-second observation period and was stopped afterward. The final
frames were visually inspected: the initial autosave notice awaits cross/Enter.

| Case | Executable source | Decoded video frames | Initial notice |
| --- | --- | ---: | --- |
| Retail | Original encrypted executable in the pinned USA ISO | 883 | Pass |
| Original decrypted | Pinned decrypted main and original overlays | 882 | Pass |
| Rebuilt | Four current verified reconstruction outputs | 881 | Pass |

All final frames are **960 x 544**, encoded losslessly with FFV1, and have RGB
SHA256 `3416dd4be6d3d734f1a6c347b7cd350e92a4955d3ee6113643ce50086b5fce30`.
Recording lengths differ slightly with launch timing. PPSSPP's AVI timebase is
fixed near 60 fps while this checkpoint produces roughly 30 rendered frames per
second, so encoded timestamps are not elapsed wall time. We compare this stable
screen, not entire videos or synchronized gameplay.

The [machine-readable result](runtime-baseline.json) records session paths,
video/log hashes and explicit unrun scenarios. Local videos and sampled PNGs are
under `build/ULUS10566/runtime/sessions/{retail,original,rebuilt}/offline-boot-v1/`.
All game-derived media remain ignored by Git. Earlier diagnostic sessions are
retained separately and are not counted as passes.

The [package workflow](runtime-packaging.md) produced two **1,754,529,792-byte**
test ISOs with identical SHA256
`59ca11719bc1ca40c1756abca3cac06a9c50643ee83f294dab5ca08b3285b4a1`.
Only the listed executable extents and main directory-size fields change from
retail; all other bytes and LBAs are independently checked. The source USA ISO
still hashes to
`df94177d3e194e82800a574b50b61ee3c7be6caa877c8e82242a41af75d1ab6e`.

All four build proofs remain current and **105 checks pass**, including eleven new
synthetic packaging safety checks. Atomic output replacement also prevents a hard
link to another image from being truncated. The final suite ran outside the
Windows sandbox because its temporary directory blocks hard-link/rename checks.
Source recovery stays at **60 C functions /
3,244 bytes**. Neither pixel equality nor packaging increases that numerator,
establishes a complete feature ownership scope, or earns a feature rank 5.

## Reproduce

```powershell
.venv/Scripts/python.exe tools/prepare_runtime_packages.py
.venv/Scripts/python.exe -m pip install -r requirements-runtime.txt
# Each invocation creates a new session; do not reuse an existing RunId.
./tools/run_boot_check.ps1 -Case retail -RunId my-boot-check -Seconds 30
./tools/run_boot_check.ps1 -Case original -RunId my-boot-check -Seconds 30
./tools/run_boot_check.ps1 -Case rebuilt -RunId my-boot-check -Seconds 30
.venv/Scripts/python.exe tools/summarize_runtime_baseline.py --run-id my-boot-check
```

`run_boot_check.ps1` holds the owned process until recording and shutdown finish.
It checks process identity before stopping it. A launch alone earns no pass.
`inspect_runtime_video.py` decodes video and exports representative/final frames;
it does not infer gameplay success. The summary revalidates source/package hashes,
current build provenance and the specifically reviewed checkpoint. A changed
screen/configuration needs fresh inspection rather than silently updating its
expected hash. A future summary replaces the current report only after passing.

No game save was created or loaded. An autosave **notice** is not a save/load test.
No emulator save state was used. These captures are muted, so audio is untested.

## Unexpected debugger behavior and next step

**Subsequent update:** the user authorized administrator execution, and six scoped
firewall rules were applied and verified for the prepared debugger sessions. The
[protected runtime milestone](protected-runtime-milestone.md) records working
local input, rebuilt profile save/load and a battle demonstration. The historical
first-screen results below remain separate from those deeper checks.

In the pinned version, [NativeApp startup](https://github.com/hrydgard/ppsspp/blob/v1.20.4/UI/NativeApp.cpp#L738-L748)
calculates separate debugger/sharing flags but starts `WebServerFlags::ALL`.
The [server](https://github.com/hrydgard/ppsspp/blob/v1.20.4/Core/WebServer.cpp#L754-L793)
also registers its local address/port with PPSSPP's discovery service, and
[HTTPServer](https://github.com/hrydgard/ppsspp/blob/v1.20.4/Common/Net/HTTPServer.cpp#L173-L188)
binds all interfaces. `RemoteShareOnStartup=False` and
`RemoteDebuggerLocal=True` do not establish an isolated startup listener in these
paths. We kept both remote startup services disabled; the launcher refuses a
template that enables either.

Automated button injection and framebuffer requests need this debugger. Before
using them, restrict the copied session executable's inbound and outbound traffic
to loopback, or use a separately isolated environment. The current account lacks
administrator privileges needed to apply Windows firewall rules.

`tools/protect_runtime_session.ps1` prepares **two program-specific block rules**
for one copied emulator, excluding IPv4 and IPv6 loopback. It adds no allow rules,
does not change global firewall defaults, and never enables a debugger. Review the
exact executable, rule names and address ranges without administrator access:

```powershell
./tools/protect_runtime_session.ps1 -SessionRoot build/ULUS10566/runtime/sessions/rebuilt/offline-boot-v1 -WhatIf
```

If choosing this route, apply from an administrator PowerShell in the project
directory, while that emulator is stopped:

```powershell
./tools/protect_runtime_session.ps1 -SessionRoot build/ULUS10566/runtime/sessions/rebuilt/offline-boot-v1
# Remove only that session's rules later:
./tools/protect_runtime_session.ps1 -SessionRoot build/ULUS10566/runtime/sessions/rebuilt/offline-boot-v1 -Remove
```

Only syntax and the review path have been exercised; **no firewall rule has been
applied**. The helper is a proposed isolation step, not a certificate. Before
enabling remote services, verify effective active rules, loopback connectivity and
blocked external traffic. Fresh session copies require their own rules.
Then implement recorded screen-driven input, title/menu checkpoints, a first
battle, and an actual in-game save/reload comparison across the cases. Preserve
untested statuses until their evidence exists.
