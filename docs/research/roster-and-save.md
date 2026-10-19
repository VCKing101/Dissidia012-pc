# Dissidia 012 (ULUS10566 v1.00): the roster, and what adding a character would touch

> Static-analysis report produced by a research subagent on 13 October 2026 and kept as written. Nothing in it has
> been confirmed at runtime.

Static analysis only. Sources: Ghidra pseudocode and `refs.tsv` under `analysis/ULUS10566/*/research/`,
`tools/re_query.py` disassembly, raw reads of `EBOOT.ELF` / `OVL_MENU_APP.ELF` / `OVL_BATTLE_APP.ELF`,
a read-only scan of the local USA ISO for two data files (`objentry.bin`, `system.bin`), and web pages.
Nothing tracked was modified, no emulator was run, nothing was downloaded. Helper scripts and extracted
chunks are in `build/scratch/roster/` (git-ignored).

Every address is tagged with its module: **main**, **menu**, **battle**. "Save root" means the
save-manager singleton at main `0x08BAED38` (pointer held in main `0x089D712C`).

---------------------------------------------------------------------------------------------------

## 0. Answer in one page

* A character is a **1-based 8-bit ID**. IDs 1..34 exist. The 31 playable ones are 1..22 and 24..32.
  23 (`0x17`) is boss Chaos, 33 (`0x21`) is Aerith (assist only), 34 (`0x22`) is an unidentified
  `P_ORG800` object. There is no free ID below 35.
* What gets drawn and loaded is an **object entry** (`objentry.bin`, 0x44-byte rows, s16 key) selected by
  `(character ID, costume index, assist flag)`. Manikins are costume 0 of the same ID. Boss Chaos's
  three forms are costume 0/1/2 of ID `0x17`.
* Most per-character static data is **not in the executables**. It is in `system.bin` chunks
  (`chrp`, `chrl`, `amas`, `acas`, `grwp`, `enmp`, `eqpm`, ...) looked up by binary search on the ID.
  The executables hold a small number of fixed lists: one 32-row roster/unlock table, three 31-byte
  "ID to save slot" lists, a 33-row select-screen layout table, and two 35-entry ID-indexed tables.
* "Playable" is defined by one thing: **having a save slot**. Main `0x088CB3B4` searches a 31-byte list;
  if the ID is not in it, main `0x088CB164` hands back a shared blank record at `0x09AEFAFC`. That is
  exactly the state boss Chaos is in, and it explains the reported cheat glitches.
* The Chaos restriction is two tests: main `0x088D5BC0..0x088D5BCC` (roster enumerator skips `0x17`
  unless the caller asks for the opponent list) and menu `0x08A80A4C` (grid builder drops `0x17` unless
  the screen mode is 1 or 3). The public CWCheat does neither; it pokes the built grid in heap memory.
* The save image is **version 10, exactly 0x3ECC8 bytes**, both checked on load, with exactly 31
  character records and no spare space. It sits in BSS with another object immediately after it.
  It is PSP-encrypted on disk (the baseline copies have 7.999 bits/byte entropy).
* Three ID-space limits bite before 32 characters do: arrays of **35** entries indexed by ID (several,
  including one inside the save), **u32 bit masks** keyed by `ID - 1`, and loops over `1..0x20`.

Effort ranking: (a) boss Chaos selectable < (b) 32nd slot cloning an existing character < (c) 32nd slot
with its own data. Details in section 3.

---------------------------------------------------------------------------------------------------

## 1. VERIFIED FACTS

### 1.1 Character IDs

Evidence for the ID order is the voice-set table, main `0x089D6CA8`: 35 string pointers indexed by ID,
read by main `0x088A3A94` in a loop bounded by `0x23` (`do { ... } while (i < 0x23)`, pointer stride 4,
output stride 0xC). Entry 0 and entry 34 are null.

| ID | code | character | ID | code | character |
|---|---|---|---|---|---|
| 1 | one100 | Warrior of Light | 18 | nin200 | Kuja |
| 2 | one200 | Garland | 19 | ten100 | Tidus |
| 3 | two100 | Firion | 20 | ten200 | Jecht |
| 4 | two200 | The Emperor | 21 | gst100 | Shantotto |
| 5 | thr100 | Onion Knight | 22 | gst200 | Gabranth |
| 6 | thr200 | Cloud of Darkness | **23 (0x17)** | org200 | **Chaos (boss)** |
| 7 | for100 | Cecil | 24 | for110 | Kain |
| 8 | for200 | Golbez | 25 | fiv210 | Gilgamesh |
| 9 | fiv100 | Bartz | 26 | sev110 | Tifa |
| 10 | fiv200 | Exdeath | 27 | eht110 | Laguna |
| 11 | six100 | Terra | 28 | ten110 | Yuna |
| 12 | six200 | Kefka | 29 | gst110 | Prishe |
| 13 | sev100 | Cloud | 30 | gst120 | Vaan |
| 14 | sev200 | Sephiroth | 31 | gst130 | Lightning |
| 15 | eht100 | Squall | 32 (0x20) | org210 | Feral Chaos |
| 16 | eht200 | Ultimecia | **33 (0x21)** | sev120 | **Aerith (assist only)** |
| 17 | nin100 | Zidane | **34 (0x22)** | org800 | **unidentified** |

The code strings are verified; the character names are read off the series codes and the `chrp` series
byte (row `+9`: gst100/gst110 = 0x0B, gst200/gst120 = 0x0C, gst130 = 0x0D). ID `0x21` is confirmed as the
DLC assist by main `0x088F9BB0`: if the `aerith.bin` check (main `0x0885C304`) fails and the equipped
assist byte is `0x21`, it is reset to `0xFF`.

Model/object names use the same codes upper-cased with a `P_` prefix (`P_ONE100`, `P_ORG200`, ...),
stored at `chrp` row `+0x74`.

There are no separate IDs for manikins, story NPC versions or assists; see 1.2.

### 1.2 Object entries: costumes, manikins, assists, Chaos forms

`objentry.bin` is loaded by main `0x08847158` into a fixed BSS buffer at main `0x09AFEE58`
(`FUN_08862854(name, 0x09AFEE58, 0x13D30, 1)`). Layout: 8-byte header `{u32 0x0C, u32 count}` then
0x44-byte rows sorted by an s16 key. Row fields used by code:

| offset | meaning |
|---|---|
| `+0x00` s16 | object ID (the key; this is the value held per side in battle setup) |
| `+0x06` u8 | character ID (0 = not a character object) |
| `+0x07` u8 | costume index |
| `+0x08` u8 | flags: `0x08` = assist variant, `0x10` = needs main `0x08844F50` at load, `0x01/0x04` set on manikin rows |
| `+0x24` char[16] | name (`P_ONE100_1P`, `P_ONE100_ZAKO`, `..._A`, `..._W`) |
| `+0x34` char[16] | file stem (`p_one100`) |

Accessors (all main): `0x088470B8(objectId)` = bsearch (`0x08934944`, row 0x44, comparator `0x08846C0C`);
`0x088470DC(charId, costume, wantAssist)` = linear search requiring `row+6 == charId`, `row+7 == costume`,
`(row+8 & 8) != 0` equal to `wantAssist`, name starting `P_` and not ending `_W`.
`0x088FB934(charId, costume, assist)` wraps that, falls back to costume 1, returns the object ID or -1
(and -1 for `charId <= 0`).

From the retail file (found in the ISO directly before `system.bin`; 1170 rows, size 0x136D0):

* **Buffer slack:** the BSS buffer holds 0x13D30 bytes = 1194 rows; the file has 1170. 24 rows free.
* 429 rows carry a character ID, covering all of 1..34.
* Per playable character: costume 0 = `..._ZAKO` (**manikin**, flags `0x05`), 1 = `_1P`, 2 = `_2P`,
  3 = `_3P`, plus `_A` (assist, flag `0x08`) and `_W` variants.
* **Chaos (`0x17`) has exactly three rows:** costume 0 `P_ORG200_S_HARD` (key 287), 1 `P_ORG200_NORMAL`
  (285), 2 `P_ORG200_HARD` (286), all flags `0x06`. No `_A`, no `_ZAKO`.
* Feral Chaos (`0x20`): `_ZAKO`, `_1P`, `_2P` (and two `_PUB` rows), no `_A` rows. Aerith (`0x21`): `_1P`..`_3P` and `_A`.
* 255 rows are named `DLC...`: placeholders. Main `0x08847068` overwrites an existing row in place
  (bsearch by key, then 0x44-byte copy), and main has the strings `dlc/obj/dlc_%03doe.bin` and
  `dlc_%03doe.bin`. DLC costumes therefore replace reserved rows rather than append.

Costume availability mask, main `0x088D5E0C(charId)`: bit 1 always, bit 2 and bit 3 from two unlock
flags in the roster table (1.3), bits 4..12 set when `0x088470DC(charId, n, 0)` finds a row for costume
n = 4..12. Main `0x088FB9BC` is the "other colour" map used for mirror matches (1 -> 2, 2 -> 3, else 1).

### 1.3 Master tables

#### In the executables

| table | module / address | shape | indexed by | readers |
|---|---|---|---|---|
| **Roster / unlock table** | main `0x089D7BE0` | 32 rows x 5 s32 (20 bytes), then a row starting `-1` at `0x089D7E60` | scanned for the ID | main `0x088D5AC8`, `0x088D5CE8`, `0x088D5E0C`, `0x088D60E4`, `0x088D617C` |
| Save-slot list A | main `0x089CBC98` | 31 bytes `01..16 18..20` | position = save slot | main `0x088CB3B4` (find), `0x088CB210` (init loop) |
| Save-slot list B | main `0x089CBA68` | 31 bytes, same content | position = ghost-record slot | main `0x088C04F8` |
| Alignment-ordered list | main `0x089CBB7A` | 31 bytes: 18 heroes `01 03 05 07 09 0b 0d 0f 11 13 15 18 1a 1b 1c 1d 1e 1f` then 13 villains `02 04 06 08 0a 0c 0e 10 12 14 16 19 20` (preceded by the 10 original heroes at `0x089CBB70`) | loop counter | main `0x088C2A48`, `0x088C31F4`, `0x088C329C`, `0x088C3368`, `0x088C35C0`, `0x088C365C`, `0x088C3714`, `0x088C4680`, `0x088C4A50` (accomplishment checks over every playable character) |
| Default equipment preset | main `0x089CB7E8` | 35 x u32, `-1` for IDs 0, `0x17`, `0x21`, `0x22` | ID directly | main `0x088B97D0` (from `0x088CBBB4`, `0x088CBCB4`) |
| Voice set codes | main `0x089D6CA8` | 35 x char* | ID directly | main `0x088A3A94` |
| **Select-screen layout** | menu `0x08A864D0` | 33 rows x 6 s16 (12 bytes); row 32 is the Random cell | grid order | menu `0x08A85ECC` (getter), used by `0x08A84FD4`, `0x08A80130`, `0x08A7F2B4`, `0x08A807EC` |
| ID list in first-game order | menu `0x08B22DEC` | 32 x s32 then `-1`: 10 heroes, 10 villains, `0x17`, `0x15`, `0x16`, `0x18..0x20` | position | menu `0x08B1E0E8` (from `0x08B1F5AC`, `0x08B21AF0`; module loads `menu_pk_md_list_*`) |

Roster table rows (main `0x089D7BE0`), columns `{charId, unlockFlag, costume2Flag, costume3Flag, seenFlag}`:

```
01: 2bc 2df 302 e11      ...      16: 2d1 2f4 317 e26
17: 39d  -1  -1 e27   <- boss Chaos: own unlock flag, no alternate-costume flags
18: 2d2 2f5 318 e28      ...      1f: 2d9 2fc 31f e2f
20: 2da 2fd  -1 e30   <- Feral Chaos
-1                       terminator at 0x089D7E60; unrelated data starts at 0x089D7E74
```

Flags are bit indices into the save flag field (1.6), tested as
`*(u32*)(flags + (idx >> 5) * 4) & (1 << (idx & 31))`. For playable characters the unlock flag is
`0x2BC + saveSlot`; the three blocks start at `0x2BC`, `0x2DF`, `0x302`, i.e. **35 apart with 31 used**.
New-game defaults (main `0x088B0788`) set `0x2BC, 0x2BE, 0x2C0, ... 0x2CE` (ten heroes) and
`0x2D2, 0x2D4, 0x2D5, 0x2D6, 0x2D8, 0x2D9` (Kain, Tifa, Laguna, Yuna, Vaan, Lightning).
The table has **no slack**: the word after the terminator row belongs to something else.

Select-screen layout rows (menu `0x08A864D0`), columns `{a, b, column key, d, charId, 0}`:

```
row 0..5   : 1f 1e 1b 1c 18 1a   (Lightning Vaan Laguna Yuna Kain Tifa)
row 6..15  : 01 03 05 07 09 0b 0d 0f 11 13
row 16..25 : 02 04 06 08 0a 0c 0e 10 12 14
row 26..31 : 15 16 1d 19 20 17   (Shantotto Gabranth Prishe Gilgamesh Feral-Chaos Chaos)
row 32     : -1 -1 0c 00 00 00   (Random)
```

Boss Chaos **has a row** (31). The table ends at `0x08A8665C`, where a different table starts.

#### In `system.bin` (container `ARC\x01`, 43 chunks; found in the ISO)

Dispatcher: main `0x088BA88C`. Tags are at main `0x089CB898..`.

| chunk | rows | key | what the loader does with it | per-character content |
|---|---|---|---|---|
| `chrp` | 34 x 0x90, header `{0x21, 0x22}` | u8 char ID, bsearch main `0x088B7080` | **memcpy into fixed BSS** main `0x09B4D4A0` (loader `0x088B70EC`); next BSS object is at `0x09B4E7C8` = exactly `+0x1328` | master parameters, model name at `+0x74`; rows for all IDs 1..34 |
| `chrl` | 31 x 0x10 + bodies | s32 char ID, bsearch main `0x088B6F00` | heap copy (main `0x088B6E60`) | abilities learned per level (used by main `0x088B6F48`). **No row for `0x17`, `0x21`, `0x22`** |
| `amas` | 31 x 0x10 + bodies | s32 char ID | heap copy (main `0x088B1B20`) | keys `1..16, 18..20`. No row for `0x17` |
| `acas` | 31 x 0x10 | s16 char ID | heap | keys `1..16, 18..1f, 21`: the assist roster (no Chaos, no Feral Chaos, includes Aerith) |
| `grwp` | 32 x 0x1C, header `{3, 0x20}` | s16 0..31 (a growth-curve index, not a character ID) | fixed BSS main `0x09B68B58` (loader `0x088C0A44`) | growth |
| `enmp` | 1267 x 0x24, header `{0x11, 0x4F3}` | s16 enemy ID, bsearch main `0x088BBD58` | fixed BSS main `0x09B4E858` (loader `0x088BBDB0`) | opponent presets: `+2` char ID, `+3` costume, `+0x23` assist packed as `id & 0x3F` plus costume in the top 2 bits. 16 rows use `0x17` |
| `eqpm`, `eqaa`, `eqao`, `eqit`, `eqac` | presets | preset ID | `eqpm` fixed BSS main `0x09B59AF8` | equipment/ability presets for built characters |

The fixed-BSS loaders all go through a pointer holder (for `chrp`, `*(0x089D7368)`), so the copy
destination can be changed in one place.

Differences in the `chrp` row for boss Chaos (row 22) against a playable row: bytes `+0x60..+0x65` are
`ff ff ff ff ff ff` (playable rows have a small value and a u16 such as `2b 03`), and the u16s at `+0x6A`
and `+0x6E` are `ff ff`. Of the four u32 at `+0x7C..+0x8B` only the first is set; the rest are zero.

### 1.4 Hard-coded counts and ranges

Roster size 31:

| site | instruction | guards |
|---|---|---|
| main `0x088CB254` | `slti $a0,$s3,0x1F` (stride `addiu $s1,$s1,0x778`) | new-game init of the 31 character records |
| main `0x088C0478` | `slti $a0,$s1,0x1F` (stride `0x57C`) | init of the 31 ghost records |
| main `0x088CB3CC` / `0x088C0510` | `addiu $s1,$s0,0x1F` | end pointer of save-slot lists A / B |
| main `0x088CB110` | `FUN_08993560(p, 0x1F, 0x778, ctor)` | array constructor for the character records |
| nine functions in main `0x088C2A48..0x088C4A50` | `iVar < 0x1f` over list `0x089CBB7A` | accomplishment conditions (min/max/sum over all playable characters) |

ID space 0..34 (arrays of 35, range test `1 <= id <= 0x22`):

| site | what |
|---|---|
| main `0x088A3A94`, `0x088A3C48` | 35 voice sets, 0xC bytes each |
| main `0x088B3004`, `0x088B3060` | **35 x 200-byte per-character statistics records in the save**, plus one total record |
| main `0x088B312C` | 35 x 0x30-byte records in the save (indexed by the opponent's ID) |
| main `0x089009D4`, `0x089010CC` | set 35 bytes at `+0x44` of their object to 1; menu `0x08A80304` / `0x08A80428` read `*(0x089D8140) + 0x44 + id` as a per-ID costume byte |
| main `0x08902734` | 3 x 35 arrays in the same object family |
| main `0x088D5AC8` | local `int[35]`; passes `0x23` as capacity to main `0x08900C90`; callers pass `0x23` as list capacity (menu `0x08A8799C`, `0x08A69E28`) |
| main `0x089293DC`, `0x08928E70`, `0x08929430`, `0x08920978`, `0x088D5638`, `0x088D5A00` | `id < 1 \|\| 0x22 < id` validity tests |
| menu `0x08A80304`, `0x08A80428`, `0x08A804B8`, `0x08A816E0`, `0x08A81740` | same test in the select screen |
| battle `0x08ADC088` | loop `id = 0..0x22` over save records |
| battle `0x08ADCFA0` | statistics update: `stats + charId*200 + 0x4C` and `stats + opponentId*0x30 + 0x1C6C`, where the ID is byte 0 of the record at `char+0x5A4` |

Loops over `1..0x20` (the playable ID range, some with an explicit `0x17` skip):

| site | what |
|---|---|
| main `0x088F3B18` (`ori $s1,$zero,0x17` at `0x088F3B2C`, `slti $a0,$s2,0x21` at `0x088F3B5C`) | zero `record+0xC` and `record+6` for every ID except `0x17` |
| main `0x088F3A7C`, `0x088F4228`, `0x088F42A8` | same shape, per-character helpers `0x088F3B80` / `0x088F414C` |
| menu `0x08A255AC` | scans every playable record's 5 sets for an item |

u32 bit masks keyed by `ID - 1`:

| site | what |
|---|---|
| main `0x088D5CE8` (`addiu $t0,$t0,-1; sllv` at `0x088D5D38`) | mask of unlocked characters; returns `0x40000000` (Lightning only) in the demo configuration |
| menu `0x08A255AC` | `mask \|= 1 << (id - 1 & 0x1f)` |
| menu `0x08AF4B6C` | builds a 31-bit mask into `+0x74` of a record |

IDs 1..32 fit; 33 and 34 alias bits 0 and 1.

Counts by immediate, for triage (not all are roster-related; I checked the ones cited above):
`slti/sltiu 0x23`: main 17 functions, menu 35, battle 8. `0x21`: main 9, menu 11, battle 6.
`0x1F`: main 16, menu 6, battle 7. Reproduce with `build/scratch/roster/imms.py <module> 1f,21,22,23 slti,sltiu`.

Chaos literal `0x17`: main `0x0891CFC4` (two tests on record byte 0), menu `0x08A80130`, `0x08A80304`,
`0x08A80428`, `0x08A804B8`, `0x08A80584`, `0x08A807EC`, `0x08A80930`, `0x08A80A4C`, `0x08A80CD4`,
`0x08A816E0`, `0x08A81740`, `0x08A8255C`, `0x08A83304`, `0x08A84458`, plus about ten more menu functions
outside the select screen, battle `0x08B14D68` (encounter text chosen on objentry `+6 == 0x17`) and
`0x08AEE6DC`.

### 1.5 The select screen and the Chaos restriction

Who can appear: main `0x088D5AC8(out, capacity, includeChaos, includeAerith)` walks the roster table and
emits 8-byte entries `{s32 charId, u16 isNew, u16 costumeMask}` for every row whose unlock flag is set.

```
088D5B9C: ori   $s6, $zero, 0x17
088D5BC0: bne   $a0, $s6, +4          ; id != 0x17 -> normal path
088D5BC8: lw    $a1, 0x8C($sp)        ; third argument & 0xFF
088D5BCC: beqz  $a1, -> next row      ; Chaos skipped unless includeChaos
```

Menu `0x08A8799C` passes `includeChaos = 0`; menu `0x08A69E28` passes 1 (both then hand the list to the
select-screen constructor menu `0x08A80D8C`). There are 13 call sites of `0x088D5AC8` in menu and one in
main; no other function walks the roster table to build a list.

How the grid is built: menu `0x08A84FD4(screen, mode)`.

* clears 40 cells: `08A85078: slti $t0,$a3,0x28` (8-byte cells at workspace `+0x678`:
  `{u16 flags, s16 sprite, s16 layoutRow, u16 level}`; s16 list of layout rows at `+0x628`);
* walks layout rows 0..31 in table order: `08A85100: slti $a0,$s4,0x20`;
* a row is kept if its ID is in the caller's available list **and** menu `0x08A80A4C` returns 0;
* appends pseudo cells `0xFFFE`, `0xFFFD` or row `0x20` (Random) depending on mode;
* splits into grid rows by count: `slti 0x1B` / `0x11` / `0x7` at `0x08A8522C / 0x08A85244 / 0x08A8525C`
  (rows of 6, 10, 10, remainder), last row centred with a table at menu `0x08A85BF8`.

The second Chaos test, menu `0x08A80A4C` (whole function):

```
08A80A4C: ori  $a2,$zero,0x17
08A80A50: bne  $a1,$a2, -> 08A80A7C     ; not Chaos: return 0 (delay slot: ori $v0,$zero,0)
08A80A58: lw   $a0,0x28($a0)
08A80A5C: lw   $a0,0x0($a0)             ; screen mode
08A80A60: ori  $a1,$zero,0x1
08A80A64: beq  $a0,$a1, -> 08A80A78     ; mode 1
08A80A68: ori  $v0,$zero,0x1            ; (delay slot) provisional result 1 = blocked
08A80A6C: ori  $a1,$zero,0x3
08A80A70: bne  $a0,$a1, -> 08A80A7C     ; not mode 3: return 1
08A80A78: ori  $v0,$zero,0x0            ; mode 1 or 3: return 0
08A80A7C: jr   $ra
```

It blocks ID `0x17` unless the mode word is 1 or 3.

Other player-side special cases in the same module, all on `id == 0x17`: level shown as 1 and the save
record never read (`0x08A80130`); costume getter/setter return 0 (`0x08A80304`, `0x08A80428`); set cycling
refused (`0x08A804B8`, `0x08A816E0`, `0x08A81740`); a form index is kept at workspace `+0x7D8`, chosen at
random 0..2, cleared, or set explicitly by menu `0x08A8161C` (this is the costume index of 1.2).

**The cheat.** "Unlock Chaos for Select" from the `ULUS10566.ini` linked at
<https://git.nixnet.services/lethe/mayvaneday/raw/branch/pages/blog/2025/04/ULUS10566.ini>
(article: <https://git.nixnet.services/lethe/mayvaneday/raw/branch/pages/blog/2025/04/dissidia.html>):

```
_L 0xE1FF0001 0x11348C74      if u8[0x09B48C74] != 1 (main BSS, written by main 0x0887B5A8/0x0887B648)
_L 0xE11A0003 0x1136D838      if u8[0x09B6D838] != 3 (BSS written only by menu 0x089E15F8/0x089E1BB0)
_L 0xE1040002 0x00F4DF9A      if u8[0x0974DF9A] == 2:
_L 0x20F4E0AC 0x00010020        u32[0x0974E0AC] = 0x00010020
_L 0x20F4E0A8 0x00200001        u32[0x0974E0A8] = 0x00200001
_L 0x20F4E0A4 0x0001001F        u32[0x0974E0A4] = 0x0001001F
_L 0x20F4E0A0 0x001F0001        u32[0x0974E0A0] = 0x001F0001
_L 0xE1050002 0x00EFCA1A      if u8[0x096FCA1A] == 2:
_L 0x10EFC9E6 0x0000001F        u16[0x096FC9E6] = 0x001F
_L 0x10EFCB2C 0x0000FFFE        u16[0x096FCB2C] = 0xFFFE
_L 0x20EFCB28 0x00200001        u32[0x096FCB28] = 0x00200001
_L 0x10EFCB24 0x0000001F        u16[0x096FCB24] = 0x001F
_L 0x20EFCB20 0x001F0001        u32[0x096FCB20] = 0x001F0001
  (two more blocks of the same shape at 0x09706Fxx/0x097070xx and 0x09705Cxx/0x09705Dxx, guard value 0x0B)
```

All data writes are in the heap, not in code or static tables. They fit the workspace layout above
exactly: taking `W = 0x096FC380`, `0x096FC9E6 = W + 0x628 + 2*31` (list entry 31 := layout row `0x1F`,
which is the Chaos row) and `0x096FCB20 = W + 0x678 + 8*37` (cell 37 := `{flags 1, sprite 0x1F,
row 0x1F}`), with cell 38 rewritten as the displaced pseudo cell. The first block gives `W = 0x0974D900`
with the same cell index 37, and in both blocks the guard byte is at `W + 0x69A`. So the cheat inserts the Chaos layout row into an already-built grid at
fixed heap addresses for four screen instances; it does not touch the two gates.

Other codes from public lists that locate battle state (PPSSPP forum,
<https://forums.ppsspp.org/showthread.php?tid=7178>): pointer codes through main `0x08B633B0` and
`0x08B633E0` with offsets `+0xE44` (assist), `+0xE48` (EX), `+0xE4C` (bravery), `+0xE60` (level).

### 1.6 Save data

On disk: `DISSIDIA.BIN` is 257,240 bytes = `0x3ECC8 + 0x10`. Both baseline copies have 7.999 bits/byte
entropy and do not start with the plaintext header, so they are encrypted by the savedata utility and
were not readable. Everything below is from code.

In memory: the image is at save root `+0xF08`, length `0x3ECC8`, ending at root `+0x3FBD0` =
main `0x08BEE908`, which is the guard byte of the next singleton (set up in main `0x088769CC`).
**There is no gap after it.**

Header, written by main `0x088AB800` and checked by main `0x088AB7C0` and `0x088AC6CC`:

```
088AB7C0: lw   $a2,0xF08($a0)
088AB7C4: ori  $a3,$zero,0xA
088AB7C8: bne  $a2,$a3, -> set flag 0x20            ; version must be 10
088AB7D0: lw   $a2,0xF0C($a0)
088AB7D4: lui  $a3,0x4 ; addiu $a3,$a3,-0x1338      ; 0x3ECC8
088AB7DC: bne  $a2,$a3, -> set flag 0x20            ; size must match
```

| image offset | value | meaning |
|---|---|---|
| `+0x00` | 10 | version |
| `+0x04` | `0x3ECC8` | total size |
| `+0x08..+0x3C` | `0x1CEAC, 0x80, 0x104, 0x280, 0x1BD0, 0x1A160, 0x1864, 8, 0x290C, 0x6A8, 0x16C, 0xC80, 0xB4, 0x590` | 14 section sizes |
| `+0x40..+0x4F` | (zeroed) | the only unassigned header bytes |
| `+0x50` | | section 1 starts (root `+0xF58`) |

Sections are contiguous: section 1 ends at root `+0x1DE04`; section 4 (0x280 bytes) is the flag field at
root `+0x1DF88` (main `0x088B0AD8` stores `root + 0x1DF88` into `0x089D726C`; 0x80 words = 4096 flags,
then 0x20 more words); section 6 starts at root `+0x1FDD8`; section 7 at `+0x39F38`. Code addresses
these with literal offsets from the root pointer (`+0xF58`, `+0xF6E0`, `+0x10D38`, `+0x12958`,
`+0x133FC`, `+0x1DF88`, `+0x1FDD8`, `+0x39F38`, `+0x3B798`, `+0x3B7A4`, ...) in all three modules.

Section 1 (`0x1CEAC` bytes), from main `0x088CB110` / `0x088CB210`:

| offset in section | size | content |
|---|---|---|
| `+0x00000` | 31 x `0x778` = `0xE788` | **character records**, slot order = list A |
| `+0x0E788` | `0x160C` | inventory block (main `0x088B8634`) |
| `+0x0FD94` | `0x2644` | statistics: 19 u32, **35 x 200** per-ID records, 1 total record, **35 x 0x30**, 19 x 0x2C, 1 u32 |
| `+0x123D8` | `0xCC` | 100 x 2 bytes + 4 |
| `+0x124A4` | 31 x `0x57C` = `0xAA04` | per-character play-style records, slot order = list B; each is 5 x `0x118` + u32 |
| `+0x1CEA8` | 4 | u32 |

Character record (`0x778` bytes; initialiser main `0x088CBA10`, new-game fill main `0x088CBBB4`):

| offset | meaning | evidence |
|---|---|---|
| `+0x000` s8 | character ID, `0xFF` when blank | `0x088CBA10`, `0x088CBA54` |
| `+0x001` u8 | level 1..100 | `0x088CB408` clamps to 100 |
| `+0x002` u8 | 0..150 value picked from an 11-entry table at main `0x089CBCC8` | `0x088CBA74` |
| `+0x006` u16, `+0x00C` u32 | zeroed together for every character by `0x088F3B18` | |
| `+0x010` | 5 equipment sets x `0xC6` | `0x088CBBB4` loop of 5; `0x08A255AC` |
| set `+0xA2` | accessory slots (s16 each, count from record `+0x775`) | battle `0x08ADC088` |
| set `+0xB8`, `+0xB9` | assist character ID, assist costume | `0x088F9BB0` |
| set `+0xC5` | level override, 0 = use `+0x001` | `0x088CBAD0` |
| `+0x3EE` | 150 x 6-byte ability entries, `-1` terminated, flags at `+4` | `0x088CBBB4` |
| `+0x773` s8 | current set 0..4 | `0x08A804B8` |
| `+0x774` s8 | costume index | `0x088F9BB0`, `0x08A80304` |
| `+0x775` s8 | accessory slot count, capped at 10 | `0x088CBD24` |

The accessor, main `0x088CB164(base, charId)`:

```
088CB174: jal   func_88CB3B4            ; slot = index of charId in list A, or -1
088CB180: bltz  $a0, -> fallback
  ...                                    ; v0 = base + slot * 0x778
088CB1AC: lui $a0,0x9AF ; lw $a0,-0x508($a0)   ; guard at 0x09AEFAF8
088CB1C8: jal   func_88CB9E8                   ; construct blank record once
088CB1D8: addiu $v0,$v0,-0x504                 ; return 0x09AEFAFC
```

It has about 60 call sites across the three modules. `0x088CB1EC` ("has a slot") has 5.

Main `0x088AC6CC` passes a version-10 image through a menu routine (`0x08A488C0`); menu `0x08A48DB0` and
`0x08A491E0` are near-identical importers that fill character records from two other record layouts
(per-record fields at `+0xB18` / `+0x968`, ability lists at `+0x660` / `+0x4B0`).

### 1.7 From menu choice to battle: the structures that carry identity

* **Battle request** (`*(0x089D8508)`): per side 0xEA8 bytes, entries of 0x2E8; entry `+0x68` = source
  type 0..9, `+0x78..` = arguments. Consumed by main `0x0891CFC4`:
  type 0 = own save record (`0x088F9BB0(side, charId, _, cpuFlag, assistId, assistCostume)`),
  1 and 9 = `enmp` row (`0x088F9F88`), 4 = `(charId, costume, assistId, assistCostume)`
  (`0x088FAA4C` / `0x088FAB10`), 5 = built CPU character (`0x088FA2AC` -> `0x088B3B98`),
  6 = stored record at `*(0x089D7F38) + side*0x6A68 + n*0x1548 + 0x70C` (`0x088FABA4`),
  7 = compact record (`0x088FA5F4`), 3 = object ID (`0x088FA9A8`), 2 and 8 = other builders.
* **Per-side battle setup** at main `0x09AFB8A0 + side * 0x1060`: `+0x000` the 0x778 record,
  `+0xF34` controller type, `+0xF3A` **object ID**, `+0xF3C` enemy-table ID, `+0xF3E` assist object ID,
  `+0xF40` play-style record (0x118), `+0x1058..+0x105B` char ID, costume, assist ID, assist costume.
  The option flags at `0x09AFD960` named in `docs/research/battle-camera.md` start right after the two
  side blocks (`0x09AFB8A0 + 2 * 0x1060`).
* Main `0x0888C4C8` copies `+0xF3A` / `+0xF3E` into the battle manager (`*(0x089D6A10)`) at `+0x7D8[side]`
  and `+0x7F0[side]`; main `0x0888BA0C` turns them into the `charData` argument of the character
  constructor with `0x088470B8`. So `charData` in the camera document is the **objentry row**.
* Main `0x0888C4C8` also hands both sides' full 0x778 records to battle `0x08AF2CBC` and then to main
  `0x088EBDD4` (object `*(0x089D7ED0)`), next to the mode and stage words.

### 1.8 Other systems

* **Statistics / records screen:** the menu function that calls `0x08A3D654` reads `root + 0x10D38 + id*200` and
  `root + 0x12958 + id*0x30` and iterates 0x25 rows with a table at menu `0x08A40720`. Indexed by ID.
* **Accomplishments:** the nine main functions in 1.3 iterate list `0x089CBB7A`.
* **Opponent presets** (story, arcade, labyrinth opponents): `enmp` rows; assist ID is 6 bits there.
* **Compact character records** (`0x088FA5F4`, `0x088B9E84`): `{s16 charId, s16 costume, s16 level, s16 x,
  ..., 0xC6-byte set, 4 x 0x80 blocks}`; the ID is a full s16.
* **Voice:** `voice_load_p_%s.bin` per ID from table `0x089D6CA8`.

---------------------------------------------------------------------------------------------------

## 2. INFERENCES (not proven statically)

* **Why cheat-Chaos is glitchy.** With the grid poked, the player side most likely goes through the
  ordinary own-save path. `0x088CB164` then returns the blank record: ID byte `0xFF`, level 1, no
  abilities, no equipment. That matches the article's list: level shown as 0 / 1 / ??? / 255, ability
  editing soft-locks (no `chrl` / `amas` row to list), missing gauges. I did not trace which request type
  the menu emits for side 0 in that state.
* **Possible save damage from the cheat.** Battle `0x08ADCFA0` indexes the statistics block with record
  byte 0. If that byte is `0xFF` the write lands at `stats + 0x4C + 255*200`, which is inside the
  play-style array (around slot 29). Whether the byte is still `0xFF` at that point needs a runtime check.
* **The roster table's fifth column** is a "seen" flag (cleared state shows the NEW marker); read off
  `0x088D60E4` / `0x088D617C` only.
* **Spare unlock flags.** The 35-wide spacing of the three flag blocks suggests indices `0x2DB..0x2DE`,
  `0x2FE..0x301`, `0x321..0x324` are unused. Not checked against every flag user.
* **Record fields** `+0x002`, `+0x006`, `+0x00C` are probably CPU strength, a counter and EXP.
* **The 31 x 0x57C array** is per-character recorded play style (used when a saved character is fought
  as CPU: `0x088F9BB0` copies one 0x118 block into setup `+0xF40` when its flag argument is 1).
* **Replays embed their characters.** The call into `0x08AF2CBC` / `0x088EBDD4` with both 0x778 records
  looks like the recorder start, which would make a replay self-describing (ID in record byte 0).
* **Menu `0x08A48DB0` / `0x08A491E0`** are the first-game and demo save importers.
* **ID 34 (`P_ORG800`)** has a model row, a weapon row and a `chrp` row but nothing else. Unidentified.
* **Menu table `0x08B22DEC`** belongs to a list screen (`md_list`); purpose not established.
* A community ISO mod ("Dual Next") is reported to make Chaos and manikins playable
  (<https://web.archive.org/web/20250330024631/https://gamergen.com/telecharger/dissidia-duodecim-dual-next-mod-49227>).
  Not examined.

---------------------------------------------------------------------------------------------------

## 3. ASSESSMENT

### (a) Boss Chaos selectable and playable: smallest

Two levels.

*Cheat-equivalent, done properly (small):* remove the two gates instead of poking heap.
Main `0x088D5BCC` (`beqz $a1`) -> `nop`; menu `0x08A80A68` (`ori $v0,$zero,1`) -> `ori $v0,$zero,0`.
Unlock flag `0x39D` must be set. Chaos already has a layout row, model rows for three forms, a voice
entry, a `chrp` row and a slot in the 35-wide statistics arrays, and bit 22 of the ID masks is unused.
The menu overlay is loaded on demand, so its patch has to be applied to the overlay file or after load.

*Properly playable (medium):* blockers, each verified above:

1. No save slot. `0x088CB164` returns the shared blank record. Either give ID `0x17` a real record
   (see the save problem in (b)) or route the player side through the CPU-character builder
   (`0x088FA2AC`) so the record at least has an ID, level and abilities.
2. No `chrl` and `amas` rows, `chrp` fields at `+0x60..+0x65`, `+0x6A`, `+0x6E` blank, default preset `-1` in
   `0x089CB7E8`. These are data, but they are what the customise menu lists.
3. About 15 `id == 0x17` branches in the select screen that hard-wire level 1, no costume, no sets.
4. Not an assist: no `acas` row, no `_A` object rows.
5. Loops over 1..0x20 that skip `0x17` (`0x088F3B18` and three siblings).
6. Battle-side behaviour of a human-controlled Chaos (HUD, victory camera) is the other investigators'
   area; the article reports defects there.

### (b) A 32nd slot that reuses an existing character's data: large

Everything in (a) that is about "an ID without a slot", plus:

1. **No free ID.** 0..34 are all taken and 35 does not fit: the 35-wide arrays (statistics inside the
   save, voice sets, the `+0x44` flag bytes, capacity `0x23`), the `id <= 0x22` tests (about 60 sites
   by immediate count) and the `ID - 1` bit masks (32 bits). Reusing ID 34 avoids the arrays and range
   tests but still falls outside `1..0x20` loops and aliases bit 1 in the masks.
2. **Save.** 31 records, no slack, size and version checked, image followed directly by another object,
   later sections addressed by literal offsets in three modules. A 32nd record cannot be inserted.
   Realistic options: keep it outside the image (own buffer, own file or an appended block with a new
   version and a migration path), and special-case it in `0x088CB164` / `0x088C04B4`. An unpatched game
   rejects any image whose version or size differs; a patched game needs code to accept version 10.
3. Three 31-byte lists and their `0x1F` loop bounds (at least 13 sites).
4. Roster table `0x089D7BE0`: no room for a 33rd row; relocate and repoint five readers.
5. Select layout `0x08A864D0`: no room; relocate, and raise `slti 0x20` at `0x08A85100` and the
   `< 0x21` row tests. The 40-cell workspace has room.
6. `chrp` must stop being copied into its exact-fit BSS buffer. `objentry` has 24 free rows.
7. Every place battle code selects behaviour by character ID must treat the new ID as the donor's.

**Cheaper alternative worth stating:** a character that is "an existing fighter with different looks" is
what the engine calls a costume. Costume slots 4..12 and 255 placeholder object rows already exist, and
the costume mask picks them up automatically. That needs no roster, ID or save change.

### (c) A 32nd slot with its own data: largest

All of (b), plus a full `chrp` / `chrl` / `amas` / `acas` / preset data set, object rows and files,
voice set, texts and portraits, `enmp` rows if it should appear as an opponent, and the moveset work
covered by the other investigators.

---------------------------------------------------------------------------------------------------

## 4. NOT DETERMINED / NEEDS A RUNTIME CHECK

* Contents of a real save (encrypted on disk). Dump root `+0xF08` in a debugger to confirm the header
  words and record layout.
* Which request type and which record the menu produces for a player-side Chaos; whether record byte 0
  is `0xFF` in battle; whether the statistics write then corrupts the play-style array.
* What the two static guard bytes in the cheat (`0x09B48C74`, `0x09B6D838`) represent.
* Meaning of select-screen mode values 0..5 (only "Chaos allowed in 1 and 3" is established).
* Where unlock flags are set by the PP catalog, and whether the apparent spare flag indices are free.
* Story-mode party tables, labyrinth and arcade line-ups, friend-card and ad-hoc record formats beyond
  the builders listed in 1.7. `party_command.bin`, `ptcommand.bin`, `friend_card.bin` and the `fcrd`
  chunk were not parsed. Arcade's allowed list comes from main `0x08901568` via `0x08900C90`.
* Whether replays really are self-contained, and their file format.
* The full triage of the `0x1F` / `0x21` / `0x23` immediates (counts in 1.4) and of ID-indexed tables in
  the overlays' data. I found the tables listed in 1.3 by content search; tables stored in other widths
  or orders may exist.
* Identity of ID 34.
* `chrp` column meanings beyond ID (`+0`), side (`+3`), series (`+9`) and model name (`+0x74`).
* The `objentry.bin` and `system.bin` ISO offsets (`0x16C8C0A0`, `0x16C9F770`) are specific to the local dump.

---------------------------------------------------------------------------------------------------

## 5. Reproducing

Scripts in `build/scratch/roster/` (run with `.venv/Scripts/python.exe`): `q.py` (refs, hex, pseudocode),
`ptrtab.py`, `tab16.py`, `findbytes.py`, `imms.py`, `ctx.py`, `sysbin.py save` (writes the 43 chunks to
`sysbin/`), `rows.py`, `keys.py`, `objentry.py [chars|all]`, `savecheck.py`.

Web sources: the two git.nixnet.services URLs and the PPSSPP forum thread above. The almarsguides
CWCheat pages for this game (USA, Europe) list no character codes.
