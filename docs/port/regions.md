# Other releases: Europe and Japan

Inspected 3 November 2026. Inputs: the owner's Europe and Japan ISOs, compared file by file with the USA v1.00 image
the port is built on. The European disc is supported since the same day ([below](#how-the-port-supports-it)).

## Summary

| | USA (supported) | Europe | Japan |
| --- | --- | --- | --- |
| Disc ID in `PARAM.SFO` | ULUS10566 | ULES01505 | NPJH50377 (disc label ULJM05814; the ULJM06121 reprint is the same image) |
| Redump entry, size and SHA-1 match | 22911 | 26793 | 25036 |
| Image size | 1,754,529,792 | 1,754,529,792 | 1,768,259,584 |
| Decrypted `EBOOT.BIN` | `0bc8e779…` | **the same** | different build, 4,811,092 bytes |
| Three overlays and `LIBSUPPREACC.PRX` | | **the same** | overlays differ and load at `0x089FBD00`, not `0x089DD4C0`; the PRX is the same |
| Other game files (`PACKAGE.BIN`, `PACKAGE_INFO.BIN`, movies) | | the same except 140 bytes at the start of `PACKAGE.BIN` | 45 files differ, 3 more files |
| In the port | | **supported** (3 November 2026, below) | not supported: it would be a second port of a different executable |

## Europe: the same game with another disc ID

Of the disc's 187 files, 181 are byte-identical to the USA disc. The six that differ:

- `EBOOT.BIN`: its encryption differs (another container, same tag `0xD91613F0`, same key), but pspdecrypt turns it
  into exactly the USA executable, SHA-256 `0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d`.
  The EU encrypted file's SHA-256 is `0c0c12c0f46e9cc37adb0a649a761975fab92a20c8d2e9252f514ef29d1ca289`.
- `PACKAGE.BIN`: same size; 140 bytes differ, all in its header:

  | Offset | USA | Europe |
  | --- | --- | --- |
  | `0x08` | `us` | `eu` |
  | `0x14` to `0x4F` | ULUS10566, NPUH10110, ULUS10437 | ULES01505, NPEH00108, ULES01270, UCES01229 |
  | `0xD10` | `ULUS10566Data` | `ULES01505Data` |
  | `0xD5D`, `0x19C0`, `0x1A20` | 16, 32 and 16 bytes | other values (keys or hashes; not read yet) |

  The IDs are this game, its demo and the earlier Dissidia releases a save can be imported from.
- `PARAM.SFO`, `UMD_DATA.BIN` and the system update: disc metadata (the disc ID and the firmware update).

No executable contains a disc ID; the save path is the format `ms0:/PSP/SAVEDATA/%s%s` and the DLC path
`ms0:/PSP/GAME` then `%s/%s.edat`. The game takes its own ID from that `PACKAGE.BIN` header, and a run confirmed it
(below): on the EU disc it asks for saves `ULES01505GameData*`, data install `ULES01505Data` and DLC under
`PSP/GAME/ULES01505`. Its save key is the same as on the USA disc (`5a802015…`). What the `eu` byte at `0x08`
changes, if anything, is not known; nothing differed on the route below.

Because the code and data are the same, the recompiled code, every patch and hook, the mods, texture packs, BGM
Maker's output and the Noctis files carry over unchanged.

### How the port supports it

- **PortableKit patch 0077** adds `RegionRelease` to the profile: another region's disc of the same executable.
  The installer takes its disc ID and its encrypted `EBOOT.BIN`, and checks that it decrypts to the profile's
  executable. At start `host/main.cpp` reads the disc ID from the image and makes that release the running one, so
  the save screens and save transfers use its save folders and the menu's About names the disc. Textures and the
  ad hoc product code stay the profile's.
- **The profile** (`port/host/d012_profile.cpp`) lists ULES01505 with its encrypted `EBOOT.BIN`'s SHA-256
  (`0c0c12c0…`) and the save folders `ULES01505GameData00` and `01`. It also gives the Japanese disc ID and the Asian
  release's two serials a sentence for the installer's refusal.
- **The DLC report** (`port/host/d012_dlc.cpp`) names the running disc's folder.
- **setup.ps1** (`tools/port/setup/game.py`) accepts either disc by its Redump SHA-1, and so does **BGM Maker**'s
  ISO export.
- **Texture packs** stay in `textures/ULUS10566` for both discs, since the images are the same.
- **Online play** announces `ULUS10566` from both, so a USA copy and an EU copy can find each other. They run the
  same code and data, but the disc ID in guest memory differs, so the boot-to-battle lockstep test (`D012_NET`,
  which hashes all of guest RAM) would report the copies as apart. The released Communication Mode battle does not
  hash RAM. A USA-against-EU battle has not been tried.

### Verified (3 November 2026)

With an installer-type clang build of this branch, in a fresh portable folder. The game code was reused from the
v0.5.1 test install, which is the same executable.

- `--install` with the EU ISO wrote `data/EBOOT.ELF` with SHA-256 `0bc8e779…`. The Japanese ISO was refused with its
  sentence.
- The start log says `Release: the European release (ULES-01505)`, and `[dlc] none in` the `PSP\GAME\ULES01505`
  folder.
- New Game ran through data setup, the opening movie and the tutorial battle, saving 257,224 bytes, encrypted, to
  `ULES01505GameData00`. The next start listed that save, autoloaded it and loaded it from Load Game.
- The USA disc in the same build still reports `PSP/GAME/ULUS10566`, and the game asks for `ULUS10566` saves.
- setup.ps1's ISO check accepts both discs by SHA-1 and refuses the Japanese one. `d012_tests` and PortableKit's
  profile and save-data tests pass.

Not tried: EU DLC files, the menu's About row and the save transfer screens, BGM Maker's ISO export from the EU
ISO, a full setup.exe install, the Steam Deck, and online play between a USA copy and an EU copy.

## Japan: a different build

Japan's release is the same engine built separately, Japanese only.

- The executable is 170 KB larger and the overlays sit 0x1E840 bytes higher. Measured on 8-instruction blocks with
  jump targets and `lui` values masked: 79% of USA main's blocks occur in Japan's main, 72% of the menu overlay's,
  69% of battle's. Masking all 16-bit immediates raises that to 83 to 96%. Most code is the same functions at other
  addresses; part of battle is changed code.
- `OVL_EXTRA_APP.ELF` is 104 KB, not 24 KB.
- It adds `LIBMD5.PRX`, a `~SCE` module pspdecrypt does not open as is.
- 45 data files differ: `PACKAGE.BIN` and `PACKAGE_INFO.BIN` have other sizes, so the file table differs; 37 movies
  differ (Japanese voices and text).

Everything the port ties to USA addresses would need a Japan counterpart: about 540 guest addresses in 20 files of
`port/host`, 217 in 9 decompiled C files in `port/decomp`, and 2 PortableKit patches. The recompiler itself is not
the obstacle; the function inventory, the hook addresses and the data tools (BGM Maker, the character mods, texture
packs for Japanese text) are. A USA-to-Japan address map built by matching functions, which the numbers above make
possible, would do most of the remapping, but each feature still needs checking in a Japan run.

Not inspected: the Asian release (UCAS40329, Redump 25390), a different image size again.

## Reproducing

Scripts used are throwaway; the facts can be checked with tools already here:

```powershell
python tools/inspect_iso.py "<Europe or Japan ISO>" --output <file>.json
```

and `tools/local/pspdecrypt/pspdecrypt -o EBOOT.ELF EBOOT.BIN` in WSL on the `EBOOT.BIN` taken from each disc.
