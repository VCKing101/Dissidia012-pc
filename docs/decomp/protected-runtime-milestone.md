# Protected debugger and initial profile save/load

**Later correction/update:** [the three-case comparison](runtime-comparison-milestone.md)
adds reference-case save/load and live combat input. The COM label interpretation
below was incorrect: on Lightning it denotes a paradigm, so it does not prove
computer control. The earlier snapshots remain valid playback evidence.

11 October 2026. The administrator setup succeeded and the rebuilt test ISO now
has a working local debugger/input workflow. **Its title and profile setup run,
the initial profile saves and loads after a full process restart, and the beginner
battle demonstration renders.** The full three-case gameplay baseline remains
incomplete: these deeper checks have run only on the rebuilt package.

## Network isolation applied

The user explicitly authorized administrator execution. The reviewed helper ran
with an elevated Windows PowerShell process and installed **six rules**, one
inbound and one outbound per copied test executable. Each blocks non-loopback
addresses across all profiles. Global firewall defaults and the user's original
portable PPSSPP installation are unchanged.

The prepared cases are
`build/ULUS10566/runtime/sessions/{retail,original,rebuilt}/debugger-baseline-v1/`.
Effective ActiveStore rules, program paths, address/port/protocol filters,
profile settings and firewall services were inspected before launch. The emulator
itself ran as the normal user. IPv6 loopback `game.status` requests succeeded;
a connection from the WSL guest to the Windows gateway at port 58102 timed out
(`connect_errno=11`). This is one tested virtual-network path, not a test of every
external network. Outbound protection was verified through effective rules;
no separate external outbound probe was performed.

The rules remain in place for these exact copied executable paths. New session
copies need their own protection. `start_protected_runtime.ps1` checks live
effective filters every launch; it does not trust a saved success marker. It also
points the disc-share listing at a checked empty directory. The normal offline
launcher/template still keeps remote services disabled.

## Runtime evidence

The [machine-readable report](protected-runtime.json) records reviewed captures,
save hashes, network observations and remaining scenarios. Detailed local evidence
is under the rebuilt session's `evidence/` directory. Game-derived recordings,
screenshots and saves remain ignored by Git.

| Checkpoint | Observed result |
| --- | --- |
| Title | New Game / Load Game / Extras menu appears and responds to input |
| New profile | Create New Data and configuration menus work |
| Settings | PPSSPP nickname, Casual plan, Monday bonus, Action-style, Standard story, English subtitles; no save transfer |
| Save | Game displays “Save completed”; creates `ULUS10566GameData00` |
| Tutorial | Lightning and an opponent render in the beginner battle demonstration |
| Reload | After stopping/restarting PPSSPP, Load Game lists the profile, displays “Load completed” and resumes the opening scene |

The tutorial capture shows **COM** labels. The original inference that both
fighters were computer-controlled was unsupported and has been corrected above.
That capture alone does not verify player-controlled combat or battle completion.
The post-load opening scene is not a verified main-mode menu. Further scenario
names and rankings must retain those distinctions.

The real PSP savedata contains `DISSIDIA.BIN` (257,240 bytes), `ICON0.PNG`
(20,867 bytes) and `PARAM.SFO` (4,912 bytes). All three file contents are unchanged
after the load check. HLE logs separately show the save write and subsequent
LISTLOAD/file read. No emulator save state was used, and no existing user save
was read or modified.

During this run the debugger's output and render-color requests failed with
`Could not download output`. The client falls back to the last complete FFV1
frame from PPSSPP's local AVI recorder. It decodes from the last keyframe and
restores the prior CPU stepping state even when readback fails. A recorded frame
may precede the CPU pause slightly; it is not an exact instruction/frame snapshot.
These observations establish semantic checkpoints, not synchronized pixel
comparisons across gameplay runs. Audio remains untested.

**108 checks pass**, including lossless final-frame decoding and CPU-state
restoration after failed readback. Runtime dependencies are optional and pinned
in `requirements-runtime.txt`; their three tests skip when those dependencies are
not installed. All four static build proofs remain current. Matching C remains
**60 functions / 3,244 bytes**. Feature ranks and whole-game percentages do not
increase from this runtime work.

Both owned emulator runs are stopped. The disposable profile, recordings and
firewall rules are retained for subsequent comparisons.
Shutdown also disables remote startup in the owned session config. The protected
launcher refuses an administrator shell and requires live isolation checks to
enable the debugger again.

## Continue and reproduce

Install `requirements-runtime.txt` in the project environment. Prepare fresh
offline sessions with `start_runtime.ps1 -PrepareOnly -CaptureVideo`, then apply
the per-session firewall helper as administrator. The batch administrator helper
`apply_runtime_isolation.ps1 -RunId <id>` applies/verifies the three prepared cases
and records its result locally. It never launches an administrator emulator.

For the already protected rebuilt case, from the project directory:

```powershell
./tools/start_protected_runtime.ps1 -SessionRoot build/ULUS10566/runtime/sessions/rebuilt/debugger-baseline-v1 -HoldSeconds 1800
# In another terminal while that owned process remains running:
.venv/Scripts/python.exe tools/runtime_debugger.py build/ULUS10566/runtime/sessions/rebuilt/debugger-baseline-v1 --name unique-checkpoint --wait 3
# Add --button cross, circle, start, up, down, etc. after inspecting the screen.
./tools/stop_runtime.ps1 -SessionRoot build/ULUS10566/runtime/sessions/rebuilt/debugger-baseline-v1
```

Capture names must be new. Inspect screens before selecting actions; elapsed
time alone does not establish a checkpoint. Preserve session/log evidence before
reusing a run. The isolated save now exists, so title defaults to Load Game;
fresh profile-creation comparisons need fresh sessions or explicitly documented
test-save seeds.

Next: repeat the title/profile/save-load route on retail and original-decrypted
packages, then verify interactive combat and a completed battle with a maintained
scenario script. Retain the prior [three-case first-screen comparison](runtime-baseline.md)
as a separate checkpoint. To remove these exact session rules later, use
`protect_runtime_session.ps1 -SessionRoot <owned-session> -Remove` as administrator.
