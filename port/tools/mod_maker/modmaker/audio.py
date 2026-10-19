"""FFmpeg for the mod maker: finding the port's own ffmpeg/ffprobe, reading a
file's tags, and decoding it to a PCM WAV at the file's own rate.

The port decodes mod music with the same FFmpeg libraries (PortableKit
host/audio/host_audio.cpp): libavformat's demuxer and libavcodec's decoder,
with the packet time base set so the decoder drops the encoder delay and
padding an MP3's LAME/Xing header gives. The ffmpeg program does the same, so
sample N of the WAV made here is sample N the port counts loops in.
MIT licence; see port/README.md.
"""

from __future__ import annotations

import glob
import hashlib
import json
import os
import shutil
import struct
import subprocess
import tempfile
import threading
import time
from typing import Optional

TOOL_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPO_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(TOOL_DIR)))
EXE = ".exe" if os.name == "nt" else ""
CACHE_DIR = os.path.join(tempfile.gettempdir(), "d012-mod-maker")
CACHE_DAYS = 7

# Where the port's build keeps FFmpeg, most likely first.
SEARCH_PATTERNS = (
    "tools/local/port/build-d012-recomp/_deps/ffmpeg/*/bin",
    "tools/local/port/build-d012/_deps/ffmpeg/*/bin",
    "tools/local/port/*/_deps/ffmpeg/*/bin",
    "tools/local/port-deps/dl/ffmpeg/*/bin",
    "tools/local/port-deps/dl/ffmpeg/bin",
)


class FFmpegMissing(RuntimeError):
    pass


class FFmpeg:
    def __init__(self, ffmpeg: str, ffprobe: str, source: str):
        self.ffmpeg = ffmpeg
        self.ffprobe = ffprobe
        self.source = source  # how it was found
        self.version = self._version()

    def _version(self) -> str:
        try:
            out = subprocess.run([self.ffmpeg, "-hide_banner", "-version"], capture_output=True, text=True,
                                 timeout=20, creationflags=_NO_WINDOW).stdout
        except (OSError, subprocess.SubprocessError) as error:
            raise FFmpegMissing(f"{self.ffmpeg} does not run: {error}") from error
        first = out.splitlines()[0] if out else "?"
        return first.replace("ffmpeg version ", "").split(" Copyright")[0]

    def describe(self) -> dict:
        return {"ffmpeg": self.ffmpeg, "ffprobe": self.ffprobe, "version": self.version, "source": self.source}


_NO_WINDOW = getattr(subprocess, "CREATE_NO_WINDOW", 0)


def _pair(folder: str) -> Optional[tuple]:
    ffmpeg = os.path.join(folder, "ffmpeg" + EXE)
    ffprobe = os.path.join(folder, "ffprobe" + EXE)
    if os.path.isfile(ffmpeg) and os.path.isfile(ffprobe):
        return ffmpeg, ffprobe
    return None


def find_ffmpeg(explicit: Optional[str] = None, root: str = REPO_ROOT) -> FFmpeg:
    """The FFmpeg to decode with: --ffmpeg, then D012_FFMPEG, then the port's
    build output and FFmpeg download folder, then the PATH (with a warning,
    as that one may be another version)."""
    looked = []
    for given, how in ((explicit, "--ffmpeg"), (os.environ.get("D012_FFMPEG"), "D012_FFMPEG")):
        if not given:
            continue
        folder = given if os.path.isdir(given) else os.path.dirname(os.path.abspath(given))
        pair = _pair(folder)
        if pair is None:
            raise FFmpegMissing(f"{how} {given}: no ffmpeg{EXE} and ffprobe{EXE} there")
        return FFmpeg(pair[0], pair[1], how)
    for pattern in SEARCH_PATTERNS:
        full = os.path.join(root, *pattern.split("/"))
        looked.append(full)
        for folder in sorted(glob.glob(full)):
            pair = _pair(folder)
            if pair is not None:
                return FFmpeg(pair[0], pair[1], "the port's build")
    on_path = shutil.which("ffmpeg"), shutil.which("ffprobe")
    if on_path[0] and on_path[1]:
        return FFmpeg(on_path[0], on_path[1], "PATH (not necessarily the port's FFmpeg)")
    raise FFmpegMissing(
        "No ffmpeg" + EXE + " and ffprobe" + EXE + " found. The mod maker decodes with the port's own FFmpeg so "
        "loop points mean the same in the tool and the game. Build the port once (cmake unpacks FFmpeg into the "
        "build's _deps/ffmpeg), or pass --ffmpeg <path to ffmpeg" + EXE + ">. Looked in:\n  " + "\n  ".join(looked))


def probe(ff: FFmpeg, path: str) -> dict:
    """The file's audio stream and tags: {"codec", "rate", "channels",
    "duration", "tags": {lower-case name: value}, "error"}."""
    command = [ff.ffprobe, "-hide_banner", "-v", "error", "-select_streams", "a:0", "-show_entries",
               "stream=codec_name,sample_rate,channels,duration:stream_tags:format=duration,format_name:format_tags",
               "-of", "json", path]
    try:
        run = subprocess.run(command, capture_output=True, timeout=60, creationflags=_NO_WINDOW)
    except (OSError, subprocess.SubprocessError) as error:
        return {"error": f"ffprobe failed: {error}"}
    if run.returncode != 0:
        return {"error": run.stderr.decode("utf-8", "replace").strip() or "ffprobe cannot read it"}
    try:
        data = json.loads(run.stdout.decode("utf-8", "replace"))
    except ValueError:
        return {"error": "ffprobe gave no answer"}
    streams = data.get("streams") or []
    if not streams:
        return {"error": "no audio stream"}
    stream = streams[0]
    fmt = data.get("format") or {}
    tags = {}
    for source in (fmt.get("tags") or {}, stream.get("tags") or {}):
        for key, value in source.items():
            tags.setdefault(str(key).lower(), str(value))
    duration = stream.get("duration") or fmt.get("duration")
    return {"codec": stream.get("codec_name"), "rate": int(stream.get("sample_rate") or 0),
            "channels": int(stream.get("channels") or 0), "duration": float(duration) if duration else None,
            "container": fmt.get("format_name"), "tags": tags, "error": None}


def loop_tags(tags: dict) -> Optional[dict]:
    """The file's LOOPSTART with LOOPLENGTH or LOOPEND, as the port reads them for "auto"."""
    def integer(name):
        try:
            value = int(str(tags.get(name.lower(), "")).strip())
            return value if value >= 0 else None
        except ValueError:
            return None
    start = integer("LOOPSTART")
    if start is None:
        return None
    length, end = integer("LOOPLENGTH"), integer("LOOPEND")
    if length:
        return {"start": start, "end": start + length}
    return {"start": start, "end": end}


def wav_info(path: str) -> dict:
    """{"rate", "channels", "bits", "samples", "data_offset"} of a PCM WAV."""
    with open(path, "rb") as handle:
        head = handle.read(12)
        if len(head) < 12 or head[:4] != b"RIFF" or head[8:12] != b"WAVE":
            raise ValueError("not a WAV file")
        rate = channels = bits = None
        while True:
            chunk = handle.read(8)
            if len(chunk) < 8:
                raise ValueError("no data chunk")
            name, size = chunk[:4], struct.unpack("<I", chunk[4:])[0]
            if name == b"fmt ":
                body = handle.read(size)
                _, channels, rate, _, _, bits = struct.unpack("<HHIIHH", body[:16])
                if size & 1:
                    handle.read(1)
            elif name == b"data":
                offset = handle.tell()
                total = os.path.getsize(path) - offset
                if size and size != 0xFFFFFFFF:
                    total = min(total, size)
                frame = channels * bits // 8
                return {"rate": rate, "channels": channels, "bits": bits, "samples": total // frame,
                        "data_offset": offset}
            else:
                handle.seek(size + (size & 1), os.SEEK_CUR)


class Decoder:
    """Decodes files to WAV in a cache folder, once per file version."""

    def __init__(self, ff: FFmpeg, cache_dir: str = CACHE_DIR):
        self.ff = ff
        self.cache_dir = cache_dir
        os.makedirs(cache_dir, exist_ok=True)
        self._locks: dict = {}
        self._lock = threading.Lock()
        self._probes: dict = {}
        self._clean()

    def _clean(self):
        limit = time.time() - CACHE_DAYS * 86400
        for name in os.listdir(self.cache_dir):
            path = os.path.join(self.cache_dir, name)
            try:
                if os.path.getmtime(path) < limit:
                    os.remove(path)
            except OSError:
                pass

    def _key(self, path: str) -> str:
        stat = os.stat(path)
        text = f"{os.path.realpath(path)}|{stat.st_size}|{stat.st_mtime_ns}|{self.ff.ffmpeg}|1"
        return hashlib.sha1(text.encode("utf-8")).hexdigest()

    def probe(self, path: str) -> dict:
        key = self._key(path)
        if key not in self._probes:
            self._probes[key] = probe(self.ff, path)
        return self._probes[key]

    def cached_wav(self, path: str) -> Optional[str]:
        wav = os.path.join(self.cache_dir, self._key(path) + ".wav")
        return wav if os.path.isfile(wav) else None

    def wav(self, path: str) -> str:
        """The decoded WAV, decoding it now if need be. RuntimeError when FFmpeg cannot."""
        key = self._key(path)
        wav = os.path.join(self.cache_dir, key + ".wav")
        with self._lock:
            lock = self._locks.setdefault(key, threading.Lock())
        with lock:
            if os.path.isfile(wav):
                os.utime(wav)
                return wav
            details = self.probe(path)
            if details.get("error"):
                raise RuntimeError(details["error"])
            partial = wav + ".part"
            # The port mixes anything above two channels down to stereo; so does -ac 2.
            channels = ["-ac", "2"] if (details.get("channels") or 0) > 2 else []
            command = [self.ff.ffmpeg, "-hide_banner", "-nostdin", "-v", "error", "-y", "-i", path, "-vn", "-sn",
                       "-dn", "-map_metadata", "-1", *channels, "-c:a", "pcm_s16le", "-f", "wav", partial]
            run = subprocess.run(command, capture_output=True, timeout=600, creationflags=_NO_WINDOW)
            if run.returncode != 0 or not os.path.isfile(partial):
                try:
                    os.remove(partial)
                except OSError:
                    pass
                raise RuntimeError(run.stderr.decode("utf-8", "replace").strip() or "ffmpeg cannot decode it")
            os.replace(partial, wav)
            return wav

    def info(self, path: str, decode: bool = True) -> dict:
        """What the editor and the validator need about a file: rate,
        channels, samples (exact, from the decoded WAV), tags, loop tags."""
        details = dict(self.probe(path))
        if details.get("error"):
            return details
        wav = self.wav(path) if decode else self.cached_wav(path)
        if wav is not None:
            exact = wav_info(wav)
            details["samples"] = exact["samples"]
            # What the decoder gives (Opus is always 48 kHz), which is what
            # the port's loop samples count in.
            details["rate"] = exact["rate"]
            details["decoded_channels"] = exact["channels"]
        details["loop_tags"] = loop_tags(details.get("tags") or {})
        return details
