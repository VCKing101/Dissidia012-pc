# Cheats

> 6 November 2026. PortableKit patch 0085 and `port/host/d012_cheats.cpp`. Windows, clang build, USA disc.

The owner asked for cheats that give the same results as PPSSPP's. The port's menu (Esc, or L3 + R3) has a
**Cheats** page, between Network and System:

- **Use cheats** turns every cheat on or off at once (`cheats.enabled`).
- **The port's own cheats**, under Money and points, Items, Battle and Opponent (AP, under Money and points, is set
  by the port itself rather than by a code: [AP after a battle](#ap-after-a-battle)). A cheat with several amounts is one
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

**Online, none run** (patch 0111). While the game has ad hoc play running, from its `sceNetAdhocctlInit` (or
`sceNetAdhocInit`) to the `Term`, that is in Communication Mode's online screens, its battles and Communications,
no cheat runs: not the port's own, not the file's, and not the AP row. A cheat changes one player's game only. The
battle cheats write "your" fighter, which is a different fighter on each player's game, so even two players with the
same cheats on would part ("Network sync failed"), and the other player would fight against them. Nothing in the
settings changes: the cheats that were on run again once the game leaves Communication Mode. The page says so while
they are paused, and the log says `[cheats] paused: the game is playing ad hoc`, then `running again`. Tested in
[Communication Mode, section 20](communication-mode.md#20-dlc-on-one-side-only-and-cheats-online-17-november-2026).

## Compared with PPSSPP

PPSSPP 1.20.4 was run beside the port with the same save (its own memory stick in `tools/local`, nothing of the
player's), and both were read at the PP Catalog through PPSSPP's debugger. The catalog's copy of PP lies at
`0x096B2DC0` in both, with the same table around it, so the public list's data codes land where they do in PPSSPP. The
catalog copies PP in only when it opens, in PPSSPP too: the list's 20000 PP shows after leaving the catalog and opening
it again. The port's own PP cheat writes the save's PP instead (next sections).

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
| KP | 250 / 1,000,000 / 99,999,999 | `0x08BEE31C` | all three, on the world map |
| PP | 20,000 / 9,999,999 | `0x08BCCB38` | both, in the PP Catalog the first time it opens |
| AP | 100 / 999 | no code: the battle's rewards, as the game works them out ([AP after a battle](#ap-after-a-battle)) | "Received 100 AP." and "Received 999 AP." where the battle gave 1 |
| All equipment ×99 | on | a count of 99 every 4 bytes from `0x08BBE41A`, 677 of them | the Equip list, every item ×99 |
| All accessories ×99 | on | the same from `0x08BBEEC6`, 589 | the accessory list, every item ×99 |
| Mala Bestia accessories ×99 | on | 9 of those counts | Growth Egg, Valor Resin, Chocobo Down, Wing and Feather, Supershine, Mog's Amulet, Qu'Bia Tail ×99 |
| All summons | on | the 61 summonstones' slots from `0x08BBF89C`: 10 uses and flags 3 | all 61 owned with 10 uses, the empty slots left alone ([All summons](#all-summons-and-the-empty-slots)) |
| 10 Skills | on | no code: Story Mode's Skills on two pages of five, L2 or R2 to turn the page ([10 Skills](#10-skills-two-pages)) | the world map's and a gateway's Skill bar, both pages, a save and a reload |
| Infinite HP, Max HP 999,999, Bravery 9,999, Infinite EX, Infinite Assist, Level 100 | on | 32 bits in the player's status block, `+0x08`, `+0x0C`, `+0x14`, `+0x18`, `+0x1C`, `+0x00` ([the fighters](#the-battle-cheats-and-the-fighters)) | HP stayed at 1242 through a 755-damage HP attack (0 without the cheat); HP 999999; Bravery 9999; Enter EX Mode offered; the Assist gauge full with Use Assist offered; Lv100 |
| Opponent: Bravery 0, Max HP 100, No EX, No Assist | on | the opponent's, `+0x14`, `+0x0C`, `+0x18`, `+0x1C` | Bravery 0 and a Break; max HP 100; against a Very High CPU, its EX and Assist gauges stayed empty (without the cheats it filled both and entered EX Mode) |
| Opponent: Defeat with Select | on | the opponent's `+0x08` = 999,999 while Select is held (a joker line) | the opponent falls at once |

**Level 100 is a real level-up.** It sets the level of the fighter in battle, and the game makes that the character's level: after the cheat is turned off, Customization still shows Lv100 with 1,969,359 EXP and HP 6,999, and the game keeps it when it saves.

The list's codes were changed in seven places:

- Its **1,000,000 Gil** writes `0x08FF93F4` and `0x08FFB2D0`; the gil shown stayed at 788, so the port writes its
  Max Gil address, `0x08BBFA00`, for both choices.
- Its **1,000,000 KP** also writes `0x09692D14`, in the game's pool, which other data uses in a battle. The other two
  KP choices write only `0x08BEE31C` and show on the world map, so 1,000,000 does the same.
- Its **PP** codes write `0x096B2DC0`, the PP Catalog's copy, at all times. The port's write the save's PP,
  `0x08BCCB38` (next section).
- Its longer **accessories** code (672 counts) reaches the summonstones' bytes, so the port uses the shorter one (589).
- Its **All summons** code starts eight slots too late and runs past the last summon, so the port's starts at the
  first slot ([All summons](#all-summons-and-the-empty-slots)).
- Its **battle** codes go through a word that is 0 in some battles, so the port's go through the battle manager
  ([the fighters](#the-battle-cheats-and-the-fighters)).
- Each **battle** code is followed in the list by a write of 0 to `0x0880052C`, below the game, which the port leaves
  out.

Left out of the list: **CP Doesn't Matter** and **Remove Bloom** (they write the game's code; the port has its own
Bloom switch), the **widescreen** and **frame rate** codes (the port's aspect ratio and frame rate settings do those,
and the codes would fight them), the **Chaos** codes (a fan mod written as cheats; the owner chose to leave it out),
and the **AP after battle** codes (`0x09AEA568`): with 255 chosen, a Quick Battle still gave 1 AP. The port's AP row
does what they meant to (next section).

## AP after a battle

> 14 November 2026. `port/host/d012_ap_cheat.cpp`. Windows, clang build, USA disc.

The **AP** row (Money and points: Off, 100, 999) makes a battle that gives AP give that much. It has no CWCheat
code. The port sets the amount as the game works the rewards out, so the results screen, the abilities mastered and
the save all agree.

**How the game gives AP.** At a battle's end (battle `0x08B07324`), each side's rewards for the fight are worked out
(battle `0x08B0AD88`) into a record at `FIELD + side × 0x1BD8 + fight × 0x948 + 0xD8`, through battle `0x08ADDCF8`
(the record in `$a1`, the fighter in `$a2`, `$t2` = 1 when the battle gives AP). Battle `0x08ADDFE4` then fills it:

| Record | What |
| --- | --- |
| `+0x170` | AP: the reward row's AP and the bonuses, times the accessories' AP bonus (battle `0x08ADE14C`) |
| `+0x174` | gil |
| `+0x000`, `+0x12C` | the abilities that AP masters, and how many (main `0x088B9998`) |
| `+0x12E`, `+0x16E` | the abilities mastering them teaches, and how many (main `0x088B9BA0`) |

The same call of `0x08B07324` then lists the rewards (battle `0x08B0B71C`: "Received N AP.", the sum of the fights'
`+0x170`, then each ability mastered and learned). Later, battle `0x08B08C14` (from `0x08B07974`) adds each fight's
AP to the abilities the fighter has on (main `0x088B8038`), each only up to the AP that masters it
(main `0x088B7F58`).

**Why the list's codes did nothing.** `0x09AEA568` is side 0's first record's `+0x170`. The game writes it and lists
it in one call, so a cheat run every 77 ms cannot change what the results show.

**What the port does.** It overrides battle `0x08ADDCF8` (called from `0x08B0AD88`, in another generated unit, so
the override takes every call): the game's own runs first; then, for the player's fighter (the side at
`*0x089D6A10 + 0x54`, its fighter at `+0x38`), in a battle that gave AP, it raises `+0x170` to the amount chosen and
works out the two ability lists again as `0x08ADDFE4` does. A battle that gives no AP stays at none, and an amount
the game gives above the one chosen is kept. With **Use cheats** off, or `D012_NO_CHEATS=1`, the game's own runs
alone. The log says `[cheats] AP: 1 from the battle, 999 with the cheat (17 abilities mastered)`.

**Checked** (Quick Battle, Lightning Lv5 against a Lv5 CPU Lightning, Sky Fortress Bahamut; a test save in
`build/port/data-ap`, `D012_UNTHROTTLED=1`, won with Defeat with Select or with `D012_TEST_LOW_HP=2`):

| Run | Results screen |
| --- | --- |
| AP row off | "Received 1 AP." |
| AP 100 | "Received 100 AP." |
| AP 999 | "Received 999 AP.", then "Mastered Launch!", "Mastered Blitz!" and on to "Mastered 17 of the abilities!" and "Learned Flourish of Steel!" |
| AP 999 again, the same save | "Received 999 AP.", nothing left to master: the game had saved the first battle's abilities |
| AP 999 with Use cheats off | "Received 1 AP." |

Not checked: Story Mode, a party battle (several fights, one record each), the European disc, Linux and the Steam Deck.

## PP and the game's pool (issues 16 and 20)

> 12 November 2026. Windows, clang build, USA disc, a save with 420 PP.

The list's PP code writes `0x096B2DC0` every pass. That word is PP only while the PP Catalog is open: the catalog
copies the save's PP there when it opens (the word read 420, with `00000000 00000001 | 00000004 00000000` around it)
and back when it closes. The rest of the time the game's pool holds other things there, and the cheat overwrote them:

| When | What `0x096B2DC0` held (seen with the cheat off) |
| --- | --- |
| Loading the save, from the title | the save data being read (`0xDD399908`) |
| Loading Yuna vs Garland, Quick Battle | Yuna's `P_TEN110` motion `_651B` (file `+0xD37E4`), its 140th Animate command's FCurve reference, `0x000C001D` (`0000004B 00000000` before it, the next command's `000080B3 00000018` after it) |
| Later in that battle | other battle data, changing |

- **Issue 20** (v0.7.1, Yuna vs Garland, FIELD mode 7): main `0x089B72F4` (a GMO motion's setup) looks up each
  Animate's FCurve with `0x089B49F4(motion, 12, ref & 0xFFF)`. With 9,999,999 (`0x0098967F`) written there, the index
  was `0x67F` in a motion of 81 FCurves: the lookup returned 0 (its count left at `0x67F - 81 = 0x62E`, the crash
  log's `a2`) and the next read at address 0 stopped the game. On the disc no Animate in any of the 1,168 object GMOs
  names a missing FCurve. With the list's code in a cheat file, Yuna vs Garland in Quick Battle showed `0x000C001D`
  overwritten four frames after it loaded; that battle went on, as its setup had already read it.
- **Issue 16** (v0.6.0, Arcade): the Brave display's free ring, slot 12, lay at `0x096B2DC0` and held `0x4E20`,
  the 20,000 PP choice (`d012_brave_pool_guard.cpp`).

The save's PP is the word at `0x08BCCB38` (save root `+0x1DE00`, the last u32 of section 1 in
[roster-and-save.md](../research/roster-and-save.md) 1.6; 420 was found there and nowhere else in the save), in the
main RAM block that also holds Gil and KP. The port's PP cheat writes it: the catalog shows the amount the first time
it opens, and Yuna vs Garland loaded and fought with the word at `0x096B2DC0` left to the game.

Buying with it (13 November 2026): with 20,000 chosen, Preset - Lv 50 (900 PP) left 19,100 in the catalog. Leaving the
catalog, the game autosaved; opened again, it showed 20,000 with the preset bought. That save, loaded with no cheats
(`D012_NO_CHEATS=1`), opened the catalog at 20,000 PP with the preset bought.

## All summons and the empty slots

> 19 November 2026. `port/host/d012_summon_slot_guard.cpp`, `port/host/d012_summon_slot_rules.hpp`. Windows, clang
> build, USA disc.

The save's inventory (section 1 `+0xE788`, main `0x08BBE418`) keeps 64 summonstone slots of four bytes at `+0x1484`
(main `0x08BBF89C`):

| Byte | What |
| --- | --- |
| `+0` | the summon's id, -1 for an empty slot |
| `+1` | uses left |
| `+2` | battles left until it recharges |
| `+3` | flags: bit 1 is owned; a new game sets 1 |

A new game fills slots 0 to 60 with the 61 summons (their number is at inventory `+0x15FC`). Slots 61 to 63 stay empty.

**The list's code.** It wrote 61 slots from `0x08BBF8BD`, slot 8's uses, so slots 8 to 68. The first eight summons were
left out, and the empty slots 61 to 63 became owned with 10 uses. "Slots" 64 to 68 are the first ten entries of the next
table (inventory `+0x1584`, 14 entries of two bytes, data tag `setc`), whose flags got 10 and 3 in turn.

**The crash.** At the end of a battle, battle `0x08B13098` goes through all 64 slots. For each owned slot it looks the
summon up (main `0x088C6CA4`) and, for the summon used in the battle, takes a use. With no summon used it is passed -1,
which is also an empty slot's id, so each such battle took a use from the empty slots. At the last use it reads the
summon's recharge through the lookup's empty result and the game stops:
`Guest memory access outside PSP RAM/EDRAM at 0x00000009`, `guest_pc=0x08B1312C`, `s1=0x3D`. That comes before the
autosave, so the save then stops at the end of every battle. The cheat puts the uses back to 10 while it is on, so the
stop came 10 battles after turning it off. Which game mode the battle is in does not matter: `0x08B13098` has one
caller, battle `0x08B08C14`.

**What the port does.** Its code writes the 61 summons' slots and nothing else. For saves that already have the marked
slots, an override of `0x08B13098` empties any slot with id -1 that is marked as owned before the game's own run, and the
autosave keeps that. The log says `[summons] 3 empty summonstone slot(s) were marked as owned: emptied`. This also covers
the public list's code in a PPSSPP cheat file. The flags left in the `setc` table are not put back, as their old values
are gone. The summons the list's code gave stay owned, and the first eight are given only by the port's code.

**Checked** (Quick Battle, won with Defeat with Select):

| Save | Run | Result |
| --- | --- | --- |
| Clean, All summons never on (before the fix) | 15 battles | no stop |
| Clean, one battle with the list's code, then off (before the fix) | battles until it stops | stopped at the end of the 10th |
| That save, stopped | `D012_SUMMON_SLOT_GUARD=0` | stops at the end of the first battle |
| That save, stopped | the fix, AP 999 | 14 battles; the three slots emptied once |
| The save that run wrote | `D012_SUMMON_SLOT_GUARD=0` | 14 battles, no stop: the save itself is repaired |
| Clean, the port's code on | slots read at a battle's end | slots 0 to 60 owned with 10 uses; slots 61 to 63 and the `setc` table unchanged |
| The save that run wrote, the cheat off | `D012_SUMMON_SLOT_GUARD=0` | 15 battles, no stop |
| One battle with the list's code, not stopped yet | the fix, the cheat off | 16 battles finished; the three slots emptied once |
| The save that run wrote | `D012_SUMMON_SLOT_GUARD=0` | 15 battles finished, no stop |
| Stopped, the port's code on | the fix | 11 battles finished; the three slots emptied once |
| Clean, All summons never on | the fix | 15 battles finished; nothing emptied |

Not checked: a battle in Arcade or Story Mode with the fix, the European disc, Linux, the Steam Deck and Android.

## 10 Skills: two pages

> 23 November 2026. `port/host/d012_skill_pages.cpp`, `port/host/d012_skill_pages_rules.hpp`. Windows, clang build,
> USA disc, the story test save at chapter 12-1-2.

Story Mode's Skills (the world map's and the gateways' Skill bar, the Moogle shop) are held in the save as a count and
six ids at save root `+0x3F548` (main `0x08BEE280`; KP follows at `+0x9C`). The game holds five. A sixth, just
obtained, waits in the last slot while the game asks which one to throw away: main `0x08920244` is "more than five",
`0x089200F0` adds (refusing a seventh), `0x08920130` removes one (moving the rest up to the sixth slot),
`0x0892017C` removes several. Everything that shows them is built for six: the Skill bar (main `0x0893xxxx`, the
world map's in the battle overlay at `0x08B49B5C`, a gateway's in the menu overlay) has six icons at six fixed
places (main `0x089CF1DC`), a gateway's table of the Skills' state (field `+0x6C0`, menu `0x08B44F40`) has six rows
of `0x14` bytes, and the shop's throw-away window has five rows. The list has no room to grow in the save: a bit
array follows it at `+0x1C`, then KP.

**What the port does.** The game only ever sees the page on screen, in its own list. The other page is packed into
the list's sixth slot, which the game leaves alone while it holds five or fewer:

| Bits | Holds |
| --- | --- |
| 31 | 0: the word is never -1, an empty slot |
| 15 | 1: the low half, which the game compares with Skill ids as a signed halfword (main `0x088BB88C`), is negative and matches none |
| the other 30 | a mark (`0xB`, 4 bits), whether the page on screen is the second (1), the other page's five ids (5 bits each, 0 for none; the game's ids are 1 to 24) |

The save keeps its size and layout. Without the cheat the game reads it as the five Skills of the page that was on
screen: its loops stop at the count, and the one that reads all six slots (menu `0x08B44F40`) skips a word that
names no Skill.

Overrides of the list's functions (main `0x089200F0`, `0x08920130`, `0x0892017C`) keep the packed word in the sixth
slot as the game moves its Skills: a removal moves it down with them, so it goes back up; a Skill that has to wait
there takes it for the while, and it comes back once the game is down to five. A Skill obtained with the page on
screen full goes on the other page while that has fewer than five. With both full, the game asks which one to throw
away as it does without the cheat.

**Turning the page** (L2 or R2, the port's extra buttons) swaps the list's first five with the packed page, and in a
gateway the state table's five rows with the port's copies. The Skill bar's update (main `0x089315E4`) then sets
its icons again (main `0x08930E10`, the in-effect mark with `0x0893116C`), and while the player is choosing puts the
hand back on the same row and reports a move (bar `+0x294`) so the owner shows that Skill's help. The page slides
40 PSP pixels to the right and fades out over 8 frames, the pages swap, and the other comes in from the right and
fades in over 8 more: each icon's root layout object (`+0xC`) is moved and given an alpha (main `0x088D9B28`) after
the game places it each frame, then its parts and texts placed again (`0x088D3F00`, `0x088D44EC`).

**The page hint.** While a Skill list shows (the Skill bar, the shop's throw-away window), the port draws
"L2/R2  Page 1/2" over the game in the style of its other hints (the BGM selector's), dimmer while the other page
is empty. By the bar it sits left of the "Skill" plate, placed from the first row's icon as the game placed it that
frame (51 PSP pixels left, 21 up) and faded as it is, so it hides with the bar while the player walks; the plate's own
layout object sits at 0, 0. The buttons are named as the game's prompts are drawn (Controls > Button prompts): LT/RT
on an Xbox pad (L2/R2 on a Steam Deck), L2/R2 on a PlayStation pad and in the PSP style, ZL/ZR on a Nintendo pad, the
first keys bound to L2 and R2 in brackets on the keyboard, and the shoulders' names with Swap shoulders and triggers.
The style is the one the game's symbol sheets were last drawn in (`d012::current_button_prompts()`).

**Skills in effect.** A Skill that lasts until the next battle stays in the list, in effect (main `0x08920718`). As
a battle starts, menu `0x08B39F78` hands each such row to `0x08B3FD7C`, which sets the battle's terms; after it,
menu `0x08B3A008` removes them. Overrides of both do the same for the other page's rows, on a copy on the guest
stack, so a Skill in effect still works when its page is turned away. Out of a gateway the other page's rows are
reset, as nothing is in effect there.

**The shop.** Menu `0x08B0D27C` (one has to go) answers no while the other page has room; the throw-away window
(menu `0x08B0D578`) turns the page with L2 or R2, with the same slide and fade, and builds its five rows again; each
Skill's row counts both pages (menu `0x08B19C14`'s count, and again after a throw-away). The window's layout objects
(shop data `+0x404`, a word each) are placed by menu `0x08B1C430`'s table: the window (`0xE1`), its rows (`0xE2`
to `0xE6`, at the window's anchors 4 to 0), each row's icon (`0xE7` to `0xEB`, at the row's anchor 5, or 8 under the
hand) and the hand's highlight (`0xEC`); the names hang on the rows by `0x08B1C498`'s table. A turn moves the rows
and the highlight, places the icons again (main `0x088D3AE8`) and the names (`0x088D44EC`). The window cannot be
opened in the gil shop to try it: that shop's layout has none of these parts, and building them there stops the game. During a turn the row under the hand is placed again after the names (menu `0x08B0D8CC`), as the game
does each frame; without it that row's name sat at the plain anchor and was drawn large.

**Without the cheat** the overrides only keep the packed word in its slot: the page on screen is the game's five,
L2 and R2 do nothing, and the other page waits for the cheat, unless the game makes a sixth Skill wait in its slot
and the player keeps it.

Seen in the game (input scripts, window captures, `D012_TRACE_SKILLS`):

| Case | Seen |
| --- | --- |
| World map, five and three, L2 with the bar shown, R2 while choosing | page 1 slides out right and fades, page 2 comes in from the right; the hand stays on its row and the help follows |
| A gateway, the same | the bar and the board's preview of the Skill under the hand follow the page; Blind used from page 2 stays in effect |
| Save and Quit with page 2 on screen, then Continue, and a new run from the file | page 2 on screen, page 1 kept; R2 brings page 1 back |
| The same save with the cheat off | three Skills, R2 does nothing, the hand stops at the third |
| The hint on the world map and in a gateway, both pages | "LT/RT  Page 1/2", then 2/2, left of the plate; hidden while walking, paused and in Customization; L2/R2, ZL/ZR and "[1]/[3]" with `D012_BUTTON_PROMPTS` playstation, nintendo and keyboard |
| The owner, on 24 November, from a copy of their Steam Deck save: the world map, a Moogle shop and a gateway | five Skills bought went on page 2; the eleventh opened the throw-away window, L2/R2 turned its page and one was thrown away; the name under the hand was drawn large during the turn (fixed: the row under the hand is placed again, menu `0x08B0D8CC`, after the names; the owner rechecked it the same day: the name keeps its size); nothing else found |
| A gateway: Blind used from page 2, the page turned to page 1, then a battle | Blind given to the battle from the other page (`other page's Skill 1 in effect for this battle`) and removed from it after, as the game does on the page on screen |

Not seen yet: the shop's held counts with both pages, ten held and an eleventh from a chest, the European disc,
Linux, the Steam Deck and Android.

Input scripts: `axis lefttrigger 0` leaves a trigger half pulled, which the port reads as L2 held; release it with
`axis lefttrigger -1`.

## The battle cheats and the fighters

> 19 November 2026. `port/host/d012_battle_cheat_codes.hpp`. Windows, clang build, USA disc.

**What went wrong.** The public list's battle codes go through the word at `0x08B633B0`. In most battles it lies just
after the opponent's status block, so the list's offsets land on the right fields. In others it holds 0 for the whole
battle, and every battle cheat then does nothing: Defeat with Select left the opponent standing. Which battles depends
on how the game laid out that battle's memory, so a few battles in a row worked and the next did not.

**What the port does.** Its codes take the game's own way to the fighters, a pointer code with two steps:

| Step | What |
| --- | --- |
| The word at `0x089D6A10` | the battle manager |
| `+0x38`, `+0x3C` | the player's fighter, the opponent's; 0 between battles, when nothing is written |
| `+0x52C` | the fighter's status block |

| Status block | What | Cheats |
| --- | --- | --- |
| `+0x00` | level | Level 100 |
| `+0x08` | damage taken | Infinite HP, Defeat with Select |
| `+0x0C` | maximum HP | Max HP 999,999, Opponent Max HP 100 |
| `+0x14` | Bravery | Bravery 9,999, Opponent Bravery 0 |
| `+0x18` | EX gauge | Infinite EX, Opponent No EX |
| `+0x1C` | Assist gauge | Infinite Assist, Opponent No Assist |

Defeat with Select's joker line now skips three lines, as the code is one line longer. The list's codes in a PPSSPP
cheat file still go through the old word.

**Checked** (won by holding Select; `d012_tests` runs each code through the cheat engine as well):

| Code | Battles | Finished | The opponent fell to Select |
| --- | --- | --- | --- |
| The list's, from a cheat file | Quick Battle | 17 | 15; the 2 with the word at 0 did not |
| The port's | Quick Battle, Select only and with every battle cheat on | 104 | 104, 5 of them with the word at 0 |
| The port's | Arcade rounds | 7 | 7 |
| The port's | Quick Battle (Party) fights | 22 | 22 |

With every battle cheat on, each field held its value in each battle, the one with the word at 0 among them.

Not checked: Story Mode with these codes (a test switch that reads the same pointers worked there), the European disc,
Linux, the Steam Deck and Android.

## Switches

| Variable | Effect |
| --- | --- |
| `D012_NO_CHEATS=1` | No cheats at all; the page says so |
| `D012_SUMMON_SLOT_GUARD=0` | Empty summonstone slots marked as owned are left as they are ([All summons](#all-summons-and-the-empty-slots)) |
| `D012_CHEATS=gil=2,kp=1,...` | The port's own that are on, for one run (the rows are then greyed) |
| `D012_TRACE_CHEATS=1` | Each pass's cheats, with what they did |
| `D012_CHEAT_INTERVAL_MS=<ms>` | Another interval than 77 ms |
| `D012_CHEATS_ONLINE=1` | Cheats run during ad hoc play too, as before patch 0111 |
| `D012_TRACE_SKILLS=1` | The Skill list's changes and the page turns ([10 Skills](#10-skills-two-pages)); `=2` the Skill bar's states and L2/R2 too |
| `D012_TEST_SKILLS=ID,...`, `D012_TEST_SKILLS_OTHER=ID,...` | Those Skills on the page on screen and on the other page, when the world map or a gateway first lists them |
| `D012_SKILL_PAGE_FRAMES=<n>` | Frames for each half of a page turn (8) |

## Verified (6 November 2026, Windows, clang build, USA disc)

- `portablekit_cwcheat_tests`: 30 checks of the parser and every code type.
- A file cheat (20000 PP) gave the same result as in PPSSPP, at the same address.
- The port's own, as in the table: on the game's screens, through input scripts and window captures.
- The page: its headings and rows, the cheat file page and back, and a change made with the pad saved to
  `settings.ini`.

Not verified: a cheat that writes code, the European disc, Linux and the Steam Deck. The battle cheats were tried
in Quick Battle only, against Lightning.
