"""Manifest building and validation against the port's rules (port/host/d012_mods.cpp),
key generation, and seconds/samples conversion.

    python -m unittest discover -s port/tools/bgm_maker/tests
"""

import json
import os
import sys
import tempfile
import unittest

TOOL = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, TOOL)

from bgmmaker import manifest as mf  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.dirname(TOOL)))
EXAMPLE = os.path.join(REPO, "port", "examples", "mods", "example-tones", "mod.json")


def base(**tracks_override):
    """A small valid manifest."""
    track = {"key": "intro-loop", "title": "Intro, then a loop", "file": "music/a.wav", "loop": {"start": 10, "end": 20}}
    track.update(tracks_override)
    return {"format": 1, "id": "test-mod", "name": "Test", "version": "1.0.0",
            "music": {"series": [{"key": "tones", "name": "TEST TONES", "order": 0, "tracks": [track]}]}}


def errors_of(document, **kw):
    return [p["message"] for p in mf.validate(document, **kw) if p["level"] == "error"]


def warnings_of(document, **kw):
    return [p["message"] for p in mf.validate(document, **kw) if p["level"] == "warning"]


class Keys(unittest.TestCase):
    def test_valid_key(self):
        for good in ("a", "example-tones", "ff15.battle_2", "0abc", "x" * 64):
            self.assertTrue(mf.valid_key(good), good)
        for bad in ("", "Abc", "-abc", ".abc", "_abc", "a b", "a/b", "a:b", "x" * 65, "café", None, 5):
            self.assertFalse(mf.valid_key(bad), bad)

    def test_id_number_matches_the_port(self):
        # docs/port/mod-folder.md section 2
        self.assertEqual(mf.id_number("example-tones:intro-loop"), 0x7D9EC9A2)
        self.assertEqual(mf.bgm_file_name("example-tones:intro-loop"), "MD7D9EC9A2.AT3")

    def test_key_from_text(self):
        self.assertEqual(mf.key_from_text("Blinded by Light"), "blinded-by-light")
        self.assertEqual(mf.key_from_text("FINAL FANTASY XV: Somnus!"), "final-fantasy-xv-somnus")
        self.assertEqual(mf.key_from_text("  Café Ünïcödé  "), "cafe-unicode")
        self.assertEqual(mf.key_from_text("!!!"), "track")
        self.assertEqual(mf.key_from_text("", fallback="series"), "series")
        self.assertEqual(mf.key_from_text("音楽"), "track")
        long = mf.key_from_text("word " * 40)
        self.assertLessEqual(len(long), 64)
        self.assertTrue(mf.valid_key(long))
        self.assertFalse(long.endswith("-"))

    def test_unique_key(self):
        self.assertEqual(mf.unique_key("battle", []), "battle")
        self.assertEqual(mf.unique_key("battle", ["battle"]), "battle-2")
        self.assertEqual(mf.unique_key("battle", ["battle", "battle-2"]), "battle-3")
        long = "x" * 64
        made = mf.unique_key(long, [long])
        self.assertEqual(len(made), 64)
        self.assertTrue(made.endswith("-2"))

    def test_unique_key_avoids_number_clashes(self):
        taken_number = mf.id_number("m:battle")
        self.assertEqual(mf.unique_key("battle", [], [taken_number], "m"), "battle-2")

    def test_series_display_name(self):
        self.assertEqual(mf.series_display_name("Final Fantasy  XV"), "FINAL FANTASY XV")


class Conversions(unittest.TestCase):
    def test_llround_halves_away_from_zero(self):
        self.assertEqual(mf.llround(2.5), 3)
        self.assertEqual(mf.llround(-2.5), -3)
        self.assertEqual(mf.llround(2.4999), 2)

    def test_seconds_and_samples(self):
        self.assertEqual(mf.seconds_to_samples(1.0, 44100), 44100)
        self.assertEqual(mf.seconds_to_samples(2.5, 48000), 120000)
        self.assertEqual(mf.seconds_to_samples(0.0000113, 44100), 0)
        self.assertAlmostEqual(mf.samples_to_seconds(88200, 44100), 2.0)
        for rate in (22050, 32000, 44100, 48000, 96000):
            for samples in (0, 1, 12345, 9_999_999):
                self.assertEqual(mf.seconds_to_samples(mf.samples_to_seconds(samples, rate), rate), samples)

    def test_port_frames(self):
        self.assertEqual(mf.port_frames(88200, 44100), 88200)
        self.assertEqual(mf.port_frames(48000, 48000), 44100)
        self.assertEqual(mf.port_frames(1, 48000), 1)  # 0.91875 rounds up
        self.assertEqual(mf.port_frames(24000, 48000), 22050)
        self.assertEqual(mf.port_frames_from_seconds(1.0), 44100)
        self.assertEqual(mf.port_frames_from_seconds(3.0), 132300)

    def test_format_and_parse_time(self):
        self.assertEqual(mf.format_time(88200, 44100), "0:02.000")
        self.assertEqual(mf.format_time(44100 * 83 + 20110, 44100), "1:23.456")
        self.assertEqual(mf.format_time(44100 * 3723, 44100), "1:02:03.000")
        self.assertAlmostEqual(mf.parse_time("1:23.456"), 83.456)
        self.assertAlmostEqual(mf.parse_time("83.5"), 83.5)
        self.assertAlmostEqual(mf.parse_time("1:01:23.5"), 3683.5)
        for bad in ("", "abc", "1:2:3:4", "-1", "1:75"):
            with self.assertRaises(ValueError):
                mf.parse_time(bad)


class Parsing(unittest.TestCase):
    def test_not_json(self):
        document, problems = mf.parse_manifest_text(b"{ not json")
        self.assertIsNone(document)
        self.assertIn("not valid JSON", problems[0]["message"])

    def test_duplicate_member_is_refused_like_the_port(self):
        document, problems = mf.parse_manifest_text(b'{"id": "a", "id": "b"}')
        self.assertIsNone(document)
        self.assertIn("appears twice", problems[0]["message"])

    def test_nan_and_bom(self):
        self.assertIsNone(mf.parse_manifest_text(b'{"format": NaN}')[0])
        document, problems = mf.parse_manifest_text(b'\xef\xbb\xbf{"id": "a"}')
        self.assertEqual(document, {"id": "a"})

    def test_too_large(self):
        self.assertIsNone(mf.parse_manifest_text(b" " * (mf.MAX_MANIFEST_BYTES + 1))[0])

    def test_too_deep(self):
        self.assertIsNone(mf.parse_manifest_text(b"[" * 70 + b"]" * 70)[0])


class Validation(unittest.TestCase):
    def test_example_mod_is_valid(self):
        with open(EXAMPLE, "rb") as handle:
            document, problems = mf.parse_manifest_text(handle.read())
        self.assertEqual(problems, [])
        self.assertEqual(errors_of(document), [])

    def test_valid(self):
        self.assertEqual(mf.validate(base()), [])

    # The cases in docs/port/mod-folder.md section 7 ("Validation").
    def test_bad_key(self):
        self.assertTrue(any("\"key\" must be lower-case" in e for e in errors_of(base(key="Bad Key"))))
        document = base()
        document["music"]["series"][0]["key"] = "Tones"
        self.assertTrue(any("\"key\" must be lower-case" in e for e in errors_of(document)))

    def test_wrong_type(self):
        self.assertTrue(any("must be a string, not a number" in e for e in errors_of(base(title=5))))
        document = base()
        document["music"]["series"][0]["tracks"] = {}
        self.assertTrue(any("\"tracks\" must be a list" in e for e in errors_of(document)))
        self.assertTrue(any("must be a string, not true or false" in e for e in errors_of(base(file=True))))

    def test_duplicate_track_key(self):
        document = base()
        document["music"]["series"].append({"key": "more", "name": "MORE", "tracks": [
            {"key": "intro-loop", "title": "Again", "file": "music/b.wav"}]})
        found = errors_of(document)
        self.assertTrue(any("appears twice in the mod" in e for e in found))

    def test_duplicate_series_key(self):
        document = base()
        document["music"]["series"].append(dict(document["music"]["series"][0], tracks=[
            {"key": "other", "title": "Other", "file": "music/b.wav"}]))
        self.assertTrue(any("appears twice; the second is left out" in e for e in errors_of(document)))

    def test_path_leaving_the_folder(self):
        for path in ("../a.wav", "music/../../a.wav", "/abs/a.wav", "C:/a.wav", "music\\a.wav"):
            self.assertTrue(errors_of(base(file=path)), path)
        self.assertTrue(any("must not leave" in e for e in errors_of(base(file="../a.wav"))))

    def test_missing_file(self):
        with tempfile.TemporaryDirectory() as folder:
            found = errors_of(base(), folder=folder)
            self.assertTrue(any("is not in the mod's folder" in e for e in found))
            os.makedirs(os.path.join(folder, "music"))
            with open(os.path.join(folder, "music", "a.wav"), "wb") as handle:
                handle.write(b"RIFF")
            self.assertEqual(errors_of(base(), folder=folder), [])

    def test_file_that_is_not_audio(self):
        self.assertTrue(any("must be .mp3, .ogg, .opus, .flac, .wav, .brstm, .bcstm or .bfstm, not \".txt\"" in e
                            for e in errors_of(base(file="music/a.txt"))))
        # The right extension, but FFmpeg cannot read it.
        found = errors_of(base(), info=lambda f: {"error": "not a file FFmpeg can read"})
        self.assertTrue(any("cannot be played" in e for e in found))

    def test_loop_that_ends_before_it_starts(self):
        self.assertTrue(any("\"end\" must be a whole number of samples after \"start\"" in e
                            for e in errors_of(base(loop={"start": 20, "end": 10}))))
        self.assertTrue(any("ends before it starts" in e
                            for e in errors_of(base(loop={"start_seconds": 3, "end_seconds": 1}))))
        self.assertTrue(errors_of(base(loop={"start": 10, "end": 10})))

    def test_track_without_title(self):
        document = base()
        del document["music"]["series"][0]["tracks"][0]["title"]
        found = errors_of(document)
        self.assertTrue(any("needs \"title\"" in e for e in found))
        self.assertTrue(any("1 to 64 bytes" in e for e in errors_of(base(title=""))))
        self.assertTrue(any("1 to 64 bytes" in e for e in errors_of(base(title="x" * 65))))

    def test_series_with_no_playable_track(self):
        found = errors_of(base(title=""))
        self.assertTrue(any("has no track that can be played" in e for e in found))
        document = base()
        document["music"]["series"][0]["tracks"] = []
        self.assertTrue(any("has no track that can be played" in e for e in errors_of(document)))

    def test_mod_fields(self):
        document = base()
        document["id"] = "Bad Id"
        self.assertTrue(errors_of(document))
        document = base()
        document["format"] = 2
        self.assertTrue(any("this build reads format 1" in e for e in errors_of(document)))
        document = base()
        document["author"] = ""  # the port refuses an empty optional text too
        self.assertTrue(any("\"author\" must be 1 to 64 bytes" in e for e in errors_of(document)))
        document = base()
        document["version"] = "v" * 33
        self.assertTrue(errors_of(document))
        document = base()
        del document["name"]
        self.assertTrue(any("needs \"name\"" in e for e in errors_of(document)))
        self.assertTrue(errors_of([]))

    def test_series_fields(self):
        document = base()
        document["music"]["series"][0]["name"] = "N" * 49
        self.assertTrue(any("1 to 48 bytes" in e for e in errors_of(document)))
        document = base()
        document["music"]["series"][0]["order"] = 2_000_000
        self.assertTrue(any("\"order\" must be a whole number" in e for e in errors_of(document)))
        document["music"]["series"][0]["order"] = 1.5
        self.assertTrue(errors_of(document))

    def test_loop_forms(self):
        for good in ("auto", "whole", "none", {"start": 0}, {"start": 5, "end": 6}, {"start": 4.0, "end": 8.0},
                     {"start_seconds": 1.0}, {"start_seconds": 0.5, "end_seconds": 2.25}):
            self.assertEqual(errors_of(base(loop=good)), [], good)
        for bad in ("loop", 5, [], {"end": 5}, {"start": -1}, {"start": 1.5}, {"start": "1"},
                    {"start": 0, "start_seconds": 1}, {"start_seconds": -1}, {"start_seconds": 4000},
                    {"end_seconds": 2}):
            self.assertTrue(errors_of(base(loop=bad)), bad)
        self.assertTrue(any("samples and seconds" in e for e in errors_of(base(loop={"start": 0, "end_seconds": 1}))))

    def test_loop_outside_the_file(self):
        info = lambda f: {"rate": 44100, "samples": 100}  # noqa: E731
        self.assertEqual(errors_of(base(loop={"start": 10, "end": 100}), info=info), [])
        self.assertTrue(any("outside the file" in e for e in errors_of(base(loop={"start": 10, "end": 101}), info=info)))
        self.assertTrue(errors_of(base(loop={"start": 99}), info=info))  # a loop of one frame
        self.assertTrue(errors_of(base(loop={"start_seconds": 1.0}), info=info))
        # 48 kHz: positions are converted to the port's 44.1 kHz frames first.
        info48 = lambda f: {"rate": 48000, "samples": 48000}  # noqa: E731
        self.assertEqual(errors_of(base(loop={"start": 0, "end": 48000}), info=info48), [])

    def test_warnings(self):
        document = base()
        document["extra"] = 1
        self.assertTrue(any("unknown member \"extra\"" in w for w in warnings_of(document)))
        self.assertEqual(errors_of(document), [])
        document = base()
        del document["music"]
        self.assertTrue(any("adds nothing" in w for w in warnings_of(document)))
        self.assertTrue(any("outside ASCII" in w for w in warnings_of(base(title="Café"))))

    def test_clashes_with_other_mods(self):
        others = [{"folder": "other", "id": "test-mod", "tracks": {}}]
        self.assertTrue(any("already has the id" in e for e in errors_of(base(), others=others)))
        others = [{"folder": "other", "id": "x", "tracks": {mf.id_number("test-mod:intro-loop"): "x:y"}}]
        self.assertTrue(any("same number" in e for e in errors_of(base(), others=others)))


class SharedCases(unittest.TestCase):
    """port/tests/manifest_cases.json, which the C++ tool's tests run against
    the game's own rules: the same problems, by level and path (and message
    where the case gives one)."""

    CASES = os.path.join(REPO, "port", "tests", "manifest_cases.json")

    def test_shared_cases(self):
        with open(self.CASES, encoding="utf-8") as handle:
            cases = json.load(handle)["cases"]
        self.assertTrue(cases)
        for case in cases:
            with self.subTest(case["name"]), tempfile.TemporaryDirectory() as folder:
                for file in case["files"]:
                    path = os.path.join(folder, *file.split("/"))
                    os.makedirs(os.path.dirname(path), exist_ok=True)
                    with open(path, "wb") as handle:
                        handle.write(b"\0")
                others = [{"folder": o["folder"], "id": o["id"], "tracks": {mf.id_number(t): t for t in o["tracks"]}}
                          for o in case.get("others", [])]
                if "text" in case:
                    document, problems = mf.parse_manifest_text(case["text"].encode("utf-8"))
                    if document is not None:
                        problems = mf.validate(document, folder, None, others)
                else:
                    problems = mf.validate(case["manifest"], folder, None, others)
                got = [(p["level"], p["path"]) + ((p["message"],) if "message" in want else ())
                       for p, want in zip(problems, case["problems"])]
                got += [(p["level"], p["path"], p["message"]) for p in problems[len(case["problems"]):]]
                expected = [(w["level"], w["path"]) + ((w["message"],) if "message" in w else ())
                            for w in case["problems"]]
                self.assertEqual(got, expected)


class Building(unittest.TestCase):
    MODEL = {
        "id": "my-music", "name": "My music", "version": "1.0.0", "author": " Me ", "description": "",
        "extra": {"homepage": "x"},
        "series": [{"key": "ff15", "name": "FINAL FANTASY XV", "order": 10, "extra": {}, "tracks": [
            {"key": "a", "title": "A", "file": "music/a.mp3", "loop": {"mode": "custom", "start": 1000, "end": 5000},
             "extra": {}},
            {"key": "b", "title": "B", "file": "music/b.mp3", "loop": {"mode": "custom", "start": 1000, "end": 9000},
             "extra": {}},
            {"key": "c", "title": "C", "file": "music/c.mp3", "loop": {"mode": "whole"}, "extra": {}},
            {"key": "d", "title": "D", "file": "music/d.mp3", "loop": {"mode": "none"}, "extra": {}},
        ]}],
    }

    def info(self, file):
        return {"rate": 44100, "samples": 9000}

    def test_build(self):
        document = mf.build_manifest(self.MODEL, self.info)
        self.assertEqual(list(document)[:5], ["format", "id", "name", "version", "author"])
        self.assertEqual(document["format"], 1)
        self.assertEqual(document["author"], "Me")
        self.assertNotIn("description", document)  # empty optional text is left out
        self.assertEqual(document["homepage"], "x")
        tracks = document["music"]["series"][0]["tracks"]
        self.assertEqual(tracks[0]["loop"], {"start": 1000, "end": 5000})
        self.assertEqual(tracks[1]["loop"], {"start": 1000})  # ends at the file's end: "end" left out
        self.assertEqual(tracks[2]["loop"], "whole")
        self.assertEqual(tracks[3]["loop"], "none")
        self.assertEqual(errors_of(document, info=self.info), [])

    def test_bytes_round_trip(self):
        document = mf.build_manifest(self.MODEL, self.info)
        data = mf.manifest_bytes(document)
        self.assertTrue(data.endswith(b"\n"))
        self.assertFalse(data.startswith(b"\xef\xbb\xbf"))
        self.assertEqual(json.loads(data.decode("utf-8")), document)

    def test_model_round_trip(self):
        document = mf.build_manifest(self.MODEL)
        model = mf.model_from_manifest(document)
        self.assertEqual(mf.build_manifest(model), document)

    def test_seconds_become_samples_when_the_rate_is_known(self):
        document = base(loop={"start_seconds": 1.5, "end_seconds": 3.0})
        model = mf.model_from_manifest(document, info=lambda f: {"rate": 48000})
        loop = model["series"][0]["tracks"][0]["loop"]
        self.assertEqual((loop["start"], loop["end"]), (72000, 144000))
        self.assertEqual(mf.build_manifest(model)["music"]["series"][0]["tracks"][0]["loop"],
                         {"start": 72000, "end": 144000})
        # Unknown rate: the seconds are kept as they are.
        model = mf.model_from_manifest(document)
        self.assertEqual(mf.build_manifest(model)["music"]["series"][0]["tracks"][0]["loop"],
                         {"start_seconds": 1.5, "end_seconds": 3.0})

    def test_unknown_mode(self):
        with self.assertRaises(ValueError):
            mf.build_loop({"mode": "sometimes"})


if __name__ == "__main__":
    unittest.main()
