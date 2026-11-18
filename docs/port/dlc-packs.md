# DLC packs: a folder for each pack

> Written 18 November 2026 for the native port (`port/`). A DLC pack is a costume, assist, attachment or music pack
> made with [DissDlcToolkit](https://github.com/adriangl/DissDlcToolkit). No `mod.json` is needed.

Conventions as in the other port notes: "V:" verified (by a run, a trace or a capture), "I:" inference.

## 1. Adding a pack

Put each pack in a folder of its own, with any name, in the data folder's `dlc` folder:

```text
<data dir>/
  dlc/
    Onion Knight - Nepnep/
      e43f7bab.edat
      45509e39.edat
      ...
    Another pack/
      ...
```

| Installed with | The folder |
| --- | --- |
| The Windows setup | `data\dlc\` in the install folder |
| The Linux setup (Steam Deck included) | `data/dlc/` in the install folder |
| `setup.ps1` (from source) | `build\port\data\dlc\` |

The folder DissDlcToolkit writes for a pack can be copied in as it is. Make the `dlc` folder if it is not there. The
game reads the packs when it starts. To remove a pack, delete its folder.

- **Costumes:** at character select, press Square (Look) to step through a character's costumes.
- **Music** joins the battle music list. A track whose title starts with a game's name, such as `FFXIII - Title`, goes
  under that game; the others go under DOWNLOADED.
- `.gmo` and `.gim` files are what DissDlcToolkit makes a pack from. The game cannot read them until it has.

## 2. Packs built for the same slots

Every pack keeps the slots it was built for when they are free. When a slot is taken, by the DLC in the memory stick
folder or by a pack that comes earlier in the order of the folders' names, the pack is given a free one:

| What | Slots | When none is free |
| --- | --- | --- |
| A costume of a character | DLC 1 to DLC 9 | The pack's costume is left out |
| An object (fighter, assist or attachment) | 1 to 255 | That object is left out |
| A music pack | 1 to 19 | The music pack is left out |

A fighter and its assist of one costume move together, with their portraits and effects. Adding a pack can change
which costume number a later pack gets, so a character may need its look chosen again.

Two packs that change the same costume of the game's own (Normal, Alt. 1, Alt. 2 or the manikin) cannot both be used:
the first one's is used. Files in the memory stick folder are never replaced.

The log says what happened (`data/logs/D012Native.log` in the install folder):

```text
[dlc] pack "Another pack": P_THR100 DLC 2 is now DLC 3
[dlc] pack "Another pack": object slot 200 is now 55
[dlc] pack "Onion Knight - Nepnep": 5 files
[dlc] warning: pack "Old pack": P_THR100_5P is left out: the character's nine DLC costumes are taken
```

## 3. How it works

The game looks for each DLC file at `ms0:/PSP/GAME/<disc id>/<hash>.edat`, the hash being BZip2's CRC-32 of a name made
from slot numbers (`dlc/obj/dlc_200oe.bin`, `obj/p_thr100_5p.gmo`, `bgm/ud_140e00.at3`;
[character-assets.md](../research/character-assets.md), section 7). Two packs for the same slot are therefore the same
file names, and in one folder the second replaces the first.

`port/host/d012_dlc_packs_plan.cpp` works out what the game should find, and `port/host/d012_dlc_packs.cpp` serves it
through PortableKit's file providers. Nothing is written to the memory stick.

- **An object table** (`dlc/obj/dlc_NNNoe.bin`) moved to another slot gets that slot's row id (`0x2C0 + slot - 1`) and
  the slot in its row (`+9`). A row that takes the place of one of the game's own keeps its id.
- **A costume** moved to another number gets it in the row (`+7`) and a model name to match (`P_THR100_6P`, `_A` for
  the assist). The `.gmo`, `.cosx`, `.exex` and portraits are served under the new name's hashes.
- **An attachment** whose files are taken is renamed `G_ATT_<slot>`, and the rows of its pack that attach it (`+0x1C`)
  follow its new id.
- **A music pack** moved to another slot brings its titles (`text/jp/dlc/bgm_NNNt.bin`). A track number or file name
  already in use is replaced in the table (`ud_<slot><number>.at3`).
- **Any other file** of a pack is served under its own name, unless that name is taken.

The order is the folders' names without regard to case, so the same folders always give the same result.

Switches: `D012_NO_DLC_PACKS=1` reads no packs; `D012_DLC_PACKS_DIR=<dir>` reads another folder.

## 4. Verification

18 November 2026, Windows, the recompiled build, the USA disc, data in a throwaway folder with the port's own New Game.

- V: **Two packs with the same file names.** Both written by following DissDlcToolkit's source: Lightning, costume
  DLC 2, object slot 200, music slot 14, nine files each. One model is cyan and the other magenta. They sat beside a
  decrypted copy of the official DLC (222 files), which holds Lightning's DLC 1, object slots 1 to 25, 35, 36 and 39 to
  54, and music slots 1 to 13. The log:

  ```text
  [dlc] pack "[Lightning][DLC 2] Magenta": P_GST130 DLC 2 is now DLC 3
  [dlc] pack "[Lightning][DLC 2] Magenta": object slot 200 is now 26
  [dlc] pack "[Lightning][DLC 2] Magenta": music slot 14 is now 15
  ```

- V: **In the game.** Look offered DLC 2 and DLC 3. A Quick Battle in DLC 2 drew the cyan model and one in DLC 3 the
  magenta model, with the pack's portrait on the HUD. Both packs' tracks were in the BGM list, and the moved pack's track
  streamed from its new name (`bgm/ud_150f00.at3`).
- V: `D012_NO_DLC_PACKS=1`: no pack is read.
- V: `port/tests/d012_tests.cpp`, made-up packs: a pack on its own unchanged, two packs for the same slots, a costume
  and slot on the memory stick, a tenth costume for a character, two packs changing one of the game's own costumes, an
  attachment and the costume linked to it, two music packs for one slot, and a pack that cannot be read.

Not verified: a pack from DissDlcToolkit's own program, an assist or an attachment in a battle, Linux, the Steam Deck,
Android, and online play with packs.
