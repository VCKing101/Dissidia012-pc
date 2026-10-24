# Phase 1 bring-up: the title screen under PortableKit's interpreter

> Log of 18 October 2026. Phase 1, step 1 of the [plan](README.md): a first PortableKit profile for ULUS10566,
> run under the interpreter with no recompiled code, as far as the title screen. PortableKit `33800c4` plus three
> local commits on the clone's branch `dissidia`. Windows 11, MSVC 19.44, Vulkan on an AMD Radeon RX 9070 XT.

Conventions as in the other port notes: "V:" verified (by a run, a trace or a capture), "I:" inference.

---

## 0. Summary

- **The title screen draws.** V: the game boots from `module_start`, shows the Square Enix logo, the credits page
  and the logo animation, and reaches the title screen with NEW GAME / LOAD GAME / EXTRAS at about frame 420
  (14 s), then stays there. 30 fps, emulation speed 100%, about 6 ms of guest time per frame, all of it
  interpreted. Captures: `build/port/title_screen.png` (frame 1500 of the last run) and `build/port/boot_logo.png`
  (frame 60).
- **What it took.** The profile (constants), three small framework fixes (entry point, a Windows build quoting
  bug, GE colour doubling), and four profile additions: `LIBSUPPREACC.PRX` loaded for real, and answers for
  `sceKernelGetGPI` and three libfont calls. No stubbed call the game made blocked it.
- **Run with nothing stubbed.** V: in the last run the game hit no logging stub. 21 of its 296 imports are still
  unimplemented; none was called before the title screen.
- **Next blockers**, ranked in section 7: no corpus yet; message dialogs answered without being shown (a risk for
  the save work); unparsed GE state (stencil and others); overlay identity for battle and extra not measured.

---

## 1. What was built

| Piece | Where | Notes |
| --- | --- | --- |
| Port tree | `port/` | `CMakeLists.txt`, profile, hooks, `config/nids.csv`, `README.md` with build and run steps |
| Build | `tools/local/port/build-d012` | Through `tools/port/pk_cmake.cmd`; configure reports `D012Native: Vulkan renderer enabled` and the bundled FFmpeg 7.1.5. The wrapper needed no change |
| Game directory | `build/port/game` | `EBOOT.ELF` copied and hash-checked, `disc.iso` a hard link to the retail ISO (read-only use), empty `ms0/` (`tools/port/prepare_game_dir.py`) |
| Data directory | `build/port/data` | `D012_DATA_DIR`; holds only the pipeline cache after these runs |
| Runner | `tools/port/run_d012.py` | Bounded runs (60 s cap), stops only the process it started, log to `build/port/logs/` |
| Overlay metadata | `tools/port/overlay_meta.py` | `meta.txt` for the `0xFFA8` overlay ELFs; `--check` against a window dump; `--recompile` runs `psp_recomp` (not run) |

The profile's `PROFILE_DIR` is `build/port` (cache variable `D012_WORK_DIR`), so PortableKit's `generated/`,
`overlays/` and `game/` all land under the ignored `build/`, never in `port/`. The profile sources are passed
to `portablekit_add_game()` as absolute paths, and `port/config/nids.csv` is copied to `build/port/config/` at
configure time, where PortableKit looks for it.

Profile values (all read from the game's files): disc id `ULUS10566`; `EBOOT.BIN` SHA-256 `8fbf3541…f3394`;
`EBOOT.ELF` SHA-256 `0bc8e779…fe6d`; tag `0xD91613F0` (word at `0xD0` of `EBOOT.BIN`), key left empty because
the decrypted ELF is supplied directly (since 24 October 2026 the profile carries the public key, and `--install`
decrypts the disc's EBOOT); load base `0x08804000`; 32 MiB; overlay window `0x089DD4C0..0x08B62F70`;
save folders `ULUS10566GameData00`/`01`; English, confirm on cross; `frame_vblanks` 2.

---

## 2. Known blockers from the gap analysis

| Blocker | Resolution |
| --- | --- |
| `EBOOT.ELF` has `e_entry` 0 | Framework patch 0001: `runtime_entry()` reads the `module_start` export from the module info when `e_entry` is 0. V: `Entry: 0x08804040` |
| `LIBSUPPREACC.PRX` never loaded | Profile hook (`port/host/d012_prx.cpp`): for this one path, `sceKernelLoadModule` loads the PRX into a user-partition block (`0x09B81000`, 18,032 bytes, 656 relocations), binds its 26 imports to PortableKit's HLE, and reads its 13 `scesupPreAcc` exports. Main's 8 imports of that library jump to the exports. It has no `module_start` (`e_entry` is -1). `D012_NO_SUPPREACC=1` restores the stubs |
| Overlays need a `meta.txt` | `tools/port/overlay_meta.py` (section 6). Not needed under the interpreter, which runs the overlays as loaded |

V: with the PRX loaded, `0x86DEBD66` and `0x13AE25B3` return 0 and `0x348BA3E2` returns `0x80010002`, after the
PRX's own `sceIoGetstat("ms0:/PSP/SAVEDATA/ULUS10566Data")` finds no folder. With the stubs, the third call
answered 0 and the game went on to open `ms0:/PSP/SAVEDATA/ULUS10566Data/PARAM.SFO`. I: the PRX is a savedata
pre-access helper (its strings: `PREAC_MSCHK`, `PREAC_POWER`, `ms0:/PSP/SAVEDATA`, `ICON0.PNG`, `PARAM.SFO`),
so it matters for the save work more than for boot. Its code runs interpreted; it uses no `$gp`-relative
accesses, so not switching `gp` on the call is safe.

---

## 3. How far the game got

| Measure | Value |
| --- | --- |
| Functions registered | 300 host functions (the 296 bound import stubs and the kernel's own); no recompiled code |
| Interpreted | 207 million instructions in a 52 s run (virtual time), hottest at `main:0x08935554` (a wait loop) |
| Imports | 296; 549 HLE functions registered by PortableKit and the profile; 21 logging stubs, none called |
| HLE functions called | 90 distinct, 71,896 calls in the last run (`PSPRECOMP_HLE_HISTOGRAM=1`); most called: `sceKernelSignalSema`/`WaitSema` (14,364 each), `sceKernelGetSystemTimeLow`, `sceKernelCpuSuspendIntr`/`ResumeIntr`, `sceAudioOutput2OutputBlocking` with `__sceSasCoreWithMix`, `sceDisplayWaitVblankStartCB`, `sceGeListEnQueue` |
| Modules | `LIBFONT.PRX` acknowledged as a stock module (HLE); `LIBSUPPREACC.PRX` loaded and run |
| Overlay | `OVL_MENU_APP.ELF` read by the game's own loader into `0x089DD4C0` and run interpreted (no corpus) |
| Frames | about 1,560 presented in the last run; 26 draws and one display list per frame on the title screen; 2 copies per frame |
| Speed | `[perf] fps 30.0 game 30.0 speed 100%`, guest 6 ms, render 0.5 ms, waiting 27 ms per frame |
| Threads at the end | `ffst_thread` (running) and `AUDIO MIXER THREAD` (delay wait) |
| Saves | savedata `LIST`/`SIZES` for `GameData*` answered "no data" (`0x801103C7`); nothing was written to `ms0/` |

Disc reads come from `PACKAGE.BIN` through the ISO (`main.bin` at offset 8,192, 1,218,448 bytes, as the archive
notes say). The game also probes about 290 `ms0:/PSP/GAME/ULUS10566/*.edat` names (downloadable content); all
answer "not found".

V: in a headless run (`D012_NO_RENDER`, where waits run on a faster clock) the game later loaded
`OVL_BATTLE_APP.ELF` while idle on the title screen. I: the title screen's idle demo; not looked at. In a 57 s
windowed run it stayed on the title screen.

---

## 4. Missing or stubbed HLE calls the game hit, in order

First run, before any profile HLE. Return addresses are the caller's.

| # | Call | How PortableKit answered | Now |
| --- | --- | --- | --- |
| 1 | `UtilsForUser::sceKernelGetGPI` (`ra 0x08860D9C`) | Logging stub, 0 | Profile answers 0, what a retail console reads |
| 2 | `ModuleMgrForUser::sceKernelLoadModule("…/module/libfont.prx")`, `sceKernelStartModule` | Acknowledged, nothing loaded (fixed answer) | Unchanged: libfont is HLE |
| 3 | `sceLibFont::sceFontSetAltCharacterCode` (`0x08881B20`) | Logging stub, 0 | Profile answers 0 |
| 4 | `sceLibFont::sceFontGetFontList` (`0x08881B70`) | Logging stub, 0 | Profile answers 0, writes nothing (PortableKit serves one font, index 0) |
| 5 | `sceLibFont::sceFontFindFont` (`0x08881BE8`) | Logging stub, 0, error code not written | Profile answers index 0 and writes error 0 |
| 6 | `sceKernelLoadModule("…/module/libsuppreacc.prx")` | Acknowledged, nothing loaded | Profile loads it (section 2) |
| 7 | `scesupPreAcc::0x86DEBD66` (`0x0886F2E8`) | Logging stub, 0 | Runs the PRX's export |
| 8 | `scesupPreAcc::0x13AE25B3` (`0x0886F30C`) | Logging stub, 0 | Runs the PRX's export |
| 9 | `scesupPreAcc::0x348BA3E2` (`0x0886F3B4`) | Logging stub, 0 | Runs the PRX's export (`0x80010002`) |
| 10 | `sceNpDrm::sceNpDrmSetLicenseeKey` | Implemented, logged as UNVERIFIED by PortableKit | Unchanged |
| 11 | `sceUtility::sceUtilityMsgDialogInitStart`, the autosave notice | Answered "ok" at once; nothing is drawn | Unchanged (see 7.2) |

The font calls set up libfont at boot (`main:0x08881B18..0x08881BE0`). The title screen's text is drawn from
images, so the font answers are not visible yet.

---

## 5. Graphics

**Colour doubling.** V: the first captures showed the title screen in mid grey (background 127, 127, 127)
instead of white. A display-list trace (`D012_TRACE_GE_LIST=800:2`) showed every textured quad with texture
function word `0xC9010100`: modulate, alpha from texture, and bit 16, colour doubling, which PortableKit did not
read. The game draws with vertex colour `0x80` and relies on the doubling. Framework patch 0003 doubles the
textured colour in the fragment shader when the bit is set; the background is now 254 and the screen looks right.

**Unparsed state.** PortableKit's end-of-run report lists 60 distinct GE commands it ignored. Set on every
frame of the title screen: the stencil commands named in the gap analysis (`0x24`, `0xDC`, `0xDD`), `0x50`,
`0xC8`, `0x36`, `0x38`, `0x1C`, `0x20`, `0x25`, `0x26`, `0x15`, `0x16`. Set 30 times in the run: `0x27`, `0x28`.
Set once (I: state initialisation): the mipmap levels `0xA1-0xA7`, `0xA9-0xAF`, `0xB9-0xBF`, the morph weights
`0x2C-0x33`, the texture environment colour `0xCA`, `0xE8`/`0xE9` and others. Nothing on the title screen looks wrong for want of them.

---

## 6. Overlay metadata

`tools/port/overlay_meta.py` writes `meta.txt` (`base`, `name`, `size`, `code_size`, `hash`) the way
`host/overlays.cpp` identifies a corpus: FNV-1a 64 over the first `64 + code_size` bytes of the loaded image.
The game writes into loaded overlays, so that range must stop before the first byte it changes.

V: a dump of the window on the title screen (`D012_DUMP_SLOT=1500`) differs from `OVL_MENU_APP.ELF` in 167 bytes
in 40 places. The first is `+0x3500` (`0x089E09C0`), the menu's `MenuRuntimePrefix` pointer that main stores;
the next is `+0xA105` (`0x089E75C5`). So the default hash
window of 4,096 bytes (`code_size` 4,032) is safe for the menu and still tells the three overlays apart. For
battle and extra it is not measured.

Values with the default window (output in `build/scratch/port/overlay_meta/`):

| Overlay | size | hash |
| --- | ---: | --- |
| menu | 1,555,864 | `0x29F9A274007A5CA2` |
| battle | 1,596,080 | `0xA3D8F60BD768B4A8` |
| extra | 24,312 | `0xBDF6E45D953CC467` |

---

## 7. Next blockers, ranked

1. **No corpus.** The title screen runs at full speed interpreted, but menus and battles will not. Generate and
   compile main (`psp_recomp build/port/game/EBOOT.ELF --auto build/port/generated`, then a build of hours) and
   the three overlays (`overlay_meta.py --recompile`). Before that, measure the overlay hash window for battle and
   extra with `D012_DUMP_SLOT` once each is loaded.
2. **Message dialogs are answered without being shown.** PortableKit's `sceUtilityMsgDialogInitStart` logs the
   text and answers at once: "ok", or "yes" to a yes/no question. Harmless for the autosave notice; before any
   work with saves it needs a dialog that is drawn and answered by the player, or the port could answer "yes" to
   a save prompt by itself.
3. **Unparsed GE state** (section 5): stencil is set every frame. Expected to matter in menus and battles (masks,
   shadows), with colour test, logic op, mipmaps and morph weights after it.
4. **`LIBSUPPREACC.PRX` results unchecked.** It runs and answers, but only the "no install data" path has been
   seen, with an empty memory stick. Its exports run interpreted; a recompiled build needs a corpus for it too or
   leaves it to the interpreter.
5. **Unimplemented imports not hit yet**: `sceKernelCancelSema`, `sceIoChstat`, `sceAudioOutput2GetRestSample`,
   `sceMpegGetPcmAu`, the four screenshot utility calls, `sceUtilityMsgDialogAbort`, ad hoc matching (10),
   `sceNetAdhocctlGetPeerInfo`, `sceNetAdhocGetPdpStat`.
6. **Fonts.** One host TrueType font with glyph metrics tuned to another game; `sceFontGetFontList` leaves the
   game's entry as it was. Visible once menus draw text.
7. **Installer.** The profile carries the tag but not the key, so `--install` is unsupported; the game directory
   route is the only one. (Since 24 October 2026 the profile carries the key too, and `setup.ps1` uses `--install`
   to decrypt the EBOOT.)

---

## 8. Not verified

- Audio: an audio device opened and the game mixed sceSas and decoded ATRAC3plus every frame, but nobody listened.
- Whether the PSP doubles colour in the replace texture function; the shader doubles after every function.
- That the `module_start` fallback is right for other games with `e_entry` 0.
- Input on the title screen: no buttons were pressed, by rule.

## Reproduce

```powershell
.\tools\port\pk_cmake.cmd -S port -B tools/local/port/build-d012 -G Ninja
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012 -j2
.\.venv\Scripts\python.exe tools/port/prepare_game_dir.py
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 55 --env D012_SCREENSHOT_DIR=build/port/shots `
    --env D012_SCREENSHOT_EVERY=60 --env D012_PERF=log --env D012_LIST_STUBS=1 --env PSPRECOMP_HLE_HISTOGRAM=1 `
    --env D012_DUMP_SLOT=1500 --env D012_DUMP_SLOT_DIR=build/port/dumps --env "D012_INPUT_SCRIPT=1560:quit"
.\.venv\Scripts\python.exe tools/port/overlay_meta.py --out build/scratch/port/overlay_meta_check `
    --check build/port/dumps/overlay_window_1500.bin build/ULUS10566/OVL_MENU_APP.ELF
```

---

# Phase 1, step 2: past the title screen

> Log of 18 October 2026, second session. Same build and machine as above, PortableKit `33800c4` plus eleven
> local commits on `dissidia` (the three above and eight new ones, section 14). Still the interpreter only, no
> recompiled code. The fresh data directory `build/port/data` (empty `ms0/`) throughout; no other saves were
> copied, read or referenced.

## 9. Summary

- **Dialogs are real.** The message dialog, the save-data dialog (list, save and delete modes) and the keyboard
  are drawn over the game and wait for the player; the answer is what the game reads. Nothing is confirmed
  without input. Tests answer them deliberately with the scripted virtual pad, or headless with
  `D012_MSGDIALOG_ANSWERS` / `D012_SAVEDATA_ANSWERS`.
- **New Game works through to the first battle.** Data setup (Create New Data, nickname, play plan, bonus day,
  battle style, story difficulty, confirm, subtitles, no Prologus transfer), the game's own save into
  `build/port/data/ms0/PSP/SAVEDATA/ULUS10566GameData00` through the drawn LISTSAVE dialog, the opening movie,
  then LOAD GAME from that save, the Moogle tutorial, its slide pages and the tutorial battle, which runs and
  pauses. The main menu/hub was not reached (section 15).
- **What it took**: two HLE bugs that blocked (glyph buffers, callbacks in the vblank wait), one missing call
  (`sceKernelCancelSema`), the dialogs, and the GE pixel mask for the battle. No logging stub is hit any more
  on this route.
- **Speed**: menus about 3 to 6 ms of guest time per frame, the battle 16 to 24 ms, all interpreted: full speed
  at 30 fps throughout. `D012_UNTHROTTLED=1` runs the menus at about 8x, which is what keeps a New Game inside
  the 90 s cap.

## 10. The dialogs

| Dialog | Before | Now | Seen with this game |
| --- | --- | --- | --- |
| Message (`sceUtilityMsgDialog*`) | Answered "OK", or "Yes" to any question, at once; nothing drawn | Drawn over the game (text between two rules, OK or Yes/No, button hints); VISIBLE until answered; writes button 1 (yes/OK), 2 (no) or 3 (back). Option `0x100` opens on No, `0x80` disables back. `sceUtilityMsgDialogAbort` added | V: the autosave notice at boot, options `0xA3` (text, no sound, OK button, no cancel), answered with the pad. No yes/no question from the game came up on this route |
| Save data, list modes and SAVE/DELETE | LISTSAVE saved to the first listed name (overwriting it if used), LISTLOAD loaded the first save, LISTDELETE deleted the first save, SAVE and DELETE went ahead | A list of the game's slots (save title, detail, date, or "New data"), then a Yes/No question: "Save the data?", "Overwrite this data?" (opens on No), "Load this data?", "Delete this data?" (opens on No). Backing out reports result 1. Work is done when the player confirms | V: LISTSAVE (20 slots `GameData00`-`19`, all new) chosen and confirmed, saved 257,224 bytes, encrypted with the game's key; LISTLOAD lists "GAME DATA" with its detail and loads it. LISTDELETE, SAVE and DELETE not exercised |
| Keyboard (`sceUtilityOsk*`) | Already the port's keyboard | Unchanged | V: the nickname (12 characters), prefilled from the port's player-name setting ("PortableKit") and accepted with Start |
| Game-data install | Reports success, copies nothing | Unchanged | Not called on this route (Extras > Data Install not chosen) |

Off and test switches: `D012_MSGDIALOG_ANSWERS=ok,no,yes,back` and `D012_SAVEDATA_ANSWERS=new,first,cancel,<name>`
answer in order without drawing (V: headless, the notice took `ok` from the list); with no window and no answer
the game waits and the log says so once (V: headless). `D012_MSGDIALOG_INSTANT` and `D012_SAVEDATA_INSTANT` skip
the dialogs but answer questions No, never overwrite and never delete. The dialogs follow the PSP's behaviour as
far as public descriptions go (an overlay band with the text and buttons, the result codes of the SDK headers);
no emulator code was looked at.

## 11. How far New Game got, and the overlay at each point

Inputs were sent with `D012_INPUT_SCRIPT` for the part already seen and `D012_INPUT_LIVE` for each new screen,
capturing before every next input. Overlays from `D012_TRACE_IO` (the game's own reads of the ELF into the
window at `0x089DD4C0`).

| # | Screen | Input | Overlay | Capture |
| --- | --- | --- | --- | --- |
| 1 | Autosave notice (now drawn) | cross | menu (loaded at boot) | `build/port/msgdialog_autosave.png` |
| 2 | Title, NEW GAME | cross | menu | `build/port/title_screen.png` (step 1) |
| 3 | Data Config: Create New Data / Transfer save data | cross | menu | `build/port/data_config.png`; before the font fix `build/port/data_config_before_font_fix.png` |
| 4 | "Enter a nickname" notice, then the keyboard | cross, Start | menu | `build/port/osk_nickname.png` |
| 5 | Play Plan, Bonus Day, Battle Mode, Story Mode | cross x4 (Casual, Mon, Action-style, Standard) | menu | `build/port/s5/v4.png`, `v5.png`, `build/port/s6/p2.png`, `w1.png` |
| 6 | "Proceed with these settings?" YES; Subtitle Language English; Prologus transfer NO | cross x3 | menu | `build/port/s6/w2.png`, `w3.png`, `w4.png` |
| 7 | LIST, SIZES, LIST, then LISTSAVE: the save dialog | cross (slot), cross (Yes) | menu | `build/port/savedata_list_save.png`, `build/port/savedata_confirm.png` |
| 8 | (White screen: the hang, fixed in 14.4) Opening movie with subtitles | Start skips | battle, after five alternating reads (battle, menu, battle, menu, battle) | `build/port/opening_movie.png` |
| 9 | Next runs: title with LOAD GAME selected, LISTLOAD dialog | cross, cross, cross | menu | `build/port/savedata_list_load.png` |
| 10 | Opening movie again (the save is from before it) | Start | battle (same five reads) | |
| 11 | Moogle tutorial in 3D: "Have you ever fought DISSIDIA battles?" | cross, down, cross ("first time") | menu then battle | `build/port/tutorial_moogle.png` |
| 12 | Tutorial slides (HP attacks, Bravery Break, ...) | cross per page | menu then battle | `build/port/tutorial_page.png` |
| 13 | Tutorial battle, Lightning against a Lightning manikin (COM/RAV/MED are her paradigms) | none, then square/circle; Start pauses | battle | `build/port/battle_tutorial.png`, `build/port/battle_pause.png`; before the pixel-mask fix `build/port/battle_before_pixel_mask.png` |

Side trips: EXTRAS (Data Install, "Prologus" Data Transfer; `build/port/s12/t1.png`, overlay not traced) and the
title's idle attract movie, which unthrottled runs reach after a few seconds of waiting.

I: in the battle the player's inputs register (a square press during a Break showed "HIT! 0"), but the battle
was not played to the end: each run spends about 35 s of its 90 s getting there.

## 12. HLE calls hit, in order

`PSPRECOMP_HLE_FIRST_CALLS=1` (new, section 14). Calls 1-75 are boot to the title, as in step 1. From there
(New Game run; the Load Game run to the battle adds the last group):

| From | Calls (first use, in order) |
| --- | --- |
| Save probe at boot | 78-81 `sceUtilitySavedataInitStart`, `GetStatus`, `Update`, `ShutdownStart` |
| Autosave notice | 82-85 `sceUtilityMsgDialogInitStart`, `GetStatus`, `Update`, `ShutdownStart` |
| Title music | 86-90 `sceAtracSetDataAndGetID`, `GetSoundSample`, `DecodeData`, `SetLoopNum`, `ResetPlayPosition`; 93 `sceAtracReleaseAtracID` |
| New Game | 91 `sceWlanGetEtherAddr`, 92 `sceUtilityGetSystemParamString` (UNVERIFIED in PortableKit), 94-95 `sceFontGetCharInfo`, `GetCharGlyphImage`, 96-101 `__sceSasSetVoice`, `SetPitch`, `SetSimpleADSR`, `SetVolume`, `SetKeyOn`, `GetPauseFlag` |
| Nickname | 102-105 `sceUtilityOskInitStart`, `GetStatus`, `Update`, `ShutdownStart` |
| Settings confirmed | 106 `sceKernelGetSystemTimeWide`, 107 `sceRtcGetDayOfWeek` (UNVERIFIED) |
| After the save | 108 `sceDisplayGetFrameBuf`, 109 `sceKernelPowerTick`, 110 `scePowerSetClockFrequency`, 111 `sceKernelVolatileMemLock`, 112-115 `sceIoSetAsyncCallback`, `sceIoLseekAsync`, `sceIoWaitAsync`, `sceIoReadAsync` |
| Opening movie | 116-126 `sceMpegInit`, `sceKernelMemset`, `sceAudioChReserve`, `SetChannelDataLen`, `sceMpegQueryMemSize`, `RingbufferQueryMemSize`, `RingbufferConstruct`, `Create`, `MallocAvcEsBuf`, `QueryAtracEsSize`, `InitAu`; **127 `sceKernelCancelSema` (was a stub)**; 128 `sceKernelDelayThreadCB`; 129-138 `sceMpegAvcDecodeMode`, `QueryStreamOffset`, `FlushAllStream`, `RegistStream`, `GetAvcAu`, `GetAtracAu`, `RingbufferAvailableSize`, `RingbufferPut`, `AvcDecode`, `AtracDecode`; 139 `sceAudioOutputPannedBlocking`; 140-150 `sceKernelWaitThreadEnd`, `sceMpegAvcDecodeFlush`, `sceKernelExitDeleteThread`, `sceMpegUnRegistStream`, `sceKernelDeleteSema`, `sceMpegFreeAvcEsBuf`, `Delete`, `RingbufferDestruct`, `sceAudioChRelease`, `sceMpegFinish`, `sceUtilityUnloadModule` |
| Moogle scene | 151-153 `sceKernelDcacheWritebackRange`, `sceDmacMemcpy`, `sceKernelDcacheInvalidateRange` |
| Tutorial battle | `sceAtracGetRemainFrame`, `GetStreamDataInfo`, `AddStreamData` (streamed music), `__sceSasSetPause`, `SetKeyOff`, `GetEnvelopeHeight`, `sceRtcGetTick` (UNVERIFIED) |

153 distinct HLE functions by the battle; lists in `build/scratch/port/bringup2/first_newgame.txt` and
`first_load_battle.txt`. No logging stub was called once `sceKernelCancelSema` existed (`D012_LIST_STUBS`).

## 13. Rendering

Checked against the gap analysis's battle list with a display-list dump of one battle frame
(`D012_TRACE_GE_LIST`, summarised by the new `tools/port/ge_list_summary.py`) and `D012_TRACE_FB_TEXTURES`:

| Item | What the battle does | State |
| --- | --- | --- |
| Stencil | Used every frame. The stage is drawn with stencil ref 0 and the characters with ref `0x80` (REPLACE); 20 fans per frame are a depth-fail shadow volume (`SOP 0x010201` then `0x010000`); the blur composite is drawn only where stencil is not `0x80` (`STST 0x808003`), so characters stay sharp | **Not emulated.** The shadow volumes show as dark shafts under the characters; I: the background blur also lands on the characters |
| Pixel mask | The shadow-volume passes write only the stencil bit: colour mask `0x007FFF`, alpha mask 0, into a 5551 buffer | **Fixed** (14.8). Before: those passes filled the buffer in colour and the composite covered the screen in blue, the scene visible only in a diamond |
| Screen copy | The frame buffer at VRAM 0 is sampled as a 512x512 texture into 256x128 and 120x68 targets (`0x04112000`, `0x04122000`, `0x040CC000` as 8888) for bloom and blur, composited back with blending | Works: PortableKit finds the render target behind the texture |
| Depth sharing an address with colour | Depth at VRAM `0x88000`, colour at `0x0`/`0x44000`; no overlap in the frame dumped. But `0x040CC000` is a 512-wide 5551 target and a 128-wide 8888 target in the same frame | No problem seen |
| Morph weights | `0x2C`-`0x33` set once at start-up only | Not used on this route |
| Mipmaps | Level registers set once; texture level mode `0xC8` set 17,191 times, ignored | I: single-level textures; not checked further |
| Others ignored in battle | `0xC0` texture map mode (9,845 times: environment or projection mapping), colour test `0x27`/`0xD8`-`0xDA` (3,270), logic op `0x28`, dither `0xE2`-`0xE5` | Not looked at; nothing obviously wrong besides the shadows |

Elsewhere: all menu text was stripes until 14.3; the 3D Moogle scene and the slides look right; the movie plays
with subtitles. Not checked against hardware: the Moogle's proportions, colours in the battle.

## 14. Framework changes (on `dissidia`, patches 0004-0011 in `port/portablekit-patches/`)

| Patch | Change |
| --- | --- |
| 0004 | 14.1 Message dialog drawn and answered by the player; `<prefix>_MSGDIALOG_ANSWERS`, `_MSGDIALOG_INSTANT`; `sceUtilityMsgDialogAbort` |
| 0005 | 14.2 `<prefix>_MEMORY_STICK_DIR`, so `run_d012.py` puts `ms0` in `build/port/data` |
| 0006 | 14.3 `sceFontGetFontInfo` reports 4 bits per pixel (byte 260). V: `D012_TRACE_FONT` showed every glyph drawn with a line length of 0 into buffers 48 bytes apart; the game sizes them from that byte |
| 0007 | 14.4 `sceDisplayWaitVblankStartCB`/`MultiCB` deliver the thread's callbacks. V: `D012_TRACE_KERNEL` showed `FMI_SeekAsyncCallback` notified and never delivered (no callback was ever delivered), the game stuck on white after `sceIoLseekAsync` |
| 0008 | 14.5 `sceKernelCancelSema` |
| 0009 | 14.6 `PSPRECOMP_HLE_FIRST_CALLS` |
| 0010 | 14.7 Save-data list/SAVE/DELETE dialog; `<prefix>_SAVEDATA_ANSWERS`, `_SAVEDATA_INSTANT` |
| 0011 | 14.8 GE pixel mask as the colour write mask (whole channels; 16-bit formats read as the 16-bit pixel) |

Each has an off switch where it changes behaviour a game sees (`D012_NO_VBLANK_CALLBACKS`, `D012_NO_PIXEL_MASK`, the
two INSTANT switches). The framework's 21 tests pass and `portablekit_host_check` builds; the two `dissidia-recomp`
patches still apply on top. In this repository: `tools/port/run_d012.py` (90 s cap, `D012_MEMORY_STICK_DIR`; the
recompiled-build agent edited the same file for `--build-dir`/`--data-dir`), and two new helpers,
`tools/port/shots_to_png.py` and `tools/port/ge_list_summary.py`. No profile or HLE file in `port/host/` changed.

## 15. Next blockers, ranked

1. **Stencil.** The battle's shadows and its stencil-masked blur need the PSP's stencil, which lives in the colour
   buffer's alpha. A renderer feature in PortableKit, not a profile change; the biggest visual gap left.
2. **Reaching the hub.** Win (or skip) the tutorial battle and see the world/story menu, then Quick Battle (needs
   unlocking). Each run spends about 35 s of 90 reaching the battle; a save made after the tutorial would shorten
   that. Expect more dialogs and the first autosave (`AUTOSAVE`, which never asks) there.
3. **Save dialog details unverified**: the cancel result (1, the common dialog value, not traced), LISTDELETE,
   SAVE and DELETE with this game, and what the PSP shows behind it (the game blanks to white).
4. **GE state still ignored in battle**: texture map mode `0xC0`, colour test, logic op, texture level mode.
5. **UNVERIFIED HLE on this route**: `sceUtilityGetSystemParamString` (the nickname default comes from the port's
   player-name setting), `sceRtcGetDayOfWeek`, `sceRtcGetTick`, `sceNpDrmSetLicenseeKey`.
6. **Imports not hit yet**: screenshot utility (4), `sceIoChstat`, `sceAudioOutput2GetRestSample`,
   `sceMpegGetPcmAu`, ad hoc matching.
7. **Corpus**: the battle runs at full speed interpreted (16-24 ms of guest time per frame), so the recompiled
   build is about later scenes and headroom, not about this route.

## 16. Not verified

- A yes/no message dialog from the game; the message dialog's error mode; the mouse in either dialog.
- Audio, again: mixed and output, not listened to.
- The 16-bit pixel-mask reading beyond this one effect; 4444 and 5650 targets; partial masks.
- That delivering callbacks in the vblank wait changes nothing else for other games (power and UMD callbacks the
  game registered at boot now run too).

## Reproduce (step 2)

```powershell
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012 -j2
# New Game to the save dialog (a fresh build/port/data/ms0); then answer the dialog live
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 90 --env D012_UNTHROTTLED=1 `
    --env D012_SCREENSHOT_DIR=build/port/shots --env D012_SCREENSHOT_EVERY=100000 --env PSPRECOMP_HLE_FIRST_CALLS=1 `
    --env "D012_INPUT_SCRIPT=460:pad a;1080:pad a;1250:pad a;1500:pad a;1800:pad start;2150:pad a;2450:pad a;2750:pad a;3050:pad a;3350:pad a;3650:pad a;3950:pad a;4300:shot save" `
    --env "D012_INPUT_LIVE=$PWD/build/port/live.txt"
# With that save: Load Game to the tutorial battle
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 80 --env D012_UNTHROTTLED=1 `
    --env D012_SCREENSHOT_DIR=build/port/shots --env D012_SCREENSHOT_EVERY=100000 `
    --env "D012_INPUT_SCRIPT=460:pad a;1080:pad a;1500:pad a;1650:pad a;2300:pad start;2800:pad a;2900:pad a;3000:pad dpdown;3030:pad a;3150:pad a;3240:pad a;3330:pad a;3420:pad a;3600:pad a;3700:pad a;3800:pad a;4300:shot battle;4400:quit"
.\.venv\Scripts\python.exe tools/port/shots_to_png.py build/port/shots
```

The frame numbers depend on load times and can drift; look at the captures before trusting a prefix.

# Stencil

> Log of 18 October 2026, third session. PortableKit `33800c4` plus fifteen local commits on `dissidia`; the two
> new ones are patch 0014 (stencil) and patch 0015 (shared depth buffers). Interpreter build
> unless noted. Every run used a copy of the data directory, `build/port/data-stencil-test`; `build/port/data` was
> not run against and its files are unchanged (checksums compared before and after).

## 17. What the game sets

Read off three consecutive battle display lists of the tutorial battle (`D012_TRACE_GE_LIST=4150:3`, summarised
with `tools/port/ge_list_summary.py --detail`, which now also prints the depth function, blend words, texture
function, alpha and colour tests, cull and the other enables; the summary is in
`build/port/logs/stencil_gelist3_summary.txt`) and the frame's sprites (`D012_TRACE_SPRITES`). Field layouts are
the PSP SDK's: `0xDC` is function, reference, mask a byte each from bit 0; `0xDD` is the operations on stencil
fail, depth fail and pass. Functions 1 always, 3 not equal; operations 0 keep, 1 zero, 2 replace. The same states
were in the earlier session's dump (`build/port/logs/battle1.log`).

| Pass | Target | State |
| --- | --- | --- |
| Clear | `0x0` or `0x44000` (5551, 512), depth `0x88000` | CLEAR `7` (colour, alpha/stencil, depth), eight 64-pixel strips, colour `0x006E665F`: stencil 0 |
| Scenery | same | Stencil test on, always, ref 0 or `0x80`, mask `0x80`, replace on pass (`0x800001`/`0x808001`, `0x020000`); depth test greater-or-equal (`0xDE` 7), depth writes on for most, off (`zmask`) for some; blend source alpha |
| Shadow buffer clear | `0x0CC000` (5551, 512), depth `0x88000` | CLEAR `3` (colour and stencil, not depth), colour 0 |
| Shadow volumes, 10 casters, 4 fans each | `0x0CC000` | Pass 1: always, ref `0x80`; fail zero, depth-fail replace, pass zero (`0x010201`); pixel mask `0x007FFF`/`0` (only the stencil bit written); depth test on, no depth writes; the two fans with opposite cull directions. Pass 2: not equal, ref 0, mask `0x80`, pass zero (`0x010000`); colour written, blend source alpha |
| Half-size copy of the scene | `0x168000` (5551, 256) | Textured from `0x0`, always/replace `0x80` |
| Fighters (the 256x256 CLUT8-textured meshes) | `0x0` | Always, ref `0x80`, replace on pass |
| Shadow buffer halved in place | `0x0CC000` | Four sprites, textured from itself, always/replace `0x80` |
| Shadow composite | `0x0` | Four sprites from the halved shadow buffer, not equal ref `0x80` mask `0x80` (not where the fighters are), replace on pass; blend reverse subtract, fixed `0x3F3F3F`/`0xFFFFFF`; colour test not equal 0, mask `0xFFFFFF` |
| Blur buffer | `0x0CC000` as 8888, 128 wide, 120x68 | A sprite with all operations zero (stencil, so alpha, 0), then four sprites downsampling `0x0` with replace `0x80` (alpha `0x80`) |
| Blur composite | `0x0` | The blur buffer stretched over the screen, no stencil test, blend source alpha, alpha test greater than 0: a 50% blend wherever the downsample wrote |

Not seen: increment, decrement, invert, the less/greater functions, 4444 targets, a stencil test on 5650. No draw
in these frames reads destination alpha in its blend. The depth buffer `0x88000` is named by all four colour
buffers; it was never a colour buffer in these frames (gap analysis item 4: it is one in the game's other mode).

## 18. Implementation

Two commits on `dissidia`, both game-agnostic (`host/gpu`, `host/perf`).

- **patch 0014, stencil.** `GeState` decodes `0x24`, `0xDC`, `0xDD` into `DrawCall::stencil`. The depth attachment
  becomes `D32_SFLOAT_S8_UINT` (or `D24_UNORM_S8_UINT`), its stencil kept from pass to pass. The stencil state is in
  the pipeline key (saved key files go to version 2). The renderer keeps the stencil as the alpha bits widened to
  8 bits (0 or `0xFF` in 5551, multiples of `0x11` in 4444, the byte in 8888), so the colour target's alpha holds
  the same number over 255. A clear with the alpha flag writes the stencil from the clear colour's alpha; the pixel
  mask limits stencil writes as it does alpha (bit 15 of `0xE8` in 5551, `0xE9` in 8888). A draw with the stencil
  test on leaves the colour's alpha alone, since on the PSP the stencil result is what lands in those bits.
  Each target records whether its stencil or its alpha is newer. Before anything reads alpha (a draw that writes
  alpha, a destination-alpha blend, a copy for sampling the target, the per-frame write-back) the alpha is rebuilt
  from the stencil: alpha cleared, then one stencil-tested additive quad per bit plane (two quads in 5551, nine in
  8888). Before a draw tests or writes the stencil after alpha was written, the stencil is rebuilt from a copy of
  the colour: stencil cleared, then one quad per bit plane, through a masked alpha test added to `ge.frag` (used only
  by these quads). A full-screen clear of both, which `sceGuClear`'s strips are recognised as, leaves them in step.
  5650 targets get no stencil. `D012_NO_STENCIL` restores the old depth-only format and ignores the stencil;
  `D012_TRACE_STENCIL` logs every rebuild; the perf line gains `stencil N` (rebuilds per frame).
- **patch 0015, shared depth buffers.** With the stencil working, the shadow volumes still marked nothing: they are
  drawn into `0x0CC000` and depth-tested against `0x88000`, which the scene was drawn into with `0x0`/`0x44000`, but
  PortableKit gave each colour target its own depth image. Each picture of a depth buffer is now numbered by the
  target that holds it, and a draw that tests or writes a depth buffer whose newest picture is in another target's
  depth image copies its depth aspect first (not the stencil, which belongs to the colour buffer). No copy for a
  full-screen clear, for different row lengths, or when it would overwrite the target's own newest picture of
  another depth buffer. One copy per battle frame. `D012_NO_SHARED_DEPTH` turns it off;
  `D012_TRACE_FB_TEXTURES` logs the copies.

On how `0x88000` interacts: the stencil lives with the colour target, as on the PSP, so the address the depth
buffer shares with a colour buffer in the game's other mode does not touch it; depth is now shared by address
between colour buffers, but depth contents are still not in guest memory, so a colour draw into `0x88000` does not
disturb the depth pictures of `0x88000` (on the PSP it would overwrite them). Not seen on this route.

## 19. Verification

Same scripted Load Game route as section 11 (`D012_UNTHROTTLED=1`), captures at the same script frames, with and
without both switches. Runs are not frame-identical (the guest clock follows the wall clock), but repeated runs
fall into a few timelines; the pairs below are from the same timeline (mean pixel difference 2.4 to 3.9 of 255).

| What | Before (`D012_NO_STENCIL=1 D012_NO_SHARED_DEPTH=1`, or the build before these commits) | After |
| --- | --- | --- |
| Battle, Lightning facing the manikin on its pedestal | `build/port/battle_stencil_before.png` | `build/port/battle_stencil_after.png` |
| Battle, both fighters on the open floor | `build/port/battle_stencil_before2.png` (build before the commits) | `build/port/battle_stencil_after2.png` |
| Both pairs side by side with a difference image (x4) | `build/port/battle_stencil_compare.png` | |
| A drop shadow, the manikin in the air | | `build/port/battle_stencil_shadow.png` |

Looked at: the grey shafts under both fighters are gone (the difference image is mostly the shafts, plus faint edges). The
shadows are now small footprint-shaped patches on the floor where the volumes meet it, soft-edged from the
halving, darkened by about a quarter of the shadow buffer's grey (0.25 x about 0.55, so roughly 14% darker than the
floor); they are clearly visible under an airborne fighter (`battle_stencil_shadow.png`) and mostly hidden by the
feet when one stands. A debug view of `0x0CC000` showed the volumes marking only the floor around the feet,
and with the composite forced to add instead of subtract the patches appeared at the feet, nowhere else. The
shadow composite is kept off the fighters by the stencil, as the game asks. The blur composite is not stencil
tested: the game blends it at 50% over the whole picture, fighters included; that now uses the stencil's `0x80`
as alpha instead of whatever alpha the scene drawing left, and the picture's softness looks about the same.

Unchanged screens (captures `build/port/stencil/base1` against `build/port/stencil/s2`): the autosave notice, the
title screen and a tutorial slide are pixel-identical; the Moogle scene differs only along edges and the moving
light beams (animation timing).

Cost, mean of the last 12 perf lines (battle) of each run, 960x544, AMD, unthrottled:

| Run | Frame | Guest | Render (CPU) | GPU | Passes | Copies | Stencil rebuilds |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Off (`stencil_off.log`) | 20.4 ms | 17.4 ms | 2.8 ms | 0.40 ms | 14.0 | 11.0 | 0 |
| On (`stencil_on.log`) | 21.2 ms | 18.0 ms | 3.0 ms | 0.97 ms | 18.7 | 12.7 | 9.4 |
| On (`stencil_on2.log`) | 21.1 ms | 17.9 ms | 3.0 ms | 0.99 ms | 19.0 | 13.0 | 10.0 |

About 0.2 ms more recording on the CPU and 0.55 ms more GPU time a frame, from five more render passes, two more
full copies (the stencil rebuild from colour, the depth copy) and about ten rebuilds a frame (five of them for the
four in-place halving sprites and the composite, which each sample the shadow buffer). Frame time is the
interpreter's either way. At higher internal scales the copies and quads grow with the target.

Builds: interpreter build and recompiled build (`tools/local/port/build-d012-recomp`, work directory
`build/port/recomp`) rebuilt. The recompiled build boots headless with `PSPRECOMP_NO_INTERPRETER=1` and
`D012_MSGDIALOG_ANSWERS=ok` to the title music and the attract cycle, installs `suppreacc`, `menu` and `battle`,
never logs `No recompiled function registered`; a windowed run reached the title (`build/port/stencil/recomp`).
PortableKit's 21 framework tests pass; `portablekit_host_check` builds. The regenerated patches apply to a fresh
`33800c4` and give the same tree as `dissidia`.

## 20. Still wrong or not verified

- **Colour test** (`0x27`, `0xD8`-`0xDA`): ignored. Here it is on only for the shadow composite, where the texels
  it rejects (black) subtract nothing; what it would also stop is that composite's stencil replace, and nothing
  reads that stencil later in the frame. No visible effect, so no change; it needs per-draw data and the push
  constants are full (128 bytes).
- **Logic op** (`0x28`): not enabled in these frames. **Dither** (`0x20`): on, ignored; the targets are 8 bits a
  channel. **Texture map mode** `0xC0` (environment/projection mapping), texture level mode `0xC8`, clip planes
  (`0x1C`), `0x26`: still ignored, not looked at.
- Not measured against hardware: the shadows' darkness and shape; how the GE widens 1- and 4-bit alpha when it
  compares stencil values; increment and decrement on 1- and 4-bit stencils (Vulkan's clamp on the widened value);
  the order of operands of less/greater; the 16-bit reading of the pixel mask (section 14.8), which the stencil
  write mask follows. The PSP SDK documentation describes `sceGuPixelMask` per 8-bit channel even for 16-bit
  formats, which would leave blue writable in the volumes' first pass; the 16-bit reading is kept because the
  game's picture only makes sense with it.
- Frame interpolation (off at the default 30 fps): replays repeat the stencil draws but not the rebuilds.
- Depth contents are not in guest memory: texturing from a depth buffer, or a colour draw over one, is not modelled.
- One run of about fifteen ended with an access violation after the game had quit (`stencil_gelist.log`, exit
  `0xC0000005` after the interpreter report); the identical next run exited normally. Cause not found; it may
  predate these commits.

## Reproduce (stencil)

```powershell
Copy-Item -Recurse build/port/data build/port/data-stencil-test   # once; never run against build/port/data
$route = "460:pad a;1080:pad a;1500:pad a;1650:pad a;2300:pad start;2800:pad a;2900:pad a;3000:pad dpdown;" +
         "3030:pad a;3150:pad a;3240:pad a;3330:pad a;3420:pad a;3600:pad a;3700:pad a;3800:pad a;" +
         "4100:shot b4100;4300:shot b4300;4700:quit"
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 88 --data-dir build/port/data-stencil-test `
    --log build/port/logs/stencil_on.log --env D012_UNTHROTTLED=1 --env D012_PERF=log `
    --env D012_SCREENSHOT_DIR=build/port/stencil/on --env D012_SCREENSHOT_EVERY=100000 --env "D012_INPUT_SCRIPT=$route"
# The same with --env D012_NO_STENCIL=1 --env D012_NO_SHARED_DEPTH=1 for the old picture
.\.venv\Scripts\python.exe tools/port/shots_to_png.py build/port/stencil/on
```

# HP bar, orbs and saves

> Log of 18 October 2026, fourth session. The owner played the recompiled build through the tutorial and reported
> three problems: the HP bar does not drop, the EX Force orbs look flat, and the port's save prompt comes up at every
> autosave. PortableKit `33800c4` plus 22 commits on `dissidia` (seven new, patches 0016 to 0022). Every run used the
> recompiled build with `PSPRECOMP_NO_INTERPRETER=1` and a copy of the data directory,
> `build/port/data-glitch-test` (the owner's save, past the tutorial; settings 60 fps, internal scale 4);
> `build/port/data` was copied once and not run against. Run logs and captures are in `build/port/glitch/`.

## 21. Summary

| Problem | Cause (traced) | Fix | Before / after |
| --- | --- | --- | --- |
| HP bar stays full | The HUD cuts each bar to length with depth: 46 through-mode draws a frame write only depth (pixel mask `0xFFFFFF`/`0xFF`, every colour bit masked), slanted 45-degree quads among them, and the bar is then drawn depth-tested (`GEQUAL`) through them. The renderer switched the depth test off for every through-mode draw, which in Vulkan also drops its depth writes | Through-mode draws test and write depth as the GE asks (patch 0018, `D012_NO_THROUGH_DEPTH`) | `build/port/hpbar_compare.png`, `hpbar_before.png`, `hpbar_after.png` |
| EX Force orbs flat | Battle particles, dust and the orbs are transformed sprites (GE primitive 6 through the transform). The GE draws those upright on the screen between the two projected corners; the renderer laid the rectangle flat in the model's x-y plane before the transform, so it faced one world direction and showed edge-on from the side. Not interpolation: the same at 30 fps | The added corners are placed in model space where the transform puts them on the screen rectangle (patch 0019, `D012_NO_SCREEN_SPRITES`) | `build/port/sprites_compare.png`, `sprites_before.png`, `sprites_after.png` |
| Save prompt at every autosave | Before autosaving, the game asks LIST for `GameData*`. PortableKit matched `?` only, found no save, and the game said "Unable to autosave. Proceed to the save and deletion screens?", then opened LISTSAVE: the port's save list. With the save found, it autosaves with AUTOSAVE, which shows nothing | LIST matches `*` (patch 0020, `D012_NO_SAVEDATA_LIST_STAR`) | `build/port/autosave_before.png`, `autosave_after.png` |
| Dialog look | | A translucent band over the game, centred white text, soft highlight, hints at the lower right (patch 0021) | `build/port/dialogs_before.png`, `dialogs_after.png` |

Frame interpolation is in neither picture problem. The profile now gives interpolation distances for a world in metres
(section 24).

## 22. Getting to a battle

The save loads to the main menu. Route, unthrottled, frames as counted by the input script: notice (460), LOAD GAME
(1080), the slot and Yes in the LISTLOAD dialog (1500, 1650), main menu, Battle Mode, Quick Battle (1-on-1), Lightning
against a CPU Lightning, stage Old Chaos Shrine (fixed, not Random), Enter Battle, the VS and Opponent Info screens
(Reproduce, below). Boot to the battle takes about 35 s unthrottled, and an unthrottled battle is over in seconds, so
the input script gained a step `speed normal` / `speed unlimited` (patch 0016): the route runs unthrottled and switches
to real time as the battle loads. The CPU's opening moves repeat from run to run, so captures at the same script
frames show the same moments, more or less (the guest clock follows the wall clock).

## 23. The HP bar

Seen: the HP numbers drop (1000 to 558), the bar turns from blue to green and stays full length
(`build/port/glitch/r7_hud.png`); the same at 30 fps with `D012_FRAME_RATE=30` (`r8_sheet.png`).

Traced: one battle display list, `D012_TRACE_GE_LIST=15150:1:400000` (the trace's 20,000-word cap cut it off before the
HUD; patch 0017 makes the cap the third field) and `D012_TRACE_SPRITES=15150`; summaries in
`build/port/glitch/r10_detail.txt`. The HUD is through-mode triangle strips into `0x04000000`, depth `0x88000`, depth
function 7 (`GEQUAL`), depth writes on. Per bar: a `CLEAR` sprite with only the depth flag clears the HUD area's
depth; 1-pixel rows and corner pixels of the bar's outline and 708x707 squares turned 45 degrees (bounding boxes in the
sprite trace, e.g. `(-276,49)-(432,756)`) are drawn with pixel mask `0xFFFFFF`/`0x0000FF`, writing depth only; the
square's diagonal edge falls across the bar where the HP fraction ends; then the coloured fill (`(66,249)-(162,256)`
for 558/1000) is drawn through that depth. 46 depth-only draws in the frame.

`host/gpu/vulkan_renderer.cpp` built the pipeline with `depth_test = test_enabled && !through`. The shader already maps a
through vertex's z (0 to 65535) to the same device depth as a transformed vertex's, so the fix is that one condition.

Not a stale-vertex problem: vertices are decoded from guest memory for every draw (GPU decode is off by default), and
the trace shows the game writing new rectangles each frame.

After: the bar shortens with a slanted end and the lost part drains in red, as on the handheld
(`hpbar_compare.png`: the player at 562/1000 and the CPU at 1333/1428). Unchanged: title, main menu, Data Config,
Options, Story Mode's help page, character select, battle setup, VS, Opponent Info and the results menu
(`build/port/glitch/r15_sheet.png`, `r19/`). Cost: none measurable (passes, copies and GPU time a frame as before).

## 24. The EX Force orbs

Traced: `D012_TRACE_SPRITES` now also lists transformed sprites with the size of the screen rectangle the GE draws and
of the rectangle laid flat (`build/port/glitch/r18.log`). A battle frame had 50 besides the half-size copies of the
screen: 16x16 ones (`0x090E98A0`), 32x16 (`0x090A42C0`) and 64x64 (`0x090A5140`). Laid flat, some were 2.4 to 6 times
wider than tall (`screen 72x72; laid flat 182x77`, `232x232; 1421x228`); which ones depends on the camera, since a flat
card faces one world direction.

The fix (`screen_sprite_corners`): project both corners, then unproject the other two corners of the screen rectangle
at the second corner's clip depth through the inverse of projection x view x world, so the draw keeps its own transform
(lighting, fog and the interpolation replay are unchanged). A corner behind the eye or a singular transform falls back
to the old expansion.

Before/after on the same timeline (runs r16 and r17: the same inputs; r16 at 30 fps with both new switches off): a
puff of dust at a foot was a thin vertical smear and is now a round cloud; cyan orbs streaked sideways and are now round
(`sprites_compare.png`).

Interpolation: not involved; the flat sprites were the same with interpolation off. `frame_interpolation.*` blends
the eye transform (view x world) of matched draws; that is right for these sprites now that they are expanded upright
from the newer frame's matrices, the error at a half-frame camera turn being a fraction of a degree. Not changed.

**Thresholds.** Dissidia's battle world is in metres or near it: the near fighter is drawn 6.4 units from the eye, the
far one 12.7, the arena's edges about 40 (`build/port/glitch/r10.log`, view and world matrices). Monster Hunter's
defaults are for a world about a hundred times finer. Measured with `D012_TRACE_INTERPOLATION=frames` (`r12.log`):
the camera moves up to 2.7 units a game frame while following a dash; draws paired with another instance of the same
mesh jump 20 to 40 units with the camera still, and the 120-unit guard let all of them blend. The profile now sets
`max_camera_distance` 4, `continuous_distance_margin` 0.6, `max_continuous_distance` 8 and `max_own_motion` 4
(`port/host/d012_profile.cpp`); angles and the matched fraction stay the defaults. In a battle (`r20.log`) the guard now
holds back about 200 pairs a second of some 87,000 (0.24%, nearly all "the same mesh drawn more than once"), cuts are
as before (one, the opening camera turn of 173 degrees), 99.5 to 99.8% of draws match.

## 25. Saves

Modes the game uses (`D012_TRACE_SAVEDATA`; the numbering is the PSP SDK's, as in pspsdk's `psputility_savedata.h`):

| When | Modes | On a PSP |
| --- | --- | --- |
| Boot | LIST (11) `GameData*`, SIZES (8), LIST, then AUTOLOAD (0) `GameData00` once LIST finds it | Nothing shown |
| Title, LOAD GAME | LIST, LISTLOAD (4) | The list, then a confirmation |
| After a battle; leaving Data Config or Options | LIST, then AUTOSAVE (1) `GameData00` | Nothing shown |
| LIST found nothing | The game's own message dialog ("Unable to autosave. Proceed to the save and deletion screens?", then "Proceed to save?"), then LISTSAVE (5) | The game's dialog and the list, as here |
| New Game (earlier session) | LISTSAVE | The list, then a confirmation |

pspsdk names the modes but does not describe their screens; AUTOLOAD and AUTOSAVE are the silent ones, and LIST and
SIZES are queries. No mode this game uses is one the PSP shows without asking, so no "saving" notice was added: AUTOSAVE
stays silent, the list modes keep their question, and nothing overwrites or deletes without the player.

Cause: `do_list` matched the pattern with `?` only, so `GameData*` (9 characters) never matched `GameData00` (10).
Every autosave therefore became the game's "Unable to autosave" question and, on Yes, the port's LISTSAVE screen, which
is what the owner saw (`autosave_before.png`: the question, the next one, the list). After the fix the boot also runs
the AUTOLOAD it skipped before, and after a battle the game goes straight from the result to its menu
(`autosave_after.png`), saving 257,224 bytes to `GameData00` with no dialog. `D012_NO_SAVEDATA_LIST_STAR=1` brings back
the old matching, which is also the way to see the game's yes/no dialogs now.

The dialogs (patch 0021): the message dialog and the save list are drawn as a translucent band across the screen with
thin light rules, centred white text, a soft pill behind the selected choice, and the pad's button hints at the band's
lower right; the save list has the slots down the left (a plain card where the PSP shows the icon, the save's title and
date) and the selected save's title, detail and date on the right. Original drawing, no console assets, no code from
other projects; the port's menus keep their look. Before: `dialogs_before.png` (captures from the second session);
after: `dialogs_after.png` (notice, load list, its question, the game's yes/no, the save list).

## 26. Changes

Framework (clone branch `dissidia`, patches regenerated with `git format-patch 33800c4..dissidia`; they apply to a
fresh `33800c4` and give the same tree):

| Patch | Change | Off switch |
| --- | --- | --- |
| 0016 | Input script `speed unlimited` / `speed normal` | (test tool) |
| 0017, 0022 | `TRACE_GE_LIST=N[:M[:W]]`, word cap W, documented | (trace) |
| 0018 | Depth test and writes for through-mode draws | `<prefix>_NO_THROUGH_DEPTH` |
| 0019 | Transformed sprites upright on the screen; `TRACE_SPRITES` lists them | `<prefix>_NO_SCREEN_SPRITES` |
| 0020 | Save-data LIST matches `*` | `<prefix>_NO_SAVEDATA_LIST_STAR` |
| 0021 | Message and save-data dialogs drawn as a band | (appearance only) |

Profile: `kInterpolationThresholds` in `port/host/d012_profile.cpp`. In this repository: `port/README.md`,
`port/portablekit-patches/`, this section.

## 27. Verification

- Recompiled build, `PSPRECOMP_NO_INTERPRETER=1`, owner's settings (60 fps): boot, autosave notice, title, LISTLOAD,
  main menu, Battle Mode, Quick Battle setup, a battle at real time with attacks (`build/port/glitch/final1.log`,
  `final1_sheet.png`): `suppreacc`, `menu` and `battle` installed, no `No recompiled function registered`, no logging
  stub called. Battle: 30 game frames, 60 presented, guest 8 to 9 ms a frame. Menus (Data Config, Options, Story
  Mode) in `r19`, a battle to its end and autosave in `r13`.
- Both builds rebuilt. Interpreter build: headless boot to the title with the fixed LIST and AUTOLOAD
  (`interp_boot.log`).
- PortableKit's 21 framework tests pass (`tools/local/port/build-framework`); `portablekit_host_check` builds.

## 28. Also noticed, ranked

1. **The profile names 2 save folders; the game uses 20.** It lists `GameData00` to `GameData19`; `kSaveFolders`
   has slots 00 and 01, so the port's own save tools see only those.
2. **GE state still ignored in battle**, per run: logic op `0x28` (56,000 sets), colour test `0x27`/`0xD8`-`0xDA`,
   texture map mode `0xC0` (13,500), texture level mode `0xC8` (23,000), clip planes `0x1C`, dither `0x20`, the region
   `0x15`/`0x16`, `0x25`, `0x26`, `0x36`, `0x38`, `0x50`. Nothing visibly wrong traced to them on this route.
3. **Save mode numbering.** pspsdk calls mode 7 LISTALLDELETE and 10 DELETE; PortableKit calls them DELETE (asks one
   question) and SINGLEDELETE (no dialog). This game uses neither.
4. **UNVERIFIED HLE on this route**: `sceRtcGetTick`, `sceRtcGetDayOfWeek`, `sceRtcTickAddDays` (new: the main menu's
   calendar), `sceNpDrmSetLicenseeKey`. No logging stub was hit in menus or battle; the 19 stubs are unchanged.
5. **Interpolation guard**: in effect-heavy moments up to about 480 pairs a second are held back and follow the camera
   for the in-between frame. Looked at in captures only, not in motion.
6. **Captures stall the frame** (about 130 ms each, the perf line's `max`): only when screenshots are taken.
7. LIST reports the folder's modification time; a copied save shows the copy time (the game's own date is in its
   detail text).
8. Audio mixed and output (sceSas, ATRAC3plus streaming); not listened to.

Not verified: other games against the depth and sprite changes (2D screens that enable the depth test now test it;
sprites crossing the near plane fall back to the old shape); the mouse and touch in the new dialogs; a whole Story Mode
chapter.

## Reproduce (HP bar, orbs and saves)

```powershell
Copy-Item -Recurse build/port/data build/port/data-glitch-test   # once; never run against build/port/data
$route = "460:pad a;1080:pad a;1500:pad a;1650:pad a;1900:pad dpdown;2000:pad a;2400:pad a;3200:pad a;" +
         "3700:pad a;4400:pad a;4900:pad a;5600:pad dpdown;5750:pad dpright;5900:pad dpup;6050:pad a;" +
         "7100:pad a;8000:pad a;8020:speed normal;8828:shot o44;9100:quit"
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 90 --build-dir tools/local/port/build-d012-recomp `
    --data-dir build/port/data-glitch-test --env PSPRECOMP_NO_INTERPRETER=1 --env D012_UNTHROTTLED=1 `
    --env D012_SCREENSHOT_DIR=build/port/glitch/after --env D012_SCREENSHOT_EVERY=100000 --env "D012_INPUT_SCRIPT=$route"
# The old picture: add --env D012_NO_THROUGH_DEPTH=1 --env D012_NO_SCREEN_SPRITES=1
# The old autosave: add --env D012_NO_SAVEDATA_LIST_STAR=1, drop "8020:speed normal" and wait for the battle to end
```
