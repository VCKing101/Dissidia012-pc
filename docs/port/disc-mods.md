# Disc mods: a modded disc on the port's own code

Prototype of 19-21 November 2026. A disc mod is a patch to the game's disc, such as [Emendatus](#emendatus), which
the community distributes as an xdelta patch for PPSSPP. The port takes the patch as it is: it applies it once to the
player's own disc image, keeps only what it changes, and plays that on its own compiled code. Nothing is recompiled,
no second disc image is made, and the mod is switched on and off with a restart.

Conventions as in the other port notes: "V:" verified (by a run, a trace or a capture), "I:" inference.

## For the player

**Very early access: expect bugs.** Disc mods were added because players asked to play Emendatus in this port.
Known so far with Emendatus: battles stutter on slower machines such as the Steam Deck (the mod's changed code runs
slower than the game's own), and a battle against Chaos stops the game as it loads.

For full Emendatus support, use [LiinkPK's Dissidia 012 recomp](https://github.com/LiinkPK/Dissidia012-PC), the
project Emendatus officially supports.

Menu (Esc, or L3 + R3), **System**, section **Disc mod**:

- **Choose the mod…** opens a file browser over the game. Choose the mod's patch (`.xdelta`), as its makers hand it
  out: the port applies it to the game's disc it was set up with, in a few seconds, and keeps only the sectors it
  changes in `disc-mods/<name>.pkdisc` in the data folder (25 MB for Emendatus). A disc image already patched
  (`.iso`) works too: it is compared with the game's disc and kept the same way, so the image is no longer needed.
- **Play the modded disc** turns it on or off from the next start; **Restart the game now** restarts at once.
- **Playing** and **Mod** say which disc this start plays and which mod is chosen.

A patch must be for the player's disc: Emendatus's is for the USA disc. Choosing a newer version of the mod's patch
prepares it again.

A modded disc keeps its saves and downloadable content apart from the game's own, in `ms0-patched` beside `ms0`
(the data folder's, or the game folder's in a copy built from source), in every copy: the setup's, a portable folder
(the Steam Deck's) and one built from source. A mod's starter saves go in `ms0-patched/PSP/SAVEDATA`.

Online, a copy playing a mod meets and fights only players of the same mod, of the same version: on the game's
server, in a hosted session, and in battles. Play online's count says "with this mod".

## How it works

### The patch

PortableKit's `host/disc_patch.hpp` (patch 0117) reads VCDIFF (RFC 3284) as xdelta3 writes it, with xdelta3's
Adler-32 of each window and its LZMA secondary compression (one xz stream per kind of section for the whole file,
each section whole LZMA2 chunks after a sync flush; the LZMA decoder is written from the public format
descriptions, as the project has no LZMA library). It applies the patch window by window to the release's image and
compares each 2 KiB sector with the release's at the same place; only the differing ones are kept, all-zero ones as
a mark. The layer file names the image it was prepared from (path and size). A disc image already patched is read
and compared the same way (`disc_patch::prepare_image`), and gives the same layer. A patch or image that changes
nothing is refused; the System page also refuses an image made from another region's disc, which would differ in
nearly every sector.

`IsoImage` opens a layer file as the patched image: listed sectors from the layer, every other one from the
release's image. Everything that reads the disc goes through `IsoImage`, so the game, the patched-edition code and
the port's own readers (the Japanese voices' index, say) see the patched disc and know nothing of the layer.

A window whose checksum does not match is applied anyway and counted: Emendatus's first window (the disc's directory
and the executable) was made from a slightly different disc ([below](#emendatus)).

### The code

The game's recompiled code was generated from the release's executable and overlays. A mod changes a little of
them, so the port keeps that code and lets PortableKit's interpreter run only what the mod changed (PortableKit patch
0117, `host/patched_edition.hpp`):

1. The disc's `EBOOT.BIN` (a plain ELF: an encrypted one cannot have been patched) is loaded over the release's.
2. Every generated unit (16 KiB of code) holding a word that differs from the release is taken out of dispatch. The
   interpreter then runs that unit from guest memory, and calls into it reach the interpreter through dispatch. A
   whole unit, because calls inside one are direct jumps the runtime never sees. The port's own replacements of
   game functions in such a unit stay.
3. An overlay no corpus matches is compared with the release's overlays (read from the release's disc), and runs on
   the closest one's corpus the same way.
4. Code the game writes later is found when it flushes a cache: the executable is compared again on every
   instruction-cache flush, and on a data-cache flush of a range of it.
5. The disc's `PARAM.SFO` asking for the larger memory (`MEMSIZE`) gives the game 64 MiB.
6. The interpreter is on for a modded start (PortableKit patch 0122). A release build and a packaged copy turn it
   off (`PSPRECOMP_NO_INTERPRETER`, `d012_portable.cpp`): without it the first call into a changed unit stopped the
   game ("No recompiled function registered at 0x08933398" with Emendatus).
7. The memory stick is `<memory stick>-patched` beside the one in use, however that was named (patch 0122): a
   packaged copy names its own `<data>/ms0` through `D012_MEMORY_STICK_DIR`, and before 0122 a modded start there
   used the game's own saves. The port's DLC check and the added abilities' side files ask PortableKit for the
   folder it serves as `ms0:` (`portablekit::memory_stick_path()`), so they follow it.
8. A patched overlay is compared with the release's image again whenever the game flushes a cache, as the
   executable is (patch 0122, `recompare_patched_overlays`). Emendatus writes hooks into its battle and menu overlays
   once they run (calls into code of its own at `0x0BFExxxx`): about 70 words the comparison at install could not see,
   which ran as the release's code until 0122.

Port side (`port/host/d012_disc_mod.cpp`): the System page's rows, `settings.ini`'s `disc_mod.image` and
`disc_mod.enabled`, and the profile's `patched_disc`. Also changed: `PACKAGE.BIN` is served up to its size on the
disc, not only up to what its index names (`d012_voices.cpp`), since a mod keeps files of its own past the index;
the decompiled-code harness lets a guest call run interpreted code (`d012_decomp_harness.cpp`), and recognises a
patched overlay as the one its corpus belongs to.

Whether a mod plays is what PortableKit opened (`portablekit::patched::active()`), which the port's memory
reserve and Bartz's original moves follow (`d012_hle.cpp`, `d012_original_moves_feature.cpp`): a chosen mod that
cannot be opened leaves the game's own disc, with them.

Test switches: `D012_PATCHED_DISC=<image or .pkdisc>` plays that disc whatever the settings say;
`D012_NO_PATCHED_DISC=1` plays the game's own; `D012_TRACE_PATCHED=1` logs each changed word that took a unit with
it, `=2` every changed word, with the nearest function a port registered at or below it (a port's replacement of a
function the patch changes runs instead of the patch's code: check it against the function's size); diagnoses:
`D012_PATCHED_INTERPRET_ALL=1` (or `main`, `overlays`) runs all of that code under the interpreter, as an emulator
would, `D012_PATCHED_INTERPRET_RANGE=lo-hi` narrows the executable's part, `D012_PATCHED_COMPARE_VBLANK=1` compares
the code at every vblank too (code written with no cache flush); `D012_TEST_DISC_PATCH=<patch or image>` prepares a mod into `disc-mods` as the game starts and logs the result.

### Online

A mod's disc has an identity: the SHA-256 of the patched image's size and of each sector the layer lists (its number
and its 2048 bytes), in order (`disc_patch::Layer::identity()`, PortableKit patch 0118). Every copy of the same
version of a mod over the same release has the same identity, whether it was prepared from the patch or from a
patched image and whatever the file is called; any other change gives another. A disc image played as it is
(`D012_PATCHED_DISC=<image>`, a test) is known only by its size, executable and `PARAM.SFO`, so it matches no
prepared mod.

Online play keeps copies apart by it twice:

- **Who meets.** The version tag a copy logs in with ([online matchmaking](online-matchmaking.md)) gets `-m` and the
  identity's first 12 digits (PortableKit's `adhoc_version_tag()`). The game's server and a hosted session already
  keep players with different tags apart (lobbies, groups, scans, chat), and Play online's count is of the players
  with the same tag, so nothing changed on the server. A copy of the game's own disc sends its tag as before.
- **Who fights.** The battle check's "mods that change the game" is `disc <identity>`
  ([Communication Mode](communication-mode.md)): a battle between copies that differ does not start, and the notice
  names that item. Watchers compare the same list. A copy of the game's own disc sends it empty, as older copies do.

Play online's count and the chooser's Play Online say "with this mod". A start logs the tag
(`[disc mod] online with the tag ...`) and the battle check (`[comm battle] battles only with copies playing this
mod's disc`).

An ad hoc server that does not know of tags (an older one, or another project's) puts modded and other copies in the
same lobbies. Their battles still do not start, but Friend Cards and quests may pass between them.

## Emendatus

Patch `Emendatus11.18.26.xdelta` (VCDIFF with xdelta3's LZMA secondary compression) applied to the USA disc:

| | Changed | Units interpreted |
| --- | --- | --- |
| Executable | 203 words, plus 16 sites the mod writes while running | 7 of 119 at start, 23 more as it runs |
| Menu overlay | 190 words, plus 3.7 KB of new code | 12 of 96 |
| Battle overlay | 768 words | 15 of 98 |
| `PACKAGE.BIN` | 50 files (fighters, text); a 23 MB archive of its own and an 8.5 KB code blob past the index | |
| `PARAM.SFO` | `MEMSIZE` 1 (64 MiB) | |
| The whole image | 29,613 of 856,704 sectors (16,856 of them emptied): 25 MB kept | |

The mod loads code from `PACKAGE.BIN` while running, patches the game's save calls to keep items numbered 678 and up
in a second save (`ExtGameData00/EMSX.BIN`, "EXTRA DATA (Emendatus)"), and puts code above the classic 32 MiB. The
patch was made against a disc named `EditedBaseSourceUSEME.iso`: 209 of its 210 windows match the clean USA disc;
the first (the disc's directory and the executable) fails its checksum, though the executable it gives decodes
correctly.

A modded ISO made with the mod's patch and pointed at as the port's own disc image does not work: the release's
executable and recompiled overlays then run beside the mod's data and code (I: what the Discord report of broken
story scenes and saves was), and since the Japanese voices change (v0.5.3) the mod's code past the index read as
zeros and the game stopped at the first menu.

## Verified (19-21 November 2026)

Windows, clang build, the USA disc, `Emendatus11.18.26.xdelta`:

- V: the patch (11 MB) prepares in 3.2 to 3.9 s into a 26 MB layer: 210 windows, the first one's checksum differing,
  29,613 sectors changed. The layer over the clean disc is byte for byte the image an independent VCDIFF decoder
  (Python's `lzma` for the sections) made from the same patch: every listed sector matches it, and every other sector
  of the clean disc does too.
- V: the game plays from the layer as from the patched image (same units interpreted, same guest time).
- V: the menu: the patch dropped on the browser, the progress panel, the ready note, Esc closing it, the switch,
  Restart into the mod. The owner played the mod on 21 November, from a patched image and then from the patch.
- V: again on main of 21 November (version 0.8.0, PortableKit patches to 0116): the release starts as before, the
  patch prepares into the same layer (3.1 s, its sectors byte for byte the earlier layer's), and Restart starts the
  modded disc at 30 frames a second, the starter save loaded from `ms0-patched`; `d012_tests` pass.

- V: the modded disc boots to the title; the menu and battle overlays install on the release's corpora; the
  units above are taken; the title demo's battle runs at 1.2 ms of guest time a frame (1.1 ms on the release disc,
  same run length, headless).
- V: with the mod's starter save: the save loads, Story Mode opens, and the prologue's first scene plays.
- V: the System page's rows: choosing the image (dropped on the browser), the switch, and Restart, which restarts
  into the modded disc. Without `D012_MEMORY_STICK_DIR` the memory stick is `ms0-patched`.
- V: with the switch off the release starts as before (460,475 functions, every overlay on its own corpus, no unit
  taken); `d012_tests` all pass; Japanese voices still pair (6,545 files) on the release disc.

- V: in the owner's session the game saved under the mod, and the mod's own save was written beside it, in the same
  second (`ULUS10566GameData00` and `ULUS10566ExtGameData00/EMSX.BIN` in `ms0-patched`).

Online, 21 November:

- V: the patch and the patched image prepare into layers of the same 29,613 sectors and the same identity,
  `58fe9ae52cb0…` (3.1 s from the patch, 1.9 s from an image on an SSD); a separate Python computation gives the same
  identity from either layer, and from the patched image compared with the clean disc directly.
- V: the System page with an image: the image dropped on the browser, "Comparing emendatus.iso with your copy of the
  game's disc", the ready note, Esc back to the game.
- V: a modded start logs its identity, the tag `<port's tag>-m58fe9ae52cb0` and the battle check; the release starts
  as before, with neither.
- V: PortableKit's `portablekit_disc_patch_tests` (a made-up mod from a patch and from an image, read back, its
  identity; the tag of a modded copy) and its other framework tests pass (MSVC; the ad hoc ones not run);
  `d012_tests` pass.
- V: three copies on this PC (main of 21 November) on PortableKit's ad hoc server on 127.0.0.1 alone
  (`--adhoc-server 27412 --loopback`; each copy with `D012_ADHOC_LOOPBACK=1` and the battle link on
  `D012_NET_BATTLE=127.0.0.1:27430`, so every socket stayed on the loopback address and Windows asked nothing): two
  playing Emendatus with copies of its starter save, one the release with the port's versus test save. The server
  logged the release copy's tag as `<port's tag>` and the modded copies' as `<port's tag>-m58fe9ae52cb0`. In the
  Online Lobby's lobby 1, the two modded copies listed each other's Friend Card as Online; the release copy, in the
  same lobby on the same server, listed only its own and heard no hello.
- V: the two modded copies, challenge to fighter select to an online 1-on-1 (Crystal World): the battle link joined
  them as players 0 and 1 of one battle, 2 frames of input delay, the must-match list (the disc's identity
  included) agreed. Random input on both for about 2,700 frames: in step at every check, the same Brave, HP and
  clock on both screens.

22 November, after the first try on the Steam Deck (main 6a4a11be, a portable Linux folder) stopped at once with
"No recompiled function registered at 0x08933398": the merged tests had used developer builds, where the interpreter
is on, and a memory stick nobody named. Both fixed in patch 0122.

- V: on Windows (clang developer build) with `PSPRECOMP_NO_INTERPRETER=1` and the memory stick named, as a packaged
  copy has them: before, the same stop at `0x08933398`; after, the modded disc (through `D012_PATCHED_DISC` and
  through `settings.ini`) runs the title demo's loop for 90 s, the memory stick `ms0-patched`, the DLC looked for
  there, Bartz's original moves left out and no memory reserved; the release as before (moves built, heaps grown,
  `ms0`); `d012_tests` pass.
- V: the Linux package in WSL, laid out as the Deck's folder (interpreter off, memory stick named): the patch
  prepared there (25 MB), then a modded start through `settings.ini` runs 150 s through the demo loop (the modded
  menu and battle overlays installed 15 times each), on `ms0-patched`; with the switch off the release as before.

On the Steam Deck, 22 November (the test install, with the fixes above): Emendatus starts, plays a Quick Battle and
saves in `ms0-patched` (`GameData00` and `ExtGameData00/EMSX.BIN`). Two problems the owner reported:

- **A battle against Chaos stops the game as it loads** (a Chaos only Emendatus makes selectable): `strcpy` (main
  `0x089359A4`) reads a name at `0x00000020`, called from battle `0x08AF2CBC`, which fills an object and copies a name
  given by its caller. Reproduced on Windows (Quick Battle, Lightning against Chaos, the pack's starter save):
  - not the port's replacements: none has a word the patch changes inside it but `0x088CB164` (the added abilities'
    record accessor), and the stop comes with `D012_NO_MOD_ABILITIES=1` too;
  - not code the comparison misses: with the overlays compared again (8 above) and at every vblank, every hook
    Emendatus writes is found before the stop, and it stops all the same;
  - not the recompiled code: with all of the executable and the overlays interpreted, one run reached the battle's
    versus screen and another stopped at the same place. It depends on timing, not on how the code runs.
  Left as the mod's (or the mod's reliance on an emulator's timing) unless a run in PPSSPP shows otherwise.
- **Battles stutter on the Deck.** The units the patch changes run interpreted: in a Quick Battle on this PC the
  game's own code takes 7.4 ms a frame against 2.7 ms on the release disc (median of about 225 s each, same route).
  The Deck's slower processor makes that a larger share of its 33 ms frame. Compiling a mod's changed units ahead of
  time would remove it; not built.

Not verified: a whole story chapter, loading the mod's second save back,
Android (the picker is desktop only), the European disc, another mod or a patch made by another tool than xdelta3.
Online across the internet or between two computers, through the game's public server, or with a Deck or phone. A
battle refused for different mods: copies of different mods never meet in a lobby to reach one. A layer names the
image it was prepared from by path: moving that image means preparing the mod again.
