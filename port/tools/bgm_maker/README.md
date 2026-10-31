# Battle BGM Maker

A local tool for setting up a mod's `mod.json`, above all its loop points. It ships with the port; MIT licence,
like the rest of `port/` (see [port/README.md](../../README.md)). The manifest format is
[docs/port/mod-folder.md](../../../docs/port/mod-folder.md); section 8 there is the guide to this tool.

```powershell
python port/tools/bgm_maker/bgm_maker.py                      # the mods folder build/port/data/mods
python port/tools/bgm_maker/bgm_maker.py D:\some\data\mods     # any mods folder
python port/tools/bgm_maker/bgm_maker.py --ffmpeg C:\ffmpeg\bin\ffmpeg.exe --port 8800 --no-browser
```

- Python 3.10 or newer and its standard library only. No network: the page and its scripts come from `web/`.
- Serves on 127.0.0.1 only, on a free port, and opens the default browser. Stop it with Ctrl+C.
- Decodes audio with the port's own FFmpeg (`ffmpeg.exe`/`ffprobe.exe` from the port's build, under
  `tools/local/port/build-d012-recomp/_deps/ffmpeg/*/bin`, or `--ffmpeg`, or `D012_FFMPEG`), so a sample in the tool
  is the same sample in the game, MP3s included. Decoded WAVs are cached in `%TEMP%\d012-bgm-maker` for a week.
- The local API needs the per-session token in the address it prints, answers only its own host name, and refuses
  any path outside the mods folder.

| File | What |
| --- | --- |
| `bgm_maker.py` | Starts the server and the browser |
| `bgmmaker/manifest.py` | The port's manifest rules (validation, keys, ids, sample and seconds conversion), building `mod.json` |
| `bgmmaker/audio.py` | Finding FFmpeg, reading tags with ffprobe, decoding to WAV at the file's own rate |
| `bgmmaker/nintendo.py` | The loop in a BRSTM, BCSTM or BFSTM header (the rules of the port's `d012_nintendo_stream.cpp`) |
| `bgmmaker/server.py` | The HTTP API and the path checks |
| `web/` | The page: waveform and seam views (`waveform.js`), the editor (`app.js`), loop suggestions in a worker (`suggest.js`) |
| `tests/test_*.py` | Unit tests: `python -m unittest discover -s port/tools/bgm_maker/tests` |
| `tests/port_check.py` | End-to-end: makes a tone mod with `BgmMaker.exe`'s command line (the C++ tool, `port/bgm_maker/`), runs the port (90 s at most) and checks the loop in its audio dump |
| `tests/stream_fixtures.py` | Synthetic BRSTM, BCSTM and BFSTM files of test tones (a small DSP-ADPCM encoder), for the tests |
| `tests/stream_port_check.py` | End-to-end for a BRSTM's header loop in the game (the mod made with `BgmMaker.exe` too), and the other stream variants in the port's log |
| `../../tests/manifest_cases.json` | Manifest cases this tool's tests and the C++ tool's (`port/tests/d012_tests.cpp`) both run, so the two keep the game's rules |
