# Playing the port on the Steam Deck

> Written 21 October 2026. The Windows build of the native port, run on the Deck through Proton. A native Linux
> build comes later. Everything below was tested on the PC only (a package started from a folder of its own, and
> deploys to a local folder); nothing has run on a Deck yet.

The package is a folder, `Dissidia012`, that holds the program, its libraries, the game (`game\EBOOT.ELF`,
`game\disc.iso`) and its own `data\` folder. The program finds `game\` and `data\` next to itself, so nothing else
needs setting up. It contains the game, so it stays in the ignored `build/` on the PC and goes nowhere but your Deck.

On the Deck the folder is `/home/deck/Games/Dissidia012`. Its `data/` folder holds the Deck's own settings, shader
cache, mods and saves (`data/ms0`); updates never touch it, and no save or mod is ever copied from the PC.

## 1. One-time setup on the Deck

1. Hold the power button, choose **Switch to Desktop**.
2. Open **Konsole** (the application launcher, then System, or search for it).
3. If you have never set a password for the `deck` user, set one (you need it for `sudo`):

   ```sh
   passwd
   ```

4. Turn on the SSH server, now and at every start:

   ```sh
   sudo systemctl enable --now sshd
   ```

5. The PC reaches the Deck as `steamdeck.local` on most home networks. If that name is not found later, use the
   Deck's IP address: Settings > Internet > your network on the Deck, or in Konsole:

   ```sh
   ip -4 addr show wlan0
   ```

   Then add `--host deck@192.168.x.y` (your address) to every `deploy_deck.py` command below.

## 2. One-time setup on the PC

In PowerShell. Windows 10 and 11 come with OpenSSH (`ssh.exe`); nothing has to be installed. Below, `KEY` stands
for your private key file and `KEY.pub` for its public half.

1. If you have no key yet, make one (press Enter at each question; a passphrase is optional, but the deploy script
   cannot type one, so use `ssh-agent` if you set one):

   ```powershell
   ssh-keygen -t ed25519 -f KEY
   ```

2. Copy the public key to the Deck (Windows has no `ssh-copy-id`; this does the same). The first time it asks
   whether to trust the Deck's host key (answer `yes`) and for the `deck` password you set above. That is the only
   time a password is typed, and you type it, not the scripts:

   ```powershell
   type KEY.pub | ssh deck@steamdeck.local "mkdir -p ~/.ssh && chmod 700 ~/.ssh && cat >> ~/.ssh/authorized_keys && chmod 600 ~/.ssh/authorized_keys"
   ```

3. Check that the key works: this must return without asking for anything.

   ```powershell
   ssh -i KEY -o BatchMode=yes deck@steamdeck.local true
   ```

4. Tell the deploy script where your key is, and the Deck's address if it is not SteamOS's default
   `deck@steamdeck.local`, in `tools/local/deck.json`. That folder is ignored by git, so your key's path and your
   Deck's address never end up in the repository:

   ```json
   {"host": "deck@steamdeck.local", "identity": "<path to KEY>", "dest": "/home/deck/Games/Dissidia012"}
   ```

   Every field is optional; `--host`, `--identity` and `--dest` on the command line win over the file.

The deploy script only ever connects with the key (`BatchMode=yes`, `ConnectTimeout=10` on every call). If it
cannot, it stops and prints these steps.

## 3. The release build (once)

From the repository root. It has its own build directory and uses the recompiled build's corpus
(`build/port/recomp`, see [port/README.md](../../port/README.md), "Recompiled build"), so make that first. The
first build compiles the whole corpus and takes several minutes.

```powershell
.\tools\port\pk_cmake.cmd -S port -B tools/local/port/build-d012-release -G Ninja -DPORTABLEKIT_RELEASE=ON `
    "-DD012_PORTABLEKIT_DIR=$PWD/tools/local/port/PortableKit" `
    "-DD012_WORK_DIR=$PWD/build/port/recomp" -DPSPRECOMP_GENERATED_JOBS=4
.\tools\port\pk_cmake.cmd --build tools/local/port/build-d012-release -j4
```

`PORTABLEKIT_RELEASE=ON` leaves out PortableKit's fallback to the checkout's game folder, so the program depends on
nothing on this PC. A release build runs its recompiled code only: no interpreter fallback (as
`PSPRECOMP_NO_INTERPRETER=1` does in `play_d012.ps1`).

## 4. First deploy

The Deck must be on and awake (Desktop or Game Mode), on the same network.

```powershell
.\.venv\Scripts\python.exe tools/port/deploy_deck.py
```

It builds the release build (only what changed), refreshes the package in `build/port/deck-package/Dissidia012`
(`tools/port/package_deck.py`), and sends it to `/home/deck/Games/Dissidia012`, making the folder if needed.
The first time that is about 1.9 GiB, 1.6 GiB of it the disc image (the program and its libraries are about
290 MB); over Wi-Fi expect a few minutes. `--dry-run` shows what it would send
without sending anything.

It also puts the Deck defaults in `data/settings.ini` the first time (only if there is none): fullscreen, internal
resolution 2x, vsync on, 60 fps frame interpolation. Change any of them in the game's menu (L3 + R3 together); the
Deck keeps your choices from then on.

## 5. Adding it to Steam

On the Deck, in Desktop Mode:

1. Open **Steam** (the desktop Steam window).
2. **Games > Add a Non-Steam Game to My Library...**, then **Browse...**.
3. Go to `/home/deck/Games/Dissidia012`, set the file type filter to **All Files**, choose `D012Native.exe`, then
   **Add Selected Programs**.
4. In the library, right-click **D012Native** > **Properties...**:
   - **Shortcut**: rename it, e.g. *Dissidia 012 Duodecim*.
   - **Compatibility**: tick **Force the use of a specific Steam Play compatibility tool** and pick
     **Proton Experimental** (or the newest stable Proton).
5. Optional, the icon: in **Properties > Shortcut**, click the empty icon square and choose
   `/home/deck/Games/Dissidia012/dissidia012.ico` (the package carries a copy of `build/port/launcher/dissidia012.ico`).
   For library artwork, right-click the game > **Manage > Set custom artwork** and pick any image you like.
6. Return to Game Mode (the desktop shortcut). The game is in the library under **Non-Steam**.

The first start makes a fresh Proton prefix (a few seconds), then a new game in `data/ms0`.

## 6. Controller layout tips

Through Steam Input the Deck's controls reach the game as an Xbox controller, as on the PC:

| Deck | Game |
| --- | --- |
| A / B / X / Y | Cross / Circle / Square / Triangle (A confirms, as cross does on a US PSP) |
| L1 / R1 | L / R |
| L2 / R2 (triggers) | L2 / R2: switch the loading screen's D-pad between its menu and the BGM playlist |
| Left stick | the analog nub (full diagonals on) |
| Right stick | the battle camera; R3 (click) recentres it |
| L3 + R3 together | the port's menu (settings, quit) |
| View / Menu | Select / Start |

Suggested back-button layout (Steam button > Controller Settings > Edit Layout > Back Grip Buttons), so the thumbs
never leave the sticks:

- **L4**: Left Trigger (L2, BGM focus)
- **R4**: Right Trigger (R2, BGM focus)
- **R5**: Right Stick Click (R3, recentre the camera)
- **L5**: optional, Left Stick Click and Right Stick Click as one press (add a second command to the button), to open
  the menu with one finger

If the game ever asks you to type (a name), Steam + X opens the Deck's keyboard.

## 7. Updating

On the PC, after any change:

```powershell
.\.venv\Scripts\python.exe tools/port/deploy_deck.py
```

Then quit the game on the Deck if it is running and start it again. Only changed files are sent: usually the
program and an overlay library, a few tens of megabytes. The disc image and `EBOOT.ELF` are sent only if they change.
Files a deploy sent that the new package no longer has are removed; anything else in the folder, and the whole
`data/` folder, is left alone.

Options: `--host deck@192.168.x.y`, `--dest /other/folder`, `--identity KEY`, `--no-build` (send the package as it is),
`--dry-run`, `--rehash` (hash the Deck's files instead of trusting the manifest the last deploy left,
`.deploy-manifest.json`), and `--local-target D:\Dissidia012` (the same deploy into a local folder, e.g. a USB
drive).

## 8. Turning SSH off again

On the Deck, in Konsole:

```sh
sudo systemctl disable --now sshd
```

Turn it back on (section 1, step 4) before the next update. To revoke the PC's key, delete its line from
`~/.ssh/authorized_keys` on the Deck.

## 9. Known caveats

- **Untested on hardware.** Built and checked on Windows only; Proton, the Deck's GPU driver and its controller
  through Steam Input are not verified yet.
- **16:10 letterboxing.** The PSP picture is 16:9 (480x272). On the Deck's 1280x800 screen it is drawn 1280x720 with
  black bars of 40 lines above and below (`video.aspect=original`). Stretch or fill (the menu's Video page) remove
  them but distort or crop the picture.
- **Resolution.** 2x renders 960x544 and scales it to 1280x720. 3x (1440x816) is sharper but costs more GPU and
  battery; try it in the menu.
- **First-run stutter.** Vulkan pipelines are compiled the first time each effect is drawn, so the first battles hitch
  for a moment. The cache (`data/pipeline_cache.bin`) is kept, and later runs are smooth. Proton may also build its
  own shader cache on the first start.
- **Game text font.** The game's text is drawn with a Japanese-capable system font. On Windows that is MS Gothic;
  under Proton it may be missing, and with no such font the game's text stays blank (the port's own menu still
  works). Then open the menu (L3 + R3), Text, and choose a CJK font: SteamOS's Noto Sans CJK is usually at
  `Z:/usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc` from inside Proton. Unverified on a Deck.
- **Saves.** The Deck has its own saves in `data/ms0`, separate from the PC's. Updates never touch them; back up that
  folder yourself if you care about it.
