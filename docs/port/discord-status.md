# Discord status

> 9 November 2026. While the Discord app runs on the same PC, the player's Discord profile shows what they are doing
> in the game. Code: `port/host/d012_discord.cpp` (the connection, the game's state, the System page's rows) and
> `port/host/d012_discord_rules.cpp` (the text and Discord's message format, tested by `d012_tests`).

## 1. What it shows

| In the game | First line | Second line |
| --- | --- | --- |
| Menus, title screen, anything not on the battle overlay | In the menus | |
| Story Mode's world map and gateway boards (FIELD mode 0) | Story Mode | |
| A Quick Battle (FIELD mode 4) | Quick Battle | Cloud vs Sephiroth |
| A Communication Mode battle (mode 5), which the port plays online | Online battle | Cloud vs Lightning |
| A replay (mode 9) | Watching a replay | the replay's fighters |
| Any other mode on the battle overlay | In a battle | the two fighters |

Under both lines: the time since the game started, and a **Get the PC port** button to the project's latest
release. The name above them ("Playing ...") and the picture are the Discord application's, set on Discord's
developer site (2).

**Where the values come from** (read every 15th pad read, on the game's thread, and handed to the connection's
thread):

- The battle overlay is resident when battle `0x08B2F2E4` starts `0x27BDFFF0 0x2CA60006` (as `d012_versus.cpp`
  tells it). The world map runs on the battle overlay too (`analog-movement.md`).
- The FIELD object is `*(0x089D6A10)`; its mode is `+0x4C`. Main `0x0888B544` picks the default battle music by
  it through a 15-entry jump table at `0x089CA500`: modes 0 to 14. Known: 0 Story Mode's world map (seen in a run,
  5), 4 Quick Battle (`menu:0x08A9D72C` sets it; seen), 5 Communication Mode, 9 a replay (its music from the
  replay, `+0x7C8 -> +0x22E4`). Every new mode goes to the log as `[discord] FIELD mode N, characters A and B`, so
  more can be named from play.
- Each side's character ID is battle setup `0x09AFB8A0 + side * 0x1060 + 0x1058` (`roster-and-save.md` 1.7);
  the names are the table in `roster-and-save.md` 1.1. On the world map the setup still holds the last battle's
  sides (0 and 0 before any battle), so mode 0 names no fighter.

## 2. The Discord application

The status needs an application on [Discord's developer site](https://discord.com/developers/applications).
Its **name** is what friends read after "Playing", and its **icon** is the status's picture. Use the project's own
art, not the game's. Both can change on the site at any time, with no new build.

The project's application is `1569315126220169347` (made by the owner on 9 November 2026), in
`discord_rules::kApplicationId`; the ID is public. Without one the System page's switch says "Not in this build"
and nothing runs.

## 3. How it talks to Discord

The Discord app listens on the PC for programs that set a status:

- Windows: the named pipe `\\?\pipe\discord-ipc-0`, up to `-9`.
- Linux and the Steam Deck: the socket `discord-ipc-0` to `-9` in `$XDG_RUNTIME_DIR`, `$TMPDIR`, `$TMP`, `$TEMP`
  or `/tmp`, or under `app/com.discordapp.Discord`, `snap.discord` or `.flatpak/dev.vencord.Vesktop/xdg-run` in
  those folders (the Flatpak, Snap and Vesktop apps).

Each message is a little-endian opcode and length, then JSON
([Discord's RPC page](https://docs.discord.com/developers/topics/rpc)). The port sends the handshake (opcode 0,
`{"v": 1, "client_id": ...}`), waits up to 5 seconds for the READY frame, then sends `SET_ACTIVITY` (opcode 1)
with its process ID each time the text changes, at most once every 5 seconds. It answers PING with PONG.
Closing the connection clears the status, so leaving the game or turning the switch off needs nothing else.
No sign-in and no OAuth: `SET_ACTIVITY` over the local connection needs neither, and nothing leaves the PC from the
port; Discord's app shows the status to the player's friends.

Without Discord it tries again every 15 seconds and says so once in the log. A refused handshake (a wrong
application ID: CLOSE with code 4000) waits 60 seconds and shows Discord's reason on the System page.

## 4. Settings and switches

| Setting | Meaning |
| --- | --- |
| `discord.show_activity` in settings.ini | The System page's **Show on Discord**; on unless set to 0 |
| `discord.application_id` in settings.ini | Another application than the built-in one |
| `D012_DISCORD=0` or `1` | Off or on over the setting; 1 also in scripted runs |
| `D012_DISCORD_APP_ID=<id>` | The application, over both |
| `D012_DISCORD_IPC=<path>` | Connect there only: a stand-in Discord for tests |

A scripted test run (`D012_INPUT_SCRIPT`) shows no status unless `D012_DISCORD=1`, so test runs never reach the
player's own Discord.

## 5. Tests

- `d012_tests` (`tools/port/pk_ctest.cmd <build dir>`): the names, the text for each scene, the message format
  (whole, half and broken messages), the handshake and `SET_ACTIVITY` JSON, Discord's answers.
- A stand-in Discord, `tools/port/discord_standin.py`: a named pipe that answers READY (or refuses with code 4000),
  sends a PING on request and logs every message. With `D012_DISCORD=1`, `D012_DISCORD_APP_ID=1234567890` and
  `D012_DISCORD_IPC=\\.\pipe\d012-fake-discord`. Windows, clang build, 9 November 2026, all passed:
  - `qb_chaos_shrine` (graphics tour): handshake, "In the menus", then `FIELD mode 4, characters 31 and 31` and
    "Quick Battle / Lightning vs Lightning", with the game's process ID; the connection closed as the game quit.
  - `story_gateway`: `FIELD mode 0, characters 0 and 0` on the world map, through the gateway board.
  - Discord opened 12 seconds after the game: "not open" logged once, connected on the next try.
  - PING answered with PONG and the same body.
  - A refused handshake: "Discord refused the connection: Invalid Client ID", no retry for 60 seconds.
  - A scripted run without `D012_DISCORD=1`: no connection.
  - The System page: the Discord section with **Show on Discord** and "Status: Showing".
- The real Discord app on the owner's PC, the project's application, `qb_chaos_shrine` with `D012_DISCORD=1`:
  connected at `\\?\pipe\discord-ipc-0`, "In the menus" then "Quick Battle / Lightning vs Lightning", Discord's
  answers carried no error (`[discord] Discord took the status`), the button included. The owner saw it on their
  profile: "Playing", the application's name, "Quick Battle", "Lightning vs Lightning" and the time.
- Linux: both files compile with GCC (`-Wall -Wextra`, no warnings); not run.

## 6. Not done

- The application has no icon yet: the status shows Discord's placeholder.
- Turning **Show on Discord** off and on by hand (the same code as the update check's switch).
- Names for the other FIELD modes (Story Mode's battles, Arcade, Labyrinth, Duel Colosseum), from the log lines of
  a play through each.
- The Steam Deck: Discord usually runs only in Desktop Mode, and the Flatpak app's socket path is untried.
- "Ask to Join" from Discord into an online room. It needs Discord's join events, which may need Discord's
  approval for the application.
