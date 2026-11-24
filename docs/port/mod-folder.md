# Mods: the folder and its manifest

> Written 19 October 2026 for the native port (`port/`). A mod **adds** to the game: no file of the game's is ever
> changed or overwritten. The one exception, added on 1 November 2026, is a mod's **package** (section 9): files that
> the game reads in place of its own package entries, such as a retail character changed with dissidia-fan's
> character tools; turning the mod off brings the game's own back. The first thing a mod can add is battle music: new series (a
> source game, such as another Final Fantasy) and their tracks, which come after the game's own series in the
> loading screen's BGM choice. The game-side choice (the 2D selector: L2/R2, then left/right for the series and
> up/down for the track) is in [bgm-playlist-design.md](bgm-playlist-design.md), section 10; this page is the
> format a modder follows and how the port plays a mod track. The format did not change for the selector.

Conventions as in the other port notes: "V:" verified (by a run, a trace or a capture), "I:" inference.

---

## 1. Where mods go

```text
<data dir>/
  mods/
    enabled.json            optional: which mods are off
    example-tones/          one folder per mod, any name
      mod.json              the manifest
      music/intro_loop.wav  the mod's own files, anywhere inside its folder
```

The data directory is `D012_DATA_DIR` when it is set (the run scripts set it, for example
`build/port/data-mods-test`), else the port's own folder: `%APPDATA%\Decomp-Dissidia\D012Native` on Windows,
`~/.local/share/Decomp-Dissidia/D012Native` on Linux, `~/Library/Application Support/Decomp-Dissidia/D012Native`
on macOS. `D012_MODS_DIR` names another mods folder.

The port reads the folder once, at start-up. Every problem is a line in the log starting `[mods] error:` or
`[mods] warning:`, with the file and line; the part with the problem is left out and the game starts as usual. A
broken mod never stops the game.

### Turning mods on and off

Every mod is on unless `mods/enabled.json` turns it off:

```json
{ "mods": { "example-tones": true, "some-other-mod": false } }
```

The names are mod ids (section 2), not folder names. A mod the file does not name is on. A file that is not valid
JSON is reported and ignored (every mod on). `D012_NO_MODS=1` turns all mods off for a run.

## 2. mod.json

UTF-8 JSON. The example in [`port/examples/mods/example-tones/mod.json`](../../port/examples/mods/example-tones/mod.json):

```json
{
  "format": 1,
  "id": "example-tones",
  "name": "Example test tones",
  "version": "1.0.0",
  "author": "Dissidia 012 native port",
  "description": "Generated sine tones that show each part of the format.",
  "music": {
    "series": [
      {
        "key": "tones",
        "name": "TEST TONES",
        "order": 0,
        "tracks": [
          { "key": "intro-loop", "title": "Intro, then a loop", "file": "music/intro_loop.wav",
            "loop": { "start": 88200, "end": 264600 } },
          { "key": "chord", "title": "Chord, whole file looped", "file": "music/chord_48k_mono.wav",
            "loop": "whole" },
          { "key": "seconds", "title": "Loop given in seconds", "file": "music/steps.wav",
            "loop": { "start_seconds": 1.0, "end_seconds": 3.0 } }
        ]
      },
      {
        "key": "sweeps", "name": "TEST SWEEPS", "order": 10,
        "tracks": [ { "key": "sweep-once", "title": "Sweep, played once", "file": "music/sweep.wav", "loop": "none" } ]
      }
    ]
  }
}
```

### The mod

| Member | Required | What it is |
| --- | --- | --- |
| `format` | no (1) | The manifest format. This build reads 1; another number leaves the mod out |
| `id` | yes | The mod's id: lower-case letters, digits, `.`, `_`, `-`, starting with a letter or digit, at most 64 characters. Ids are what everything else is named by; keep it the same in every version of the mod |
| `name` | yes | Shown to the player, 1 to 64 bytes |
| `version` | yes | Any text, 1 to 32 bytes, such as `1.0.0` |
| `author`, `description` | no | Text (64 and 1,024 bytes at most) |
| `music` | no | What the mod adds to the music, below. A mod with neither it nor `package` adds nothing (a warning) |
| `package` | no | A folder of the mod (a path relative to it, inside it) of files the game reads in place of its package entries: section 9. A bad one is an error and is left out; the rest of the mod still loads |

Members the port does not know are reported as warnings and ignored, so a newer manifest still loads.

### A series (`music.series[]`)

| Member | Required | What it is |
| --- | --- | --- |
| `key` | yes | Unique in the mod, same rules as `id` |
| `name` | yes | The series' name as the selector shows it, 1 to 48 bytes. Plain ASCII is safest: the game's font may not have every character |
| `short` | no | 1 to 16 bytes, for the Battle BGM Maker's "Make a PSP / PPSSPP ISO" only: the stock selector has one flat list, so each track is named `<short> - <title>` there ("FFXVI - Titan Lost"). Without it the tool makes one from `name` (`FINAL FANTASY XVI` becomes `FFXVI`). The port ignores it; a bad one is a warning |
| `order` | no (0) | Where the series sorts. Mod series always come after the game's own, sorted by `order`, then by id |
| `tracks` | yes | The series' tracks, in the order they are offered |

### A track (`music.series[].tracks[]`)

| Member | Required | What it is |
| --- | --- | --- |
| `key` | yes | Unique in the whole mod (not only in its series), same rules as `id` |
| `title` | yes | Shown to the player, 1 to 64 bytes |
| `file` | yes | The audio file, a path relative to the mod's folder with `/` between folders. It must stay inside the folder (no `..`, no absolute path) and end in `.mp3`, `.ogg`, `.oga`, `.opus`, `.flac`, `.wav`, `.brstm`, `.bcstm` or `.bfstm` |
| `loop` | no (`"auto"`) | How the track loops, below |

`loop` is one of:

- `"auto"`: the file's own loop tags if it has them (`LOOPSTART` with `LOOPLENGTH` or `LOOPEND`, in samples, as
  Ogg and FLAC tools write them; `LOOPEND` is taken as the first sample after the loop), else `"whole"`. For a
  BRSTM, BCSTM or BFSTM, the loop in its header (section 3): from its loop start to the end of the stream, or
  `"whole"` when its loop flag is off.
- `"whole"`: the whole file, over and over.
- `"none"`: plays once. What the game does when a battle's music ends is the game's own behaviour (not tested).
- `{"start": S, "end": E}`: from sample `S` to just before sample `E`, counted in samples of the file at its own
  rate (a 48 kHz file counts 48,000 a second). `end` may be left out: the loop runs to the end of the file. The
  music before `S` plays once, as an intro.
- `{"start_seconds": A, "end_seconds": B}`: the same in seconds (decimals allowed), `end_seconds` optional.

A loop outside the file falls back to looping the whole file, with a log line.

### Ids and numbers

Every series and track has an id made of the mod's id and its key: `example-tones:tones`, `example-tones:intro-loop`.
Nothing about it depends on which other mods are present or the order they load in, so a choice the game saves
stays meaningful when mods are added, removed or turned off. For places that can only hold a number, each id also
has a 32-bit number: the FNV-1a hash of the id text (`example-tones:intro-loop` is `0x7D9EC9A2`). If two tracks'
numbers ever clash, the one whose id sorts later is left out with an error naming both; changing either key fixes it.

## 3. Audio files

MP3, Ogg Vorbis, Opus (in Ogg), FLAC and WAV (16, 24, 32-bit integer or 32-bit float PCM), mono or stereo, any
sample rate. The port decodes a track to 44,100 Hz 16-bit stereo, what the game's own music decodes to, when the
game starts it, and keeps it in memory while it plays: about 10 MB a minute. The last three tracks played stay
decoded. Tracks may be up to 30 minutes long. A file that cannot be read is found at start-up (each track's file is
opened and checked) and left out with the reason. ATRAC3plus encoding is never needed.

Decoding a track takes milliseconds for WAV and FLAC and tens to hundreds of milliseconds for a few minutes of MP3
or Ogg (I: not measured on a long file); it happens when the selector shows the track, before the battle loads.

Master a track as you would any music file. The port plays every mod track at the volume the game gives most of
its own battle music (the median of its rows, 56 percent of full scale), so a track as loud as a commercial
recording sits about as loud as the game's. A title is shown on the game's band in its own font: plain ASCII
shows as written, any other character as `?` (the series name, drawn by the port, may use any character).

Only add music you have the right to use. Never add the game's own music to a mod; it is already in the game.

### Nintendo stream files: BRSTM, BCSTM, BFSTM

Game-music libraries share many tracks as Nintendo stream files with the loop built in: BRSTM (Wii), BCSTM (3DS),
BFSTM (Wii U, big-endian, and Switch, little-endian). A mod can use them as they are: put the file in the mod and
leave `loop` out (or `"auto"`), and the track loops where its header says.

- **The loop.** The port reads the header itself (`port/host/d012_nintendo_stream.cpp`): the stream info's loop
  flag, loop start and sample count, through the header's own references (BRSTM: HEAD's first reference; BCSTM and
  BFSTM: the INFO block's stream info reference). Both are in samples of the file at its own rate; the loop runs
  from the loop start to the end of the stream, which is where these formats end their loop (the sample count is
  the loop end). A file whose loop flag is off loops whole with `"auto"`; `"none"` plays it once. A loop flag with
  a start at or past the end is a warning, and the file loops whole. A BFSTM's later "original loop" fields are not
  used (players use the loop start). FFmpeg's own `loop_start` metadata for these files is in microseconds, so it is
  not used either.
- **An explicit loop wins.** `{"start": ...}` or any other `loop` in the manifest is used as written, in the file's
  samples, like any track.
- **Decoding.** FFmpeg's `brstm` and `bfstm` demuxers (the second reads both BFSTM and BCSTM, in both byte orders)
  and its `adpcm_thp`/`adpcm_thp_le` decoders. Only DSP-ADPCM streams are taken, which is what nearly every file
  holds. FFmpeg cannot decode IMA-ADPCM (some BCSTMs), and it reads PCM8/PCM16 streams wrongly (7.1: the last block
  is cut to the length DSP-ADPCM would have, and a BFSTM's PCM starts 24 bytes early), so those are refused with an
  error that says to use a DSP-ADPCM BRSTM, or a WAV or FLAC with the loop in the manifest.
- **Exact samples.** DSP-ADPCM has no encoder delay (8-byte frames of 14 samples, each block's decoder history in
  the ADPC or SEEK table), and FFmpeg decodes exactly the header's sample count, so sample N of the header is
  sample N of the decoded track (V: section 7).
- **Channels.** A file with more than two channels (several tracks of a layered piece) is mixed down to stereo,
  all its channels together. Files at 32,000 or 48,000 Hz are resampled to 44,100 Hz like any other.
- **Not supported:** BWAV and NUS3AUDIO. FFmpeg has no demuxer for either, and the port would need its own decoder
  for them; a manifest naming one is an error that says to use the track's BRSTM version.

## 4. The example mod

[`port/examples/mods/example-tones/`](../../port/examples/mods/example-tones/) uses only generated sine tones.
Copy the folder into `<data dir>/mods/` and make its tones there:

```powershell
Copy-Item -Recurse port/examples/mods/example-tones build/port/data-mods-test/mods/
.\.venv\Scripts\python.exe build/port/data-mods-test/mods/example-tones/make_tones.py
```

`make_tones.py` needs only Python's standard library. What each tone is, and so what a correct playback sounds
like, is in its docstring: for `intro-loop`, 440 Hz for two seconds, then 660 Hz and 880 Hz a second each for ever.

## 5. How a mod track is played

The game streams a battle's music from `disc0:/PSP_GAME/USRDIR/DATA/BGM/<name>.AT3` (V: `D012_TRACE_IO` shows
`U_13BATTLE.AT3`, `B01_01.AT3`, `U_01CHOS.AT3` on three runs of the same route): it reads the first 96 KiB, hands it
to `sceAtracSetDataAndGetID`, and then reads more of the file wherever `sceAtracGetStreamDataInfo` says, with
`sceIoLseek`/`sceIoReadAsync`, and adds it with `sceAtracAddStreamData`.

A mod track is a file the port serves at `disc0:/PSP_GAME/USRDIR/DATA/BGM/MD<number>.AT3`, with the track's number
in eight upper-case hexadecimal digits (`d012::mods::bgm_path()`); no such file is on the disc. What the game reads
there is a stand-in ATRAC3plus WAVE file: the track's length and loop in its header, a private chunk naming the
track, and silent frames, 560 bytes each like the game's 96 kbps music. PortableKit's sceAtrac recognises the
chunk and fills each frame from the decoded file instead of the ATRAC decoder. Everything else is the game's own
streaming code running as for one of its tracks. The pieces:

| Where | What |
| --- | --- |
| PortableKit `hle_extension::add_file_provider` (`host/hle/hle_io.cpp`) | A file the host serves at a guest path |
| PortableKit `host/audio/host_audio.*` | Registering a host file, decoding it, the stand-in file |
| PortableKit `host/hle/hle_atrac.cpp` | Frames of a stand-in come from the decoded file |
| `port/host/d012_mods.*` | Mods, the catalogue of series and tracks, the BGM file provider, the test switches |

For the game side: `d012::mods::catalogue()` lists the series (in display order) and their tracks with ids, numbers
and titles; making the game open `bgm_path(track)` where it would open one of its own BGM files plays the track.
`d012::mods::prefetch(track)` decodes it ahead, on a worker thread, when the selector lands on it. The BGM playlist
(`port/host/d012_bgm_playlist.cpp`) does exactly that: each track becomes a row of the game's BGM table whose file
is `MD<number>.AT3`, so the game's own selector, field and streaming handle it.

## 6. Test switches

Off by default; none of them is how the feature ships.

| Variable | What it does |
| --- | --- |
| `D012_TEST_BGM_OVERRIDE=<track id>` | The **next** BGM file the game opens plays this mod track instead (the id, `example-tones:intro-loop`, or its number in hexadecimal). The one thing that replaces anything, and only for one track of one run; made to hear a mod track before the selector existed (now the selector chooses one) |
| `D012_TEST_MODS_DECODE=1` | Decodes every track at start-up and logs its codec, rate, length, level and (for a pure tone) its pitch from zero crossings, and checks its `MD<number>.AT3` path serves a stand-in |
| `D012_NO_MODS=1` | No mods |
| `D012_NO_HOST_AUDIO=1` | PortableKit's off switch for host audio: nothing is decoded or served, so the game plays its own music even with the override set |
| `D012_MODS_DIR=<dir>` | Another mods folder |

Hear-check without listening: `D012_AUDIO_DUMP=<file.wav>` writes the port's mixed output (at the game's levels,
even with the volume muted in the settings). Reproduce the verification below with the battle route of the
[bring-up log, section 22](bring-up-log.md#22-getting-to-a-battle):

```powershell
$route = "460:pad a;1080:pad a;1500:pad a;1650:pad a;1900:pad dpdown;2000:pad a;2400:pad a;3200:pad a;" +
         "3700:pad a;4400:pad a;4900:pad a;5600:pad dpdown;5750:pad dpright;5900:pad dpup;6050:pad a;" +
         "7100:pad a;8000:pad a;8020:speed normal;9100:quit"
.\.venv\Scripts\python.exe tools/port/run_d012.py --seconds 90 --build-dir tools/local/port/build-d012-recomp `
    --data-dir build/port/data-mods-test --env PSPRECOMP_NO_INTERPRETER=1 --env D012_UNTHROTTLED=1 `
    --env D012_TEST_BGM_OVERRIDE=example-tones:intro-loop --env D012_AUDIO_DUMP=build/port/mods-test/override.wav `
    --env "D012_INPUT_SCRIPT=$route"
```

## 7. Verification (19 October 2026)

Recompiled build, `PSPRECOMP_NO_INTERPRETER=1`, the bring-up log's Quick Battle route, data in
`build/port/data-mods-test` (a copy of `build/port/data`), settings muted, output dumped with `D012_AUDIO_DUMP`
and scanned for the test frequencies with a Goertzel filter every 0.25 s.

- V: **Whole-file track.** `example-tones:intro-loop` (6 s, 74,100-byte stand-in, smaller than the game's 96 KiB
  buffer) in place of `B01_01.AT3`: the dump has 440 Hz for 2.0 s, then 660 Hz and 880 Hz alternating each second
  for the remaining 34 s of the run, never 440 again (the loop from sample 88,200). Level about 4,500 against the
  file's 8,192: the game's music volume.
- V: **Streamed track.** A 30 s test track (twelve 2.5 s steps, 300 to 850 Hz, loop from 10 s; 363,060-byte stand-in)
  in place of `U_01CHOS.AT3`: the game streamed it in 16 `sceAtracAddStreamData` calls, every step is in the dump in
  order, then 500 Hz again at 30 s (the loop start), with the game re-reading the file from the loop's offset.
- V: **Formats.** `D012_TEST_MODS_DECODE=1`: MP3 (1,000 Hz), Ogg Vorbis (1,250 Hz), FLAC (1,500 Hz), Opus at 48 kHz
  (1,750 Hz) and WAV decode to the right length and pitch; a 48 kHz mono WAV is resampled and doubled to stereo; an
  Ogg's `LOOPSTART`/`LOOPLENGTH` tags give its loop. The test files were made from generated tones with the bundled
  FFmpeg's `ffmpeg.exe` and live only in the test data copy.
- V: **Validation.** A manifest that is not JSON, a bad key, a wrong type, a duplicate track key, a path leaving the
  folder, a missing file, a file that is not audio, a loop that ends before it starts and a track without a title
  are each logged with file and line and left out; the rest of the same mod loads, and the game runs on.
  `enabled.json` turns a mod off and warns about a name no mod has.

- V (19 October 2026, the selector): `example-tones:intro-loop` chosen on the loading screen (series TEST TONES)
  with no test switch replacing anything: the game opened `DISC0:/PSP_GAME/USRDIR/DATA/BGM/MD7D9EC9A2.AT3`, the
  host served its stand-in, and the dump has 440 Hz for 2 s, then 660 and 880 Hz alternating
  ([bgm-playlist-design.md](bgm-playlist-design.md), section 10.6).

- V (19 October 2026, Nintendo streams): a 4 s stereo 44.1 kHz DSP-ADPCM BRSTM of test tones (440 Hz to sample
  88,207, which is not a multiple of a 14-sample frame; then 660 Hz, 880 Hz and a 990 Hz marker to the end, sample
  176,407), its header saying loop flag on, loop start 88,207, imported through the Battle BGM Maker's API and saved as
  `"auto"`, chosen with `D012_TEST_BGM_START` in a Quick Battle. The log: `the BRSTM header's loop, samples 88207
  to 176407` and `176407 frames at 44100 Hz, loop 88207..176406`. A Goertzel scan of the dump (10 ms windows, 5 ms
  steps) shows the marker up to each of 9 matched seams, 2.000 s apart, and 660 Hz right after it, never 440 Hz;
  matched to the sample against the expected stream, every seam's shift is 0 (the three with no battle sound over
  them to -80 dB), and a shift of one sample is far worse. The same run's decode check: the same tones as BCSTM,
  Wii U BFSTM, Switch BFSTM at 48 kHz (header loop 96,008, frame 88,207 after resampling), a BRSTM without a loop
  flag (whole file) and a BRSTM with `{"start": 132300}` in the manifest (the manifest wins) all loop where
  expected; an IMA-ADPCM BCSTM and a BWAV are refused with their errors.
  `port/tools/bgm_maker/tests/stream_port_check.py`, output in `build/port/brstm-test/analysis.txt`.

Not verified: a long real-world MP3 or Ogg (decode time), a track that plays once to its end in battle, the
source-built FFmpeg on macOS and Linux, a real (not synthetic) BRSTM, BCSTM or BFSTM.

## 8. Making a mod with the Battle BGM Maker

The Battle BGM Maker (`BgmMaker.exe`, source in `port/bgm_maker/`) is a program of its own beside `D012Native.exe`
that writes `mod.json` for you, above all the loop points, which an MP3 cannot carry in its tags. It checks a mod with
the game's own manifest code (`port/host/d012_mod_manifest.cpp`, which both programs build) and decodes with the
game's own FFmpeg libraries. It never uses the network and never runs the game's code.

- **Installed with the setup** when you tick *Battle BGM Maker* there (about 4 MB; run the setup again to add or
  remove it): *Battle BGM Maker* in the Start menu. It works on the install's `data\mods`.
- **Built from source:** `setup.ps1` builds it, as `tools\local\port\build-d012-clang\bin\BgmMaker.exe`; it finds
  the checkout's `build\port\data\mods` by itself.
- **On Linux and the Steam Deck:** `Dissidia012-BgmMaker-<version>-x86_64.AppImage`, one file that runs where it is
  with nothing installed (mark it executable first: *Properties > Permissions*, or `chmod +x`). Put in the game's
  folder, beside `D012Native`, it works on that game's `data/mods`, and *Try in game* starts that game. Kept anywhere
  else, it opens the folder last opened in it; the first time, it looks for copies of the game where players keep
  them (the folder the Linux setup's menu entry names, Steam's shortcuts to `D012Native`, which it only reads, and
  the folders in `~/Games`), each with its `data/` folder. One copy is used as it is; with several it starts on the
  one played last (newest log, settings or save) and asks which one, and *Copies of the game...* on the Mod step asks
  again. The Windows version under Proton (`D012Native.exe`) is listed too: its mods can be made, but *Try in game*
  cannot start it. It carries atracdenc for the ISO export and a Japanese font for systems without one. Built from
  WSL with `tools/port/linux_bgm_maker.sh` (the Steam Runtime SDK container, as the Linux setup).
- `BgmMaker.exe D:\my-data\mods` works on any mods folder, and *Open another...* picks one in the window; with no
  game beside it, the tool opens that folder again next time.

The window follows the system's light or dark theme. Its four steps are tabs: any can be clicked, *Back* and *Next* walk
them in order, and each shows when it is done or how many problems it has. **Ctrl+Z** and **Ctrl+Y** undo and redo
every change until the next save. Nothing is written until *Check & save*; closing the window or switching mod with
unsaved changes asks first.

1. **Mod.** *+ New mod* asks for the id (checked as the port checks it), name, version, author and description. Or
   pick a mod from the list.
2. **Music**: the series and their tracks in one tree.
   - *+ Series*, then its name as the selector shows it (*ABC* turns it upper case, like the game's own: FINAL
     FANTASY XV), its key (made from the name), its order, and optionally its short name for a PSP / PPSSPP ISO
     (below). ▲ and ▼ move a series in the game's list (they renumber `order`, which is what the port sorts by).
   - Drop audio files from Explorer on a series, or *Add files...* (MP3, Ogg, Opus, FLAC, WAV, BRSTM, BCSTM,
     BFSTM). Each file is **copied** into the mod's `music/` (the original is never touched) under a plain name.
     The title comes from the file's tags (or its name, as for BRSTM and its kin, which have no tags) and the key
     from the title, unique in the mod. Edit either; a key stays as it is once saved, because saved choices refer
     to it.
   - Drag tracks to reorder them or to move them to another series. A removed track's copied file is deleted only
     when the mod is saved, so Ctrl+Z can bring it back.
3. **Loops.** Pick *Custom loop*, *Whole file*, *None* (plays once) or *Auto* (the file's loop tags, if any).
   A BRSTM, BCSTM or BFSTM with a loop comes in as *Auto*, its header's loop shown "from the file" and drawn on the
   waveform; *Custom loop* starts from those points to change them. It is saved as `"auto"` until it is changed, so
   the loop stays in one place, the file, which the port reads with the same rules as the tool: replacing the file
   with a better rip keeps the right loop, and no number in the manifest can drift from the file it belongs to. A
   changed loop is saved in samples like any custom loop.
   For a custom loop:
   - the waveform: the wheel zooms (**+** and **-** too, **F** shows all of the file), dragging pans, the strip
     below shows the whole file; a click puts the cursor;
   - **S** and **E** set the loop start and end at the playhead (while playing) or the cursor; the flags drag; or
     type samples or `m:ss.mmm`;
   - **←** and **→** nudge the chosen marker (**[** or **]**) by the step: 1 or 10 samples, 1 or 10 ms, a beat or a
     bar at the BPM given (Shift: ten steps); **Z** moves it to the nearest zero crossing;
   - **Space** plays from the cursor and follows the loop as the game does (**Home**: from the start); **A**,
     *Audition seam*, plays the few seconds before the loop end and then the loop start, sample-exact (*repeat*
     plays it again and again). A seam you can hear is a seam to fix; the seam view below the markers shows both
     sides of the jump, with what lies beyond each side faintly, and how far apart they are;
   - *Suggest loops* looks for a start and end whose audio repeats: it compares spectral fingerprints of the whole
     file at every loop length, then fixes the length to the sample by cross-correlating the waveform and puts the
     end where the samples on both sides of the seam match best. *Starts for my end* keeps your end and ranks
     starts. They are only suggestions: audition each and choose. They need the file to hold the repeat (most rips
     play the loop twice); a file that stops exactly where the loop would restart has nothing to compare.
4. **Check & save.** The manifest is checked with the port's rules (the same messages as the log's `[mods] error:`
   lines), plus a loop outside the file, a file FFmpeg cannot read, a track over 30 minutes, and an id or track
   number another mod already has. Each problem jumps to its step, and nothing is written until there are none.
   *Save* (Ctrl+S) keeps the previous `mod.json` in the mod's `.backups/` (the last 20). Loops are written in samples
   of the file at its own rate; a loop that ends at the end of the file leaves `end` out. Then *Try in game* starts
   the game (when it is not running already) with `D012_TEST_BGM_START` on the selected track, so the loading
   screen's choice starts on it, and `D012_MODS_DIR` when the mods folder is not the game's own. The game reads mods
   when it starts.

Without the window, for scripts and the end-to-end checks (`port/bgm_maker/cli.hpp`):

```powershell
BgmMaker.exe --check <mod folder>
BgmMaker.exe --import <mod folder> <series key> <file>...
BgmMaker.exe --set-loop <mod folder> <track key> auto|whole|none|<start> [<end>]
BgmMaker.exe --psp-iso <original ISO> <new ISO> [<mods folder>]
```

The Python tool it replaces, `port/tools/bgm_maker/` (a local page in the browser), stays in the repository for
developers, frozen (bug fixes only); its tests and the C++ ones run the same manifest cases
(`port/tests/manifest_cases.json`).

### Making a PSP / PPSSPP ISO (optional)

At the end of *Check & save*, *Make a PSP / PPSSPP ISO (optional)* writes a copy of your ISO with your mods' tracks
added to the game's own battle music list, for PPSSPP or a PSP with custom firmware. The port does not need it.

- **Which tracks:** every track of every mod the game would load, as it loads them: `enabled.json` read the same
  way, folders in order, a mod with errors or a duplicate id left out, series by `order`, a track number another
  mod already has left out. The section lists them before you start, and what is left out and why.
- **Their names:** the stock selector has one flat list, so each track is named `<short> - <title>`
  ("FFXVI - Titan Lost"). `short` is the series' short name (section 2's series table; *Short name (PSP)* in the
  Music step); without one the tool makes it from the name (`FINAL FANTASY XVI` becomes `FFXVI`). A name longer
  than 37 characters, the disc's longest, may not fit the selector's box: the section warns.
- **The original** must be a clean dump the setup accepts (USA ULUS10566 or European ULES01505, v1.00, checked by
  size and SHA-1; the European one is accepted but not tried yet). It
  is only read, never written. The installed game's ISO is filled in for you (`settings.ini`'s `disc_image`).
- **The new ISO** is written as `<name>.part`, then renamed; nothing is left behind if it fails or you cancel.
  Each track is encoded to ATRAC3 (132 kbps, stereo, 44.1 kHz) by atracdenc (`atracdenc.exe` beside the tool, LGPL
  2.1; `tools/port/build_atracdenc.cmd` builds it from its source), with its loop points. A loop shorter than 6,144
  samples (0.14 s) is too short for the PSP's decoder. The disc's two music archives get a row and a name for
  each track, the music files go in `DATA/BGM` as `MD00000000.AT3` and on; nothing else of the image changes.
- **Room:** the game's list holds 256 rows and the disc uses 131, so 125 tracks at most. DLC music on the memory
  stick shares that room (the game refuses rows past 256): with all 43 USA DLC tracks, 82. The same goes for mods in
  the port ([README](../../README.md#downloadable-content-dlc)).
- **Windows N editions** need Microsoft's Media Feature Pack: the encoder reads its input through Media Foundation.

Checked on 30 October 2026: an ISO with a 31-track mod (52 s to make, 1,789 MB) in PPSSPP, where the selector offers
69 tracks, the longest name fits and a real song's custom loop joins within 4 samples, with no drift over several
loops; and `port/tools/bgm_maker/tests/psp_port_check.py`, which makes an ISO of the end-to-end check's tone mod
and runs the port on it with its audio dump: the loop lands where the tool set it. **Not verified on a real PSP.**

### What a sample means in the tool and in the game

`BgmMaker.exe` decodes every file in the program with the game's own FFmpeg libraries, as the game does (with the
packet time base set, PortableKit `host/audio/host_audio.cpp`), to 16-bit samples at the file's own rate, and draws
and plays exactly those; so both drop what an MP3's LAME/Xing header says is encoder delay and padding. Its decode
is byte for byte the Python tool's (`ffmpeg.exe` to a WAV), which the measurements below were made with; the
end-to-end check `port_check.py` now runs on `BgmMaker.exe` and finds the same.

- V (19 October 2026, FFmpeg n7.1.5): a 5 s, 220,500-sample tone track encoded to MP3 with libmp3lame (1,105
  samples of encoder delay; the file's stream starts at 0.025 s) decodes in the tool to **220,500 samples**, with
  the original's timing (sample lag 0 against the source at a change of tone; `tests/test_server.py`). The port's
  log for the same file: `220500 frames at 44100 Hz ..., loop 88200..176399`, the tool's numbers.
- V: in the game the loop jumps where the tool set it (below). Nothing needs compensating.
- I: a file at another rate (48 kHz) is resampled by the port to 44,100 Hz, and a loop sample S lands on frame
  `round(S × 44100 / rate)`, within half a frame of the tool's point. Not measured.

### Verification of the Python tool (19 October 2026)

- `python -m unittest discover -s port/tools/bgm_maker/tests`: 69 tests (manifest rules against the port's,
  including every case in section 7's validation list; keys and ids; samples and seconds; the API's token, host
  check and path safety; decoding; Nintendo stream headers). One is skipped on Windows without the right to make a
  symbolic link. `tests/test_streams.py` writes its own BRSTM, BCSTM and BFSTM files (`tests/stream_fixtures.py`:
  FFmpeg has no muxer or encoder for them, so a small DSP-ADPCM encoder with eight fixed predictors and FFmpeg's
  decoding arithmetic) and checks the header parser on each variant, without a loop, with a loop past the end, with
  unsupported codecs and broken headers, and with the numbers of a real song's header patched in (loop 3,641,344 to
  9,734,769 at 44,100 Hz). With FFmpeg: every variant decodes to exactly the samples encoded and the header's count;
  silence, then a tone from the loop start (sample 3,001, mid-frame, small blocks), decodes with its first sound at
  sample 3,001; importing through the API pre-fills the loop and saves `"auto"`; BWAV and IMA-ADPCM are refused.
- `python port/tools/bgm_maker/tests/port_check.py` (end to end, test tones only): makes an MP3 of tones (440 Hz
  for 2 s, 660 Hz for 1 s, 880 Hz for 0.95 s, a 50 ms 990 Hz marker, then 1320 Hz), creates the mod
  `modmaker-test` **through the tool's HTTP API** in `build/port/data-modmaker-test/mods` (a muted copy of
  `build/port/data`), imports the MP3 and saves the loop 88,200 to 176,400. The recompiled port then runs (90 s at
  most) to a Quick Battle with `D012_TEST_BGM_START=modmaker-test:mod-maker-test-tones` and `D012_AUDIO_DUMP`.
  V: twelve loop seams in the dump, 2.000 s apart; a Goertzel scan (10 ms windows, 5 ms steps) shows the marker
  right up to every jump and 660 Hz right after it, never 440 Hz (what a 1,105-sample delay would put after each
  jump) or 1320 Hz (what it would put before). Matched to the sample against the tool's decode, 11 seams are at
  shift 0 exactly and one is drowned by the battle's sounds; all seams together give 0, with ±1 sample 26 % worse
  and ±1,105 fifty times worse. Output in `build/port/modmaker-test/analysis.txt`.
- The page was checked in a Chromium browser: making a mod, adding a file through the same upload a drop uses,
  the shortcuts, marker dragging, playing and auditioning, suggestions (on a synthetic tone piece with an intro, a
  22.25 s loop played twice and a fade: the loop found to the sample, match 100 %), saving with a backup.

Not verified: a real song in the suggester (only tone pieces were used), a file at another rate in the game,
browsers other than Chromium, macOS and Linux.

## 9. Package files: changing a character

> Added 1 November 2026. The scope, and the check of dissidia-fan's tools against the USA disc:
> [move-studio.md](move-studio.md).

Almost everything the game loads, every character's moves (`obj/p_sev100.objx`) and models
(`obj/p_sev100_1p.gmo`) among it, is an entry of one file on the disc, `PSP_GAME/USRDIR/DATA/PACKAGE.BIN`, found
through its index, `PACKAGE_INFO.BIN`. A mod's `package` folder holds files the game reads in their place:

```text
mods/cloud-heavy/
  mod.json                  {"format": 1, "id": "cloud-heavy", "name": "Heavier Cloud", "version": "1",
                             "package": "package"}
  package/obj/p_sev100.objx the entry at that game path
  package/by_crc/4E88D7FF.bin  the same, by the CRC of the path (for entries whose path is not known)
```

- **A file at its game path** replaces the entry with that path, in any case, with `/` between folders. A path is
  found by the CRC of its lower-case text (BZip2's CRC-32), as the game finds it.
- **`by_crc/<8 hex digits>.bin`** replaces the entry with that CRC.
- **dissidia-fan's `charmod.py build`** writes exactly this layout: give it the package folder as its output,
  `charmod.py --mods <data dir>/mods/<id>/package --disc <disc0> build <name>`, and add the `mod.json` above. Its own
  notes in that folder (`roster.txt`, `readme_charmod_*.json`) are not package files and are passed over. Their
  Move Studio and tools are their own, with no licence yet, and are not part of this repository; see
  [move-studio.md](move-studio.md).
- **What changes:** the game is given an index with each replaced entry's new size. A file that fits in the sectors
  the entry had stays there; a bigger one is given sectors past the end of `PACKAGE.BIN`. The game's reads inside a
  replaced entry come from the mod's file (zeros after its end, to the end of the sector). Nothing on the disc or
  in the ISO is written.
- **Which wins:** a file of a mod that is off counts for nothing. When two mods replace the same entry, the one
  whose id sorts first gives it, and the other's file is an error in the log (`[mods] error: ... replaces the same
  package entry as ...`). The same bytes in both are no clash: `charmod.py` puts `main.bin`
  (`by_crc/93307C1C.bin`) in every folder it builds, identical each time.
- **Bigger models:** a model file (`obj/*.gmo`, `*.objx`) over the game's 2,506,752 bytes is given room (below), up to
  about 9.7 MB. A bigger one is warned about and will not load.
- **Not yet:** a file whose path is no entry of the package is left out with a warning: adding new entries (a new
  character's files) needs the bigger index of tier 3 in [move-studio.md](move-studio.md).
- **The log:** `[mods] package: N file(s) from M mod(s)` at start-up, and when the game first opens the package
  `[mods] package: N entries replaced, A in place and B moved past the end`.

Switches: `D012_NO_PACKAGE_MODS=1` turns package files off (the mods' music stays); `D012_NO_MODS=1` turns every
mod off. `D012_TRACE_PACKAGE_MODS=<n>` logs the first n reads of `PACKAGE.BIN` that touch a mod's file
(`[mods] package read at <offset>, <length> bytes: <mod id>/<file> from byte <n>`).

The port itself replaces package files the same way for [Bartz's original moves](original-moves.md), which it
builds from the disc when the game first opens its package (`package_mods::add_late_source`), and for the
[Manikin look](manikin-look.md)'s menu texts. A mod's file of the same entry wins, and the port then leaves all of
that feature's own out. `D012_NO_MODS=1` does not turn those off;
`D012_NO_PACKAGE_MODS=1` does.

The code: `port/host/d012_package_index.cpp` (the index, and which bytes the game reads where; checked by
`port/tests/d012_tests.cpp` on a made-up package), `port/host/d012_package_mods.cpp` (the mods' folders, and
PortableKit's file providers, which serve the game the index and the package), `port/host/d012_model_memory.cpp`
(room for bigger models).

### Abilities

A package that adds abilities to a character (rows in `system.bin`'s ability table, as
`tools/original_moves.py` writes for Bartz's original Dissidia moves) lists them in mod.json:

```json
"abilities": [{"id": "0920", "learn": 1}, {"id": "0921", "learn": 10}, {"id": "0927", "learn": 0}]
```

- **`id`** is the ability's id, four hex digits: the character's id times 256 plus its key.
- **`learn`** is the level from which the port puts it in the character's learned list, whatever save is loaded.
  Abilities are given only when the game's ability table has them (with package files off, none are). 0 leaves it
  to the game, for an ability unlocked by mastering another.
- **Saves stay the game's own.** Right before the game's save is written, the mod's abilities are taken out of the
  learned lists and the equipped slots, and put back right after. The save file is one the game reads without the
  mod (PPSSPP or a PSP).
- **A side file of the port's** keeps their AP, mastery and slots: `ms0/PSP/MODS/abilities/<game><save>_<file>.txt`.
  It is read again after a load. The slots come back only while the save is the one written with the file.
- `D012_NO_MOD_ABILITIES=1` turns this off; the code is `port/host/d012_mod_abilities.cpp`.

### Hitbox groups placed at battle start

A moveset can name hitbox groups 0 to 63. A package with more (Bartz with both move sets) parks the rest:
record byte 0 = 63, byte 1 = the group's number, and events naming 63 + 256 x number. When the battle works out a
fighter's enabled commands, the port puts the parked groups those commands use on ids nothing else of the fighter
can use. `D012_NO_SHAPE_IDS=1` turns it off; the code is `port/host/d012_shape_ids.cpp`.

### Room for bigger models

The game loads each model file into a staging block of a fixed 0x264000 (2,506,752) bytes, so a bigger file used to
crash the battle load (an allocation came back null and the load read guest address 0). Traced in the USA build
(1 November 2026; dissidia-fan's notes on the EU build gave the figures, and EU and USA code are the same):

- V: model slots are registered by main `0x0884417C(manager, id, heap, 0x264000, 0)`, from battle `0x08B45310` (four
  slots: ids 0x54E, 0x134, 0x408, 0x548) and main `0x08844C14`, `0x089121F4` and `0x08922B90`. Those are the only
  uses of the constant. `0x0884417C` hands the slot to `0x08843014`, which takes the slot's whole size from the
  heap (`0x0885C6CC`), loads the file into it and frees it.
- V: that heap is the file heap, built by main `0x088907C4`: `0x0888204C(0x275800)` takes a block from the game's
  main heap and `0x0885C40C(block, 0x275800, name, 0x10)` makes a heap of it, whose handle is the block's address.
  `0x089110B4` returns it for the slots.
- V: both fighters' models are built in the battle model pool, made the same way by main `0x088667DC` with 0x780000
  bytes at game start.
- V: guest memory is full in a battle. The game's image and heaps run to `0x09B71000`, its 4 MB effect allocator
  follows, and thread stacks fill the top of the 32 MiB (`D012_TRACE_KERNEL=1`).

So the profile now gives the game 64 MiB, a PSP-2000's. The port keeps everything above `0x0A000000` from the
kernel as soon as the kernel is set up, so the game's own blocks are where they are in 32 MiB (the same addresses,
checked with `D012_TRACE_KERNEL=1`). When a mod's model is bigger than the game's block, the port hooks four of the
game's functions:

- **`0x08843014`:** a slot of 0x264000 bytes taken from the grown file heap gets the model's size instead, rounded up
  to 4 KiB.
- **`0x0888204C`:** the file heap's and the pool's blocks come from the kept memory, grown by the slot's growth (the
  file heap) and by twice that plus 512 KiB (the pool: two fighters).
- **`0x0885C40C`:** the heaps are made at those sizes.
- **`0x0888208C`:** a block given back is kept for the next time it is asked for.

The pool's growth is an estimate. Padded models do not test it, since padding adds nothing to the built model.
Each hooked function is in another generated unit than its callers, so every call reaches the hook. With no model
bigger than the game's, nothing is hooked.

The log: `[mods] model memory: model files up to N bytes ...; file heap ... at 0x0A000000, battle model pool ...`
at start-up, and `[mods] model memory: a model slot of N bytes` at the first big slot. `D012_NO_MODEL_MEMORY=1`
keeps the game's sizes. `D012_TRACE_MODEL_MEMORY=1` logs each use.

### Verification (1 November 2026)

Recompiled build in `tools/local/port/build-d012-package`, data folder `build/port/data-package-test` (a muted copy
of `build/port/data`, the port's own New Game), the scripted Quick Battle route of the BGM Maker's `port_check.py`
(Lightning against a CPU Lightning in her second costume, Old Chaos Shrine), screenshots in
`build/port/package-test/`.

- V: the game opens `DISC0:/PSP_GAME/USRDIR/DATA/PACKAGE.BIN` once, by path, then seeks and reads
  (`D012_TRACE_IO`), so a file provider sees it. `PACKAGE_INFO.BIN` is opened first.
- V: **in place.** Lightning's 1P model (`obj/p_gst130_1p.gmo`) with every palette magenta, the same size: one entry
  replaced in place, the game read all 1,741,984 bytes from the mod, and the player's Lightning is magenta in battle
  while the CPU's (her 2P model, from the disc) is not. The battle played on and the run ended normally.
- V: **moved.** The same file 100,000 bytes longer: moved past the end (1,843,200 bytes), the game read it at
  offset 1,034,358,784, the real file's end, and the player's Lightning is magenta.
- V: **charmod.py's output as it is.** A retail Lightning folder (as the Move Studio's *Save as mod* writes it) with
  all 70 of her hits at power 99, built with `charmod.py build` into the package folder: `main.bin` was read from
  the mod at boot and `obj/p_gst130.objx` once for each Lightning. In battle single Brave hits did 146 where the
  unmodded run's did 23 to 42, and player 1 was beaten within the route. (The CPU's choices differ between runs,
  so the two battles are not the same frame for frame.)
- V: two mods with the same files: no error; with one byte different: the error, and the mod whose id sorts first
  is used. `D012_NO_PACKAGE_MODS=1`: `[mods] package mods off`, nothing replaced.
- V: `port/tests/d012_tests.cpp` (the CRC against the USA index, the index written and read back, unsorted and short
  indexes refused, in place, moved, a missing CRC, the first of two replacements, and every byte of a made-up
  package read back in uneven pieces; the manifest's `package` member).

- V: **bigger models.** Without room, the magenta model padded to 2,641,984 bytes crashed the battle load
  ("Guest memory access outside PSP RAM/EDRAM at 0x0000A100"). With it: slots of 2,646,016 bytes for ids 596, 597
  and 1140, the player's Lightning magenta in battle, and the battle played on. Padded to 5,741,984 bytes: the same.
  `D012_NO_MODEL_MEMORY=1`: the warning, then the old crash. No mods: nothing hooked, the battle as before, and
  every kernel block at its 32 MiB address.

Not verified: a model whose built data is bigger (the pool's growth), a second battle in one run (a heap's block
given back and asked for again), the Steam Deck and Linux, a mod's music and package together in one run, and the
Move Studio's own *Save as mod* (the folder was made by a script that writes the same layout).
