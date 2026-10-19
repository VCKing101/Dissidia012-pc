# Dissidia 012 (ULUS10566 v1.00): how a playable character's assets are stored and loaded

> Static-analysis report produced by a research subagent on 13 October 2026 and kept as written. Nothing in it has
> been confirmed at runtime.

Scope: storage, formats and loading only. Roster/save data and the moveset semantics are covered by other investigators.
All addresses are in the **main** executable unless a module is named. Helper scripts and extracted samples are under `build/scratch/assets/` (git-ignored); nothing tracked was modified and no emulator was run.

Helper scripts (run from the repo root with `.venv/Scripts/python.exe`):

| Script | Purpose |
| --- | --- |
| `build/scratch/assets/pkg.py list [--magic]` / `get <index or 0xhash or name> [out]` / `hash <name>...` | PACKAGE_INFO/PACKAGE.BIN reader and extractor, reads straight from the ISO |
| `build/scratch/assets/containers.py <file>` | lists `MPK ` and `ARC\x01` containers |
| `build/scratch/assets/objentry.py` | dumps `objentry.bin` to `objentry.tsv` |
| `build/scratch/assets/gmo.py <file> [--motions] [--tree N]` | GMO chunk walker (bones, meshes, vertices, textures, motions) |
| `build/scratch/assets/survey_gmo.py` | runs the GMO walker over every object model, writes `gmo_survey.tsv` |
| `build/scratch/assets/names.py` | resolves hashes to names from tables and executable strings, writes `names.tsv` |

---

## 1. Archive format

### Verified facts

**PACKAGE_INFO.BIN** (132,532 bytes) = 16-byte header + 11,043 records of 12 bytes (16 + 11,043 x 12 = 132,532 exactly).

| Offset | Size | Value / meaning |
| --- | --- | --- |
| 0x00 | 4 | `0x20090218` (looks like a date stamp) |
| 0x04 | 4 | `"pack"` |
| 0x08 | 4 | entry count = 0x2B23 = 11,043 |
| 0x0C | 4 | 0 |
| 0x10 + 12n | 4 | name hash, records sorted ascending by this value |
| +4 | 4 | low 20 bits = start sector in PACKAGE.BIN, in 2,048-byte units (high 12 bits are 0 in every record) |
| +8 | 4 | low 28 bits = byte size; high 4 bits = category 0..3 |

Evidence:
- Loader `0x08871938` reads `package_info.bin` (up to 0x20F8C bytes) to `0x09B26BC0` and opens `package.bin`. Count is read from `0x09B26BC8`, records from `0x09B26BD0` with a 12-byte stride (`0x08871684`).
- Lookup `0x088713E4`: hashes the name with `0x08871288`, then binary-searches the records on word 0. Files are found **by hash of name only**; no names are stored in the index.
- Read `0x088714F4` / `0x08871718` / `0x08871818`: seek to `(word1 & 0xFFFFF) << 11`, read `word2 & 0x0FFFFFFF` bytes.
- Hash `0x08871288`: CRC-32, polynomial 0x04C11DB7, initial 0xFFFFFFFF, MSB first, no reflection, each byte folded to lower case first (ctype table at `0x089CF591`), final bitwise NOT. That is CRC-32/BZIP2 of the lower-cased virtual path.
- Hash verified against real entries: `sound/snd_menu.scd` -> 0xE68DB6D2 (index 9939), `obj/R_SYM100.gmo` -> 0xEAD4F4A2 (index 10114), `language.bin` -> 0x58044513 (index 3810, sector 0), `general_archive/main/main.bin` -> 0x93307C1C (index 6409, sector 4), `obj/p_gst130.objx` -> 0x17E5D0C8 (index 1026). Names with a language directory are rewritten first: `text/JP/common/pause_help.bin` misses, `text/EN/common/pause_help.bin` hits (substitution in `0x08867984`, language codes at `0x089C940C`: EN JP FR DE IT ES RU, plus `ZQ` and `M_` placeholders).
- Layout check: sorted by sector the 11,043 extents are contiguous with no gaps and no overlaps; last entry ends at byte 1,034,357,568 of the 1,034,358,784-byte PACKAGE.BIN. Total payload 1,023,031,706 bytes.
- **No compression and no encryption**: every entry starts with a plain magic. Count by magic: RIFF/WAVE 6,173 (215.8 MB), `ARC\x01` 2,060 (124.5 MB), `MIG.00.1PSP` 993 (29.6 MB), `OMG.00.1PSP` 779 (536.1 MB), `MPK ` 187 (36.8 MB), `menu pak` 142 (39.7 MB), `SSCF` 131 (22.4 MB), `DES4` 55, `mlng` 45, `VOLD` 32, `mess` 25, `SEQ ` 17, PNG 9, small tables for the rest.
- Category nibble: 0 = 7,010 entries (190.8 MB), 1 = 1,547 (506.5 MB), 2 = 1,510 (78.8 MB), 3 = 976 (246.9 MB). `0x0886FB98` treats category 0 as "never in the data install" and accepts a non-zero category only if it is <= the installed level at `0x08B63804`. So the nibble is the data-install tier.

**File-manager front end** (global object pointer at `0x089D5EA0`: +4 device manager, +8 package reader `0x089D5FF0`, +0xC install cache `0x089D5FB4`):
- `0x088FE708(name, buffer, size, direct)` and `0x088FE7C8(name, direct)` (size query) are the calls used by the object loader. Order: language rewrite -> optional in-memory preload cache (`0x08883538`, enabled by `0x089D69DC`) -> if `direct == 0`: data-install cache (`0x088701F8`), else PACKAGE.BIN by hash (`0x08862854` -> `0x088714F4`); if `direct != 0`: device manager vtable `0x089C8FE0` slot +0x3C (`0x0886402C`), which reads a loose `.edat` from the memory stick (section 7).

**Nested containers** (all verified by parsing real files):
- `MPK ` : `"MPK "`, u32 `0x20070606`, u32 count, u32 0; then count x {u32 name offset, u32 data offset, u32 size, u32 0}; then NUL-terminated names. Sub-files are found by name (`0x08869AB4`). Example: `general_archive/main/main.bin` holds `battle.bin`, `objentry.bin`, `system.bin`, `mapentry.bin`, `resident.frr`, `snd_menu.scd`, `assist.bin`, ...
- `ARC\x01`: `"ARC\x01"`, u32 count, 8 zero bytes; then count x {u32 chunk type, 4-char tag, u32 offset, u32 size}. `0x0885C93C` relocates offsets to pointers in place; `0x0885CA3C(arc, type, n)` finds the n-th chunk of a type, `0x0885CA98(arc, type, tag, n)` also matches the tag. Used for `battle.bin`, `system.bin` and every `.objx`.
- `menu pak`: `"menu pak"`, u16 1, u16 count, then {u32 offset, u32 size} pairs; members (GIM images, sequences) are addressed **by index**, no names.
- Others seen but not parsed: `mess`/`mlng` (text), `DES4`, `SEQ `, `FRR`.

### Inference
- The header word `0x20090218` is a format date; not used by any code I read.
- PACKAGE_INFO capacity: the load call passes 0x20F8C bytes and the next referenced global is at `0x09B47B50` (= buffer + 0x20F90), so the stock buffer holds at most (0x20F8C - 16) / 12 = **11,253 records, 210 more than retail**. Sector field is 20 bits (2 GiB of archive), size field 28 bits (256 MiB per file).

### Community cross-check
- A published hash-to-name list exists: `PACKAGE_INFO.json` in https://github.com/hearhellacopters/dissidia_filelist names 10,848 of the 11,043 ULUS10566 entries and includes an extractor/rebuilder. Its README notes that a PPSSPP log breakpoint on `0x08871288` printing `{a0:s}` reveals every name the game asks for. My own resolver named 4,330 entries (754 MB of 1,023 MB) from code and data alone, which was enough for this report.

---

## 2. What files make up one playable character

### The object table (verified)

`objentry.bin` (79,568 bytes, inside `general_archive/main/main.bin`; loaded to `0x09AFEE58` by `0x08847158`, buffer 0x13D30 bytes): u32 `0x0C`, u32 count = 1,170, then 0x44-byte rows sorted by id.

| Row offset | Size | Meaning | How established |
| --- | --- | --- | --- |
| 0x00 | s16 | object id (bsearch key, `0x08846C98` / compare at `0x08846C0C`) | code |
| 0x02, 0x04 | s16 x2 | unknown; 0x04 is the id of the matching `D_` object for full costumes (Cloud 200 = `D_SEV100`, Lightning 545 = `D_GST130`) | data |
| 0x06 | s8 | character id (1..34, 0 = none) | code `0x08846CCC` |
| 0x07 | s8 | costume number (1, 2, 3; 0 = manikin; 4..12 reserved for DLC) | code, menu `0x08A32284` |
| 0x08 | u8 | flags: 0x08 = assist-type row, 0x01/0x04 manikin, 0x10 selects `_01` voice bank | code |
| 0x09 | u8 | "load from memory stick" flag (0 in all 1,170 retail rows) | code `0x08845230`, `0x08844BC4` |
| 0x0A | 18 | unknown byte list, 0xFF padded | data |
| 0x1C | s16 x4 | attached object ids, loaded with the owner (`0x08844C14`) | code |
| 0x24 | char[16] | model name, e.g. `P_GST130_1P` | code |
| 0x34 | char[16] | data name, e.g. `p_gst130` | code |

Name prefixes in the table: `P_` 387 (battle characters), `Z_` 309 (cutscene models), `DLC_001..DLC_255` 255 (empty placeholder rows, ids 704..958), `F_` 94 (stages), `W_` 54 (separately loaded weapons/summons), `R_` 35 (world map), `D_` 33 (per-character camera rigs), `C_` 3.

Character codes (string table `0x089CAD40..0x089CAE40`, pointer array of 35 slots at `0x089D6CA8`, ids 1..33): one100, one200, two100, two200, thr100, thr200, for100, for200, fiv100, fiv200, six100, six200, sev100, sev200, eht100, eht200, nin100, nin200, ten100, ten200, gst100, gst200, org200, for110, fiv210, sev110, eht110, ten110, gst110, gst120, gst130, org210, sev120. Id 34 is `org800` (no voice slot). Mapping to people is by position in the series (sev100 = Cloud, gst130 = Lightning, org200 = Chaos, org210 = Feral Chaos, sev120 = Aerith): inference from naming plus texture names, consistent with community tools.

### File names (verified from the loader `0x08845230` and by hash hits)

For an object row: `obj/<name>.gmo`, `obj/<name>.cosx`, `obj/<name>.exex`, `obj/<name2>.objx` (format string `obj/%s.%s` at `0x089C88D8`, extensions at `0x089C88A8`, `0x089C8898`, `0x089C88A0`, `0x089C8850`). Voice: `voice/<LANG>/battle/voice_<CODE8><suffix>.scd` where CODE8 is the 8-character code at +0x74 of the character's 144-byte `chrp` record (`system.bin`, e.g. `P_GST130`) and the suffix is read from offset 0x3C of the `.cosx` file (`_00` in every sample), plus `voice/<LANG>/voice_load_<CODE8>.bin`.

### Lightning (gst130, character id 31) and Cloud (sev100, id 13), English files only

| Asset | Files (Lightning) | Magic / format | Bytes L | Bytes C |
| --- | --- | --- | --- | --- |
| Battle model + skeleton + **all animations**, one per costume | `obj/P_GST130_1P.gmo`, `_2P`, `_3P`, `_ZAKO` (manikin) | `OMG.00.1PSP` (Sony GMO) | 1,741,984 / 1,741,984 / 1,723,768 / 1,705,996 | 2,017,616 / 2,014,724 / 2,025,708 / 1,915,708 |
| Assist-version model (reduced motion set) | `obj/P_GST130_1P_A.gmo`, `_2P_A`, `_3P_A`, `_ZAKO_A` | GMO | 580,312 / 580,320 / 561,632 / 545,244 | 580,748 / 548,700 / 587,884 / 545,708 |
| World-map model (6 motions) | `obj/P_GST130_1P_W.gmo` | GMO | 433,032 | 323,740 |
| Camera rig for the character's scripted cameras | `obj/D_GST130.gmo` (also embedded in the objx) | GMO, bones + 9 motions, no mesh | 17,440 | 23,960 |
| Battle data archive | `obj/p_gst130.objx` | `ARC\x01`, 19 chunks | 1,639,664 | 1,352,328 |
| Assist battle data | `obj/p_gst130_a.objx` | `ARC\x01` | 284,364 | 309,532 |
| World-map data | `obj/p_gst130_w.objx` | `ARC\x01` | 73,724 | 68,124 |
| Costume parameters | `obj/P_GST130_<costume>.cosx` x4 | 68-byte table, voice suffix at 0x3C | 68 each | 68 each |
| EX-mode parameters | `obj/P_GST130_<costume>.exex` x4 | table of 80-byte records (3 for Lightning, 1 for Cloud) | 248 each | 88 each |
| Battle voice bank | `voice/EN/battle/voice_P_GST130_00.scd`, `..._zako.scd`, `..._a_00.scd` | `SSCF` (47 clips, 16 kHz in the main bank) | 291,504 / 291,504 / 60,384 | 214,224 / 213,136 / 58,864 |
| Streamed voice list | `voice/EN/voice_load_P_GST130.bin` | `VOLD`, 24-byte rows {char[16] name, u32 id, 2x u16} | 1,648 (68 rows) | 1,936 (80 rows) |
| Streamed voice clips | `voice/EN/battle/li_battle_NN.at3` etc. | RIFF WAVE, format tag 0x0270 (ATRAC3), mono 44.1 kHz | 66 files, 953,112 | 72 files, 937,632 |
| Character-select art | `menu/JP/character_select/chara2/p_gst130_1p.gim` | GIM 256x256 8-bit indexed | 66,848 | 66,848 |
| Battle HUD portrait per costume | `menu/JP/battle/chara_image/P_GST130_1p.gim`, `_2p`, `_3p` | GIM 128x128 8-bit | 17,696 each | 17,696 each |
| Name plate | `menu/EN/common/chara_name/p_gst130_name.gim` | GIM 128x32 | 5,408 | 5,408 |
| Encounter caption | `menu/EN/battle/encount/encount_text_tim2/encount_gst130.gim` | GIM 256x64 | 17,700 | 17,700 |
| Story-mode icon | `menu/JP/story_mode/chara2/p_gst130_1p.gim` | GIM 64x64 | 5,408 | 5,408 |
| Arcade ending art | `menu/JP/arcademode/arcade_last_chara_tim2/arcade_last_gst130.gim` | GIM 480x272 (Cloud 512x512) | 131,876 | 263,460 |

Totals of the files listed: Lightning about 13.2 MB, Cloud about 13.9 MB; roughly 9.6 to 10.6 MB of that is ten GMO files, because **every costume file carries its own full copy of the animation set**.

Note the art directories: several per-character images live under a literal `menu/JP/` path on the US disc (not language-substituted), others under `menu/EN/`.

Contents of `obj/p_gst130.objx` (chunk type, tag, bytes), verified by parsing; meanings marked (code) come from `0x088459E0`, the rest are labels only:

| Type | Tag | Bytes | Content |
| --- | --- | --- | --- |
| 7, 8, 5, 6 | `p_gs` | 1,208 / 27,644 / 2,992 / 664 | per-object tables loaded by type id (code); type 7 looks like a motion id table, type 8 an offset-indexed script/table set (guess) |
| 9 | `p_gs` | 12,670 | begins `hibg`; hit/body geometry (guess) |
| 4 | `effe` | 1,151,904 | `MPK ` with `p_gst130.fep` (`FEP`, 1.09 MB effect pack), `.feb` (`FEPA`), `.fxseb` (`FXSE`) |
| 14 | `snd_` | 363,360 | `SSCF` sound-effect bank |
| 1 | `plya`, `comb`, `atkp`, `magi`, `brat`, `text` | 6,852 / 1,812 / 5,788 / 11,508 / 876 / 2,006 | player-action, combo, attack-parameter, projectile (`MAGI`), bravery-attack tables and `mlng` text. These are the moveset data (other investigator) |
| 3 | `wind`, `spec` | 2,332 / 9,504 | GIM images |
| 17, 13 | `spec` | 20,600 / 4 | `mlng` text containing `SEQ ` data |
| 18, 2 | `d_gs` | 120 / 17,440 | embedded `d_gst130.objx` and `D_GST130.gmo` |

The assist objx has the same chunk set at smaller sizes (no type 9, small `effe`); the world objx has no `effe`/`magi`.

### Not resolved to a file
- Menu images that are members of index-addressed `menu pak` archives (`menu/menu_pk_*_gim_N.bin`, 142 archives, 39.7 MB): face icons, party UI, etc. Which pack indices belong to which character was not traced.
- `menu/JP/common/%s/p_%s%s.gim` (battle overlay string `0x08B57E38`) did not hit with the argument guesses I tried.
- Story/event voice (most of the 4,117 unnamed RIFF files) and cutscene scripts are outside "battle character" scope.

---

## 3. Formats and community knowledge

| Asset | Format | Public? | Tools / documentation |
| --- | --- | --- | --- |
| Model, skeleton, animation | Sony GMO (`OMG.00.1PSP`), chunked: File 2, Model 3, Bone 4, Part 5, Mesh 6, Arrays 7, Material 8, Layer 9, Texture 0xA, Motion 0xB, FCurve 0xC, BlindBlock 0xF, plus 0x80xx commands | Yes (Sony SDK format, community-documented) | Chunk ids: `GmoEnums.cs` in https://github.com/pionome/PiosGmoLibrary ; older notes http://wiki.ffrtt.ru/index.php/PSP/GMO_Format and https://amicitia.miraheze.org/wiki/GMO ; Sony GmoConv/GmoView (closed), Noesis import, community "GMOTool" (no source found) |
| Textures / UI images | Sony GIM (`MIG.00.1PSP`), here always 8-bit indexed with a 32-bit 256-colour palette | Yes | Sony GimConv; open code in https://github.com/nickworonekin/puyotools (GimSharp) |
| Texture animation, eye/lip sets | BlindBlock inside the GMO named `TexAnm` with entries `0_left`, `1_right`, `2_lip` (verified present in `P_GST130_1P.gmo`) | Game-specific, **undocumented** | none found |
| Sound banks | Square Enix `SSCF` (.scd), PSP variant | Partly: reader only | https://github.com/vgmstream/vgmstream/blob/master/src/meta/sqex_scd.c ; **no builder found** |
| Streamed voice | RIFF ATRAC3 | Yes | standard AT3 tools |
| EX-mode table `.exex` | small binary table | Community-documented | https://github.com/adriangl/DissDlcToolkit parses it as an EX aura/colour table |
| Costume table `.cosx` | 68 bytes | Named "costume effects" by DissDlcToolkit, fields not documented | partial |
| Object table `objentry.bin` | 0x44-byte rows | Community-documented; layout matches mine | DissDlcToolkit wiki and `GlobalData.cs` |
| Battle data `.objx` chunks (`plya`, `comb`, `atkp`, `magi`, `brat`, types 5..9) | Square Enix proprietary | **Undocumented** | one unanswered help thread: https://gbatemp.net/threads/dissidia-012-help-with-attack-data-modifications.566632/ |
| Effects `FEP` / `FEPA` / `FXSE` | Square Enix proprietary | **Undocumented** | none found |
| `VOLD`, `menu pak`, `MPK `, `ARC\x01` | simple tables/containers | Not documented publicly as far as found; layouts in this report | - |

Other community resources: Dissidia Modding Suite (closed source, https://archive.org/details/DissidiaModdingSuite); custom-DLC thread https://gbatemp.net/threads/dissidia-012-custom-mod-dlc.484253/ ; modder notes https://deraj8mblock.tumblr.com/post/148213031694/ ; early decomp/tools repo https://github.com/RocherBrockas/DissidiaDecompProject.

Community status relevant to scope (from the web research, not verified by me): importing custom models as costumes is routine (Noesis + Milkshape/Fragmotion + GMOTool + PNG2GIM + DLC toolkit). **No evidence was found that anyone has added a true new playable slot**; every "new character" is a model swap that keeps the host's moveset (https://www.neogaf.com/threads/dissidia-012-unofficial-new-dlc-brings-new-characters-as-costume-swaps.494682/).

---

## 4. Budgets (measured with `gmo.py` over the 31 playable `P_*_1P.gmo` files)

| Quantity | Min | Median | Max |
| --- | --- | --- | --- |
| GMO file size | 1,493,288 | 2,017,616 | 2,280,840 |
| Skeleton bones (includes camera, weapon and helper nodes) | 43 | 60 | 83 |
| Body part triangles | 1,436 | 1,728 | 2,332 |
| Body part vertices | 4,238 | 5,163 | 6,996 |
| Whole file triangles (body + weapon / alternate parts) | 1,783 | 3,675 | 5,398 |
| Meshes (draw groups) | 41 | 146 | 231 |
| Bones a part is skinned to (BlendBones) | 27 | 39 | 50 |
| Bones per mesh subset (BlendSubset count) | 4 | 5 | 6 |
| Textures per file | 3 | 6 | 12 |
| Texture bytes per file (embedded GIMs) | 134,996 | 237,248 | 469,248 |
| Motions per file | 63 | 78 | 101 |
| Motion bytes per file | 1,243,152 | 1,511,936 | 1,778,052 |

Lightning: 56-node skeleton, body 1,807 triangles / 5,421 vertices skinned to 36 bones, nine `wep0..wep8` parts (74 to 166 triangles each), 2,839 triangles in total, five textures (two 256x256, two 256x128, one 128x256; 235,928 bytes), 79 motions at 30 fps (1,332,884 bytes, 6,138 FCurves). Cloud: 54 nodes, body 1,681 triangles, 86 motions (1,687,000 bytes).

Details:
- Geometry is stored as **unindexed-style triangle lists**: every vertex array has exactly three vertices per triangle (585-vertex array drawn as one 585-index triangle primitive), so vertex counts are 3x triangle counts.
- Vertex types seen: `0x..131A`, `0x..531A`, `0x..931A`, `0x..D31A`: 16-bit positions, 16-bit UVs, 16-bit colour, 8-bit weights, 1 to 4 weights, **no normals** (lighting is in vertex colour). 12 bytes per vertex with one weight.
- Texture sizes across the 31 files: 256x256 (67), 256x128 (46), 128x128 (26), 128x256 (23), a few smaller; all 8-bit indexed.
- Motion names are numeric (`_000A`, `_010A`, ... `_911A`); the same numbering is used by every character, so motion id is the link between the model file and the battle data.
- Assist models: 12 to 21 motions, 0.4 to 0.8 MB. World-map models: 6 motions, about 0.35 MB.
- **Hard limit**: the object loader allocates a 0x264000-byte (2,506,752) scratch buffer per object file (`0x08844C14` -> `0x0884417C` -> `0x08843014`); the largest retail object GMO is 2,280,840 bytes (`P_THR200_2P`). The costume-preview path in the menu overlay uses 0x270800.
- Largest single package entry is 2,324,540 bytes.

Reading for an import: a retail character is roughly 1,500 to 2,300 triangles for the body, 40 to 80 skeleton nodes, about 240 KB of 8-bit textures, and 60 to 100 animations that occupy three quarters of the file. A "much more detailed" model must fit the same 2.5 MB file together with all of its animations, or the buffer size has to be patched (memory impact unknown). The only community figure found is a rule of thumb of "less than 12k triangles" (GBAtemp thread snippet, unverified).

---

## 5. Non-playable and partial characters on the disc

Verified from `objentry.bin`, package lookups and GMO parsing.

| Character / object | Rows | Present | Missing versus a full playable character |
| --- | --- | --- | --- |
| Chaos, boss (`org200`, id 23) | `P_ORG200_NORMAL`, `_HARD`, `_S_HARD` (flags 0x06) | Three battle GMOs (1.6 / 1.6 / 2.1 MB; 47 to 60 motions; 2,088-triangle body), three full objx (`p_org200_00/01/02`, 1.5 to 1.7 MB), cosx, one exex, `D_ORG200` camera rig, voice bank `voice_p_org200_s_hard2.scd`, `voice_load_P_ORG200.bin` (856 bytes) | No numbered costumes, no assist (`_A`) or world (`_W`) model, fewer motions than a playable character |
| Feral Chaos (`org210`, id 32) | `P_ORG210_1P`, `_2P`, `_ZAKO` | Full set: 2.18 MB GMO (77 motions, 2,332-triangle body), objx 1.45 MB, cosx, exex, voice bank (302 KB) and `voice_load` (88 bytes) | 1P/2P/ZAKO are the same size (no distinct third costume); no `_A`/`_W` rows |
| Aerith (`sev120`, id 33) | `P_SEV120_1P_A`, `_2P_A`, `_3P_A`; `P_SEV120_1P/2P/3P` | **Assist only**: three assist GMOs (about 405 KB, 14 motions, 1,750 to 1,875-triangle body, own 40-node skeleton), `p_sev120_a.objx` (355 KB), assist voice bank `voice_P_SEV120_a_00.scd`, cosx for the 1P/2P/3P rows, `F_SEV120` stage row, a `chrp` record | No full battle GMO, no `p_sev120.objx` (no moveset, effects or SE bank for a playable version), no full motion set, no `voice_load` list |
| Manikins | `P_<code>_ZAKO`, `_ZAKO_A`, `_ZAKO_W` for every character | Own GMO per character (same geometry and motions, fewer/different textures), own cosx/exex, `_zako.scd` voice bank; they reuse the character's objx | Nothing: they are costume 0 of each character |
| Cosmos (inferred: `Z_OR100_*`, part name `s_or100`) | 6 `Z_` rows | Cutscene GMOs only (645 to 725 KB, 1,988 triangles, 91 bones, 8 to 10 motions), small `z_or100_*.objx` | No `P_` row, no battle objx, no battle motion set, no voice bank, no character id |
| Chaos cutscene (`Z_OR200_*`) | 2 rows | Cutscene GMO sharing the boss geometry | - |
| Cutscene versions of the cast (`Z_ON100`, `Z_SE100`, `Z_GS130`, ...) | about 280 rows | Per-scene GMOs with the battle geometry (Cloud's has the same 1,681-triangle body) but a **different, larger skeleton** (60 vs 54 nodes for Cloud, 72 vs 56 for Lightning) and only that scene's motions | Not usable as battle models without re-rigging (community says the same) |
| `org800` (id 34) | `P_ORG800_1P`, `_1P_W`, `R_ORG800..809`, `Z_OR800_5000` | 214 KB GMO, 1,948 triangles, 3 motions, cosx | No objx for the `P_` row, no voice slot. Identity not established |
| Low-poly figures `Z_OR300..Z_OR390` | 10 rows | About 80 to 98 KB each, 800 to 970 triangles, one motion, textures named after villains (`p_ten200_01.tga`, `p_sev200_01.tga`) | Props only |
| `Z_OR910/920/930`, `Z_*700` (`ff9_crystal` etc.), `Z_OB500` | about 30 rows | Crystals and props | - |
| `W_FIV101..106`, `W_FIV211..218`, `W_TEN111..115` | 54 rows | Separately loaded weapon/summon objects with their own GMO and objx (Bartz, Gilgamesh, Yuna, by code) | - |
| `P_ELS*`, `P_*_PUB` | about 80 rows | Rows only; no GMO under those names in PACKAGE.BIN | Purpose not established |

---

## 6. How loading is keyed

### Verified chain (character id + costume -> files)

1. Battle setup block at `0x09AFB8A0`, one 0x1060-byte record per side: +0x1058 character id, +0x1059 costume, +0x105A assist character id, +0x105B assist costume.
2. `0x088F98E0` converts these to object ids with `0x088FB934(character, costume, assist)`, storing them at +0xF3A (fighter) and +0xF3E (assist). `0x088FB434` swaps to another costume when both sides picked the same one.
3. `0x088FB934` -> `0x088470DC` -> `0x08846CCC`: **linear scan of `objentry.bin`** for the first row with matching character byte (+6), costume byte (+7), assist flag (bit 3 of +8), whose name starts with `P_` and does not end in `_W`. If the costume is absent it falls back to costume 1.
4. `0x08844C14(object id, heap, flag)` loads the object, every `W_` object whose code matches the owner's, and the four attached ids at +0x1C.
5. `0x08845230` does the file work: `obj/<name>.gmo`, then `.cosx`, `.exex`, `obj/<name2>.objx`, then the voice bank `voice/<LANG>/battle/voice_<CODE8><suffix>.scd` and `voice/<LANG>/voice_load_<CODE8>.bin`, CODE8 from `0x088B70C8(character)` + 0x74 (`chrp` record). `0x088459E0` then pulls the objx chunks by type id.
6. Menu costume choice (menu overlay `0x08A32284`, `0x08A3210C`): costumes 4..12 are offered when a row with (character, 4..12) exists and its GMO is present (`0x08844BC4`).

So loading is keyed by **table rows and name formatting**, not by fixed file slots. Fixed-size, character-indexed structures seen on this path: the 34-record `chrp` chunk in `system.bin`, the 35-slot voice-code pointer array at `0x089D6CA8` (loop bound 0x23 in `0x088A3A94`; one unused slot at index 34), and `objentry.bin` itself (buffer 0x13D30 bytes = room for 1,194 rows, 24 spare).

### What a new character would need on the storage side
- New files reachable by name: either **new PACKAGE_INFO records** (the index must be rebuilt because it is sorted by hash; 210 spare records fit the stock buffer; PACKAGE.BIN can simply be extended and the ISO rebuilt), or the memory-stick route below.
- New `objentry.bin` rows (inside `main.bin`, an `MPK ` that must be repacked) or reuse of the 255 `DLC_nnn` placeholder rows.
- A `chrp` record, voice-table slot and the roster/menu tables (other investigators).

### Loading from outside PACKAGE.BIN (verified in code)
- **Yes, for models**: when row byte +9 is non-zero, `0x08845230` passes `direct = 1` for `.gmo`, `.cosx` and `.exex`, and the file is read from `ms0:/PSP/GAME/<dir>/<8-hex-hash>.edat` (section 7). If the file is missing it falls back to the character's costume 1 (`0x08844FE4`, `0x088450A8`).
- **Not for a fighter's battle data**: for names starting `P_` the `.objx` is always read from PACKAGE.BIN even on a flagged row (the `direct` argument is forced to 0 unless the name is not `P_`); the voice bank and `voice_load` file are also always package reads. Non-`P_` flagged rows (attachments) do read their objx from the memory stick.
- **Data install** (`DISSINST.DAT`, `0x088701F8` and neighbours): a cache of existing package entries keyed by index, with a 16-bit check value and a 20-byte digest per record. It cannot introduce new names.
- Direct `disc0:/PSP_GAME/USRDIR/data/...` reads exist only for `package.bin`, `package_info.bin`, modules, movies and BGM.

---

## 7. DLC

### Verified in code
- Directory: `ms0:/PSP/GAME/` + the string at `0x089D6010`, which `0x08867BD4` points at offset 0x14 of `language.bin`. On this disc `language.bin` contains `ULUS10566` at 0x14 (also `NPUH10110` at 0x24 and `ULUS10437` at 0x34, uses not traced).
- File name: `0x088626AC` formats `%s/%s.edat` with the **same name hash printed as `%08x`** (`0x08861B84` duplicates the CRC routine). Example: `dlc/obj/dlc_001oe.bin` -> `ms0:/PSP/GAME/ULUS10566/eac7bf53.edat`.
- Protection: PSP NPDRM EDAT. `0x08862650` loads utility module 0x500 and calls `sceNpDrmSetLicenseeKey` (import `0x089C5D58`, NID 0xA1336091) with the 16 bytes at `0x089C8EC6`; opens with the flag set call `sceNpDrmEdataSetupKey` (import `0x089C5D50`, NID 0x08D98894) and fail if it returns negative (`0x08863F24`).
- Three DLC scanners share one state machine (`0x08861C64`, vtables at `0x089C8E70/E88/EA0`):
  - BGM: `dlc/bgm/dlc_%03d.bin`, slots 0..19 (`0x08861F78`), tracks as `bgm/<name>`.
  - BGM text: `text/jp/dlc/bgm_%03dt.bin`, slots 0..19 (`0x088621A4`).
  - **Objects: `dlc/obj/dlc_%03doe.bin`, slots 0..255 (`0x08862288`)**. On load, `0x08862254` -> `0x08847068` takes the 0x44-byte row at file offset 8, binary-searches `objentry.bin` for the same id and **overwrites that row in memory**. The id must already exist, which is what the 255 `DLC_nnn` rows are for.
- A DLC costume is therefore: one `dlc_NNNoe.bin` row (character id, costume 4..12, flag +9 set, a model name) plus `obj/<name>.gmo`, `.cosx`, `.exex` as hashed `.edat` files. It reuses the character's on-disc objx, voice and motions-by-id.
- Aerith: all of her assist assets are on the disc. The "DLC" is a key file: `0x0885C01C` reads `dlc/aerith.txt` (1,020 bytes, in PACKAGE.BIN) and the EDAT for `dlc/aerith.bin` (`2f9c99ab.edat`), de-obfuscates the latter with three LCG streams (rotate, subtract, xor) and unlocks her if the bytes match.

### From community sources (not verified by me)
- DissDlcToolkit (https://github.com/adriangl/DissDlcToolkit, C#, source public) builds character, assist, attachment and BGM DLC and documents the same row layout; it also ships `menu/jp/battle/chara_image/<model>.gim` with a costume. Its wiki states 9 DLC costume slots per character and 255 slot files, matching the code above.
- Real hardware loads custom (unencrypted) `.edat` files with the NPloader plugin (https://github.com/codestation/nploader).
- PPSSPP: per the subagent's reading of `Core/HLE/scePspNpDrm_user.cpp` and `sceIo.cpp` in https://github.com/hrydgard/ppsspp, a file that is not a real PGD/EDAT is read as plain data ("Proceeding anyway"), so plain files named `<hash>.edat` load without any plugin.
- Other regions' folders reported as ULES01505 / NPJH50377 (https://www.ff6hacking.com/forums/thread-2734.html).

### Why this matters
The DLC object path is the game's own mechanism for adding models from loose files, and it already accepts a row with an arbitrary character id and costume number. It stops short of a new character because a fighter's objx, voice bank and voice list are read only from PACKAGE.BIN.

---

## Could not determine / needs a runtime check

1. Whether a flagged DLC row with a **new character id** (and a package-side objx) is accepted end to end; only static reading was done. Needs a PPSSPP test with a hand-made `dlc_NNNoe.bin`.
2. The 8-byte header of `dlc_NNNoe.bin` (probably the `objentry.bin` header `0x0C`, count 1) and the exact bytes of an official DLC; no sample DLC was available.
3. That PPSSPP reads plain `.edat` files for this game (community claim, not run here).
4. Meaning of objentry fields +0x02, +0x04 (partly), +0x0A..0x1B, and of the `_PUB` and `P_ELS*` rows.
5. Field-level layout of `.cosx` beyond the voice suffix, and of objx chunk types 5 to 9 and the `plya/comb/atkp/magi/brat` tables.
6. Which `menu pak` members hold per-character UI art, and how the menu maps a character to them.
7. Memory headroom: whether a larger object file (above the 0x264000 buffer) or more than the retail number of simultaneously loaded objects fits in RAM. Needs heap inspection at battle start.
8. GMO command labels below 0x8050 in `gmo.py` are provisional (they differ from PiosGmoLibrary's table); counts of bones, meshes, vertices, textures and motions do not depend on them.
9. The identity of `org800` and of Cosmos's rows is inferred from naming and geometry only.
10. Uses of the second and third title ids in `language.bin`.
