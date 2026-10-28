Dissidia 012 Duodecim Final Fantasy - native PC port
https://github.com/VCKing101/Dissidia012-pc

This installs a native PC port of the PSP game. It is not an emulator: the game's code is recompiled for your
PC. No game data comes with it. You need your own copy of the game: the USA disc ULUS10566, as an .iso file.

What Setup does
  1. Copies the port into the folder you choose (no administrator rights needed).
  2. Reads your .iso, checks it is the right disc, and decrypts the game's program from it.
  3. Compiles the game's code for your PC with the compiler that comes with the port. This takes a few
     minutes (about 4 on a recent 8-core PC, longer on a laptop) and uses about 2 GB of disk while it works.
  Nothing is downloaded, and nothing made from your disc leaves your PC.

Playing
  Start "Dissidia 012 (PC port)" from the Start menu or the desktop. Esc on the keyboard, or L3 + R3 on a
  controller, opens the port's menu: video, controls and audio.

Battle music mods
  Mods add music to the battle music selector; they never replace the game's own. Put a mod's folder in
  data\mods (the Start menu has a "Mods folder" shortcut). To make your own, tick "Battle BGM Maker" in Setup:
  it opens in your browser, lets you add tracks (MP3, OGG, FLAC, WAV, BRSTM) and set where each one loops, and
  writes the mod for you. Close its window to stop it.

Your files, in the install folder
  data\ms0\PSP\SAVEDATA   your saves (back this folder up)
  data\settings.ini       your settings
  data\mods\              battle music mods
  data\logs\              the last run's log, for bug reports

Updating
  Run the new Setup and install into the same folder. Your saves, settings and mods stay. The game's code is
  compiled again only when the new version needs it.

Uninstalling
  From Windows' Settings > Apps, or the Start menu. You are asked whether to keep your saves.

Help and bug reports: https://github.com/VCKing101/Dissidia012-pc/issues

Not affiliated with or endorsed by Square Enix or Sony. Use your own legally obtained copy of the game.
The licences of the port and of the software that comes with it are in the licenses folder.
