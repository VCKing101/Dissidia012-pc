# An installer release: design study

> Written 24 October 2026. Design only: nothing here is built. It answers how players should get the native port
> without Visual Studio, Python or Git. Stage one is the `setup.ps1` script, which another agent is writing. Stage
> two is a downloadable release that holds the program, the recompiler and a compiler, but no game. On the player's
> PC it makes the game's code from their own disc and compiles it.
>
> Sources read: PortableKit's `docs/DESKTOP_APP.md`, `RELEASING.md`, `BUILDING.md`, `PROFILE_GUIDE.md` and
> `HLE_EXTENSIONS.md`; `apps/portablekit/` and `scripts/package_desktop_windows.ps1` on the clone's branch `dissidia`
> (patches 0001 to 0053); this repository's `port/`, [port/README.md](../../port/README.md),
> [recompiled-build.md](recompiled-build.md), [steam-deck.md](steam-deck.md), [performance.md](performance.md) and
> `tools/port/package_deck.py`.

Conventions as in the other port notes: "V:" verified here (by a run, a measurement or the code), "I:" inference,
"D:" taken from PortableKit's documents without being re-checked.

---

## 0. Summary

- **Recommendation: build our own installer around `D012Native`. Don't ship PortableKit's desktop app.**
  PortableKit already has the pieces we need: the port's own first-run setup screens (`host/install/`), the app's
  keys file, its compile driver and corpus loader (`apps/portablekit/src/`, MIT), and the Windows packaging script
  with its checks. We would borrow those pieces and add "compile on first run" to the port. The app's model is many
  games, a generic identity, no overlays and hot swap under the interpreter. Dissidia needs a branded program with
  four recompiled overlay libraries, `--dispatch-call` rows and native overrides, and it runs with no interpreter.
- **Effort.** About 8 to 12 days of work for Windows, plus the owner's test sessions. The app route would take
  about 15 to 20 days. Native Linux and Steam Deck add 4 to 7 days later. In the meantime the Windows release runs
  on the Deck through Proton, compile included (unverified).
- **Toolchain.** The same trimmed llvm-mingw 20260922 the app ships (D: 334 MB unpacked, 83 MB zipped; Apache-2.0
  with LLVM exception, plus mingw-w64's permissive licences).
  - **Compile time is the main risk.** Estimated for our 360 MB corpus, scaled from the app's LocoRoco 2 numbers:
    about 2 to 2.5 minutes at -O0 and about 28 to 33 minutes at -O2 on a 6-core Ryzen.
  - **Two cheap levers to measure first.** -O1 took 1.8 times as long as -O0 on one of our units with GCC; -O2 took
    5.7 times as long. Halving the unit span to 8 KiB cut -O2's time for the same code by 44%.
- **Inputs.** The player's ISO, plus either a decrypted EBOOT or a `keys.txt` in the app's format.
  - Every part of the disc that matters is checked by SHA-256 against hashes already in our profile.
  - The overlays and the PRX are plain files on the disc, so we can read them out without Python. The dispatch-call
    rows and overlay metadata become data files that the C++ setup step reads.
- **Updates.** A new release zip goes over the old folder. `data/` (settings, saves, mods) and `game/` are never in
  a zip.
  - The compiled game is cached under a key: the recompiler and ABI sources, `dispatch-calls.txt`, the overlay
    table, the flags and the toolchain.
  - Host-only changes, which include every native override, the BGM playlist and the camera, never trigger a
    recompile.
- **Legal.** Today's `D012Native` links PortableKit's built-in console keys (`crypto_keys_builtin.cpp`, 17 constants).
  The Deck package therefore carries keys. A public release must be built with `EXTERNAL_KEYS`, and its packaging
  must check for key bytes as well as game data.

---

## 1. The desktop app or our own installer

### 1.1 What the app does today, and what is designed only

| | State (D: DESKTOP_APP.md, V: read in `apps/portablekit/src`) |
| --- | --- |
| Add a game from an ISO, identify it by PARAM.SFO and executable SHA-256 | Done (Mac, Windows) |
| Keys file `keys.txt`, fingerprint checks, `keys status` | Done; checked on the Mac only |
| Decrypted executable given by the player (`--executable`) | Done |
| Compile on the player's machine: -O0/-O1/-O2/tiered, jobs by memory | Done; on Windows smoke-tested only, no whole -O2 compile in the app |
| Hot swap from the interpreter to compiled code | Done on the Mac; not tried on Windows |
| Hand profiles compiled in (`PORTABLEKIT_APP_PROFILES`), matched by executable hash | Done, but one source file per profile, renamed with `-Dgame=...` |
| HLE extension modules (`PORTABLEKIT_HLE_EXTENSION_DIRS`) | Done |
| **Overlay corpora** | **Not done.** The app compiles only the main corpus (V: `corpus.cpp` runs `psp_recomp <EBOOT.ELF> --auto <dir>` with no base, prefix or `--dispatch-call`). Whether the app should use the framework's overlay tooling is listed as open |
| Linux, Steam Deck | **Designed only.** The Linux paths were never compiled |
| macOS `.app`, notarized | Open |
| Windows packaging (`package_desktop_windows.ps1`) | Done: trimmed toolchain, licences, game-data and path checks, a probe compile |

### 1.2 How our extras fit the app's model

V: configured in a scratch directory with MSVC (`-DPORTABLEKIT_APP_PROFILES=port/host/d012_profile.cpp`). The app
configures, and our profile compiles inside it under the rename. It cannot link, because nothing in the app's CMake
adds the profile's other sources.

| Our extra | Fit in the app |
| --- | --- |
| Profile `d012_profile.cpp` | Compiles as a hand profile (V). It is matched by `executable_sha256` `0bc8e779...` |
| 14 more host sources (`d012_hle`, `_prx`, `_mods`, `_json`, `_bgm_playlist`, `_loading_layout`, `_camera`, `_decomp_*`, `_test_switches`, `_crash_report`, `_nintendo_stream`) and the C library `d012_decomp` | **No slot.** The app needs a per-profile source list. An HLE extension module could carry them as a library, but the profile's hooks (`register_extra_hle`, `patch_loaded_image`) still live in the profile, and our code reaches into `host/` internals, which the module interface says change without notice |
| `config/nids.csv` | Not read: the app's `PROFILE_DIR` is `apps/portablekit`. It only supplies names |
| The three overlays and `LIBSUPPREACC.PRX` as recompiled libraries | **Missing.** A release build has no interpreter, so without these the game stops at the first overlay call |
| `config/dispatch-calls.txt` (`--dispatch-call` for main and battle) | Missing. Without it the pad and walk overrides lose the calls from their own unit |
| Decompiled overrides (`register_function` at the first miss, again when battle loads) | **Conflict with hot swap.** The app registers a newly compiled corpus at a heartbeat. That overwrites a main-module override such as `0x08872138` until the next overlay load. It needs a "corpus registered" hook so the overrides can be applied again |
| `d012_portable.cpp` (paths, interpreter off) | Does not apply. The app sets `PORTABLEKIT_DATA_DIR` per game and forces the prefix `PORTABLEKIT`. Our `portablekit::env()` reads would then follow `PORTABLEKIT_*`, while the direct `std::getenv("D012_...")` reads (camera, BGM, decomp, crash dump) would still read `D012_*`. That is a mixed switch namespace |
| Mods (`<data dir>/mods`) | Work, but in `data/games/ULUS10566-0bc8e779/mods`, not where [mod-folder.md](mod-folder.md) and the Deck guide say |
| 53 framework patches | The app builds from the same tree, so they come along. **But none has been built with llvm-mingw.** The app's Windows build was checked before our patches, and our port was only ever built with MSVC. This risk is the same on both routes |

### 1.3 Effort and what players get

| | A. The app, with our profile and extras compiled in | B. Our installer around `D012Native` |
| --- | --- | --- |
| Framework work | Per-profile sources and libraries in the app's CMake; **overlay corpora in the app** (generate with base and prefix, write the entry, link one library each, meta); per-profile recompile arguments; re-registering overrides after hot swap | `EXTERNAL_KEYS` for a port, plus a keys-file loader (the app's `keys_file.cpp` moved to `host/`); the port installer accepting a decrypted EBOOT or a keys file; a port built `NO_CORPUS` loading `cache/.../main.dll` (about 30 lines around `open_corpus_library`); a "compile" stage in the setup screens |
| Our work | Reconcile the switch prefix, data layout and portable defaults; test every extra inside the app | Our compile plan: main plus 4 libraries, the overlay table, the PRX relocation, the cache key, the setup text |
| Rough effort, Windows | 15 to 20 days | 8 to 12 days |
| Player sees | "PortableKit (Dissidia 012 build)": a library window, one game, a game's page, play under the interpreter while compiling (Dissidia's battles interpreted: not measured), keys status, export | "Dissidia 012": the port's own first-run screens (choose the disc, then the EBOOT or keys, then compile with a progress bar), then the game. One `.exe` to add to Steam, as today |
| Upstream fit | Good if TeamGDB wants overlay games in the app (an open item there) | Good: each seam is one the docs already name (`crypto_keys_file.cpp`, the port installer, editions loading corpus libraries) |
| Divergence risk | High: the app is young, and per-game exceptions inside it are what its design avoids | Low: the port stays a port |

**Recommendation: B.** It borrows from A rather than competing with it:

- `keys_file.cpp`, `corpus.cpp` (compile arguments, link arguments, jobs, status file), `process.cpp` and
  `corpus_abi.cmake` are MIT. They can be used nearly as they are, best as framework code shared by the app and ports.
- `package_desktop_windows.ps1` is the model for our packaging script (section 5).
- If the app later gains overlay corpora and full hand ports, moving Dissidia into it is a repackaging, not a rewrite.

---

## 2. The toolchain

### 2.1 What to ship

**llvm-mingw 20260922 (ucrt, x86_64), trimmed exactly as `package_desktop_windows.ps1` trims it**, for the reasons
the app chose it (D, section "Windows: ship an LLVM toolchain"):

- **Redistributable.** LLVM, clang, lld, libc++, libc++abi and libunwind are Apache-2.0 with LLVM exceptions; the
  mingw-w64 headers, CRT and winpthreads are under permissive licences or in the public domain. MSVC's UCRT and
  Windows SDK libraries cannot be redistributed, which rules out shipping clang-cl.
- **Size.** D: 334 MB unpacked, 83 MB zipped (54 MB as tar.xz).
- **The program is built with it too.** That gives one C++ runtime: `libc++.dll` and `libunwind.dll` beside the
  program. With the thin corpus ABI, a corpus library links `-shared -static` and imports nothing from the program.

**Consequence: the release `D012Native.exe` must be built with llvm-mingw, not MSVC.** Building it that way is the
first thing to try (phase 0). The app needed one `#undef interface`. Our host code has MSVC-only parts:

- `d012_crash_report.cpp` writes a minidump with symbols from the PDB. MinGW has dbghelp, but has no PDB unless the
  build links with `-gcodeview`.
- Patches 0045 to 0053 (the present thread and the timing spin) were only ever built with MSVC.

### 2.2 Compile time for our corpus

Our corpus, generated with the default 16 KiB units ([recompiled-build.md](recompiled-build.md), section 3):

- main: 113 units, 117.7 MB
- menu: 95 units, 117.4 MB
- battle: 98 units, 122.9 MB
- extra: 2 units, 1.8 MB
- suppreacc: 1 unit, 1.1 MB

In all, **309 units and 361 MB**. `psp_recomp` takes 8 s for main and 14 s for the overlays.

Known numbers:

| Build | Code | Time |
| --- | --- | --- |
| MSVC `/O2 /Ob0`, i7-12700K, 4 jobs | ours | main 80 s wall, 335-363 s summed (about 3.1 s a unit); everything, host included, 272 s wall (V: Ninja logs) |
| D: llvm-mingw clang 23, Ryzen 5 5600, 6 jobs | LocoRoco 2, 195 units, 193 MB | -O0 **74 s**, -O2 **1,064 s** (about 2.3 s and 33 s a unit) |
| V: GCC 14 (MSYS2), this PC, one unit (`generated_unit_0074`, 1.0 MB) | ours | -O0 2.8 s, **-O1 4.9 s**, -O2 15.8 s, -O2 `-fno-inline` 17.6 s (MSVC: 3.1 s) |
| V: GCC 14, the same code generated with 8 KiB units (`generated_unit_0148`, `_0149`) | ours | -O0 1.7 + 1.5 = 3.2 s, **-O2 4.6 + 4.4 = 8.9 s** |

The GCC figures are a proxy, because no clang is installed here. They show two things that matter for clang too:

- **-O1 costs a fraction of -O2.**
- **-O2 grows faster than the function's size.** Halving the unit span cut -O2 by 44% and raised -O0 by about 16%.

Inlining is not what costs: `-fno-inline` made -O2 slower, not faster.

Estimates for llvm-mingw, scaled from LocoRoco 2 by units (1.58x) and by size (1.87x):

| PC | Jobs | -O0 | -O2 |
| --- | --- | --- | --- |
| 6-core Ryzen 5 5600 class, 16 GB (the app's measurement machine) | 6 | **2 to 2.5 min** | **28 to 33 min** |
| The same, all threads while the game is not running | 11 | under 2 min | I: 22 to 27 min |
| 4-core laptop, 8 GB | 4 | about 4 min | I: 45 to 60 min |
| Steam Deck (4 Zen 2 cores, 16 GB), under Proton | 4 to 6 | I: about 5 min | I: 60 to 90 min |

Peak memory is about 0.3 GB a job at -O0 and 0.5 to 0.9 GB at -O2 (D). Disk at peak:

- the generated C++ (361 MB)
- the objects (D: about 2 times the library)
- an -O0 library (D: about 2.5 times an -O2 library; ours at -O2 is about 170 MB)

That makes **about 1.5 GB during setup and about 200 MB after it**, with the C++ and objects deleted.

How fast the code then runs (D, and our numbers):

- LocoRoco's -O0 code was 6.5 times slower than its -O2 code.
- Our MSVC `/O2 /Ob0` build spends 6.5-7.3 ms of guest time a frame in battle, part of it HLE. [performance.md](performance.md)
  estimates 18-22 ms of that on the Deck.

I: -O0 is fine for menus and probably holds 30 fps in battle on a fast PC, but not on the Deck. The Deck needs
-O2-class code, or -O1 if -O1 turns out close.

### 2.3 Cutting the time

| Lever | Expected | Cost | Status |
| --- | --- | --- | --- |
| **-O1 everywhere** instead of -O2 | GCC proxy: 31% of -O2's compile time. Run speed unknown | none: `--opt 1` already exists in the app's driver | **measure first** (phase 0): compile time and battle guest time with clang |
| **8 KiB units** (`psp_recomp ... --auto <dir> <base> 0x2000`; the span is already a positional argument, V) | GCC proxy: -O2 44% faster | More calls between units go through dispatch, so slower code (not measured). Which calls an override takes changes too, so the dispatch-call rows must be checked again | measure with the perf tour |
| **Hot units at -O2, the rest at -O1/-O0**, from a hot list we ship | most of -O2's benefit for a small part of its time (D: LocoRoco's hot set was 14 of 193 units) | the list is unit indexes and counts from our own perf tours, facts about the game rather than game code. It must be kept in step with the unit span | after the two above |
| **Two stages with a restart**: -O0 at setup (about 2 to 4 min, then play), -O2 in the background at low priority; the next start loads -O2 | the wait goes from about 30 min to about 3 | about 8% more compiling. No hot swap needed: `D012Native` loads the best ready library at start | recommended default if -O1 is not good enough |
| **All cores at setup** (the game is not running yet) | about 1.3 to 1.8 times faster than the app's half the cores | none | do it |
| **Object cache in the setup tool** (ccache-like): `objects/<sha256 of unit text + flags + toolchain>.o` | after a recompiler change, only the units whose text changed are compiled again (for example, a new `--dispatch-call` row touches a few units) | about 0.4 to 0.6 GB of disk if kept; about 100 lines. Bundling real ccache is possible (GPL-3.0, as a separate program with its source offer) but not needed | phase 2 |
| Large functions split by the recompiler | not measured | recompiler work upstream | not now |

### 2.4 Linux and the Steam Deck

- **First, Proton.** The Windows release runs under Proton on the Deck today (unverified on hardware). Its setup
  step would run `toolchain\bin\clang++.exe` under Wine too, and the result is the Windows DLL the Proton build
  loads.
  - I: clang under Wine is widely used and the work is CPU-bound, so this probably works with modest overhead.
  - It must be tested: process spawning, path translation, and `-static` linking under Wine.
  - This is the quickest route to "Deck without a PC".
- **Then native Linux** (D: designed only). It needs four things:
  1. A program built in the Steam Runtime 3 "sniper" SDK (Debian 11, **glibc 2.31 floor**), with no corpus, and
     libstdc++ linked statically (`PORTABLEKIT_RELEASE`).
  2. A bundled LLVM: clang and lld, trimmed to about 100 to 150 MB compressed (D).
  3. A **sysroot** for the corpus: sniper's glibc 2.31 headers and the libstdc++ (or libc++) headers matching the
     program. The corpus links `-shared` (D: `-nostdlib` keeps it off the system's libstdc++ altogether). glibc
     headers are LGPL, and libstdc++'s static library is GPL with the runtime exception; both can be redistributed.
  4. The folder layout as on Windows.

  **SteamOS's read-only root does not matter:** everything (program, toolchain, cache, data) lives in
  `/home/deck/Games/Dissidia012` and nothing is installed on the system. PortableKit's `release_linux.sh` cannot be
  used as it is, because it packs a game *with* its generated code. We would use only its SDK container step, for a
  build with `NO_CORPUS`. The Flatpak form does not fit the portable folder (D). Use a tarball.

---

## 3. The player's inputs

### 3.1 What the player gives, and how each is checked

All the hashes below are already in this repository. They are fingerprints, not game content.

| Input | Check (SHA-256) | Source |
| --- | --- | --- |
| The disc image, `.iso` only (CSO/ZSO are refused by the framework's installer, V: `installer.cpp`) | `PARAM.SFO` disc id `ULUS10566`; `PSP_GAME/SYSDIR/EBOOT.BIN` = `8fbf3541...3394` (`d012_profile.cpp`). **Not a hash of the whole ISO**: dumps of the same UMD can differ in padding | the player's own disc |
| `PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF`, `OVL_BATTLE_APP.ELF`, `OVL_EXTRA_APP.ELF` | `f021f96f...4d0a`, `1f96cfb3...49dc`, `f6b7be5e...ca01` (V: hashed here; identical to `build/ULUS10566/`) | read from the ISO: they are plain, unencrypted ELFs |
| `PSP_GAME/USRDIR/DATA/MODULE/LIBSUPPREACC.PRX` | `aed4d30f...8778` (V) | read from the ISO: a plain PRX |
| **Either** the decrypted EBOOT | `0bc8e779...fe6d` (`executable_sha256`) | Since 24 October 2026 the profile has the tag key, so the port's own `--install` makes it from the ISO (V: same SHA-256), and `setup.ps1` does that. A file the player already has (**pspdecrypt** on `EBOOT.BIN`, what `tools/decrypt_main.sh` does) is still accepted. PPSSPP's "Dump decrypted EBOOT.BIN on game boot" was tried and wrote no file, so it is not recommended |
| **Or** a keys file, the app's `keys.txt` format | `kirk.aes.5D`, `kirk.cmd1` and `tag.D91613F0` decrypt `EBOOT.BIN`. The result must hash to `0bc8e779...`, which checks the tag key. The fixed keys are checked by the app's compiled-in fingerprints | made by the player. The release never names a source of keys |

Notes:

- Without keys, saves are written unencrypted. The game cannot tell, and PPSSPP reads them (D: the framework's
  no-keys mode). With the save keys in `keys.txt`, saves are a PSP's own, and import and export work.
- Our rule never to copy PPSSPP saves into the port is about our own testing. A player importing their own save is
  their choice.
- The tag `0xD91613F0` is the one Tenkawa's profile uses (D), so the app's existing path covers it.
- The player chooses whether the disc image is used **in place** (recorded in `game/settings.ini`, as the app does)
  or **copied** (1.6 GB). The default should be in place. Copying is for a removable drive.

### 3.2 No Python

The two Python steps of the developer pipeline become C++ in the setup step:

- **Overlay extraction.** The framework's `IsoImage` (`host/kernel/iso_image.hpp`) reads the four files out of the
  ISO, and the setup checks their hashes. Nothing is decompressed or decrypted.
- **`overlay_meta.py`.** What it computes are facts about the release: base `0x089DD4C0` or `0x09B81000`, size,
  `code_size` 4,032, an FNV-1a 64 hash of the first 4,096 bytes, and the prefix
  `ovl089DD4C0_menu_29F9A274007A5CA2` and so on.
  - They go in a small table shipped as `port/config/overlays.txt`. This is our own data file, like
    `dispatch-calls.txt`.
  - The setup **recomputes each hash from the extracted file and refuses on a mismatch.**
  - The PRX is relocated to `0x09B81000` first, with `Elf32Image::apply_relocations`. `overlay_meta.py` copies that
    logic.
- **`dispatch-calls.txt`** is shipped as it is. The setup parses its rows (`main` and `battle` today) into
  `--dispatch-call` arguments, so the developer pipeline and the release read one file.
- **The compile plan** is five `psp_recomp` runs (main with base `0x08804000`; each library with its base and
  `--prefix`). Each output gets an entry file: the app's `entry_source()` for main, the framework's
  `overlay_module.cpp.in` for a library. Then come the compile with the bundled clang (the app's
  `compile_arguments()`) and five `-shared -static` links into `cache/<key>/`.

`D012Native` then needs only two things. It loads `cache/<key>/main.dll` where it now calls the linked registry.
And `d012_portable.cpp` sets `D012_OVERLAY_DIR` to `cache/<key>/overlays`.

---

## 4. Updates

### 4.1 The folder

```
Dissidia012\
  D012Native.exe  psp_recomp.exe  SDL3.dll  avcodec-61.dll ... libc++.dll  libunwind.dll   from the release zip
  share\d012\include\  share\d012\config\  (dispatch-calls.txt, overlays.txt)              from the zip
  toolchain\  licenses\  README.txt  LICENSE.txt  release-manifest.txt                       from the zip
  game\       EBOOT.ELF (prepared by setup), settings.ini naming the disc image             made by setup, kept
  cache\<key>\ main.dll, overlays\*.dll, build.log, status                                  made by setup
  data\       settings.ini, pipeline_cache.bin, ms0\ (saves), mods\                          the player's, kept
```

- **A new version is a new zip extracted over the folder.**
  - The zip holds no `game\`, `cache\` or `data\`, so the player's saves, settings, mods and disc stay as they are.
    This is the rule `package_deck.py` and `deploy_deck.py` already follow.
  - `release-manifest.txt` lists the program's files. At start, the program removes the files an earlier manifest
    listed that this one does not, so nothing stale is left. That is `package_deck.py`'s logic, moved to the player's
    side.
- **On the Deck the folder and the update are the same.** `deploy_deck.py` keeps its own job, the owner's
  development deploys, and the rule against deploying until asked.

### 4.2 When the game is compiled again

The cache directory is named by a SHA-256 over:

- the app's corpus ABI inputs (`apps/portablekit/cmake/corpus_abi.cmake`: `corpus_abi.hpp`, `allegrex_context.hpp`,
  the decoder, the analysis, the ELF reader, `codegen_main.cpp`, and the compile definitions)
- `dispatch-calls.txt` and `overlays.txt`
- the unit span and the optimisation level
- the toolchain's version line
- the four input hashes

At start, `D012Native` looks for `cache/<this build's key>/`.

- If the directory is there, the game starts.
- If not, the setup screens say "this version needs to compile the game again (about N minutes)" and run the
  compile, the object cache first.
- Libraries of older keys are removed once the new one is ready.

**What does not cause a recompile:** host code. That covers every native override (`port/decomp/` and
`d012_decomp_*`, because the overrides are registered at run time), the BGM playlist, the camera, mods and the
renderer patches. Most of our releases would be like that. Patches that change `codegen_main.cpp` (0012, 0029) or a
new dispatch-call row would change the key. That is the right outcome: those change the generated code.

---

## 5. Legal and safety review

### 5.1 What a release must never contain

| Item | Today | For the release |
| --- | --- | --- |
| Game files (ISO, EBOOT, overlays, PRX, PARAM.SFO, saves) | `package_deck.py`'s folder contains the ISO and EBOOT.ELF, by design, for the owner's Deck only | none. The zip is made from a staging folder by a script that refuses them (below) |
| Generated C++, objects, compiled corpus or overlay libraries | `D012Native.exe` (80 MB) has the main corpus linked in, and `overlays\*.dll` are the game's code | **none**. The release program is built `NO_CORPUS` and the zip has no `cache\` |
| **Console keys** | **`D012Native` links `crypto_keys_builtin.cpp`** (V: `portablekit_add_game` adds it unless `EXTERNAL_KEYS`; our `port/CMakeLists.txt` does not pass that). The current Deck package carries the framework's KIRK and save keys | built with `EXTERNAL_KEYS`; keys come only from the player's `keys.txt` |
| The tag key for `0xD91613F0` | in the profile since 24 October 2026 (owner's decision): the public value pspdecrypt and the emulators carry, so `--install` decrypts the player's own disc. `setup.ps1` uses it | in the public source, so a release program carries it too |
| Paths of the build machine, the owner's user name, work identity | the `PORTABLEKIT_RELEASE=ON` build drops the checkout fallback | checked by the script (below), as the app's is |
| Decompiled C (`port/decomp/`) | already published in the public repository as source | compiled into the program. That is the same exposure as the public source, a stance the owner has already taken; noted here so it is a conscious choice |

### 5.2 Checks the packaging script must make

The checks below are adapted from `package_desktop_windows.ps1` (V: read).

1. **File names.** No `eboot*`, `data.bin`, `param.sfo`, `umd_data*`, `*.iso|cso|pbp|prx|elf|ovl`. **Added for us:**
   `generated_unit_*`, `generated_registry*`, `*_corpus*`, `ovl089DD4C0_*`, `ULUS10566*`, and any `.dll` other than
   the known third-party list and `libc++`/`libunwind`.
2. **File contents.**
   - The PortableKit checks, by header: no PSP ELF (machine 8), PBP, `~PSP`, PSF or ISO 9660 `CD001` at 32769.
   - **Added for us:** the program's size must be within a bound for a corpus-free build, and the program must not
     export or contain `portablekit_corpus_register` or `recomp_unit_` symbols.
3. **Keys (new).**
   - The build's `CMakeCache.txt` must show `EXTERNAL_KEYS`, which needs a new cache variable in
     `port/CMakeLists.txt`.
   - Every file must be byte-searched for each value in `host/crypto_keys_builtin.cpp`, as read from the clone at
     packaging time. That is 17 values. Any hit fails the package.
4. **Machine paths.** `%USERPROFILE%`, the build and source directories, in both slash forms, as PortableKit does.
5. **It starts.** The packed `D012Native.exe --version`, with only the package on `PATH`. The packed `clang++.exe`
   compiles and links a probe DLL `-shared -static`.
6. `SHA256SUMS` and `BUILDINFO.txt`: commits of this repository and of the framework branch, toolchain, corpus ABI.

### 5.3 Notices to ship (`licenses\` plus a `THIRD_PARTY_NOTICES.md` of our own)

| Component | Licence | Obligation |
| --- | --- | --- |
| Our port | MIT | `LICENSE.txt` |
| PortableKit (with our 53 patches, which are MIT like it) | MIT | `PortableKit-LICENSE.txt`. Note that the framework is modified, and point to our patch series in the public repository |
| SDL3 3.4.18 (ours, not the app's 3.4.16) | zlib | `SDL3-LICENSE.txt`; the notice must name the version we ship |
| **FFmpeg 7.1.5, BtbN prebuilt `win64-lgpl-shared-7.1`** (avcodec, avutil, avformat, swresample) | **LGPL-3.0-or-later** on Windows (built `--enable-version3`), not 2.1 as for the macOS source build | dynamically linked and unmodified; `FFmpeg-LICENSE.txt`, `FFmpeg-SOURCE.txt` with the exact build and commit (`7d0e842004`); **a written source offer** (PortableKit's wording, pointed at our issue tracker) or the source archive on the release page; the player may replace the DLLs. Never link FFmpeg statically |
| llvm-mingw 20260922: LLVM, clang, lld, libc++, libc++abi, libunwind | Apache-2.0 with LLVM exceptions | `llvm-mingw-LICENSE.txt`, also `toolchain\LICENSE.TXT` |
| mingw-w64 headers, CRT, winpthreads | ZPL, public domain, permissive | `mingw-w64-COPYING*.txt` (the script copies them) |
| Dear ImGui, tiny-AES-c, xxHash, stb | MIT, Unlicense, BSD-2, public domain | as PortableKit's notices |
| Vulkan | the system's `vulkan-1.dll`, not shipped | none |

The README must keep the wording the PortableKit app uses: the player supplies their own legally obtained copy, and
what the program makes from it stays on their PC for their own use. The setup screen says this once before
compiling. Nothing is ever downloaded at run time.

---

## 6. Plan

| Phase | Work | Effort | Verify |
| --- | --- | --- | --- |
| **0. Measure** (needs the owner's approval to download llvm-mingw 20260922, about 83 MB zip, from `github.com/mstorsjo/llvm-mingw`) | Build the framework branch and `D012Native` with llvm-mingw; compile our corpus at -O0, -O1 and -O2, at 16 and 8 KiB units, timing each; run the perf tour's battle scenes on each. | 2 days | MinGW builds warning-clean; PortableKit's 21 tests pass (`tools/port/pk_ctest.cmd`, never ad hoc tests); compile minutes and battle guest ms per level. **Go/no-go on -O1 and the unit span** |
| **1. Framework seams** (patches 0054 and up, upstream-friendly) | `crypto_keys_file.cpp` (from the app); the port installer accepting a decrypted executable or keys; a `NO_CORPUS` port loading a main corpus library from a configured path; a compile stage in the setup screens | 2 to 3 days | the installer refuses a wrong EBOOT and wrong keys with a sentence; no keys in a release build (byte scan) |
| **2. Our compile step** | `overlays.txt`; extraction and hash checks; PRX relocation; five `psp_recomp` runs; entry files; compile and link into `cache/<key>`; object cache; two stages (-O0, then -O2) if phase 0 says so; `d012_portable` pointing at `cache` | 2 to 3 days | the libraries made by setup equal what the developer build makes (same registered counts; `[overlay] installed menu`, `battle`, `suppreacc`; no `No recompiled function registered`); overrides active (`[decomp] overrides ... registered`) |
| **3. Packaging** | `tools/port/package_release.ps1` modelled on `package_desktop_windows.ps1`; notices; checks of section 5.2; GitHub release on VCKing101/Dissidia012-pc | 1 to 2 days | the script fails on a planted test file of each forbidden kind; the zip has no game bytes or key bytes |
| **4. Player test** | A Windows user account (or VM) without Visual Studio, Python or Git: unzip, choose the disc, choose the EBOOT, compile, play to a Quick Battle; update to a host-only change (no recompile) and to a key change (recompile); saves, settings and mods survive | 2 days plus the owner's sessions | and on the Deck through Proton, **only when the owner asks for a deploy** |
| **5. Native Linux and Deck** (later) | sniper build with `NO_CORPUS`; bundled clang, lld and sysroot; tarball | 4 to 7 days | compile and play on the Deck from the folder alone |

**Risks, in order:**

1. **Compile time with clang -O2** (about 30 minutes to over an hour) and whether -O1 or -O0 code holds 30 fps in
   battle. Phase 0 settles both.
2. **llvm-mingw build of our 53 patches and host code.** None of it has been built with MinGW. Possible trouble:
   the present thread's timing, `d012_crash_report.cpp`, and `<windows.h>` macros.
3. **Proton compile on the Deck.** Unverified. The fallback is to compile on the player's PC and copy the folder to
   the Deck.
4. **A PPSSPP dump that differs from pspdecrypt's output.** Moot: in a test PPSSPP's dump setting wrote no file,
   and the port now decrypts the EBOOT itself.
5. **Overlay hash windows.** The extra overlay's window is still inferred, not measured
   ([recompiled-build.md](recompiled-build.md), section 8). The PRX corpus depends on the load address
   `0x09B81000`. The setup's own hash check catches a mismatch on the files, not at run time.
6. **Keys in the current builds.** Every `D012Native` built so far carries the framework's keys. They must never be
   attached to a public release.

### Decisions for the owner

1. **Our own installer (recommended) or PortableKit's app** with our extras.
2. **The compile default.**
   - -O1 for everything, if phase 0 shows it close to -O2 (probably about 5 to 10 minutes).
   - Or -O0 then -O2 with a restart: play in about 3 minutes, full speed from the next start.
   - Or -O2 only, one wait of 30 minutes or more.
3. **Accept a keys file at all**, or only a decrypted EBOOT. The keys file covers players without PPSSPP and allows
   PSP-format saves; the EBOOT-only form is simpler to explain.
4. **The disc image** used in place (default) or copied.
5. **The Deck**: Proton first (recommended), or wait for native Linux.
6. **Upstreaming** the framework seams of phase 1 to TeamGDB/PortableKit, or keeping them as local patches.
7. **The llvm-mingw download** for phase 0, and the release name and version scheme.
8. **Decompiled C in the release binary.** It is already public as source; confirm.
