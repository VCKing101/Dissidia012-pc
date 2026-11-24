# Making a release

([Repository README](../../README.md) for what players see; [port/README.md](../../port/README.md) for the
builds.)

The steps for a release of the port, from a commit on `main` to the GitHub release. Every release ships these files,
all named with the version in `tools/port/release-version.txt`:

| File | For | Made by |
|---|---|---|
| `Dissidia012-PC-<version>-Windows-x64-Setup.exe` | Windows, the setup wizard (see below the table) | `tools/port/package_installer.py` |
| `Dissidia012-PC-<version>-Windows-x64.zip` | Windows through [RecompLauncher](https://recomplauncher.com/game/dissidia012-pc): the same files, `recomplauncher.json` at the top | `tools/port/package_installer.py` |
| `Dissidia012-Setup-<version>-x86_64.AppImage` | Linux and the Steam Deck | `tools/port/linux_setup.sh` (WSL) |
| `Dissidia012-BgmMaker-<version>-x86_64.AppImage` | the Battle BGM Maker on Linux and the Steam Deck, on its own | `tools/port/linux_bgm_maker.sh` (WSL) |
| `Dissidia012-<version>-Android-arm64.apk` | Android 10 and later, arm64 | `tools/port/android_build.sh --release` (WSL) |
| `SHA256SUMS` | all five, `sha256sum -c` format (two spaces) | collected by hand from the scripts' |
| `SHA256SUMS.sig` | the game's **Download and install**: the list's signature | `tools/port/release_signing.sh sign` |

The Windows setup is not called `Dissidia012-PC-Setup-<version>.exe` (as up to 0.8.0) on purpose. That is the name
0.8.0's and 0.8.1's **Download and install** look for, and a program installed that way, the game downloading the
setup and running it without a window, is flagged by Microsoft Defender's cloud as `Trojan:Win32/Bearfoos.A!ml`
within seconds: on 19 and 23 November 2026, three times out of three, while every setup run by hand passed. Once
flagged, that program is flagged on every PC. Under another name those copies offer only **Open the download page**
on Windows; Linux and Android still install from the menu. Keep it so until Windows installs from the menu in a way
Defender accepts, tested with throwaway builds, never with a release's program.

None of them carries anything of the game: each setup and the app make the game's code on the player's machine from
their own disc. The scripts check that (no game file by name or content, no program with the game's code linked in,
no path of the build machine; the APK not debuggable and without overlay libraries). Keep it that way.

Pushing, tagging and publishing are the owner's calls: each step under [Publishing](#publishing) waits for their yes.

## 1. Before building

1. **The version.** `tools/port/release-version.txt` holds `X.Y.Z` (no `-suffix` for a release), and
   `docs/port/platforms.md` names it ("One version for every download"). Commit the bump on `main`. The Android
   version code is made from it (`X*10000 + Y*100 + Z`), so Y and Z stay below 100 and a version is never reused.
2. **What changed.** `git log --oneline v<previous>..main` and the merged branches' notes, for the release notes.
3. **PortableKit.** The port builds with PortableKit's `dissidia` branch, which is the patch series in
   `port/portablekit-patches` (0001 to the newest). Check that the series still makes that tree: `git am` every
   patch, in file order, on upstream commit `33800c4` in a scratch worktree of the PortableKit clone
   (`git -c core.autocrlf=false am --keep-cr port/portablekit-patches/0*.patch`), and compare the tree with
   `dissidia`'s (`git rev-parse HEAD^{tree} dissidia^{tree}`); for 0.7.2, 0001 to 0106 gave `dissidia`'s tree exactly. A change made on `dissidia` without a patch, or a patch without the change, is a
   release that cannot be rebuilt from the repository. PortableKit itself is not pushed: its only remote is upstream.
4. **A release worktree,** detached at the commit to ship, so nothing uncommitted gets in:
   `git worktree add --detach .claude/worktrees/release-<version> <commit>`. It needs the main checkout's local tools
   as junctions (from PowerShell, `cmd /c mklink /J`; Git Bash variables in `cmd //c` made a stray junction once):
   `tools/local/llvm-mingw-20260922-ucrt-x86_64`, `tools/local/innosetup`, `tools/local/port-deps`,
   `tools/local/port/build-atracdenc` and `build/external`. Remove them with `cmd /c rmdir` before removing the
   worktree, or the removal deletes their contents.

## 2. Windows: the setup and the RecompLauncher zip

```powershell
python tools/port/package_installer.py --portablekit <main checkout>/tools/local/port/PortableKit
```

A release worktree has no PortableKit clone of its own: `--portablekit` names the main checkout's, on `dissidia`.
A build folder left half-configured by a failed first run must be deleted before trying again.

It builds the installer build (`tools/local/port/build-d012-installer`, llvm-mingw), stages the folder with the
trimmed toolchain, checks it, compiles the setup with Inno Setup and zips the same folder as the portable zip
(`recomplauncher.json` at its top), then writes `SHA256SUMS` for both into `build/port/installer/dist`. It keeps the
release's `D012Native.exe` and `BgmMaker.exe` in `build/port/installer/symbols/<version>` for crash reports (never
shipped). Run `d012_tests` from the build (`tools/port/pk_ctest.cmd <build>`) and keep the result for the notes.

Tests:
- A silent install from the setup into a test folder (`setup.exe /VERYSILENT /CURRENTUSER /ISO="<iso>"
  /DIR="<folder>"`): it compiles the game's code and the game starts. A silent install registers with Windows:
  uninstall it at the end.
- The setup again, over the previous release's install: "already compiled and up to date" when the game's C++ did not
  change, or only the changed parts compiled (`game-code-cache/`); saves, settings and mods kept.
- **Download and install**, from the previous release's install, before publishing: start it with `D012_UPDATE_URL=<a
  copy of GitHub's release JSON naming this version>` and `D012_UPDATE_ASSETS=<the folder with this setup, SHA256SUMS and
  SHA256SUMS.sig>`. The System page offers the update; **Close the game and install** closes the game, the setup runs
  without asking anything, and the new version starts with saves, settings and mods kept.
- The zip: unpacked, `D012Native.exe --prepare --game <iso>` prints `progress` lines and ends in a compiled game
  (it is a windowed program: from PowerShell, `Start-Process -Wait -RedirectStandardOutput`, since `&` does not
  wait for it and its output is lost);
  `--game <iso>` starts it. The first time a release has the zip, tell the RecompLauncher maintainer (Xarishark, on
  PR 21) where it is; keep its name exactly `Dissidia012-PC-<version>-Windows-x64.zip`, the pattern their catalogue
  looks for.

## 3. Linux: the AppImage

```bash
wsl -d Ubuntu-24.04 -- bash tools/port/linux_setup.sh
```

The installer build runs in the Steam Runtime SDK container with GCC (`~/d012-linux/build-installer`; a build folder
configured with the clang trial must not be reused), then the AppDir, the checks and appimagetool. The result goes
to `build/port/installer-linux/dist`, the symbols to `build/port/installer-linux/symbols/<version>`. PortableKit comes
from the main checkout's clone; its text files are made LF on the way in, as PortableKit commits them. That matters
beyond the scripts: `sdk/` is copied from it and is in the game code's key, and with Windows line ends a 0.7.2 build
recompiled every Linux player's game for no change at all.

It refuses to make a setup whose program lacks the update signing key's public half (below).

The Battle BGM Maker, an AppImage of its own (not in the setup):

```bash
wsl -d Ubuntu-24.04 -- bash tools/port/linux_bgm_maker.sh
```

BgmMaker as the installer build makes it (`~/d012-linux/build-bgm-maker`), atracdenc built from the commit
`tools/port/build_atracdenc.cmd` names with libsndfile, and their libraries, licences and atracdenc's source; the
same checks as the setup's. The result goes to `build/port/bgm-maker-linux/dist`. The game's **Download and
install** looks for the setup's name only, so this file is never taken for it.

Tests:
- Run the new AppImage over the previous release's install in WSL (`~/d012-linux/setup-test`): the version is the
  new one, the compile skipped or partial as on Windows, the game starts.
- **Download and install**, from the previous release's install, before publishing: start it with
  `D012_UPDATE_URL=<a copy of GitHub's release JSON naming this version>` and `D012_UPDATE_ASSETS=<the folder with
  this AppImage, SHA256SUMS and SHA256SUMS.sig>`. The System page offers the update; **Install and restart** puts the
  new files in place, the game closes and starts again (Steam starts it when Steam started it), compiling first only
  when the game's C++ changed, with saves, settings and mods kept. On a Steam Deck, test it in Gaming Mode, from a
  Steam shortcut: a restart started inside the game left the screen black there.

**Cross-play check:** the installed Windows copy hosts and the installed Linux copy joins (relay), a whole battle in
step, both back to the room. Run the Windows copy from a folder with the owner's firewall Allow rule: a new program
listening on the network raises a Windows Firewall prompt, so never start it, or the ad hoc tests, unattended
(`tools/port/pk_ctest.cmd` leaves those tests out).

## 4. Android: the APK

The app carries clang, lld and the recompiler, and compiles the game's code on the phone
([platforms.md](platforms.md#android)). In WSL, with the NDK, SDK and JDK in `~/d012-android`
(`tools/port/android_build.sh` lists them):

1. **The compiler,** only when its pins change (LLVM, AOSP, API level), or on a new machine. Both are made from
   source and give the same bytes from the same sources:

   ```bash
   wsl -d Ubuntu-24.04 -- bash tools/port/android_runtime.sh
   wsl -d Ubuntu-24.04 -- bash tools/port/android_compiler.sh
   ```

   A new compiler changes the game code's key, so every player's next start compiles all of it again (about 7 minutes
   on a fast phone). Say so in the notes when it happens.
2. **The APK,** with the release key (below):

   ```bash
   export D012_RELEASE_KEYSTORE_PASS=...   # typed by the owner, never written in a file
   wsl -d Ubuntu-24.04 --cd <release worktree> -- bash tools/port/android_build.sh --release
   ```

   It writes `~/d012-android/dist/Dissidia012-<version>-Android-arm64.apk`, not debuggable, with the version code
   from the version, and keeps the unstripped `libmain.so` in `dist/symbols-<version>/` (never shipped). It refuses
   a debuggable APK or one with overlay libraries.
3. **Tests on a phone,** over adb (`tools/local/android/platform-tools/adb.exe`):
   - `adb install -r` over the previous release: it installs (same key, higher version code), the saves stay, the
     first start compiles only what changed or nothing.
   - A clean install (`adb uninstall io.github.vcking101.dissidia012`, which deletes the app's data: only on a test
     phone or with the owner's yes): choose the ISO, the setup copies it, compiles the game's code on its own screens,
     shows "All set", and the game starts.
   - The log: `adb shell run-as` does not work on a release APK (not debuggable); test with a `--player` build of the
     same commit when the log is needed.
   - **Download and install**, before publishing, with `--player` builds under a test app id (so the phone's own install
     is never touched): build the previous version and this one with `D012_APP_ID_PLAYER=<test id>` and
     `D012_UPDATE_PUBLIC_KEY=<a throwaway key's public half>`, install the first, and give it the ISO. Then write
     `files/test-environment.txt` in its storage (`adb shell run-as <test id>`; lines `D012_UPDATE_URL=<a copy of
     GitHub's release JSON naming this version>` and `D012_UPDATE_ASSETS=<a folder holding this APK, SHA256SUMS and
     SHA256SUMS.sig, signed with the throwaway key>`, both in the app's storage). The System page offers the update;
     **Install the update** shows Android's question, and after it the app is the new version with its data kept. Delete
     the test app at the end.

### The release key

Android installs an update only when it is signed with the same key as the installed app. Lose the key and no later
release can update the app: every player would have to uninstall, which deletes their saves. So:

- It is made once, by the owner, who chooses its password:

  ```bash
  keytool -genkeypair -keystore ~/d012-android/keys/release.keystore -alias dissidia012 \
      -keyalg RSA -keysize 4096 -validity 36500 -dname "CN=Dissidia 012 port"
  ```

- It never goes into the repository or a release, and is backed up off the machine (with the password, kept apart).
- The developer key (`keys/dev.keystore`) signs only `android_build.sh` and `--player` builds for the owner's own
  phone; those cannot update a release install, nor a release one of them.

### The update signing key

The game's **Download and install** (copies the Windows or Linux setup installed, and the Android app) installs only a
setup, AppImage or APK listed in a `SHA256SUMS` signed with this key. Its public half is
`tools/port/update-signing-key.txt`, compiled into the program; `package_installer.py`, `linux_setup.sh` and
`android_build.sh --release` refuse to build a release without it.

- It is made once, by the owner, who chooses its password:

  ```bash
  wsl -d Ubuntu-24.04 -- bash tools/port/release_signing.sh new
  ```

  Commit the public half it writes. The private key is `~/d012-android/keys/update-signing.pem` in WSL.
- It never goes into the repository or a release, and is backed up off the machine (with the password, kept apart).
- Lost or replaced, installed copies refuse the next release's signature: their players update once from the release's
  page, and the version they install then trusts the new key.

## 5. The release files and notes

Copy the five files into `build/port/release/v<version>-files/` and write `SHA256SUMS` there for all five
(`sha256sum Dissidia012-* > SHA256SUMS`; check with `sha256sum -c SHA256SUMS`). Then sign the list, with the owner
typing the key's password in a terminal:

```bash
wsl -d Ubuntu-24.04 -- bash tools/port/release_signing.sh sign build/port/release/v<version>-files
```

It writes `SHA256SUMS.sig` and checks it with the public key in the repository. Any change to a file after this means
writing `SHA256SUMS` and signing it again. A release without a good signature can still be installed from its page, but
**Download and install** does not offer it.

The notes go in `build/port/release/v<version>-notes.md`. They are a changelog, kept short:

- **What's new** and **Fixes:** one bullet each, with the issue or pull request.
- **Install:** one line per system naming its file and linking to the README's quick start
  ([Windows](../../README.md#quick-start-windows), [Linux and Steam Deck](../../README.md#quick-start-linux-and-steam-deck),
  [Android](../../README.md#quick-start-android)), which hold the installation steps; RecompLauncher's page is
  <https://recomplauncher.com/game/dissidia012-pc>. Steps go in the notes only for something new (Android's in 0.7.2).
  Say whether updating recompiles the game and whether online play needs everyone on the new version.
- **Not tested yet:** what is new but not tried, as a call for players' reports, not as a problem.
- **Known issues:** only real problems that are new or changed, then a link to the previous release for the rest.
- The Android key's certificate SHA-256 (from `apksigner verify --print-certs`), so a player can tell our APK from
  another.

Install steps change in the README, not in the notes. Player-facing text: short plain statements of what it does,
"online" as one word.

## Publishing

Each with the owner's yes, in this order:

1. Push `main` (`gh auth status` shows VCKing101 active; never the work account).
2. An annotated tag `v<version>` on the release commit (tagger VCKing101 with the noreply address), pushed.
3. The GitHub release `v<version>`, full (latest) unless the owner says pre-release, with the five files, `SHA256SUMS`,
   `SHA256SUMS.sig` and the notes. Check that GitHub's digests match `SHA256SUMS`.
4. Tell those waiting on it (RecompLauncher, an issue's reporter); a Discord announcement, which the owner posts.

## Afterwards

Uninstall the Windows test install, remove the junctions (`cmd /c rmdir`) and then the release worktree. Keep the
release files, the notes and the symbols of every release.
