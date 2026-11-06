# Cheats

> 6 November 2026. PortableKit patch 0085 and `port/host/d012_cheats.cpp`. Windows, clang build, USA disc.

The owner asked for cheats that give the same results as PPSSPP's. The port's menu (Esc, or L3 + R3) has a
**Cheats** page, between Network and System:

- **Use cheats** turns every cheat on or off at once (`cheats.enabled`).
- **The port's own cheats**, under Money and points, Items, Battle and Opponent. A cheat with several amounts is one
  row with Off and each amount. They are saved in `settings.ini` as `cheats.builtin=key=choice,...`.
- **PPSSPP cheat file** opens a page of the cheats in the player's own PPSSPP cheat file, which turns them on and off
  and writes the file back, as PPSSPP's cheat screen does.

## How cheats run

PortableKit's engine (`host/cheats/`, patch 0085) reads CWCheat codes with PPSSPP's meaning and runs them as PPSSPP
does: every cheat that is on, from its first line, every 77 ms of the game's time. The port's own run first, then the
file's. An address in a code is an offset from `0x08800000`.

The file is PPSSPP's: `PSP/Cheats/ULUS10566.ini` in the memory stick folder (`data/ms0` in an install;
`ULES01505.ini` with the European disc). PPSSPP's own file for the game can be copied in as it is. Action Replay
lines (`_M`) are read but not run.

A recompiled game does not change when its code is written, so a write into the game's code (the executable's code,
or an overlay's) is left out. The page marks such a cheat **Changes code**; its other writes are made. Running those
needs the interpreter (a later step).

## Compared with PPSSPP

PPSSPP 1.20.4 was run beside the port with the same save (its own memory stick in `tools/local`, nothing of the
player's), and both were read at the PP Catalog through PPSSPP's debugger. PP lies at `0x096B2DC0` in both, with the
same table around it, so the public list's data codes land where they do in PPSSPP. The catalog reads PP only when it
opens, in PPSSPP too: 20000 PP shows after leaving the catalog and opening it again.

One difference: the 3.9 MB block the game asks the system for (its descriptor at `0x09B4D420`) starts at `0x09B85700`
in the port and at `0x09B95A00` in PPSSPP, `0x10300` lower, with the same contents after the shift. No code in the
public list touches it.

Saves the port writes are 8 bytes longer than PPSSPP's (257,248 against 257,240), and PPSSPP cannot load them; cut
to PPSSPP's size, it can. A separate fix.

## The port's own cheats

The codes are those of the public CWCheat list for ULUS-10566 (addresses and values: facts about the game's memory);
the names, the grouping and the choices are the port's. The CWCheat database (cheat.db) has no licence and is not
used or shipped.

| Row | Choices | Writes | Seen in the game |
| --- | --- | --- | --- |
| Gil | 1,000,000 / 99,999,999 | `0x08BBFA00` | both, in Customization and on the world map |
| KP | 250 / 1,000,000 / 99,999,999 | `0x08BEE31C` (1,000,000 also `0x09692D14`) | 1,000,000 on the world map |
| PP | 20,000 / 9,999,999 | `0x096B2DC0` | 20,000 in the PP Catalog |
| All equipment ×99 | on | a count of 99 every 4 bytes from `0x08BBE41A`, 677 of them | the Equip list, every item ×99 |
| All accessories ×99 | on | the same from `0x08BBEEC6`, 589 | the accessory list, every item ×99 |
| Mala Bestia accessories ×99 | on | 9 of those counts | no |
| All summons | on | 61 summonstones from `0x08BBF8BD`: 10 uses and a 3 | every summonstone, 10 uses left |
| Infinite HP, Max HP 999,999, Bravery 9,999, Infinite EX, Infinite Assist, Level 100 | on | 32 bits at the word at `0x08B633B0` minus `0xE58`, `0xE54`, `0xE4C`, `0xE48`, `0xE44`, `0xE60` | Bravery 9999; Enter EX Mode offered; Lv100 in battle and in Battle Setup |
| Opponent: Bravery 0, Max HP 100, No EX, No Assist | on | the same minus `0x2C`, `0x34`, `0x28`, `0x24` | Bravery 0 and a Break; max HP 100 |
| Opponent: Defeat with Select | on | `0x38` = 999,999 while Select is held (a joker line) | the opponent falls at once |

The list's codes were changed in three places:

- Its **1,000,000 Gil** writes `0x08FF93F4` and `0x08FFB2D0`; the gil shown stayed at 788, so the port writes its
  Max Gil address, `0x08BBFA00`, for both choices.
- Its longer **accessories** code (672 counts) reaches the summonstones' bytes, so the port uses the shorter one (589).
- Each **battle** code is followed in the list by a write of 0 to `0x0880052C`, below the game, which the port leaves
  out.

Left out of the list: **CP Doesn't Matter** and **Remove Bloom** (they write the game's code; the port has its own
Bloom switch), the **widescreen** and **frame rate** codes (the port's aspect ratio and frame rate settings do those,
and the codes would fight them), the **Chaos** codes (a fan mod written as cheats; the owner chose to leave it out),
and the **AP after battle** codes (`0x09AEA568`): with 255 chosen, a Quick Battle still gave 1 AP.

## Switches

| Variable | Effect |
| --- | --- |
| `D012_NO_CHEATS=1` | No cheats at all; the page says so |
| `D012_CHEATS=gil=2,kp=1,...` | The port's own that are on, for one run (the rows are then greyed) |
| `D012_TRACE_CHEATS=1` | Each pass's cheats, with what they did |
| `D012_CHEAT_INTERVAL_MS=<ms>` | Another interval than 77 ms |

## Verified (6 November 2026, Windows, clang build, USA disc)

- `portablekit_cwcheat_tests`: 30 checks of the parser and every code type.
- A file cheat (20000 PP) gave the same result as in PPSSPP, at the same address.
- The port's own, as in the table: on the game's screens, through input scripts and window captures.
- The page: its headings and rows, the cheat file page and back, and a change made with the pad saved to
  `settings.ini`.

Not verified: KP 250 and 99,999,999, PP 9,999,999, Mala Bestia, Infinite HP under real damage (the CPU barely
attacked), Max HP 999,999, Infinite Assist, the opponent's No EX and No Assist, whether Level 100 stays after the
cheat is turned off and the game saves, a cheat that writes code, the European disc, Linux and the Steam Deck.
