"""The local API: the token, the Host check, path safety, create/open/save
with backups, and (when the port's FFmpeg is there) decoding, including an
MP3's encoder delay.

    python -m unittest discover -s port/tools/mod_maker/tests
"""

import http.client
import json
import math
import os
import struct
import subprocess
import sys
import tempfile
import threading
import unittest
import wave

TOOL = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, TOOL)

from modmaker import audio, manifest as mf  # noqa: E402
from modmaker.server import TOKEN_HEADER, App, safe_file_name, serve  # noqa: E402


def read(path):
    with open(path, "rb") as handle:
        return handle.read()


def write_tone(path, seconds=1.0, rate=44100, freq=440.0, channels=1):
    frames = int(seconds * rate)
    with wave.open(path, "wb") as out:
        out.setnchannels(channels)
        out.setsampwidth(2)
        out.setframerate(rate)
        data = bytearray()
        for i in range(frames):
            value = int(8000 * math.sin(2 * math.pi * freq * i / rate))
            data += struct.pack("<h", value) * channels
        out.writeframes(bytes(data))
    return frames


class StubFF:
    ffmpeg = ffprobe = "stub"
    version = "stub"

    def describe(self):
        return {"ffmpeg": "stub", "ffprobe": "stub", "version": "stub", "source": "test"}


class StubDecoder:
    """Reads only WAV files, with the standard library; enough for the API tests."""

    def __init__(self):
        self.ff = StubFF()

    def info(self, path, decode=True):
        try:
            details = audio.wav_info(path)
        except (OSError, ValueError, struct.error) as error:
            return {"error": str(error)}
        return {"codec": "pcm_s16le", "rate": details["rate"], "channels": details["channels"],
                "samples": details["samples"], "tags": {}, "loop_tags": None, "error": None}

    def wav(self, path):
        audio.wav_info(path)
        return path


class ServerCase(unittest.TestCase):
    decoder_factory = StubDecoder

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = self.temp.name
        self.mods = os.path.join(self.root, "mods")
        os.makedirs(self.mods)
        # Something outside the mods folder that must stay out of reach.
        self.secret = os.path.join(self.root, "secret.wav")
        write_tone(self.secret, 0.1)
        self.app = App(self.mods, self.decoder_factory())
        self.server = serve(self.app)
        self.thread = threading.Thread(target=self.server.serve_forever, daemon=True)
        self.thread.start()

    def tearDown(self):
        self.server.shutdown()
        self.server.server_close()
        self.temp.cleanup()

    def request(self, method, path, body=None, token=True, host=None, headers=None, raw=None):
        connection = http.client.HTTPConnection("127.0.0.1", self.app.port, timeout=30)
        send = {"Host": host or f"127.0.0.1:{self.app.port}"}
        if token:
            send[TOKEN_HEADER] = self.app.token if token is True else token
        send.update(headers or {})
        data = raw if raw is not None else (json.dumps(body).encode() if body is not None else None)
        if data is not None:
            send["Content-Length"] = str(len(data))
        connection.request(method, path, body=data, headers=send)
        response = connection.getresponse()
        payload = response.read()
        connection.close()
        kind = response.getheader("Content-Type") or ""
        return response.status, (json.loads(payload) if "json" in kind else payload)

    def create(self, mod_id="tones"):
        status, result = self.request("POST", "/api/mod/create", {"id": mod_id, "name": "Tones", "version": "1.0.0"})
        self.assertEqual(status, 200, result)
        self.assertTrue(result["ok"], result)
        return os.path.join(self.mods, mod_id)

    def model(self, mod_id="tones", file="music/a.wav", loop=None):
        return {"id": mod_id, "name": "Tones", "version": "1.0.0", "author": "", "description": "", "extra": {},
                "series": [{"key": "tones", "name": "TEST TONES", "order": 0, "extra": {}, "tracks": [
                    {"key": "a", "title": "A", "file": file, "extra": {},
                     "loop": loop or {"mode": "custom", "start": 100, "end": 2000}}]}]}


class Security(ServerCase):
    def test_token_is_required(self):
        self.assertEqual(self.request("POST", "/api/state", {}, token=False)[0], 403)
        self.assertEqual(self.request("POST", "/api/state", {}, token="wrong")[0], 403)
        self.assertEqual(self.request("POST", "/api/state", {})[0], 200)
        self.assertEqual(self.request("GET", "/api/track/audio?folder=x&file=music/a.wav", token=False)[0], 403)

    def test_foreign_host_and_origin_are_refused(self):
        # DNS rebinding: a page on another name that resolves here.
        self.assertEqual(self.request("POST", "/api/state", {}, host="evil.example:80")[0], 403)
        self.assertEqual(self.request("GET", "/", host="evil.example")[0], 403)
        status, _ = self.request("POST", "/api/state", {}, headers={"Origin": "http://evil.example"})
        self.assertEqual(status, 403)
        status, _ = self.request("POST", "/api/state", {}, headers={"Origin": f"http://127.0.0.1:{self.app.port}"})
        self.assertEqual(status, 200)

    def test_only_its_own_static_files(self):
        self.assertEqual(self.request("GET", "/", token=False)[0], 200)
        self.assertEqual(self.request("GET", "/static/app.js", token=False)[0], 200)
        for path in ("/static/../modmaker/server.py", "/static/..%2fmod_maker.py", "/static/x.js", "/mod_maker.py"):
            self.assertEqual(self.request("GET", path, token=False)[0], 404, path)

    def test_bad_mod_folders(self):
        for folder in ("", ".", "..", "../mods", "a/b", "a\\b", "C:\\Windows", "/etc", "x\x00y", "name.", " x"):
            status, result = self.request("POST", "/api/mod/open", {"folder": folder})
            self.assertIn(status, (400, 403), folder)
        status, _ = self.request("POST", "/api/mod/open", {"folder": "missing"})
        self.assertEqual(status, 404)

    def test_bad_file_paths(self):
        self.create()
        for file in ("../../secret.wav", "music/../../../secret.wav", "/secret.wav", "C:/secret.wav",
                     "music\\..\\..\\..\\secret.wav", "../mod.json", "music/a.exe"):
            status, _ = self.request("POST", "/api/track/info", {"folder": "tones", "file": file})
            self.assertIn(status, (400, 403), file)
            status, _ = self.request("GET", f"/api/track/audio?folder=tones&file={file}")
            self.assertIn(status, (400, 403), file)
        status, _ = self.request("GET", "/api/track/audio?folder=..&file=secret.wav")
        self.assertIn(status, (400, 403))

    def test_link_out_of_the_folder_is_refused(self):
        folder = self.create()
        link = os.path.join(folder, "music", "link.wav")
        try:
            os.symlink(self.secret, link)
        except (OSError, NotImplementedError):
            self.skipTest("cannot make a symbolic link here")
        status, _ = self.request("POST", "/api/track/info", {"folder": "tones", "file": "music/link.wav"})
        self.assertEqual(status, 403)
        _, result = self.request("POST", "/api/mod/save", {"folder": "tones", "model": self.model(file="music/link.wav")})
        self.assertFalse(result["ok"])

    def test_import_keeps_files_inside_music(self):
        self.create()
        data = read(self.secret)
        for name in ("..\\..\\evil.wav", "../../evil.wav", "/abs/evil.wav", "C:\\evil.wav"):
            status, result = self.request("POST", f"/api/track/import?folder=tones&name={name}", raw=data,
                                          headers={"Content-Type": "application/octet-stream"})
            self.assertEqual(status, 200, result)
            self.assertTrue(result["file"].startswith("music/evil"), result)
            self.assertNotIn("..", result["file"])
        self.assertFalse(os.path.exists(os.path.join(self.root, "evil.wav")))
        self.assertEqual(self.request("POST", "/api/track/import?folder=tones&name=x.exe", raw=data)[0], 400)
        self.assertEqual(self.request("POST", "/api/track/import?folder=..&name=x.wav", raw=data)[0], 400)

    def test_safe_file_name(self):
        self.assertEqual(safe_file_name("../a b/Ça va.MP3"), "Ca va.mp3")
        self.assertEqual(safe_file_name("x:y*z?.wav"), "x_y_z.wav")
        self.assertEqual(safe_file_name("...wav.wav"), "wav.wav")


class Workflow(ServerCase):
    def test_create_import_save_backup(self):
        folder = self.create()
        with self.assertRaises(Exception):
            self.create()  # the folder exists
        data = read(self.secret)
        status, result = self.request("POST", "/api/track/import?folder=tones&name=a.wav", raw=data)
        self.assertEqual((status, result["file"]), (200, "music/a.wav"))
        self.assertEqual(result["info"]["samples"], 4410)
        # The originals are untouched; the copy is the same bytes.
        self.assertEqual(read(os.path.join(folder, "music", "a.wav")), data)
        status, result = self.request("POST", "/api/track/import?folder=tones&name=a.wav", raw=data)
        self.assertEqual(result["file"], "music/a-2.wav")

        _, result = self.request("POST", "/api/mod/save", {"folder": "tones", "model": self.model()})
        self.assertTrue(result["ok"], result)
        self.assertTrue(result["backup"].startswith(".backups/mod-"))
        with open(os.path.join(folder, "mod.json"), "rb") as handle:
            document, _ = mf.parse_manifest_text(handle.read())
        self.assertEqual(document["music"]["series"][0]["tracks"][0]["loop"], {"start": 100, "end": 2000})

        # An invalid manifest is never written.
        before = read(os.path.join(folder, "mod.json"))
        _, result = self.request("POST", "/api/mod/save", {"folder": "tones", "model": self.model(
            loop={"mode": "custom", "start": 100, "end": 99999})})
        self.assertFalse(result["ok"])
        self.assertTrue(any("outside the file" in p["message"] for p in result["problems"]))
        self.assertEqual(read(os.path.join(folder, "mod.json")), before)
        _, result = self.request("POST", "/api/mod/save", {"folder": "tones", "model": self.model(file="music/none.wav")})
        self.assertFalse(result["ok"])

        # Open reads it back.
        _, result = self.request("POST", "/api/mod/open", {"folder": "tones"})
        self.assertEqual(result["model"]["series"][0]["tracks"][0]["loop"]["start"], 100)
        self.assertEqual([p for p in result["problems"] if p["level"] == "error"], [])

    def test_create_refuses_a_bad_id(self):
        _, result = self.request("POST", "/api/mod/create", {"id": "Bad Id", "name": "x", "version": "1"})
        self.assertFalse(result["ok"])
        self.assertFalse(os.listdir(self.mods))

    def test_keys(self):
        _, result = self.request("POST", "/api/key", {"kind": "track", "text": "Blinded by Light", "taken": ["blinded-by-light"],
                                                      "mod_id": "tones"})
        self.assertEqual(result["key"], "blinded-by-light-2")

    def test_port_commands(self):
        _, result = self.request("POST", "/api/port/commands", {"track_id": "tones:a"})
        self.assertIn("D012_TEST_BGM_START = 'tones:a'", result["powershell"])
        self.assertIn("-DataDir", result["powershell"])


FF = None
try:
    FF = audio.find_ffmpeg()
except audio.FFmpegMissing:
    pass


@unittest.skipIf(FF is None, "no FFmpeg found")
class Decoding(unittest.TestCase):
    def test_wav_decodes_to_the_same_samples(self):
        with tempfile.TemporaryDirectory() as folder:
            source = os.path.join(folder, "a.wav")
            frames = write_tone(source, 0.5, 48000, channels=2)
            decoder = audio.Decoder(FF, os.path.join(folder, "cache"))
            info = decoder.info(source)
            self.assertEqual((info["rate"], info["channels"], info["samples"]), (48000, 2, frames))
            self.assertEqual(read(decoder.wav(source))[-400:], read(source)[-400:])

    def test_mp3_encoder_delay_is_removed(self):
        """An MP3 decodes to the original's length and timing: FFmpeg drops the
        encoder delay and padding from the LAME header, so sample N of the
        decoded file is sample N of what was encoded."""
        with tempfile.TemporaryDirectory() as folder:
            source = os.path.join(folder, "a.wav")
            rate = 44100
            # 440 Hz then 660 Hz from sample 44100: a change the lag shows at.
            samples = [int(8000 * math.sin(2 * math.pi * (440 if i < rate else 660) * i / rate)) for i in range(2 * rate)]
            with wave.open(source, "wb") as out:
                out.setnchannels(1); out.setsampwidth(2); out.setframerate(rate)
                out.writeframes(b"".join(struct.pack("<h", v) for v in samples))
            mp3 = os.path.join(folder, "a.mp3")
            run = subprocess.run([FF.ffmpeg, "-hide_banner", "-v", "error", "-y", "-i", source, "-c:a", "libmp3lame",
                                  "-b:a", "192k", mp3], capture_output=True)
            if run.returncode != 0:
                self.skipTest("this FFmpeg has no MP3 encoder")
            decoder = audio.Decoder(FF, os.path.join(folder, "cache"))
            info = decoder.info(mp3)
            self.assertEqual(info["samples"], len(samples))
            with wave.open(decoder.wav(mp3)) as handle:
                decoded = struct.unpack(f"<{handle.getnframes()}h", handle.readframes(handle.getnframes()))

            def error(lag):
                return sum((samples[i] - decoded[i + lag]) ** 2 for i in range(rate - 1024, rate + 1024))
            best = min(range(-1200, 1201, 1), key=error)
            self.assertEqual(best, 0)


if __name__ == "__main__":
    unittest.main()
