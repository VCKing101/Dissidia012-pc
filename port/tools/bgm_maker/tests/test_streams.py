"""BRSTM, BCSTM and BFSTM: the header parser (loop flag, loop start, sample
count) on synthetic files the tests write (stream_fixtures.py; test tones and
silence only), and, with the port's FFmpeg, that FFmpeg decodes them to
exactly the samples encoded, so a loop start in the header is the same sample
in the decoded audio. Also the import and save path in the API.

    python -m unittest discover -s port/tools/bgm_maker/tests
"""

import json
import os
import struct
import sys
import tempfile
import unittest
import wave

TOOL = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, TOOL)
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import stream_fixtures as sf  # noqa: E402
from bgmmaker import audio, manifest as mf, nintendo  # noqa: E402

RATE = 32000
LOOP = 12345  # not a multiple of 14 (a DSP-ADPCM frame) nor of a block


def stereo(count=40007, rate=RATE):
    return [sf.tone(440, count, rate), sf.tone(660, count, rate, 7000)]


# Every variant the parser takes: (name, extension, builder).
VARIANTS = [
    ("BRSTM", ".brstm", lambda ch, loop, **k: sf.brstm(ch, RATE, loop, **k)),
    ("BRSTM little-endian", ".brstm", lambda ch, loop, **k: sf.brstm(ch, RATE, loop, big_endian=False, **k)),
    ("BCSTM", ".bcstm", lambda ch, loop, **k: sf.cstm(ch, RATE, loop, kind="CSTM", big_endian=False, **k)),
    ("BFSTM Wii U", ".bfstm", lambda ch, loop, **k: sf.cstm(ch, RATE, loop, kind="FSTM", big_endian=True, **k)),
    ("BFSTM Switch", ".bfstm", lambda ch, loop, **k: sf.cstm(ch, RATE, loop, kind="FSTM", big_endian=False, **k)),
]


def stream_info_at(data: bytes) -> int:
    """Where the stream info is in a fixture (to patch its numbers)."""
    big = data[4:6] == b"\xfe\xff"
    e = ">" if big else "<"
    if data[:4] == b"RSTM":
        head = struct.unpack_from(e + "I", data, 0x10)[0]
        return head + 8 + struct.unpack_from(e + "I", data, head + 0x0C)[0]
    info = struct.unpack_from(e + "I", data, 0x18)[0]
    return info + 8 + struct.unpack_from(e + "I", data, info + 0x0C)[0]


def patched(data: bytes, offset: int, fmt: str, *values) -> bytes:
    out = bytearray(data)
    e = ">" if data[4:6] == b"\xfe\xff" else "<"
    struct.pack_into(e + fmt, out, stream_info_at(data) + offset, *values)
    return bytes(out)


class Header(unittest.TestCase):
    def test_every_variant_gives_its_loop(self):
        for name, extension, build in VARIANTS:
            with self.subTest(name):
                data, _ = build(stereo(), LOOP)
                info = nintendo.parse_header(data)
                self.assertEqual(info["format"], extension[1:].upper())
                self.assertEqual(info["big_endian"], name in ("BRSTM", "BFSTM Wii U"))
                self.assertEqual((info["codec_name"], info["channels"], info["rate"]), ("DSP-ADPCM", 2, RATE))
                self.assertTrue(info["loop_flag"])
                self.assertEqual((info["loop_start"], info["samples"]), (LOOP, 40007))
                self.assertEqual(info["loop"], {"start": LOOP, "end": 40007})
                self.assertEqual(info["block_samples"], 40007)  # the blocks hold the samples the header says

    def test_no_loop_flag(self):
        for name, _, build in VARIANTS:
            with self.subTest(name):
                info = nintendo.parse_header(build(stereo(3000), None)[0])
                self.assertFalse(info["loop_flag"])
                self.assertIsNone(info["loop"])

    def test_large_numbers_from_a_real_song_header(self):
        """Loop 3,641,344 to 9,734,769 at 44,100 Hz (the numbers of a song from the site, patched into a
        small fixture: the parser reads only the header)."""
        for name, _, build in VARIANTS:
            with self.subTest(name):
                data = build(stereo(2000), 0)[0]
                rate_fmt = "H" if data[:4] == b"RSTM" else "I"
                data = patched(data, 4, rate_fmt, 44100)
                data = patched(data, 8, "II", 3641344, 9734769)
                info = nintendo.parse_header(data)
                self.assertEqual((info["rate"], info["loop"]), (44100, {"start": 3641344, "end": 9734769}))

    def test_bfstm_original_loop_fields_are_not_the_loop(self):
        data, _ = sf.cstm(stereo(3000), RATE, 140, kind="FSTM", big_endian=False, original_loop=(133, 2999))
        self.assertEqual(nintendo.parse_header(data)["loop"], {"start": 140, "end": 3000})

    def test_mono_and_many_channels(self):
        for count in (1, 4):
            data, _ = sf.brstm([sf.tone(440, 2000, RATE)] * count, RATE, 100)
            self.assertEqual(nintendo.parse_header(data)["channels"], count)

    def test_loop_start_past_the_end(self):
        data = patched(sf.brstm(stereo(3000), RATE, 100)[0], 8, "I", 3000)
        info = nintendo.parse_header(data)
        self.assertIsNone(info["loop"])
        self.assertIn("at or past its end", info["loop_problem"])

    def test_codecs_ffmpeg_cannot_take(self):
        data, _ = sf.cstm([[0] * 2000], RATE, 0, codec="ima", kind="CSTM", big_endian=False)
        with self.assertRaisesRegex(nintendo.StreamError, "IMA-ADPCM audio, which FFmpeg .* cannot decode"):
            nintendo.parse_header(data)
        data, _ = sf.brstm(stereo(2000), RATE, 0, codec="pcm16")
        with self.assertRaisesRegex(nintendo.StreamError, "PCM16 audio, which FFmpeg .* does not decode correctly"):
            nintendo.parse_header(data)

    def test_broken_files(self):
        good = sf.brstm(stereo(2000), RATE, 0)[0]
        cases = {
            "shorter than a header": good[:0x20],
            "no byte order mark": good[:4] + b"\0\0" + good[6:],
            "no HEAD chunk": good[:0x40] + b"XXXX" + good[0x44:],
            "cut short": good[:0x50],
            "not a BRSTM, BCSTM or BFSTM": b"OggS" + good[4:],
            "no channels": patched(good, 2, "B", 0),
            "sample rate of 0 Hz": patched(good, 4, "H", 0),
            "no samples": patched(good, 0x0C, "I", 0),
        }
        for why, data in cases.items():
            with self.subTest(why), self.assertRaisesRegex(nintendo.StreamError, why):
                nintendo.parse_header(data)
        cafe = sf.cstm(stereo(2000), RATE, 0)[0]
        with self.assertRaisesRegex(nintendo.StreamError, "no INFO block"):
            nintendo.parse_header(cafe[:0x40] + b"XXXX" + cafe[0x44:])

    def test_read_header_reads_files(self):
        with tempfile.TemporaryDirectory() as folder:
            path = os.path.join(folder, "Song (loop).bfstm")
            with open(path, "wb") as handle:
                handle.write(sf.cstm(stereo(), RATE, LOOP)[0])
            self.assertEqual(nintendo.read_header(path)["loop"], {"start": LOOP, "end": 40007})
            wrong = os.path.join(folder, "a.brstm")
            with open(wrong, "wb") as handle:
                handle.write(b"\0" * 256)
            with self.assertRaisesRegex(nintendo.StreamError, "not a valid BRSTM file"):
                nintendo.read_header(wrong)

    def test_encoder_reproduces_a_tone(self):
        """The fixtures' DSP-ADPCM encoder is good enough for tone tests (about 40 dB SNR or better)."""
        source = sf.tone(440, 4000, RATE)
        _, decoded, _ = sf.dsp_encode(source, sf.predictors(RATE))
        noise = sum((a - b) ** 2 for a, b in zip(source, decoded))
        signal = sum(a * a for a in source)
        self.assertGreater(signal / noise, 10 ** 4)


class Manifest(unittest.TestCase):
    def test_extensions(self):
        for name in ("music/a.brstm", "music/a.BCSTM", "music/a.bfstm"):
            self.assertIsNone(mf.check_file_text(name))
        for extension in ("bwav", "nus3audio"):
            why = mf.check_file_text(f"music/a.{extension}")
            self.assertIn("not supported", why)
            self.assertIn("BRSTM", why)

    def test_header_problems_are_warnings(self):
        doc = {"format": 1, "id": "m", "name": "M", "version": "1", "music": {"series": [
            {"key": "s", "name": "S", "tracks": [{"key": "t", "title": "T", "file": "music/a.brstm"}]}]}}
        stream = {"format": "BRSTM", "samples": 1000, "loop_problem": "the file's loop starts at sample 1000, at or "
                  "past its end (1000 samples)"}
        found = mf.validate(doc, info=lambda f: {"rate": 32000, "samples": 999, "stream": stream})
        self.assertEqual(mf.errors(found), [])
        text = " ".join(p["message"] for p in found)
        self.assertIn("the port loops the whole file", text)
        self.assertIn("FFmpeg decodes 999 samples, but the BRSTM header says 1000", text)


FF = None
try:
    FF = audio.find_ffmpeg()
except audio.FFmpegMissing:
    pass


def decoded_wav(path):
    with wave.open(path) as handle:
        n, ch = handle.getnframes(), handle.getnchannels()
        values = struct.unpack(f"<{n * ch}h", handle.readframes(n))
    return [list(values[c::ch]) for c in range(ch)], handle.getframerate()


@unittest.skipIf(FF is None, "no FFmpeg found")
class FFmpegDecoding(unittest.TestCase):
    def test_decodes_to_exactly_the_encoded_samples(self):
        """Every variant: FFmpeg's sample i is the encoder's sample i, and there are exactly as many as
        the header says, so the header's loop start is the decoded file's sample of that number."""
        with tempfile.TemporaryDirectory() as folder:
            decoder = audio.Decoder(FF, os.path.join(folder, "cache"))
            for name, extension, build in VARIANTS:
                with self.subTest(name):
                    data, expected = build(stereo(), LOOP)
                    path = os.path.join(folder, name.replace(" ", "_") + extension)
                    with open(path, "wb") as handle:
                        handle.write(data)
                    info = decoder.info(path)
                    self.assertIsNone(info.get("error"))
                    self.assertEqual((info["rate"], info["samples"]), (RATE, 40007))
                    self.assertEqual(info["loop_tags"]["start"], LOOP)
                    self.assertEqual(info["loop_tags"]["end"], 40007)
                    self.assertIn("header", info["loop_tags"]["source"])
                    channels, rate = decoded_wav(decoder.wav(path))
                    self.assertEqual(rate, RATE)
                    self.assertEqual(channels, expected)
                    # FFmpeg's own loop tag is in microseconds (not used: the header is in samples).
                    self.assertEqual(int(info["tags"]["loop_start"]), round(LOOP * 1_000_000 / RATE))

    def test_loop_start_is_the_decoded_sample(self):
        """Silence, then a tone from the loop start: the first sample FFmpeg makes that is not silent is
        the header's loop start, in every variant, with blocks small enough to cross several."""
        with tempfile.TemporaryDirectory() as folder:
            decoder = audio.Decoder(FF, os.path.join(folder, "cache"))
            start = 3001  # inside the third 1,400-sample block, mid-frame
            pcm = [0] * start + [12000 if (i // 20) % 2 else -12000 for i in range(5000)]
            for name, extension, build in VARIANTS:
                with self.subTest(name):
                    data, _ = build([pcm, pcm], start, block_samples=1400)
                    path = os.path.join(folder, "marker_" + name.replace(" ", "_") + extension)
                    with open(path, "wb") as handle:
                        handle.write(data)
                    self.assertEqual(nintendo.read_header(path)["loop"]["start"], start)
                    channels, _ = decoded_wav(decoder.wav(path))
                    first = next(i for i, v in enumerate(channels[0]) if v != 0)
                    self.assertEqual(first, start)
                    self.assertEqual(len(channels[0]), len(pcm))

    def test_unsupported_codec_is_an_error_not_a_bad_decode(self):
        with tempfile.TemporaryDirectory() as folder:
            path = os.path.join(folder, "a.brstm")
            with open(path, "wb") as handle:
                handle.write(sf.brstm(stereo(2000), RATE, 0, codec="pcm16")[0])
            info = audio.Decoder(FF, os.path.join(folder, "cache")).info(path)
            self.assertIn("only DSP-ADPCM streams are supported", info["error"])


if FF is not None:
    from test_server import ServerCase  # noqa: E402

    class Import(ServerCase):
        def decoder_factory(self):
            return audio.Decoder(FF, os.path.join(self.root, "cache"))

        def test_import_prefills_the_loop_and_saves_auto(self):
            folder = self.create()
            data, _ = sf.cstm(stereo(), RATE, LOOP, kind="FSTM", big_endian=False)
            status, result = self.request("POST", "/api/track/import?folder=tones&name=03%20Battle_Theme.bfstm",
                                          raw=data)
            self.assertEqual(status, 200, result)
            self.assertEqual(result["file"], "music/03 Battle_Theme.bfstm")
            self.assertEqual(result["title"], "Battle Theme")  # from the name: these files have no tags
            self.assertEqual(result["info"]["loop_tags"], {"start": LOOP, "end": 40007, "source": "the BFSTM header"})
            self.assertEqual(result["info"]["stream"]["description"], "BFSTM (Switch, little-endian), DSP-ADPCM")
            # The page keeps the file's loop as "auto" until the loop is edited.
            model = self.model(file=result["file"], loop={"mode": "auto"})
            status, saved = self.request("POST", "/api/mod/save", {"folder": "tones", "model": model})
            self.assertTrue(saved["ok"], saved)
            with open(os.path.join(folder, "mod.json"), encoding="utf-8") as handle:
                track = json.load(handle)["music"]["series"][0]["tracks"][0]
            self.assertEqual(track["loop"], "auto")
            # Edited: written in samples, with the end left out when it is the file's end.
            model = self.model(file=result["file"], loop={"mode": "custom", "start": LOOP + 7, "end": 40007})
            status, saved = self.request("POST", "/api/mod/save", {"folder": "tones", "model": model})
            self.assertTrue(saved["ok"], saved)
            with open(os.path.join(folder, "mod.json"), encoding="utf-8") as handle:
                track = json.load(handle)["music"]["series"][0]["tracks"][0]
            self.assertEqual(track["loop"], {"start": LOOP + 7})

        def test_unsupported_files_are_refused_on_import(self):
            self.create()
            status, result = self.request("POST", "/api/track/import?folder=tones&name=a.bwav", raw=b"BWAV" + b"\0" * 60)
            self.assertEqual(status, 400)
            self.assertIn("BWAV files are not supported", result["error"])
            self.assertIn("BRSTM version", result["error"])
            ima, _ = sf.cstm([[0] * 2000], RATE, 0, codec="ima", kind="CSTM", big_endian=False)
            status, result = self.request("POST", "/api/track/import?folder=tones&name=a.bcstm", raw=ima)
            self.assertEqual(status, 400)
            self.assertIn("IMA-ADPCM", result["error"])
            self.assertEqual(os.listdir(os.path.join(self.mods, "tones", "music")), [])


if __name__ == "__main__":
    unittest.main()
