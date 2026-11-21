# How textures get from files to the GE

The texture path of Dissidia 012 (USA ULUS10566), read from the retail code and traced in the PC port on
21 November 2026. It replaces the load-path parts of [texture-loading.md](texture-loading.md) (1 November), which
keeps the texture pack key and pop-in measurements. The C layouts are in [texture_types.h](texture_types.h)
(compiles as C11 and C++11, sizes asserted). EU and USA code are identical (texture-loading.md), so the addresses
hold for both.

Marks: **V** read in the disassembly (offsets, widths and constants checked against the assembly, or in Ghidra's
pseudocode where its offsets were confirmed by the call sites); **R** seen at run time in the recompiled port
(section 11 says how); **P** read in the port's own source; **I** inferred from use; **D** taken from the
1 November doc or the GIM format, not re-checked. All addresses are `main` (EBOOT) unless a module is named.
"a0..a3, t0..t3" are the PSP EABI argument registers 1-8. Run-time addresses (heap blocks, VRAM slots) are those of
the traced runs: they repeat for the same route and timeline, but a port must never key on them.

## 0. Summary, and what changes from the 1 November doc

Where each kind of texture is when the GE draws it (R):

| Kind | Pixels when drawn | Put there by | Change after loading? |
| --- | --- | --- | --- |
| Menu, title, HUD GIMs, portraits | RAM, the image library's copy in the GRAPHICS heap | `FUN_088DE0E8` → `0x0899BB68` → `FUN_0899BC64` (memcpy `0x08935524`) | No |
| Fighter textures (256x256 / 256x128, 8-bit) | RAM, the GMO's own records in the GRAPHICS heap | GMO parse, word copy `FUN_089BE908` | **Yes**: face parts are block-transferred into the atlas (section 5.4) |
| Stage and world-map terrain (128x128, 4-bit), world-map `R_` props (I for the props) | VRAM (GRAPHICS::EDRAM heap) | `FUN_089BCC84(tex, 2)` → `FUN_089BBE20` / `FUN_089BE908`, GE list by `FUN_089BCAC4` | No |
| Effects (FEP `TEX`, 128x64 / 128x128, 8-bit, swizzled) | Battle: VRAM cache slots; world map: RAM | Load: memcpy into effect memory. Battle: GE block transfer on first use (`0x08974370`) | Slots are reused: one texture can sit at several VRAM addresses in one battle |
| Text lines, render targets, screen copies | RAM / VRAM, rewritten | The game, every frame | Yes (not loadable, not prefetchable) |

- **Four texture families, four emitters.** Every literal GE texture command (TBP/TBW/TSIZE/TMODE/TPF/CBP/
  CLOAD/CMODE) in all four modules is written by one of: the image library (`0x0899C8B0`, `0x0899C944`), the GMO
  texture library (`0x089BD8CC`), the effect binder (`0x08974370`, `0x08974630`, plus libGu calls in
  `0x08972C14`/`0x08972EBC`), or libGu itself (`sceGuTexImage 0x089A0F04`, `sceGuTexMode 0x089A2008`,
  `sceGuClutLoad 0x089A1024`, `sceGuClutMode 0x089A0FE8`) (V for each function; I that the list is complete: it
  comes from a whole-module scan for `lui reg, 0xA000/0xA800/0xB800/0xC200/0xC300/0xB000/0xB100/0xC400/0xC500/0xCB00`
  and for `li a0, cmd; jal` patterns). The only block-transfer emitters (`0xB2`..`0xB5`, `0xEA`) in main are
  `0x08974370`, `0x08974630` and libGu's `0x089A09A8`; the battle overlay has none (V, scan of both modules for
  `lui reg, 0xB200/0xEA00`).
- **Correction: package GIMs are copied, not used in place** (V R). `FUN_0899BC64` copies each GIM image block and
  palette block (`memcpy 0x08935524`) into a 16-byte-aligned allocation from the GRAPHICS::GRAPHICS heap, then
  makes its level/frame pointer table absolute. The package file sits in a staging buffer that is freed or reused
  right after. The 1 November doc's observations (pixel offset 0x40 at `0x08C63E20` overwritten with
  `0x08C63E30`, a word set at `0x08C63DF2`) are this copy's relocation and refcount: the title atlas's image object
  is `0x08C63D50` and its GIM was in staging at `0x0973D8D0` (R, section 2.1).
- **Correction: format dispatch.** `0x0899BB68` sends `MIG.` to `FUN_0899E998` (check) + `FUN_0899E9EC` (find
  picture) + `FUN_0899BC64` (attach); `TIM2` to `FUN_0899E4B0`; `BM` to `FUN_0899D348` (the **BMP** parser, not the
  GIM parser); anything else to `FUN_0899DEF4`, the **TGA** reader (V). Every load seen at run time was a GIM (R).
- **Correction: `FUN_08862854`'s first argument is a path string** (`char*`), localised by `FUN_08867984`
  (`ZQ`/`JP` replaced by the language code), hashed by `FUN_08871288` (CRC32, poly 0x04C11DB7, MSB-first, init and
  final xor 0xFFFFFFFF, upper case folded to lower) and binary-searched in the PACKAGE_INFO index (V; R: the paths
  of section 1.3 were read from `a0` at its entry).
- **Correction: `FUN_088D0810`** is "make a sprite instance and load one whole package entry as one GIM":
  `(mgr, group, id, path)`, not "(path in a3, slot code in a2)" in the old doc's sense (V).
- **The "model pool" `FUN_0888204C(0x780000)` is the GRAPHICS::GRAPHICS heap**: image objects, image copies, GMO
  models and their RAM textures all live in it (V). The "VRAM heap from 0x04110000" is the GRAPHICS::EDRAM heap:
  from `0x04000000 + *(0x09B26AB0)` (0x110000, or 0x154000 in the 32-bit-colour mode) to the end of EDRAM (V); it
  uses the same allocator `FUN_0885C6CC`, with its 16-byte block headers inside VRAM (R).
- **Model textures go to VRAM only on request** (V R). Only models whose holder has flag 8 get their textures
  copied into VRAM (`FUN_089BCC84`). Stages, the world-map terrain and the world map's `R_` models are in VRAM;
  fighters are in RAM (R: Kuja, Lightning and the CPU's assist in a Quick Battle, Lightning and the moogle on the
  world map). Fighter files are named `obj/P_<ID>_1P.gmo`, never `R_` (R).
- **New: fighter textures change after loading.** A model draw copies face parts (eyes, mouth) from a sheet into
  the fighter's atlas with `sceGuCopyImage` whenever the expression changes, so an atlas has one texture key per
  expression (R, section 5.4).
- **FEP effect textures are bound in `main`** (`FUN_08972C14`), not in the battle overlay; they are always
  swizzled and, in battle, copied into a VRAM cache by **GE block transfers**, not by CPU stores (V R). The cache
  evicts: one effect texture was sent to two VRAM slots in one battle (R). Effect data sat at
  `0x09BE5A00`-`0x09E7E900`, not in the `0x0A010000`-`0x0A3ED000` the old doc gave (R).

## 1. Package files

### 1.1 Identifying a file

| Step | What | Mark |
| --- | --- | --- |
| Path | Game code passes a `char*` such as `menu/menu_pk_party_battle_title_gim_0.bin` | V R |
| Localise | `FUN_08867984(path, flag)` copies the path to `0x08B63600` and replaces `ZQ` (or `JP`) with a 2-letter language code from the table at `0x089D5ED0` (`EN`, `JP`...) | V R (`0x08B63600` passed on as the path of later loads) |
| Hash | `FUN_08871288(path)`: CRC32 (BZip2 style) of the path with A-Z folded to a-z | V |
| Index | `FUN_088713E4(pkg, path)`: binary search of 12-byte entries `{crc, sector (low 20 bits), size (low 28 bits) \| flags<<28}` at `0x09B26BD0`, count at `0x09B26BC8` (PACKAGE_INFO.BIN is read whole into `0x09B26BC0`); one-entry cache of the last path at `pkg+8` (0x50 chars) | V R (the read into `0x09B26BC0`) |
| Read | `FUN_088714F4(io, path, dst, size)`: seek to `sector << 11` in PACKAGE.BIN and read `size` bytes into `dst` | V R |
| Sub-file | Packs (`*_gim_N.bin`, `*_seq_N.bin`): header `+0x0A` count, `+0x0C` base id, entries `{offset, size}` from `+0x10`; a sub-file may be an `mlng` container (one part per language) | V |

### 1.2 Loader functions

| Function | Address | Size | Role | Args / return | Callers | Mark |
| --- | --- | --- | --- | --- | --- | --- |
| `pkg_load` | `0x08862854` | 188 | Load a package file into a buffer | `(const char *path, void *dst, u32 size, u8 flag)`; path localised; if `*(u8*)*0x089D5EA0 == 1` runs `FUN_0885CBD8` (synchronous, through `FUN_088629DC`), else tries `FUN_08863148` (object `0x089D5FB4`) then queues `FUN_088714F4` | 64 call sites: main 24, menu 34, battle 3, extra 3 | V R |
| `pkg_size` | `0x08862C40` | 112 | Size of a package file | `(const char *path, int)` -> size | `FUN_088D2980`, `FUN_088DAFC8`, ~27 | V |
| `file_cache_read` | `0x088FE708` | 192 | Read through the preloaded-file cache | `(path, dst, size, u8 alt)` -> bytes; if `*0x089D69DC` and `FUN_08883538` finds it, `memcpy` from the cache, else `FUN_08862854` (or `FUN_08862984` when `alt`) | fighter loader, texture manager when `mgr+0x23`, stage and map loaders | V R |
| `file_cache_size` | `0x088FE7C8` | 136 | Size through the same cache | `(path, int)` -> size | same | I |
| `lang_select` | `0x08870B10` | 196 | `mlng` part for the current language | `(char *data, void **out_data, u32 *out_size)` -> 1 if `mlng` | `FUN_088D2980`, `FUN_088DAE0C` | V |
| `pack_load_list` | `0x088D00FC` | 200 | Allocate a staging buffer, load a list of packs, free the buffer | `(mgr, heap, u32 capacity, u8 group, D012PkgListEntry *list, int file_obj)`; staging = `FUN_0885C6CC(heap, capacity, 0x40, 0, 1, 1)` when `file_obj == 0`; returns `FUN_088D2980`'s value | 39 call sites: main `0x0888C4C8`, `0x088D0FD4`, `0x088EBAA8`, `0x0891E34C` (2); 30 in menu screens; battle `0x08AF6B70`, `0x08AF756C`, `0x08AFBE4C`; extra `0x089DE7A0` | V |
| `pack_load_into` | `0x088D2980` | 1264 | Walk the list: load each pack, register each sub-file | `(mgr, void *buf, u32 cap, u8 group, list, int file_obj)`; `file_obj != 0` uses an already-loaded file (`FUN_08869AB4(file_obj)+4`); kind 0: `'M'` -> D012TexEntry via `FUN_088DE0E8`, `'F'` -> `FUN_0887D5C8(*(*(0x089D6998)+0xF2C0), data)` (copies 0x10 + 0x300 bytes into that object, I: a text colour table); kind 1: pairs -> seq object via `FUN_088DCBA8`. Returns 0xFFFFFFFF in practice (its max-id update uses an unsigned compare against 0xFFFFFFFF) | `0x088D00FC`, `0x088D01C4`, `0x088D1028` | V R (pack reads return to `0x088D2B40`) |
| wrappers | `0x088D01C4` (32), `0x088D1028` (84), `0x088D0FD4` (84) | | `088D2980(...,0)`; `088D1028`/`088D0FD4` then call `FUN_088D0E64(mgr, group, list)` (I: applies group state). The source names them "voice_*": wrong | | main `0x08915F88`, `0x089174E4`, `0x089176B0`; menu ~12 | V |
| `tex_register` | `0x088D01E4` | 396 | Register one GIM already in memory | `(mgr, u8 group, gim, size, u16 id)` -> `id + 1` | menu `0x08A20AE8` | V |
| `sprite_new_gim` | `0x088D0810` | 176 | New sprite instance of the layout object `(group, id)` (`id` goes on to `seq_find` `0x088D0608`); if `path`, load that whole entry as one GIM | `(mgr, u8 group, id, const char *path)` -> instance (0x2C bytes) | menu 5, battle 7 (`0x08AF6B70`, `0x08B14D68`...) | V |
| `sprite_new_mem` | `0x088D08C0` | 196 | Same with a GIM already in memory | `(mgr, group, id, gim, size)` | menu `0x08AC925C`, battle `0x08B21588`, `0x08B21AB8` | V |
| `entry_load_gim` | `0x088DAFC8` | 324 | Load one package entry (whole file = one GIM) through staging | `(instance, path, u8 use_cache)`; staging from `*(mgr+8)` or `*(mgr+4)`; then `FUN_088DAE0C`; frees staging | `0x088D0810` | V R (per-fighter GIMs at a battle load) |
| `gim_register_inst` | `0x088DAE0C` | 444 | Register a GIM for an instance | `(instance, gim, size, u16 a3, u16 id)`; mlng select, D012TexEntry, stored at `instance+0x10` | `0x088CF188`, `0x088D08C0`, `0x088DAFC8` | V |

Concrete examples (V: the strings are referenced next to each call; group and capacity read for the first only):

| Caller | Group | Capacity | List |
| --- | --- | --- | --- |
| `0x0891E34C` (party-battle title) | `0x23` | `0x7800` or `0xC800` | `{1, "menu/menu_pk_party_battle_title_seq_0.bin"}`, `{0, "menu/menu_pk_party_battle_title_gim_0.bin"}`, and in the second variant `{0, "menu/EN/menu_lpk_party_battle_title_gim_0.bin"}`; end `0xFFFF` |
| `0x0888C4C8` (field) | | | `general_archive/field/menu.bin`, `general_archive/field/JP/menu_lang.bin` (R: served from the file cache at a battle load) |
| battle `0x08AF6B70` (party loading screen) | | | `menu/menu_pk_loading_party_battle_seq_0.bin`, `menu/JP/menu_lpk_loading_party_battle_gim_0.bin` (R) |
| battle `0x08AFBE4C` (movie loading) | | | `menu/menu_pk_loading_movie_seq_0.bin`, `menu/menu_pk_loading_movie_gim_0.bin` |
| menu `0x089E9EB4`, `0x089EA4D0` (arcade, character select) | | | `menu/menu_pk_arcade_{seq,gim}_0.bin`, `menu/JP/menu_lpk_arcade_{seq,gim}_0.bin`; plus single-GIM files through `0x088D0810` built from `menu/JP/arcademode/arcade_last_chara_tim2/arcade_last_%s.gim` and `menu/JP/character_select/chara2/p_%s_1p.gim` (portraits) |

Fighters do not use this path: `FUN_08843014(self, id, heap, size, mode)` (V) allocates a staging block of `size`
(align 0x40), runs `FUN_08845230` (1968 B), which builds paths `obj/<name>.gmo`, `.cosx`, `.exex`, `.objx`
(`"obj/%s.%s"` in `FUN_08844FA4`; name at `+0x24` of the character record from `FUN_088470B8`, extensions at
`0x089C88A8/98/A0/50`) and reads them with `FUN_088FE708` (V), then frees the block. R: the staging block is the
fixed 0x264000-byte block at `0x095DE880` (Quick Battle) or `0x095D9880` (world map), and the reads are async
PACKAGE.BIN reads into it.

### 1.3 What a Quick Battle and the world map load (R)

Read from `a0` at the entry of `0x08862854` and `0x088FE708` (section 11), with the read that followed.

| When | Files (destination) |
| --- | --- |
| Start-up | `general_archive/main/main.bin` (0x180000 bytes → `0x09644A80`) and `general_archive/main/JP/main_lang.bin` (0x80000 → `0x097C4A80`), still in memory when a battle load starts (the model staging block and the stage file are read over them later in that load, I from the overlapping destinations); title packs `menu/menu_pk_title_gim_0.bin`, `menu/JP/menu_lpk_title_gim_0.bin` (+ seq) → staging `0x0973D880` |
| Main menu | `menu/menu_pk_upper_start_menu_gim_0.bin` + JP lpk → `0x096BD880` |
| Player select (Quick Battle) | `menu/menu_pk_free_battle_gim_0.bin` .. `_3`, `menu/JP/menu_lpk_free_battle_gim_0.bin` (+ seq) → staging `0x092B4F80` |
| Battle load, per fighter | `obj/P_<ID>_1P.gmo`, `.cosx`, `.exex`, `obj/p_<id>.objx`, voices, all through `0x088FE708` → PACKAGE.BIN → staging `0x095DE880`. Kuja `P_NIN200` (gmo 2,105,300 bytes at offset 783335424; objx 1,448,360), Lightning `P_GST130` (gmo 1,741,984 at 871415808), the CPU's assist `P_FOR200_1P_A` |
| Battle load, per fighter GIMs | `menu/JP/common/chara_name/p_<id>_name.gim`, `menu/JP/story_mode/chara2/p_<id>_1p.gim` (`0x088DAFC8`), `menu/JP/battle/chara_image/P_<ID>_1P.gim` (battle `0x08B21588`), `menu/EN/battle/encount/encount_text_tim2/encount_<id>.gim` |
| Battle load, stage | `map/one00.id`, `map/one00.gmo` (548,384 bytes → `0x097BCA40`), `map/maparc_one00_2.mpk`, `map/maparc_one00_1.mpk` |
| Served from the file cache (no read) | `general_archive/field/menu.bin`, `general_archive/field/JP/menu_lang.bin`, `general_archive/field/field.bin`, `general_archive/field/ZQ/field_lang.bin`, `battle_non_residing.bin`, `effect/system/system_effect.mpk` |
| World map (story, Continue) | `worldmap.bin`, `wld02.bin`, `obj/P_GST130_1P.gmo` (Lightning), `obj/P_ORG800_1P_W.gmo` (moogle), `obj/R_SYM256.gmo`, `obj/R_SYM240.gmo`, `obj/R_SYM600.gmo`, `obj/R_ORG801.gmo`, `obj/F_WLD300.gmo` (each with its `.objx`), `map/wld02.gmo` (2,212,604 bytes → `0x09621580`), `map/wld02.id`, `map/maparc_wld02_1.mpk`, `map/sky02_p12.gmo`, `effect/system/system_effect_w.mpk` (→ `0x09DA73C0`), `menu/menu_pk_world_map_gim_0.bin` + EN lpk, `battle_non_residing.bin` |

So fighter models come from PACKAGE.BIN at every battle load; the file cache served the field menus, the battle's
non-resident data and the system effects.

## 2. The 2D texture manager (package GIMs: menus, title, HUD, portraits in packs)

Manager `*(0x089D7830)` = `0x08B87E7C`, constructed by `FUN_088D11C8` (V R: `0x08B87E7C` is in `s1` at the
binds). Pools (V): 252 sprite instances of 0x2C bytes (`+0x081C`), 490 objects of 0x44 (`+0x42C4`), 80 seq objects
of 0x74 (`+0xC780`), **270 texture entries of 0x14** (`+0xF444`), each with a ring of `{object, next}` slots and
take/return pointers.

Layout, confirmed by matching the constructor (V): sprite stock slots `+0x3C`, objects `+0x81C`, take `+0x336C`,
return `+0x3370`; 490 0x44-byte objects at `+0x3374` (slots) / `+0x42C4`; the seq list (`+0xC4F4`) sits between the
object stock and the seq stock (slots `+0xC500`, objects `+0xC780`, take `+0xEBC0`); the texture list (`+0xEBC8`)
between the seq stock and the texture stock (slots `+0xEBD4`, entries `+0xF444`, take `+0x1095C`). Each stock is
built by the array constructor `FUN_08993560` with the element constructors `0x088DA8AC` / `0x088D9988` /
`0x088DCB50` / `0x088DE098`, then zeroed. `+0x20` = 1 (drawing on); `+0x24` and `+0x30` are the two sprite draw
lists `{head, tail, count}`, drawn by `0x088CED50` / `0x088CEE0C`. A sprite instance links through `+0x24` next /
`+0x28` prev, sorts on `+0x15` (layer, `0x088DB4E4`), and `+0x14` bit 4 = in draw list 1. A seq object keeps its
block at `+0x5C` (manager heap a), an id freed through `0x088A1638` at `+0x60` (-1 = none), refcount `+0x64`
(short), id `+0x66`, group `+0x68`. `tex_entry_load` takes its two ids as ints and stores them as u16.

| Function | Address | Size | Role | Args / return | Mark |
| --- | --- | --- | --- | --- | --- |
| `tex_entry_load` | `0x088DE0E8` | 176 | Fill a D012TexEntry from a GIM | `(D012TexEntry *e, gim, size, u16 a3 -> +6, u16 id -> +8, u8 group -> +0xA)`; `e->image = 0x0899B9F0(0)`, `0x0899BB68(e->image, gim, size, 0)`; on failure `0x0899BB1C` and `e->image = 0`; on success `+4 = 1`. Returns `image != 0` | V R |
| `tex_entry_destroy` | `0x088DE050` | 72 | `0x0899BB1C(image)`, `+6 = +8 = 0xFFFF`, `image = 0` | | V |
| `tex_addref` / `tex_release` | `0x088DE198` / `0x088DE1A8` | 16 / 64 | `+4` ++ / -- (destroy at 0, returns 1) | | V |
| `tex_match` | `0x088DE1E8` | 60 | `(e, group, id)`: group equal and (`id == -1` or `+8 == id`) | | V |
| `tex_find` | `0x088D0684` | 124 | First list entry matching `(group, id)`; addrefs it | `(mgr, group, id)` -> D012TexEntry* | V |
| `tex_find_opt` | `0x088D09D8` | 68 | `id == -1` -> 0, else `0x088D0684` | callers `0x0888C4C8`, `0x088EBAA8` | V |
| `sprite_set_tex` | `0x088D0984` | 84 | `(mgr, instance, group, id)`: find, release old, `instance+0x10 = entry` | 68 call sites: main 6, menu 62 | V |
| `group_unload` | `0x088D0370` | 664 | Release every seq object and texture entry of a group | `(mgr, group, u8 also_text)` | V |
| `group_refresh` | `0x088D0E64` | | Hand every seq object and texture of the group to all sprites of both draw lists (`0x088DB28C` / `0x088DB2CC`); third argument unused | `(mgr, group, unused)` | V |

Draw side (V R): `FUN_088D16F4` (4748 B, the sprite-list renderer, called by `0x088CED50`/`0x088CEE0C`) walks the
instance list (`+0x24` next) and for each visible instance calls `0x0899D2F8(instance->+0x10->image)` (the image
is loaded in the delay slot at `0x088D18A8`), then draws quads with libGu. `FUN_088CE0E4` binds
`0x0899D2F8(*(obj+4))` the same way for a full-screen picture. R: from the title to the player select,
`0x0899D2F8` was entered 44,296 times, 37,366 from `FUN_088D16F4` (return `0x088D18AC`) and 6,930 from
`FUN_0887CA50` (return `0x0887CCF0`).

### 2.1 One load, in order (R)

Lightning's player-select portrait (256x256, 8-bit, drawn at `0x08E42030`), route title → player select:

```
[script] frame 2400: pad a
[io] read fd=5 DISC0:/PSP_GAME/USRDIR/DATA/PACKAGE.BIN filtered offset=16435200 got=851056 bytes -> 0x092B4F80
... object init (FUN_0899B888), object 0x08E41F50:
[watch-write] uid=258 name=ffst_thread pc=0x0899B918 op=store32 address=0x08E41FC0 size=4 old=0x0 new=0x1
[trace-pc] 0x0899BB68 ... a0=0x08E41F50 a1=0x09327650 a2=0x00010520 ... ra=0x088DE140 a1str="MIG.00.1PSP"
[watch-write] ... pc=0x0885C6CC op=store32 address=0x08E41FE0 ... new=0x92b4f40     heap block (size 0x10050)
[watch-write] ... pc=0x08935524 op=store8 address=0x08E41FF0 ...                     memcpy of the image data
[watch-write] ... pc=0x0899BDA4 op=store16 address=0x08E41FF2 size=2 old=0x0 new=0x1                  refcount
[watch-write] ... pc=0x0899BDA4 op=store32 address=0x08E42020 size=4 old=0x40 new=0x8e42030   table[0] made absolute
[watch-write] ... pc=0x0885C6CC op=store32 address=0x08E41FE0 ... new=0x8e52030     next block: the palette
```

- The GIM came from a pack in staging (`0x09327650` = staging `0x092B4F80` + 0x726D0); the drawn pixels are the
  copy: image data `0x08E41FF0` + 0x40. With the blocks allocated back to back, pixels = object + 0xE0 for every
  GIM loaded this way (title atlas: object `0x08C63D50`, pixels `0x08C63E30`).
- **Portraits are preloaded.** Entering the player select reads three packs (offsets 14743552, 15632384, 16435200)
  and loads 257 GIMs: every fighter's portrait, name plate and grid icon. Moving the cursor draws another one
  (`0x08E31AF0`) with no read and no load.
- **Battle HUD GIMs come from start-up data.** At a battle load, 47 GIMs are loaded through `FUN_088DE0E8` from
  `0x097CDF20`-`0x0980EBA0`, inside `main_lang.bin` as read at start-up (section 1.3), before the fighter and
  stage files are read; the HUD
  draws the copies (`0x08C73A70` digits, `0x09115E70`/`0x09111930` HUD portraits...).

## 3. The image library (Sony, `0x0899B6B0`-`0x0899F030`; SHA-1 follows at `0x0899F030`)

### 3.1 Functions

| Function | Address | Size | Role | Args / return | Callers | Mark |
| --- | --- | --- | --- | --- | --- | --- |
| `img_create` | `0x0899B9F0` | 300 | Allocate and initialise a 0x90-byte D012ImageObject (class 0); if `pic != 0` attach it | `(gim_picture *pic)` -> image or 0 | 21 call sites: main 17 (`0x0884C4DC`... `0x088DE0E8`), menu 1, battle 3 (`0x08ABAC10`, `0x08AC61AC`, `0x08B52F88`) | V R |
| `img_destroy` | `0x0899BB1C` | 76 | `--refcount`; at 0 `0x0899BEE0` then free | `(image)` | 20 | V R |
| `img_load` | `0x0899BB68` | 252 | Parse a file in memory into the image | `(image, data, u32 size, int index)` -> 1/0. Size 1-3 rejected. `MIG.` -> `E998(data,size)` (checks `MIG.00.1PSP`; size 0 or >= 0x20) then `E9EC(data,size,index)` (index-th picture block, type 3, under the root at `data+0x10`) then `BC64(image, picture)`; `TIM2` -> `0x0899E4B0`; `BM` -> `0x0899D348`; else `0x0899DEF4` (same args) | 20 call sites: main 17 (incl. `0x088DE0E8`), battle 3 | V R |
| `img_attach_picture` | `0x0899BC64` | 636 | Release old data; for each child of the picture: type 4 image -> copy data (`child_offset - data_offset` bytes) to class 1, `+0x0C`; type 5 palette -> `+0x10`; type 6 sequence -> appended to `+0x54` (count `+0x58`); relocate each copy's level/frame table (`+0x18`, `levels*frames` words) to absolute addresses; refuse width or height > 512, palette width > 256; then `0x089C6040` (dcache writeback) and `0x0899BFE0` | `(image, picture)` -> 1/0 | `0x0899B9F0`, `0x0899BB68` | V R |
| `img_finish` | `0x0899BFE0` | 480 | Fill `+0x04` flags, `+0x1C` TBW, `+0x1E` CLUT blocks, NPOT UV scale `+0x28/+0x2C`; then `0x0899C2C4(image, image+0x60)` (CMODE shift/mask/start into `+0x64/+0x68/+0x6C`) | `(image)` | BC64, EB28, EE30 | V R |
| `img_release_data` | `0x0899BEE0` | 256 | Free VRAM copy and list (`0x0899C6F8`), image/palette copies (refcount `+0x02`), sequences | `(image)` | | V |
| `tim2_parse` | `0x0899E4B0` | 576 | TIM2 -> synthesised image/palette blocks via `0x0899EAEC`/`0x0899EE30` | as `img_load` | | V (calls), I (internals) |
| `bmp_parse` | `0x0899D348` | 812 | BMP -> same | as `img_load` | | V (calls), I |
| `tga_parse` | `0x0899DEF4` | 892 | TGA (header `0x0899E270`, pixels `0x0899E364`; colour-mapped or true colour, RLE, bottom-up unless descriptor bit 5) -> same | as `img_load` | | V |
| `img_make_block` | `0x0899EB28` | 776 | Synthesise a GIM-style image block (header 0x30, table at +0x30) in class 1; `w,h <= 512`, levels <= 8 | `(image, format, order, w, h, levels, frames, pixels_or_0)` | `0x0899EAEC` | V |
| `img_set_param` | `0x0899C364` (not in the inventory) | 456 | Set one animation, palette or UV parameter | `(image, id, float)`: jump table `0x089D131C` (ids 0-0x27): 1 time -> `0x0899C2C4`; 3 image frame `+0x44/+0x48`; 11 palette frame `+0x4C/+0x50`; 16-19 UV `+0x20..+0x2C`; 32 `+0x70`; 34-39 `+0x74..+0x88` | no direct callers found (I: function pointer); R: not entered from the title to the player select | V |
| `img_upload` | `0x0899C52C` (not in the inventory) | 432 | Copy to VRAM and/or precompile the texture commands | `(image, flags)`: flags 2 -> copy image+palette blocks into class 2 (EDRAM) and set `+0x18`; flags 1 -> precompile the GE texture commands (`0x0899C944` + `RET`) into class 1 and set `+0x14` | only caller: menu `0x08B45380`; R: not entered from the title to the player select | V |
| `img_bind` | `0x0899D2F8` | 80 | Write the image's texture commands into the current libGu list | `(image)` -> 1/0; `cur = 0x089A0144(&left)`; `0x0899C764(image, &cur, &left, 0xFFFF)`; `0x089A0180(cur)` | 50 call sites: main 20 (`0x088D16F4`, `0x088CE0E4`, `0x0887CA50`, `0x0887EF84`...), menu 3, battle 27 (I: HUD) | V R |
| `img_emit` | `0x0899C764` | 332 | `image == 0` -> `0x0899C8B0` (1x1 dummy texture at `0x089D13C0`); flags&1: list at `+0x14` -> `BASE`+`CALL`, else `0x0899C944`; flags&2 -> `0x0899D014` (TEXLEVEL, UV scale/offset, blend, TFUNC) | `(image, u32 **cur, u32 *left, u32 flags)` | `0x0899D2F8` | V |
| `img_emit_texture` | `0x0899C944` | 1744 | **The only place image-library textures reach the GE.** Uses `+0x18`'s VRAM pair if set, else `+0x0C/+0x10`. Simple path (`flags == 0`): `TMODE = order`, `TPF = format`, `TBP0/TBW0 = table[0] / +0x1C`, `TSIZE0 = log2 h, log2 w`, `TFLUSH`; palette: `CMODE = +0x6C<<16 \| +0x68<<8 \| +0x64<<2 \| pal.format`, `CBP/CBPH = pal.table[0]`, `CLOAD = +0x1E`. Mip/animated path: one TBP/TBW/TSIZE per level of frame `+0x44`, or two frames (TBP0/TBP1) when `+0x08 & 1` | `(image, u32 **cur, u32 *end)` | `0x0899C764`, `0x0899C52C` | V |

Allocators (V): `FUN_0899B888(class, size)` uses an arena at `0x09AF6790[class]` if one is set (none is set
anywhere: I) else the callback `0x089DA4C0[class]`; frees go to `0x089DA4CC[class]`. `FUN_088667DC` installs
`FUN_088664C4`/`FUN_088664FC` (GRAPHICS::GRAPHICS heap, `*(0x089D5EC0)+0x710`, align 0x10) for classes 0 and 1;
`FUN_088668C0` installs `FUN_08866528`/`FUN_08866560` (GRAPHICS::EDRAM heap, `+0x714`) for class 2. The same
functions are installed into the GMO libraries (`0x089BA5BC`, `0x089BA5F8`, `0x089BA634`) and the model library
(`0x089AB464`, `0x089AB4A0`, `0x089AB4DC`) and `0x08940360`/`0x089403A0`/`0x089403D8` (I: the GMO file reader).
R: the allocations end in the game heap `FUN_0885C6CC`, whose 16-byte block header (`D012HeapBlockHeader` in the
header file) is `{next, prev, size, u8 0x10}`: the portrait's block `0x08E41FE0` read
`{0x092B4F40, 0x08E41F40, 0x10050, 0x10}`, its `next` becoming `0x08E52030` once the palette block was taken.

Direct callers of `0x0899BB68` seen at a battle load (R, from the return addresses): `FUN_0884C4DC` (three pairs
of loads), `FUN_08856ACC` (3), `FUN_0885911C` (5), `FUN_08854254`, `FUN_088586AC`, `FUN_0885AF78`, battle
`FUN_08ABAC10`, battle `FUN_08AC61AC` (3), besides 47 from `FUN_088DE0E8`; at start-up `FUN_088CDFBC` (once).

### 3.2 Image object layout

See `D012ImageObject` and `D012GimImageData` in [texture_types.h](texture_types.h). Where the GE inputs are, for
the first level of the current frame:

| GE input | Where | Mark |
| --- | --- | --- |
| Pixels | `img->image_addr` -> `D012GimImageData`; address = `*(u32*)(data + data->table_offset)` (absolute after load) | V R |
| Format | `data->format` (GE TPF value) | V |
| Swizzle | `data->order` | V |
| Width, height | `data->width`, `data->height` (TSIZE uses log2, rounded up for NPOT) | V |
| Buffer width | `img->buffer_width` (`+0x1C`) = `align_up(width, pitch_align*8/bpp)` | V R |
| Palette | `img->palette_addr` -> data; address `table[0]`, format `pal->format`, entries `pal->width`, CLUT load `img->clut_blocks`, CMODE shift/mask/start `img+0x64/+0x68/+0x6C` | V R (addresses, CLUT blocks) |
| Mips / frames | `data->level_count`, `data->frame_count`, table `[frame][level]` | V |
| Filter, wrap | `img->draw_flags` 0x400: TFILTER (0xC6) = `+0x7C << 8 \| +0x80` (mag/min, 1 linear and 7 linear-mipmap-linear at create); 0x800: TWRAP (0xC7) = `+0x88 << 8 \| +0x84` (u/v) | V |
| Blend, UV | `draw_flags` 1 two frames/palettes, 2 TEXLEVEL, 4 UV offset/scale (float bits >> 8; default block `0x089D13D0` = {0, 0, 1.0, 1.0}), 0x100 ALPHA from table `0x089D13E0` (entries {op, src, dst, FIXB}: 0 fixed, 1 normal, 2 additive, 3 reverse subtract), 0x200 TFUNC | V |

Checked against memory (R). At the bind (`0x0899D2F8`, 32 bytes at `a0`) of Lightning's portrait:

```
0x08E41F50: 00000001 00000000 00000000 08E41FF0 08E52040 00000000 00000000 00200100
```

`refcount` 1, `flags` 0, `draw_flags` 0, `image_addr` `0x08E41FF0` (data copy: `+0x02` set to 1, `+0x30` = table[0]
= `0x08E42030`, the drawn address), `palette_addr` `0x08E52040` (heap block `0x08E52030` + 0x10), `list_addr` 0,
`vram_addr` 0, `buffer_width` 256, `clut_blocks` 32 (256 RGBA8888 entries). The stores of `FUN_0899B888` match the
header's initial values: `+0x28`/`+0x2C` 1.0, `+0x30` -1000000.0, `+0x34` 1000000.0, `+0x38` 12.0, `+0x5C` -1,
`+0x68` 0xFF, `+0x70` 1, `+0x74` 0, `+0x78` 1, `+0x7C` 1, `+0x80` 7.

## 4. libGu (`0x0899F3FC`-`0x089A2C24`, context `*(0x09AF6830)`, list cursor at `+8`)

| Function | Address | Size | Emits | Mark |
| --- | --- | --- | --- | --- |
| `sceGuTexImage(mip, w, h, tbw, tbp)` | `0x089A0F04` | 128 | TBW, TBP, TSIZE, TFLUSH | V |
| `sceGuTexMode(tpsm, maxmips, a2, swizzle)` | `0x089A0DB8` -> `0x089A2008` | 60 / 68 | TMODE, TPF, TFLUSH | V (emits), I (arg names) |
| `sceGuClutMode(cpsm, shift, mask, start)` | `0x089A0FE8` | 60 | CMODE | V |
| `sceGuClutLoad(blocks, cbp)` | `0x089A1024` | 76 | CBPH, CBP, CLOAD | V |
| `sceGuTexFlush` / `sceGuTexSync` | `0x089A0FC8` / `0x089A0EE4` | 32 / 32 | TFLUSH / TSYNC | V |
| `sceGuTexFunc(tfx, tcc)` | `0x089A0E20` | 64 | TFUNC | V |
| `sceGuCopyImage` | `0x089A09A8` | 232 | TRXSBW before TRXSBP `0xB3/0xB2`, TRXDBP/TRXDBW `0xB4/0xB5`, TRXPOS, TRXSIZE, TRXKICK `0xEA`. Emits nothing unless `sx + width` and `dx + width` are both below 1024 (pspsdk's version has no such check) | V (emits; callers main `FUN_0886D618`, battle `FUN_08ACD138`) |
| list cursor get / set | `0x089A0144(int *left)` / `0x089A0180(cur)` | 60 / 16 | | V |
| `sceGuTexFilter` / `sceGuTexWrap` | `0x089A0DF4` / `0x089A0E60` | | | I |

`sceGuTexImage` callers (V addresses, I purposes): `0x0885E150` (512x512 copy), `0x08861208` (7 calls: 512x512,
256x128, 128x64, 64x32 - the screen-effect/bloom chain), `0x088710E8`, `0x08875FDC`, `0x088763B8`, `0x088EB65C`,
`0x088ED340` (512x512 screen copies), `0x0887BC2C` (two calls, `w = 512`, height/tbw/pointer from a text-line
record: the 512x32 text textures), `0x08972C14` (effects), `0x0897371C`/`0x0897381C` (effect render textures, I),
battle `0x08ACD1E8`. `sceGuClutLoad` callers: `0x08861208`, `0x08875FDC` (3), `0x0887BC2C` (text), `0x08972EBC`
(effects).

## 5. Model (GMO) textures

### 5.1 Chain

| Function | Address | Size | Role | Args / return | Callers | Mark |
| --- | --- | --- | --- | --- | --- | --- |
| `model_holder_new` | `0x0886D2C4` | 144 | Free holder (of 0xC0, 0x13C bytes from `*(0x089D5F94)+0x1C`), `FUN_0886DFC0` | `(name, data, size)` -> holder (I: data and size pass through in a1/a2) | fighter `0x0884333C`, others | V |
| `model_holder_build` | `0x0886DFC0` | 1552 | Split the file into GMOs (`0x0894045C(data,size,i)`), `model = 0x089AC120()`, `0x089B7B94(model, gmo)` per part | `(holder, name, data, size)` | `0x0886D2C4` | V |
| `gmo_model_load` | `0x089B7B94` | 168 | Plan (`0x089B72F4`), allocate (`0x089ABD28`), parse and copy the GMO into model memory (`0x089B54E4`, 7696 B), dcache writeback | `(model, gmo)` -> 1/0 | `0x0886DFC0` | V (calls), R (the texels are copied: section 5.3) |
| `gmo_parse` texture branch | in `0x089B54E4` | | Counts blocks 4/5/8/10/0xB into `model+0x20..+0x28` and their tables into `+0x0C..+0x1C`; for each block 10 (texture): sub-chunk 0x12 = name, 0x13 = `{u32 size; GIM}`; `tex = 0x089BB164()` (0x40-byte object), `0x089BEAC0(tex, gim, size, 0)` (GIM/TIM2/BMP/raw like the image library); on success `entry+4 = tex` and the name is registered (`0x089ABFD0` -> callback `*0x09AF7190`); otherwise the name is looked up (`0x089ABF9C` -> `*0x09AF7198`) and the found texture shared (`0x089BC7D8`, refcount `+0x00`) | | `0x089B7B94` | V |
| fighter setup | `0x0884333C` | 268 | Holder for the fighter; `holder+0x90 \|= 4`; `\|= 8` only if the name starts `R_` and the current part's `+4` is 0 | `(self, data, size)` | `0x08845230` | V R |
| stage setup | `0x08894F38` | 404 | Holder, `\|= 1`, `\|= 8` (always VRAM), `0x0886E86C` | `(self, name, data, size)` | `0x08892A78` ("SUPER SKILL"), `0x08892C50` and `0x08893218` (`map/%s.gmo`) | V R (name in a1) |
| stage setup 2 | `0x08893FF4` | 2928 | Holder at `self+0x74`: `\|= 8` if `*(desc+0x48) == 0`, else `\|= 4`; `0x0886E86C` | | `0x08892940`, `0x08893218` | V R (the world-map terrain's holder) |
| `model_lazy_build` | `0x0886A6A4` | 528 | Once per holder (`+0x90 & 0x40`): `0x089B0720(model, f)` for flags 4, 8 (and 1, 2 on the `0x10` path when `*(gfx+0x2CCF) == 0`); sets `0x20` when done | `(holder)` | `0x0886A3F4` (draw), `0x0886D550` (update over all 0xC0 holders), `0x0886E86C` | V |
| `gmo_build` | `0x089B0720` | 148 | Plan (`0x089AB7A0`, `0x089B02F0`), allocate (`0x089ABD28`), run `0x089B04E0`, free plan | `(model, flags)` -> 1/0 | `0x0886A6A4` only | V |
| `gmo_build_run` | `0x089B04E0` | 576 | flags 4: per part, per 0x40-byte item `0x089AF1E8` (I: materials/lists); flags 8 or 2: for each texture entry (`model+0x18`, 16 B, count `+0x26`) with `+4 != 0`: flags 8 -> `0x089BCC84(tex, ((flags>>1)&1) \| 2)`, else flags 2 -> `0x089BCC84(tex, 1)` (does nothing for textures) | `(model, flags, plan)` | `0x089B0720` | V R |
| `gmo_tex_place` | `0x089BCC84` | 148 | Same plan/allocate/run pattern: `0x089BC984` sizes, `0x089BAE80` allocates per class, `0x089BCAC4` runs, `0x089BADF8` frees | `(D012GmoTexture *tex, u32 mode)` -> 1/0; mode bit 1 = place in VRAM | `0x089B04E0` | V R |
| `gmo_tex_place_run` | `0x089BCAC4` | 448 | If no kind-1 record yet: `0x089BC2E0(image_rec, 1, 0x80000001, plan)` copies the record and its pixels (`0x089BBE20` -> `0x089BE68C` new record, `0x089BE908` per level/frame copy, same pixel order) into class 2 (EDRAM), `+8 = 1`, appended to `tex+0x0C` (`0x089BCE14`); palette likewise (`0x089BC174`, `0x089BCE74`); unless `tex->flags & 0x10`: compile the texture commands with `0x089BD8CC` into class-2 memory + `RET`, `tex+0x04 = list` (`0x089BCEDC`) | `(tex, mode, plan)` | `0x089BCC84` | V R |
| `gmo_tex_copy` | `0x089BE908` | 440 | The texel copy: 16 bytes per step (`lw`/`sw` x4), no calls | | `0x089BE68C` (same code unit) | V R |
| `gmo_tex_emit` | `0x089BD8CC` | 2980 | **The only place GMO textures reach the GE.** If `tex+0x04` and `(int)flags >= 0`: `BASE` + `CALL tex+0x04`. Else uses the first kind-1 (VRAM) record, falling back to the chain head (RAM): `TMODE = order`, `TPF = rec.format`, `TBP0/TBW0 = table[0] / align_up(w, rec.width_align)`, `TSIZE`, `TFLUSH`; palette `CMODE (tex+0x1D/1E/1F, pal.format)`, `CBP`, `CLOAD = pal.clut_blocks`; mips/animation like the image library | `(tex, u32 **cur, u32 *left, u32 flags)` | model draw `0x089B2DE8` (texture = `*(s1+0x28)` -> `+4`; I: s1 is the material being drawn), `0x089BC984`, `0x089BCAC4` (2), `0x089BEF84` | V |

Holder fields (V, from matching): `+0x94`/`+0x98` are two precompiled GE lists of the model (sizes `+0x9C`/`+0xA0`),
made by `0x0886E86C` (model_build); holders with a list are drawn through it (`0x0886A24C`). `0x0886C9AC` turns the
`CALL`s in such a list (command 0x0A, used for the BASE + CALL texture binds) into NOPs and back, so a port that
replaces texture binds must also see these lists. `+0x8E` motion count, `+0x10C` 3 bytes per motion, `+0x128`
release countdown (`0x0886CAA0` frees the holder's blocks at 0). `model_set_alpha` (`0x0886A298`) sets a fixed
blend from the view's alpha (`+0x40`); the model draw skips the model when it returns 0.

Stages (V): the map is loaded asynchronously by the state machine at `0x08893218` (states 2-5: the stage through
`0x08893FF4`; 6-9: the sky through `0x08894F38`, name `"SKY"` at `0x089CA7A8`); `0x08892C50` loads the same sky
synchronously.

Draw chain (V): `0x0886A3F4` (holder draw) -> `0x089B4798` -> `0x089B426C`/`0x089B4480` -> `0x089B2DE8` (5252 B)
-> `0x089BD8CC`. The model draw `FUN_0886ACCC` also calls `FUN_0886D618` (face patches, section 5.4) when
`*(*(model+0x84)) != 0` (P: the port's copy of `FUN_0886ACCC`, `run_model_draw` in
[d012_split_view.cpp](../../port/host/d012_split_view.cpp), makes the same calls as the game).

### 5.2 Where things live

| Thing | Where | Mark |
| --- | --- | --- |
| Source GIM | Inside the GMO: texture block 10, sub-chunks 0x12 (name) and 0x13 (`{u32 size; GIM}`; 0x8000 in the id = short header), parsed into `D012GmoTexRecord`s (kind 0) in model memory (GRAPHICS heap) | V (block ids, records), R (in the GRAPHICS heap: fighter atlases at `0x08D5xxxx`-`0x08FDxxxx`) |
| Texture i of a model | `model->textures[i].texture_addr`; entry i corresponds to the GMO's i-th 0x000A block (`FUN_08869F4C` looks a texture up by its 0x8012 name that way) | V |
| Texture name | The block's 0x12 sub-chunk (up to 0x20 chars kept). The graphics object keeps a registry `{char name[0x20]; u32 tex}` x 0x80 at `gfx+0x1AC8` (count `gfx+0x2CC8`), filled by `FUN_088663E0` only while `gfx+0x2CCC` is set and searched by `FUN_08866340`; `FUN_08893FF4` (stage setup 2) clears it (`0x08865828`), turns it on (`0x08865814(1)`) for its models and off again, so stage parts share textures by name | V |
| VRAM copy | A kind-1 record appended to `tex->image_chain` (and palette chain); pixels in GRAPHICS::EDRAM | V R |
| VRAM address choice | The game heap `FUN_0885C6CC` over `[0x04000000 + reserve, end of EDRAM)`, reserve = 0x110000 (16-bit framebuffers 0x0/0x44000, depth 0x88000) or 0x154000 when `*(gfx+0x2CCF)`; heaps are created in `FUN_0886603C`/`FUN_088668C0` | V (heap bounds), R (first stage texture at `0x04110080`), I (first fit) |
| Format, size | `rec.format`, `rec.width`, `rec.height`, `rec.bpp`, `rec.width_align`, `rec.order` | V |
| RAM textures | Every model without holder flag 8 (fighters, the world-map moogle) is drawn from its kind-0 record in RAM | V R |
| Per-texture GE state | `FUN_089BB0E0` initialises the texture object: `+0x18` 0, `+0x19` -1, `+0x1E` 0xFF, `+0x28/+0x2C` 1.0, `+0x30` 1, `+0x33` 1, `+0x34` 1 and `+0x35` 7, `+0x36/+0x37` 0: the same defaults as the image object's UV scale, TFILTER mag/min and TWRAP fields | V (stores), I (meanings) |

### 5.3 Fighter textures (R)

Quick Battle on Old Chaos Shrine, Kuja against Lightning. Each fighter has a 256x256 8-bit atlas (face, body,
hands) and 256x128 or 256x256 8-bit sheets (hair, gear, weapon), all in RAM; no fighter texture is in VRAM.

| Fighter | Atlas | Others |
| --- | --- | --- |
| Kuja (`P_NIN200`) | `0x08E09880` 256x256 | `0x08E19E80` 256x128, face sheet `0x08E22480` |
| Lightning (`P_GST130`) | `0x08FB8D00` 256x256 | `0x08FA8700` 256x256, `0x08FC9300` 256x128, face sheet `0x08FD9F00` |
| Lightning's assist (`P_FOR200_1P_A`; the versus screen shows an assist on the CPU's side), drawn from frame 8725 | `0x08E4EC00` 256x256 | face sheet `0x08E67800` |

The atlas is written once at the battle load, 16,384 word stores covering exactly its 64 KiB, right after the
fighter's model file is read:

```
[io] read fd=5 DISC0:/PSP_GAME/USRDIR/DATA/PACKAGE.BIN filtered offset=783335424 got=2105300 bytes -> 0x095DE880
[watch-write] uid=258 name=ffst_thread pc=0x089BE8D0 op=store32 address=0x08E09880 size=4 old=0x202d4d47 new=0x332b3338
...
[watch-write] uid=258 name=ffst_thread pc=0x089BE8D0 op=store32 address=0x08E1987C size=4 old=0xf7f7f7f7 new=0x4e4e4e4e
```

`0x089BE8D0` is the return point in `FUN_089BE68C` after its call to `FUN_089BAB64`; the stores themselves are in
`FUN_089BE908`, the word copy of the same code unit. This is the GMO parse (`0x089B54E4` → `0x089BEAC0` → ...),
before any `FUN_089BCC84` call: in that battle `FUN_089BCC84` was entered for the stage's 42 textures only. The
same model file gives the same atlas key on the world map at another address (`0x08D6ED80`): addresses move with
the heap, keys do not.

### 5.4 Face patches (R)

During a battle the CPU never stores to a fighter atlas (watch over the whole atlas: no store after the load), yet
its texture key changed six times. The changes are GE block transfers (`D012_TRACE_TRANSFERS`; widths and x in
16-bit units, so in pairs of 8-bit texels) from the fighter's face sheet (128 texels wide) into the atlas:

```
[transfer] frame 8401 0x8e22480 (0,48 row 64) -> 0x8e09880 (2,48 row 128) 24x48 x2: not from a render target
[transfer] frame 8401 0x8e22480 (32,0 row 64) -> 0x8e09880 (28,12 row 128) 8x48 x2: not from a render target
[transfer] frame 8401 0x8e22480 (48,0 row 64) -> 0x8e09880 (0,96 row 128) 16x32 x2: not from a render target
[transfer] frame 8401 0x8fd9f00 (0,32 row 64) -> 0x8fb8d00 (0,32 row 128) 32x32 x2: not from a render target
[transfer] frame 8401 0x8fd9f00 (32,32 row 64) -> 0x8fb8d00 (24,112 row 128) 24x32 x2: not from a render target
```

- Kuja: atlas texels x 4-51 y 48-95, x 56-71 y 12-59, x 0-31 y 96-127. Lightning: x 0-63 y 32-63, x 48-95
  y 112-143, x 0-31 y 96-127.
- They repeat when the expression changes (Lightning: about 90 transfers in 850 battle frames), in battle and on
  the world map (`0x08D8FF80` → `0x08D6ED80`, 69 transfers by frame 4800).
- Mechanism (P, from the pseudocode of `FUN_0886D618`; not matched yet): GMO texture animation. For GMO blocks
  named `"TexAnm"` (`0x0886DFC0`), `0x0886D8D4` sets up part entries (0x60 bytes; `+0x44` pending flag) that name
  two GMO textures (`+0x34` destination, `+0x38` source) and a rectangle (`+0x28`-`+0x32`); `FUN_0886D618` copies
  that rectangle from source to destination with `sceGuCopyImage` for every mip level (`>> level`), then
  `TFLUSH`. Any model can do this, not only fighters: a port keyed on texture contents must expect GPU-side writes
  into model textures.
- Issued by `FUN_0886D618` (700 B): GMO texture accessors (`0x089BCD44`, `0x089BD078`, `0x089BD0FC`,
  `0x089BD008`, `0x089BD08C`, `0x089BD1BC`, `0x089BD030`), then `sceGuCopyImage` (`0x089A09A8`) and
  `sceGuTexFlush` (`0x089A0FC8`) (V). It is called from the model draw `FUN_0886ACCC` (P). In the port that draw
  is host code (`run_model_draw`), so these calls never pass the dispatcher and could not be traced (section 11).
- Consequence: an atlas has one texture key per expression. A pack must hold one image per expression, or a
  native port replaces the atlas and the face sheet and redoes the copy at the higher scale (hook `FUN_0886D618`
  or the transfer).

### 5.5 VRAM placement (R)

World-map terrain, texture `0x08F8D920` (a `D012GmoTexture`), traced at `0x089BCC84` with a watch on VRAM
(`a0mem` = the first 32 bytes at `a0`):

```
[trace-pc] 0x089BCC84 ... a0=0x08F8D920 a1=0x00000002 ... ra=0x089B067C a0mem=0x00000001,0x08F8FAC0,0x08F8FAC0,0x08F8D980,0x08F8D9B0,0x00000000,0x0000FF00,0x00FF0000
[watch-write] ... pc=0x089BA6CC op=store32 address=0x041512EC ... new=0x414f240     record link (FUN_089BA654)
[watch-write] ... pc=0x089BC084 op=store32 address=0x04150000 ... new=0x216abd42    pixels, then CLUT (FUN_089BBE20 → FUN_089BE908)
[watch-write] ... pc=0x089BCBB4 op=store32 address=0x041512C0 ... new=0xc2000001    GE list (FUN_089BCAC4)
[watch-write] ... pc=0x0885C6CC op=store32 address=0x04151340 ... new=0x4200000     heap header of the next block, in VRAM
[trace-pc] 0x089BCC84 ... a0=0x08F8D920 a1=0x00000001 ... ra=0x089B0710 a0mem=0x00000001,0x041512C0,0x08F8FAC0,...
```

- One VRAM block per texture: record at `0x0414F240`, pixels `0x0414F280`-`0x0415127F` (128x128, 4-bit), CLUT
  `0x04151280` (16 RGBA8888 entries), GE list at `0x041512C0`:
  `C2000001 C3000004 A014F280 A8040080 B8000707 CB000000 C500FF03 B0151280 B1040000 C4000002 0B000000`
  (TMODE, TPF 4-bit, TBP0/TBW0 = `0x0414F280`/128, TSIZE0 128x128, TFLUSH, CMODE, CBP/CBPH = `0x04151280`,
  CLOAD 2, RET).
- Every texture is placed by the mode-2 call (return `0x089B067C`) and passed again with mode 1 (return
  `0x089B0710`), which does nothing for textures (V); the texture's `+0x04` goes from a RAM list (the value also in `+0x08`) to
  the VRAM list. 101 + 97 calls on the world map; 42 + 42 for the Quick Battle stage (first GE list `0x041120C0` =
  pixels `0x04110080` + 0x2000 + 0x40, the address the GE drew).
- So the GE list (`tex+0x04`) gives everything a texture key's pixel half needs: address, format, buffer width and
  size, for RAM and VRAM textures alike.

### 5.6 World map and story models (R)

The story's Continue loads `OVL_BATTLE_APP.ELF`: the world map runs in the battle overlay with the battle's model
path. The traced route reached the world map; the gateway board, story scenes and movies were not reached.

| Model | Loaded by | Holder flag 8 (VRAM)? | Textures seen |
| --- | --- | --- | --- |
| Lightning `P_GST130_1P` (and a `D_GST130` part, 0x4420 bytes), moogle `P_ORG800_1P_W` | fighter loader → `0x0884333C` | No | RAM (atlas `0x08D6ED80`, face transfers as in battle; moogle `0x08E39F00`, `0x08E31900`) |
| `R_SYM256`, `R_SYM240`, `R_SYM600`, `R_ORG801` | fighter loader → `0x0884333C` | Yes (`R_` name) | 8-bit textures in VRAM (`0x041FCE00` 64x32, `0x041A8800` 128x128; I: these are the `R_` models') |
| `F_WLD300` | fighter loader | No | |
| `map/sky02_p12.gmo` | `0x08892C50` → `0x08894F38`, name "SKY" | Yes, but its textures stayed in RAM | Two 512x256 and two 128x128 4-bit textures, drawn from RAM (`0x09058E00`, `0x09069000`, `0x09056B80`, `0x09079280`): their mode-2 call came last (frame ~4450), when the EDRAM heap was nearly full (last VRAM list `0x041FB940`, heap end `0x04200000`), and no mode-1 pass followed (I: the VRAM allocation failed) |
| `map/wld02.gmo` (terrain) | stage setup 2 `0x08893FF4` (holder "wld02" made from inside it, return `0x0889405C`) | Yes (reaches `0x089BCC84` mode 2) | 79 textures 128x128 4-bit in VRAM, `0x0413A600`-`0x041F9900` |

Holders made on the map (R, `a0` at `0x0886D2C4`): `P_GST130_1P`, `D_GST130`, `P_ORG800_1P_W`, `R_SYM256`,
`R_SYM240`, `F_WLD300`, `R_SYM600`, `R_ORG801` from the fighter setup (return `0x08843368`), "wld02" from
`0x08893FF4` and "SKY" from `0x08894F38`. A trace of `0x08893FF4`'s own entry saw nothing: its callers are in
the same code unit, so the call never passes the dispatcher.

So a flag-8 model can still be drawn from RAM when EDRAM runs out (I); a port must handle both for any model.

## 6. Effects (FEP `TEX` blocks)

| Function | Address | Size | Role | Args / return | Callers | Mark |
| --- | --- | --- | --- | --- | --- | --- |
| `fep_tex_bind_idx` | `0x08973400` | 184 | If index changed: `0x08972C14(effmgr[0][idx], cache?, filter/wrap...)`; cache = `effmgr+0x30` when `effmgr+0xC50 != 0` | `(effmgr, idx, minf, magf, wrapu, wrapv)` | `0x089C2BE8` (effect draw, called indirectly) | V R (cache at `effmgr+0x30`) |
| `fep_tex_bind_pal` | `0x08972BC8` | 76 | `0x08972C14` then `0x08972E64` (own palette) | | `0x08973334` | V |
| `fep_tex_bind` | `0x08972C14` | 592 | `TFUNC(0,1)`, `0x089A0E8C(0,0)`, `TexMode(fmt_table[tex+0x10], levels-1, 0, swizzle=1)`; no cache: `sceGuTexImage(level, w, h, align_up(w,16), tex + off)` per level; cache: `0x08974370` for level 0; then filter (table `0x089D0648`) and wrap (`0x089D0660`) | `(D012FepTex **tex, cache, minf, magf, wrapu, wrapv)` | `0x08972BC8`, `0x08973400` | V |
| `fep_vram_tex` | `0x08974370` | 704 | Find slot by `tex+0x04` (`0x089742B4`); if missing allocate (`0x08974084`, evicts the oldest) and emit a GE block transfer RAM -> VRAM (`TRXSBP/TRXSBW 0xB2/0xB3`, `TRXDBP/TRXDBW 0xB4/0xB5`, `TRXPOS`, `TRXSIZE 0xEE`, `TRXKICK 0xEA000001`, `TSYNC`); then `TBP0/TBW0/TSIZE0/TFLUSH` at the VRAM slot | `(cache, u32 **cur, fmt, w, h, pixels, key)` | `0x08972C14` | V R (transfers) |
| `fep_clut_bind` | `0x08972EBC` | 352 | `ClutMode(cfmt_table[clut+4], 0, 0xFF, 0)`; `ClutLoad(count / per_block, cbp)` or via the cache `0x08974630` | `(tex, cache, cbp)`; `cbp` is the TEX block's own palette (`0x08972E64`) or a caller's tinted copy (`0x089734B8`, cache `effmgr+0x640` when `+0xC54`) | `0x08972E64`, `0x089734B8` (from `0x08982350`, `0x089C2BE8`) | V R (cache at `effmgr+0x640`) |

Format tables (V): texture `0x089D0628` = `{3,0,1,2,5,4,6,7}` (index 0 = RGBA8888, 4 = T8, 5 = T4, 6/7 = DXT);
CLUT `0x089D066C` = `{3,0,1,2}`, entries per load block `0x089D067C` = `{8,16,16,16}`.

### 6.1 Effect data and the VRAM cache (R)

- **Load.** Effect files are copied whole by memcpy (`0x08935524`) from staging into effect memory; no read lands
  there directly. A fighter's effects come from its `.objx` (Kuja's `p_nin200.objx`, 1,448,360 bytes, copied at
  the battle load: the pixels at `0x09CCA2A0` were written by 8,208 byte stores after that read); common effects
  are copied at start-up and stay resident (the palette at `0x09BF6F50` was written once, before the menu overlay
  loaded, and never again in the run). Effect texture data sat at `0x09BE5A00`-`0x09E7E900`.
- **Cache layout** (V, 0x610 bytes): `+0` VRAM base, `+4` size, `+8` 64 slot records, `+0x508` free-slot stack,
  `+0x608` free count, `+0x60C` slot list; `FUN_089742F8` splits a slot. The effect manager holds two: `+0x30`
  textures and `+0x640` palettes (flags `+0xC50` / `+0xC54`); the current texture is `+0x18`. `0x08973EC8` gives
  the bits per pixel of a GE format.
- **Cache.** In battle the effect manager was at `0x09B857E0`: `*(effmgr+0x30)` = `0x04158C40` (texture cache base)
  and `*(effmgr+0x640)` = `0x04190C80` (palette cache base), found by a whole-RAM search from the battle load on
  (`0x09B85810`, `0x09B85E20`; both values also side by side at `0x09B86430`). The texture cache therefore holds
  at most 0x38040 bytes, the space between the two bases (I).
- **First use in battle.** Each effect texture is block-transferred into a slot the first time it is drawn, its
  256-entry palette into a 0x400-byte palette slot:

```
[transfer] frame 8401 0x9cca2a0 (0,0 row 32) -> 0x4158c40 (0,0 row 32) 32x64 x4: not from a render target
[transfer] frame 8401 0x9ccc2b0 (0,0 row 256) -> 0x4190c80 (0,0 row 256) 256x1 x4: not from a render target
[transfer] frame 8475 0x9bf2f40 (0,0 row 32) -> 0x415ac40 (0,0 row 32) 32x128 x4: not from a render target
```

  Slots from `0x04158C40`, 0x2000 per 128x64 and 0x4000 per 128x128 texture, up to `0x0418CC40` in one battle;
  palette slots `0x04190C80`-`0x04194080` (14). The GE then draws the effect from the slot.
- **Evictions.** One battle (Kuja's EX Mode and EX Burst included) made 31 pixel transfers of 27 textures into 25
  slots, and 118 palette transfers of 27 palettes into 14 slots. Slot `0x04158C40` held `0x09CCA2A0` and later
  `0x09CD2F60`; `0x09CCA2A0` was sent again to `0x0415AC40`; the palette `0x09BF6F50` went to `0x04191080`,
  `0x04191480`, then `0x04191080` again. A port keyed on the VRAM address sees one texture at several addresses and
  several textures at one address.
- **Palettes.** All 27 palette sources were the TEX blocks' own palettes (pixels + size + 0x10); no tinted copy
  from `0x089734B8` was sent in that battle, and the watched palette was not rewritten in place.
- **World map.** The same effect textures (same keys) are drawn straight from RAM (`0x09BF7380`, `0x09BF2F40`,
  `0x09C35F70`...) with no effect transfer: the cache was used in battle only, on the traced routes.

## 7. Other texture sources

| Source | Path | Mark |
| --- | --- | --- |
| Direct image loads | `0x0899B9F0` + `0x0899BB68` called by game code on GIMs already in memory: `0x0884C4DC` (two images per slot at `+0x2590/+0x2594`, stride 0x14; called by `0x088E897C`, `0x08910B54`, battle `0x08B13CA0`: I portraits), `0x08851C78` (refcounted single image; called by the fighter `objx` loader `0x088459E0`), `0x08852378`, `0x08854254`, `0x08854A5C`, `0x08856278`, `0x08856ACC`, `0x088586AC`, `0x0885911C`, `0x08859DA4`, `0x0885AC3C`, `0x0885AF78`, `0x0886646C`, `0x088AD460` ("SpecialDraw"), `0x088CDFBC` ("replay_CO.gim"), battle `0x08ABAC10`, `0x08AC61AC`, `0x08B52F88`. All end in the same image objects and the same `0x0899C944` | V (calls), R (eight of them load at a battle load, `0x088CDFBC` at start-up; section 3.1), I (purposes) |
| Story / world map | 2D parts through the texture manager (`menu/menu_pk_world_map_gim_0.bin`); 3D parts through the GMO path (section 5.6) | R |
| Face patches | `FUN_0886D618` → `sceGuCopyImage`, section 5.4 | V R |
| Text lines | `0x0887BC2C` builds 512-wide 4-bit textures in RAM and binds them with `sceGuTexImage`/`sceGuClutLoad` (noted only) | V (calls), D (format) |
| Render targets / screen copies | `sceGuTexImage` with VRAM framebuffer addresses (`0x08861208` etc.) | I |
| Default texture | 1x1 at `0x089D13C0` (`0x0899C8B0`, image == 0) and `0x089DAF80` (`0x089BD8CC`, tex == 0) | V R (the 1x1 at `0x089DAF80` was drawn on the world map) |

## 8. Memory map of texture data

| Where | What | Mark |
| --- | --- | --- |
| Staging buffers (texture manager heaps `mgr+4/+8`, caller heaps; `0x0973D880` title, `0x092B4F80` player select, `0x095DE880` / `0x095D9880` the 0x264000-byte model block) | Package files while loading; freed or reused after | V R |
| Start-up data (`main.bin` at `0x09644A80`, `main_lang.bin` at `0x097C4A80`) | Source of the battle HUD GIMs, copied at the start of a battle load; the same memory then takes the model staging block and the stage file | R (I: the reuse) |
| GRAPHICS::GRAPHICS heap (`FUN_0888204C(0x780000)`, handle `*(0x089D5EC0)+0x710`) | Image objects and their image/palette copies (all 2D GIM/TIM2/BMP textures), GMO models with their RAM texture records (`0x08C1xxxx`-`0x091Bxxxx` in the runs) | V R |
| GRAPHICS::EDRAM heap (`0x04110000`/`0x04154000` to `0x04200000`, handle `+0x714`) | GMO VRAM copies (flag-8 models: stage from `0x04110080`, world map `0x0413A600`-`0x041F9900`), then the effect caches; `0x0899C52C` image copies (one menu caller) | V R |
| Effect texture cache (`*(effmgr+0x30)` = `0x04158C40` in battle) and palette cache (`*(effmgr+0x640)` = `0x04190C80`) | FEP textures and palettes copied by GE block transfer | V R |
| Effect memory (`0x09BE5A00`-`0x09E7E900` in the runs) | FEP effects and their TEX blocks: start-up copies (system effects) and per-fighter copies from `.objx` | R |
| RAM, rewritten | Text lines, screen copies, fighter atlases (face patches) | D R |

## 9. Override points for a port

| # | Point | Signature (best reading) | Sees at entry | Sees at return | A replacement must fill | Covers |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `0x0899BC64` `img_attach_picture` (or the return of `0x0899BB68`) | `int f(D012ImageObject *img, const D012GimBlockHeader *picture)` | The GIM picture in the loaded file (still in staging); image blocks under it | `img->image_addr/palette_addr` copies with absolute tables, `flags`, `buffer_width`, `clut_blocks`, UV scale, CMODE fields | Recommended: let the original run (game code reads these fields back) and record `img -> HD texture`. A full replacement must build `D012GimImageData` blocks with the same header fields and absolute table and then run (or reproduce) `0x0899BFE0` | Every 2D GIM, TIM2 needs `0x0899E4B0`, BMP `0x0899D348` (or hook `0x0899BB68`) |
| 2 | `0x0899C944` `img_emit_texture` (or `0x0899D2F8` `img_bind`) | `int f(D012ImageObject *img, u32 **cursor, u32 *end)` | The image object | GE commands written at `*cursor`, cursor advanced | Native: bind the HD texture for `img` (key by the image object or by `table[0]` pixel address + format/size), and either write no texture commands or write the original ones for state the GE still needs (TFLUSH, CLUT) | All 2D drawing of image-library textures, including `0x0899C52C`'s precompiled lists (only one menu caller) |
| 3 | `0x089BD8CC` `gmo_tex_emit` (keys: the GMO texture name from block 0x12, plus the model's file name) | `int f(D012GmoTexture *tex, u32 **cursor, u32 *left, u32 flags)` | The GMO texture with its RAM and (maybe) VRAM records | GE commands at `*cursor` | Same as 2, keyed by `tex` or by `image_chain->level_table[0]` of the kind-0 record (stable: the kind-1 address depends on EDRAM heap order). Must also stop the precompiled path: either skip `0x089BCAC4`'s list (leave `tex+0x04` at the RAM list) or handle `CALL tex+0x04`. For fighters, the atlas's pixels change with each face patch (point 8) | All model textures (stages, fighters, world-map models) |
| 4 | `0x089BCC84` `gmo_tex_place` | `int f(D012GmoTexture *tex, u32 mode)` | The texture with its kind-0 record (RAM) | Kind-1 VRAM record appended, list compiled | A native port can return 1 without copying (keeps the texture in RAM, as fighters already are) and upload the HD texture instead | VRAM placement of flag-8 models |
| 5 | `0x08972C14` `fep_tex_bind` (+ `0x08972EBC`) | `void f(D012FepTex **tex, void *cache, int minf, int magf, int wrapu, int wrapv)` | The TEX block (always swizzled), level table, key `+0x04` | GE state set through libGu / block transfer | Bind the HD texture by `tex+0x04` key or pixel address (never the cache slot's VRAM address: slots are reused); set filter/wrap; palette from `0x08972EBC`'s `cbp` (may be a tinted copy, so re-read per bind) | Effects |
| 6 | `0x088DE0E8` `tex_entry_load` | `int f(D012TexEntry *e, const void *gim, u32 size, u16 a3, u16 id, u8 group)` | GIM bytes, `(group, id)` | `e->image_addr` filled | Fill `e` (`+0..+0xA`); `e->image_addr` must be a usable D012ImageObject because `0x088D16F4` passes it to `0x0899D2F8` | Package GIMs; gives a stable key `(package path, group, id)` |
| 7 | `0x08862854` `pkg_load` / `0x088FE708` `file_cache_read` | `void f(const char *path, void *dst, u32 size, u8 flag)` | The path (before localisation) | File bytes at `dst` (sync path) or a queued request | Same bytes (a replacement GIM must keep the original layout); best place to know *which file* a texture came from. Fighter, stage and map files go through `0x088FE708`, and some come from the file cache without reaching `0x08862854` | All package reads |
| 8 | `0x0886D618` (face patch) or the block transfer it issues | `void f(model)` (I) | The model, its face sheet and atlas (through the GMO texture accessors) | Transfers queued in the display list | Redo the copy into the HD atlas from an HD face sheet, at the HD scale, or key the atlas on its expression | Fighter faces |

Recommended split: keep the game's own load path untouched (1, 6, 7 as observation hooks to learn names and
register keys), and replace the three emitters 2, 3 and 5 with native binds, plus point 8 for fighter faces. The
emitters are the only places the texture state is decided, their inputs are fully mapped, and the game reads
nothing back from them.

What must stay as it is, whichever point is replaced (V):

- **The original sizes.** Game code reads image sizes back: e.g. `0x08859194` gets the image data through
  `0x0899C1C0` and uses `width` (`+0x08`) to place and size its quads; sprites are drawn in through mode with
  texel UVs (`0x088D16F4`, `0x0899C944` NPOT scale). An HD texture must be bound as a scaled stand-in for the
  original size, with the GE state (TSIZE, UV scale) of the original.
- **The objects' bookkeeping.** `D012TexEntry +0x04..+0x10`, image `refcount`, the `+0x02` refcounts of the copied
  blocks, the GMO record chains and `kind` are walked by release code (`0x088D0370`, `0x0899BEE0`, `0x089BCD70`);
  a replacement that does not allocate them must still leave valid values.
- **The palette path.** Effects can pass a tinted CLUT copy at bind time (`0x089734B8`; not seen in the traced
  battle), and the image library can switch CLUT banks through `+0x60..+0x6C` (I: animated GIMs). An HD texture
  keyed on pixels alone would lose those tints, so either keep HD textures indexed or key them on (pixels,
  palette).

## 10. Open questions

1. Whether `FUN_089BCC84` fails for lack of EDRAM (the world-map sky's textures stayed in RAM after their VRAM
   call, with the heap nearly full): its return value was not seen.
2. Whether `0x088D2980`'s 0xFFFFFFFF return is ever used by a caller (the max-id logic looks dead).
3. The file cache (`*0x089D69DC`, `0x08883538`): what it held at a battle load is in 1.3; when it is filled and
   whether fighter files are ever served from it was not seen.
4. The GMO texture object (0x40 bytes, I) fields `+0x1C`, `+0x20..+0x3F`; whether the name-registry callbacks share
   textures outside stage setup 2 (they are installed once by `0x088665F0` -> `0x089ABF88` with
   `FUN_08866340`/`FUN_088663E0`, and only register while `gfx+0x2CCC` is set).
5. Whether GMO records are swizzled in retail files (`rec.order`); `0x089BE908` never converts (a plain copy).
6. The effect texture cache's size field (`cache+4`; it does not hold 0x38040, the space between the two bases)
   and its slot list at run time (`cache+0x60C`); `fep_vram_tex`'s arguments could not be traced (section 11).
7. Which caller supplies tinted CLUTs to `0x089734B8` and how often (none in one Quick Battle).
8. Purposes of the direct image loaders in section 7 (portraits, world map pieces, replay, "SpecialDraw").
9. The one menu caller of `0x0899C52C` (`0x08B45380`): which screen (not reached from the title to the player
   select).
10. `0x0899C364` (`img_set_param`) has no direct caller and was not entered on the menu route: which screens use it.
11. Story scenes, the gateway board and movies: not reached by the traced route.

## 11. How it was traced

The port's clang build (`tools/local/port/build-d012-summons`, `PSPRECOMP_AOT_PRODUCTION_FASTPATHS=OFF`), run by
[gfx_tour.py](../../tools/port/gfx_tour.py) (variant `nointerp`, data copy `build/port/data-gfx-tour`, at most 90 s)
on the routes `qb_kuja` (Kuja against Lightning, Old Chaos Shrine, EX Mode and Burst), `story_gateway` and an
ad hoc menu route (title to the player select and one cursor move). Switches:

| Switch | Gives |
| --- | --- |
| `D012_TEXTURE_PACK=<folder with a one-key textures.ini>` + `D012_TRACE_TEXTURE_PACK=1` | Every texture at first use: `[texpack] 0x<addr> WxH fmt=N key <16-hex cache key><8-hex data hash>` |
| `D012_TEXTURE_DUMP=<dir>` | The textures as PNGs, by key |
| `D012_TRACE_IO=1` | Each read with its destination |
| `D012_TRACE_TRANSFERS=1` | Each GE block transfer (`[transfer] frame F src (x,y row R) -> dst (x,y row R) WxH xBPP`) |
| `PSPRECOMP_WATCH_WRITE=<addr>`, `PSPRECOMP_WATCH_WRITE_SIZE=<bytes>` | Every guest store to the range, with the thread and the last address entered through the dispatcher |
| `PSPRECOMP_NO_CHAIN=1` | Every cross-unit call goes through the dispatcher, so the watch's `pc` is the function entered (or the return point after its last cross-unit call, or a callee in the same 16 KiB code unit) |
| `PSPRECOMP_TRACE_PC=<addr>` + `PSPRECOMP_TRACE_MEM=1` | Registers, `ra`, the strings and the first 32 bytes at `a0`/`a1` when the dispatcher enters the address |
| `D012_FIND_INT32=<value>` | Every RAM word holding the value, every 900 frames |

Limits met:

- GE block transfers write guest memory in the host (`block_transfer` in PortableKit's `host/hle/hle_media.cpp`)
  and never reach the watch; only `D012_TRACE_TRANSFERS` shows them.
- `PSPRECOMP_TRACE_PC` uses the slow diagnostic dispatch loop; with `PSPRECOMP_NO_CHAIN` a Quick Battle route stops
  at the 90 s limit on the versus screen (frame ~7400, the battle starts at 8377), so functions called only in
  battle (`0x08974370`) could not be traced. With chaining on, a function entered only through chained calls is
  never seen (`PSPRECOMP_CHAIN_DEPTH=2`: no hit for `0x08974370`).
- Guest functions called from the port's host code (`g_call`, e.g. `FUN_0886D618` from `run_model_draw`) never
  pass the dispatcher, and neither do calls inside one code unit (`0x08893FF4` from its callers): such functions
  are found through the return addresses of the functions they call.
- `PSPRECOMP_COUNT_PC` counts but never prints (`report_counted_pcs()` has no caller), and no switch dumps guest
  memory at a point (`D012_DUMP_SLOT` writes only the overlay window).
