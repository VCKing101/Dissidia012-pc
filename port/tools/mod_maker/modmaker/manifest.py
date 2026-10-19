"""mod.json as the port reads it.

The rules are port/host/d012_mods.cpp's (read_mod, read_loop, track_file) and
the JSON reader's (port/host/d012_json.cpp); the format is
docs/port/mod-folder.md. Where the port leaves a broken part out and carries
on, this module reports an error, because the mod maker never writes a
manifest the port would read differently from what the modder set.

Loop positions are samples of the file at its own rate, as decoded by FFmpeg
(encoder delay and padding already removed). The port converts them to
frames at 44,100 Hz with llround(samples * 44100 / rate); port_frames() is
that conversion. MIT licence; see port/README.md.
"""

from __future__ import annotations

import json
import math
import os
import re
import unicodedata
from typing import Any, Callable, Iterable, Optional

FORMAT = 1
HOST_RATE = 44_100
MAX_MANIFEST_BYTES = 1 << 20
MAX_KEY_LENGTH = 64
MAX_JSON_DEPTH = 64
AUDIO_EXTENSIONS = (".mp3", ".ogg", ".oga", ".opus", ".flac", ".wav")
# The port's limit on a track's length (host_audio.cpp kMaxFrames).
MAX_TRACK_SECONDS = 30 * 60

MOD_MEMBERS = ("format", "id", "name", "version", "author", "description", "music")
MUSIC_MEMBERS = ("series",)
SERIES_MEMBERS = ("key", "name", "order", "tracks")
TRACK_MEMBERS = ("key", "title", "file", "loop")
LOOP_MEMBERS = ("start", "end", "start_seconds", "end_seconds")
LOOP_MODES = ("auto", "whole", "none", "custom")

_KEY_CHARS = re.compile(r"^[a-z0-9._-]+$")


# --- Keys, ids and numbers -------------------------------------------------

def valid_key(key: Any) -> bool:
    """A mod id, series key or track key: 1 to 64 of a-z 0-9 . _ -, starting with a letter or digit."""
    if not isinstance(key, str) or not key or len(key.encode("utf-8")) > MAX_KEY_LENGTH:
        return False
    return bool(_KEY_CHARS.match(key)) and key[0].isascii() and key[0].isalnum()


def id_number(text: str) -> int:
    """The 32-bit FNV-1a hash of an id's UTF-8 text (d012::mods::id_number)."""
    value = 0x811C9DC5
    for byte in text.encode("utf-8"):
        value ^= byte
        value = (value * 0x01000193) & 0xFFFFFFFF
    return value


def bgm_file_name(track_id: str) -> str:
    """The stand-in file the game opens for a track (d012::mods::bgm_path)."""
    return "MD%08X.AT3" % id_number(track_id)


def ascii_fold(text: str) -> str:
    """`text` with accents removed and anything else outside ASCII dropped."""
    return unicodedata.normalize("NFKD", text).encode("ascii", "ignore").decode("ascii")


def key_from_text(text: str, fallback: str = "track") -> str:
    """A valid key made from a title or name: "Blinded by Light!" -> "blinded-by-light"."""
    folded = ascii_fold(text or "").lower()
    key = re.sub(r"[^a-z0-9]+", "-", folded).strip("-._")
    key = key[:MAX_KEY_LENGTH].rstrip("-._")
    if not key or not key[0].isalnum():
        key = fallback
    return key


def unique_key(base: str, taken: Iterable[str], numbers: Optional[Iterable[int]] = None,
               mod_id: Optional[str] = None) -> str:
    """`base`, or `base-2`, `base-3`... so that it is not in `taken` (and, with
    `mod_id`, so that the track's number does not clash with `numbers`)."""
    taken = set(taken)
    clash = set(numbers or ())
    candidate = base
    counter = 1
    while True:
        number_free = mod_id is None or id_number(f"{mod_id}:{candidate}") not in clash
        if candidate not in taken and number_free and valid_key(candidate):
            return candidate
        counter += 1
        suffix = f"-{counter}"
        candidate = base[: MAX_KEY_LENGTH - len(suffix)].rstrip("-._") + suffix


def series_display_name(text: str) -> str:
    """The game's style for series names: upper case ("Final Fantasy XV" -> "FINAL FANTASY XV")."""
    return " ".join((text or "").split()).upper()


def byte_length(text: str) -> int:
    return len(text.encode("utf-8"))


def clip_bytes(text: str, limit: int) -> str:
    """`text` cut to at most `limit` UTF-8 bytes, never in the middle of a character."""
    data = text.encode("utf-8")[:limit]
    return data.decode("utf-8", "ignore").rstrip()


# --- Samples and seconds ---------------------------------------------------

def llround(value: float) -> int:
    """C's llround: to the nearest whole number, halves away from zero."""
    return int(math.floor(value + 0.5)) if value >= 0 else -int(math.floor(-value + 0.5))


def seconds_to_samples(seconds: float, rate: int) -> int:
    return llround(seconds * rate)


def samples_to_seconds(samples: int, rate: int) -> float:
    return samples / rate


def port_frames(samples: int, rate: int) -> int:
    """Where the port puts a sample of the file, in its 44,100 Hz frames (host_audio.cpp to_frames)."""
    if rate <= 0:
        return samples
    return llround(samples * HOST_RATE / rate)


def port_frames_from_seconds(seconds: float) -> int:
    """The same for a loop given in seconds: the port keeps microseconds, then frames."""
    micros = llround(seconds * 1e6)
    return llround(micros * HOST_RATE / 1e6)


def format_time(samples: int, rate: int) -> str:
    """m:ss.mmm (h:mm:ss.mmm past an hour)."""
    millis = llround(samples * 1000 / rate) if rate > 0 else 0
    hours, rest = divmod(millis, 3_600_000)
    minutes, rest = divmod(rest, 60_000)
    seconds, millis = divmod(rest, 1000)
    if hours:
        return f"{hours}:{minutes:02d}:{seconds:02d}.{millis:03d}"
    return f"{minutes}:{seconds:02d}.{millis:03d}"


def parse_time(text: str) -> float:
    """Seconds from "83.5", "1:23.5" or "1:01:23.5". ValueError when it is not a time."""
    text = text.strip()
    if not re.fullmatch(r"\d+(:[0-5]?\d){0,2}(\.\d*)?", text):
        raise ValueError(f"not a time: {text!r}")
    parts = text.split(":")
    seconds = float(parts[-1])
    for index, part in enumerate(reversed(parts[:-1])):
        seconds += int(part) * (60 if index == 0 else 3600)
    return seconds


# --- Parsing ---------------------------------------------------------------

def problem(level: str, path: str, message: str) -> dict:
    return {"level": level, "path": path, "message": message}


class _Duplicate(ValueError):
    pass


def _pairs(pairs):
    seen = {}
    for name, value in pairs:
        if name in seen:
            raise _Duplicate(f"member \"{name}\" appears twice")
        seen[name] = value
    return seen


def _no_constant(name):
    raise ValueError(f"{name} is not JSON")


def _depth(value, level=0) -> int:
    if level > MAX_JSON_DEPTH:
        return level
    if isinstance(value, dict):
        return max([_depth(v, level + 1) for v in value.values()] or [level])
    if isinstance(value, list):
        return max([_depth(v, level + 1) for v in value] or [level])
    return level


def parse_manifest_text(data: bytes) -> tuple[Optional[Any], list]:
    """The manifest's JSON, as the port's reader takes it (UTF-8, an optional
    BOM, no duplicate members, no NaN), or None and the problem."""
    if len(data) > MAX_MANIFEST_BYTES:
        return None, [problem("error", "mod", "larger than 1 MiB")]
    if data.startswith(b"\xef\xbb\xbf"):
        data = data[3:]
    try:
        text = data.decode("utf-8")
    except UnicodeDecodeError as error:
        return None, [problem("error", "mod", f"not valid JSON: not UTF-8 ({error})")]
    try:
        document = json.loads(text, object_pairs_hook=_pairs, parse_constant=_no_constant)
    except _Duplicate as error:
        return None, [problem("error", "mod", f"not valid JSON: {error}")]
    except ValueError as error:
        return None, [problem("error", "mod", f"not valid JSON: {error}")]
    if _depth(document) > MAX_JSON_DEPTH:
        return None, [problem("error", "mod", "not valid JSON: nested too deeply")]
    return document, []


# --- Validation ------------------------------------------------------------

def _kind(value: Any) -> str:
    if value is None:
        return "null"
    if isinstance(value, bool):
        return "true or false"
    if isinstance(value, (int, float)):
        return "a number"
    if isinstance(value, str):
        return "a string"
    if isinstance(value, list):
        return "a list"
    if isinstance(value, dict):
        return "an object"
    return "?"


def _is_number(value: Any) -> bool:
    return isinstance(value, (int, float)) and not isinstance(value, bool)


def whole_number(value: Any) -> Optional[int]:
    """The port's whole_number: a finite number with no fraction, at most 9e15."""
    if not _is_number(value) or not math.isfinite(value) or value != math.floor(value) or abs(value) > 9.0e15:
        return None
    return int(value)


class _Report:
    def __init__(self):
        self.items: list = []

    def error(self, path, message):
        self.items.append(problem("error", path, message))

    def warning(self, path, message):
        self.items.append(problem("warning", path, message))

    @property
    def errors(self) -> int:
        return sum(1 for item in self.items if item["level"] == "error")


def _member(obj: dict, name: str, kinds: tuple, kind_label: str, required: bool, report: _Report, path: str,
            context: str):
    if name not in obj:
        if required:
            report.error(path, f"{context} needs \"{name}\"")
        return None
    value = obj[name]
    ok = (_is_number(value) if kinds == ("number",) else isinstance(value, kinds) and not isinstance(value, bool))
    if not ok:
        report.error(f"{path}.{name}", f"{context} \"{name}\" must be {kind_label}, not {_kind(value)}")
        return None
    return value


def _text(obj: dict, name: str, required: bool, report: _Report, path: str, context: str,
          limit: int = MAX_KEY_LENGTH) -> Optional[str]:
    value = _member(obj, name, (str,), "a string", required, report, path, context)
    if value is None:
        return None
    if not value or byte_length(value) > limit:
        report.error(f"{path}.{name}", f"{context} \"{name}\" must be 1 to {limit} bytes long")
        return None
    return value


def _unknown(obj: dict, known: tuple, report: _Report, path: str, context: str):
    for name in obj:
        if name not in known:
            report.warning(f"{path}.{name}", f"{context} has an unknown member \"{name}\", which is ignored")


def check_file_text(text: str) -> Optional[str]:
    """Why a track's "file" text is not a path the port accepts, or None."""
    if not text:
        return "\"file\" must be a path inside the mod's folder"
    if "\\" in text:
        return "\"file\" must use / between folders (a \\ only works on Windows)"
    if text.startswith("/") or re.match(r"^[A-Za-z]:", text):
        return "\"file\" must be a path inside the mod's folder"
    if ".." in text.split("/"):
        return "\"file\" must not leave the mod's folder (\"..\")"
    name = text.rsplit("/", 1)[-1]
    extension = os.path.splitext(name)[1].lower()
    if extension not in AUDIO_EXTENSIONS:
        return f"\"file\" must be .mp3, .ogg, .opus, .flac or .wav, not \"{extension}\""
    return None


def inside(folder: str, path: str) -> bool:
    """True when `path` is `folder` or below it, after resolving links."""
    base = os.path.realpath(folder)
    target = os.path.realpath(path)
    try:
        return os.path.commonpath([base, target]) == base
    except ValueError:  # another drive
        return False


def _loop_check(loop: Any, report: _Report, path: str, context: str) -> Optional[dict]:
    """The port's read_loop. Returns {"mode", "start", "end", "unit"} or None when it is refused."""
    if loop is None:
        return {"mode": "auto"}
    if isinstance(loop, str):
        if loop in ("auto", "whole", "none"):
            return {"mode": loop}
        report.error(path, f"{context} \"loop\" must be \"auto\", \"whole\", \"none\" or an object")
        return None
    if not isinstance(loop, dict):
        report.error(path, f"{context} \"loop\" must be \"auto\", \"whole\", \"none\" or an object")
        return None
    _unknown(loop, LOOP_MEMBERS, report, path, f"{context} \"loop\"")
    has_samples = "start" in loop or "end" in loop
    has_seconds = "start_seconds" in loop or "end_seconds" in loop
    if has_samples and has_seconds:
        report.error(path, f"{context} \"loop\" gives samples and seconds; use one or the other")
        return None
    if has_seconds:
        def seconds(name):
            if name not in loop:
                return None
            value = loop[name]
            if not _is_number(value) or not math.isfinite(value) or value < 0.0 or value > 3600.0:
                report.error(f"{path}.{name}", f"{context} \"loop\" \"{name}\" must be a number of seconds from 0 to 3600")
                return -1
            return llround(value * 1e6)
        first = seconds("start_seconds")
        last = seconds("end_seconds")
        if first is None:
            report.error(path, f"{context} \"loop\" needs \"start_seconds\"")
            return None
        if first < 0 or (last is not None and last < 0):
            return None
        if last is not None and last <= first:
            report.error(f"{path}.end_seconds", f"{context} \"loop\" ends before it starts")
            return None
        return {"mode": "custom", "unit": "seconds", "start": loop["start_seconds"], "end": loop.get("end_seconds")}
    if "start" not in loop:
        report.error(path, f"{context} \"loop\" needs \"start\" (a sample of the file) or \"start_seconds\"")
        return None
    first = whole_number(loop["start"])
    last = whole_number(loop["end"]) if "end" in loop else None
    if first is None or first < 0:
        report.error(f"{path}.start", f"{context} \"loop\" \"start\" must be a whole number of samples, 0 or more")
        return None
    if "end" in loop and (last is None or last <= first):
        report.error(f"{path}.end", f"{context} \"loop\" \"end\" must be a whole number of samples after \"start\"")
        return None
    return {"mode": "custom", "unit": "samples", "start": first, "end": last}


def loop_range_problem(loop: dict, rate: int, samples: int) -> Optional[str]:
    """Why the port would not take a loop (and would loop the whole track
    instead), from host_audio.cpp resolve_loop; None when it would."""
    if loop.get("mode") != "custom" or rate <= 0 or samples <= 0:
        return None
    frames = port_frames(samples, rate)
    if loop.get("unit") == "seconds":
        first = port_frames_from_seconds(loop["start"])
        last = port_frames_from_seconds(loop["end"]) - 1 if loop.get("end") is not None else frames - 1
    else:
        first = port_frames(loop["start"], rate)
        last = port_frames(loop["end"], rate) - 1 if loop.get("end") is not None else frames - 1
    if 0 <= first < last < frames:
        return None
    length = format_time(samples, rate)
    return (f"the loop is outside the file ({samples} samples, {length}); the port would loop the whole file "
            f"instead")


def validate(document: Any, folder: Optional[str] = None,
             info: Optional[Callable[[str], Optional[dict]]] = None,
             others: Optional[list] = None) -> list:
    """Every problem the port would log for this manifest, plus the mod
    maker's own checks. Errors are what the port would leave out.

    folder: the mod's folder, for the file checks (None: skip them).
    info(file_text): {"rate", "samples", "error"} of a track's file, or None
        when not known yet (the loop range is then not checked).
    others: the other mods in the mods folder, [{"folder", "id", "tracks":
        {number: track id}}], for the clashes the port checks across mods.
    """
    report = _Report()
    if not isinstance(document, dict):
        report.error("mod", "the manifest must be an object")
        return report.items
    root = document
    _unknown(root, MOD_MEMBERS, report, "mod", "the mod")
    if "format" in root:
        fmt = _member(root, "format", ("number",), "a number", False, report, "mod", "the mod")
        if fmt is not None and whole_number(fmt) != FORMAT:
            report.error("mod.format", f"\"format\" is {fmt}; this build reads format {FORMAT}")
            return report.items
    mod_id = _text(root, "id", True, report, "mod", "the mod")
    if mod_id is not None and not valid_key(mod_id):
        report.error("mod.id", "\"id\" must be lower-case letters, digits, '.', '_' or '-', starting with a letter or digit")
        return report.items
    name = _text(root, "name", True, report, "mod", "the mod")
    version = _text(root, "version", True, report, "mod", "the mod", 32)
    _text(root, "author", False, report, "mod", "the mod")
    _text(root, "description", False, report, "mod", "the mod", 1024)
    if mod_id is None or name is None or version is None:
        return report.items
    for other in others or ():
        if other.get("id") == mod_id:
            report.error("mod.id", f"the folder \"{other.get('folder')}\" already has the id \"{mod_id}\"; the port "
                                   "would leave one of them out")

    music = _member(root, "music", (dict,), "an object", False, report, "mod", "the mod")
    if music is None:
        if "music" not in root:
            report.warning("mod", "the mod adds nothing this build knows (no \"music\")")
        return report.items
    _unknown(music, MUSIC_MEMBERS, report, "music", "\"music\"")
    series_list = _member(music, "series", (list,), "a list", True, report, "music", "\"music\"")
    if series_list is None:
        return report.items
    other_numbers = {}
    for other in others or ():
        other_numbers.update(other.get("tracks", {}))
    series_keys: set = set()
    track_keys: set = set()
    numbers: dict = {}
    for s_index, entry in enumerate(series_list):
        s_path = f"series.{s_index}"
        if not isinstance(entry, dict):
            report.error(s_path, "each series must be an object")
            continue
        key = _text(entry, "key", True, report, s_path, "a series")
        context = f"series \"{key or '?'}\""
        _unknown(entry, SERIES_MEMBERS, report, s_path, context)
        series_name = _text(entry, "name", True, report, s_path, context, 48)
        if key is None or series_name is None:
            report.error(s_path, f"{context} is left out")
            continue
        if not valid_key(key):
            report.error(f"{s_path}.key", f"{context}: \"key\" must be lower-case letters, digits, '.', '_' or '-'")
            continue
        if key in series_keys:
            report.error(f"{s_path}.key", f"{context} appears twice; the second is left out")
            continue
        series_keys.add(key)
        if "order" in entry:
            order = _member(entry, "order", ("number",), "a number", False, report, s_path, context)
            if order is not None:
                value = whole_number(order)
                if value is None or value < -1_000_000 or value > 1_000_000:
                    report.error(f"{s_path}.order", f"{context}: \"order\" must be a whole number from -1000000 to "
                                                    "1000000; the port would use 0")
        tracks = _member(entry, "tracks", (list,), "a list", True, report, s_path, context)
        if tracks is None:
            continue
        playable = 0
        for t_index, item in enumerate(tracks):
            t_path = f"{s_path}.tracks.{t_index}"
            if not isinstance(item, dict):
                report.error(t_path, f"{context}: each track must be an object")
                continue
            track_key = _text(item, "key", True, report, t_path, f"{context}, a track")
            t_context = f"track \"{track_key or '?'}\""
            _unknown(item, TRACK_MEMBERS, report, t_path, t_context)
            title = _text(item, "title", True, report, t_path, t_context)
            file_text = _member(item, "file", (str,), "a string", True, report, t_path, t_context)
            if track_key is None or title is None or file_text is None:
                report.error(t_path, f"{t_context} is left out")
                continue
            if not valid_key(track_key):
                report.error(f"{t_path}.key", f"{t_context}: \"key\" must be lower-case letters, digits, '.', '_' or '-'")
                continue
            if track_key in track_keys:
                report.error(f"{t_path}.key", f"{t_context} appears twice in the mod; the second is left out")
                continue
            track_keys.add(track_key)
            ok = True
            why = check_file_text(file_text)
            if why is not None:
                report.error(f"{t_path}.file", f"{t_context} {why}")
                ok = False
            elif folder is not None:
                full = os.path.join(folder, *file_text.split("/"))
                if not os.path.isfile(full):
                    report.error(f"{t_path}.file", f"{t_context} \"file\" {file_text} is not in the mod's folder")
                    ok = False
                elif not inside(folder, full):
                    report.error(f"{t_path}.file", f"{t_context} \"file\" {file_text} leads outside the mod's folder")
                    ok = False
            loop = _loop_check(item.get("loop"), report, f"{t_path}.loop", t_context)
            if loop is None:
                ok = False
            if not ok:
                report.error(t_path, f"{t_context} is left out")
                continue
            track_id = f"{mod_id}:{track_key}"
            number = id_number(track_id)
            if number in numbers:
                report.error(f"{t_path}.key", f"{t_context} has the same number as {numbers[number]}; change one key")
                continue
            if number in other_numbers:
                report.error(f"{t_path}.key", f"{t_context} has the same number as {other_numbers[number]} of another "
                                              "mod; change the key")
                continue
            numbers[number] = track_id
            details = info(file_text) if info is not None else None
            if details:
                if details.get("error"):
                    report.error(f"{t_path}.file", f"{t_context} ({file_text}) cannot be played: {details['error']}")
                    report.error(t_path, f"{t_context} is left out")
                    continue
                rate, samples = int(details.get("rate") or 0), int(details.get("samples") or 0)
                if rate and samples and samples > rate * MAX_TRACK_SECONDS:
                    report.error(f"{t_path}.file", f"{t_context} is longer than 30 minutes")
                    continue
                why = loop_range_problem(loop, rate, samples)
                if why is not None:
                    report.error(f"{t_path}.loop", f"{t_context}: {why}")
                    continue
            if not title.isascii():
                report.warning(f"{t_path}.title", f"{t_context}: the game's band shows a character outside ASCII as ?")
            playable += 1
        if playable == 0:
            report.error(s_path, f"{context} has no track that can be played; it is left out")
    return report.items


def errors(problems: list) -> list:
    return [item for item in problems if item["level"] == "error"]


# --- The editor's model ----------------------------------------------------
#
# model = {"id", "name", "version", "author", "description", "extra": {},
#          "series": [{"key", "name", "order", "extra": {},
#                      "tracks": [{"key", "title", "file", "extra": {},
#                                  "loop": {"mode": "auto"|"whole"|"none"|"custom",
#                                           "start": int, "end": int or None}}]}]}
#
# "extra" keeps members this version does not know, so they are written back.

def build_loop(loop: Optional[dict], samples: Optional[int] = None):
    """The manifest's "loop" for the editor's loop. A custom loop that ends at
    the file's end leaves "end" out (the port then loops to the end, which is
    also safe for files the port resamples)."""
    loop = loop or {"mode": "auto"}
    mode = loop.get("mode", "auto")
    if mode in ("auto", "whole", "none"):
        return mode
    if mode != "custom":
        raise ValueError(f"unknown loop mode {mode!r}")
    if "start_seconds" in loop:
        out = {"start_seconds": loop["start_seconds"]}
        if loop.get("end_seconds") is not None:
            out["end_seconds"] = loop["end_seconds"]
        return out
    out = {"start": int(loop.get("start") or 0)}
    end = loop.get("end")
    if end is not None and not (samples and int(end) == samples):
        out["end"] = int(end)
    return out


def build_manifest(model: dict, info: Optional[Callable[[str], Optional[dict]]] = None) -> dict:
    """mod.json's document for the editor's model, members in the documented order."""
    doc: dict = {"format": FORMAT, "id": model.get("id", ""), "name": model.get("name", ""),
                 "version": model.get("version", "")}
    for name in ("author", "description"):
        if (model.get(name) or "").strip():
            doc[name] = model[name].strip()
    series_out = []
    for series in model.get("series", []):
        entry = {"key": series.get("key", ""), "name": series.get("name", ""), "order": int(series.get("order") or 0)}
        tracks = []
        for track in series.get("tracks", []):
            details = info(track.get("file", "")) if info is not None and track.get("file") else None
            samples = details.get("samples") if details and not details.get("error") else None
            item = {"key": track.get("key", ""), "title": track.get("title", ""), "file": track.get("file", ""),
                    "loop": build_loop(track.get("loop"), samples)}
            item.update({k: v for k, v in (track.get("extra") or {}).items() if k not in TRACK_MEMBERS})
            tracks.append(item)
        entry["tracks"] = tracks
        entry.update({k: v for k, v in (series.get("extra") or {}).items() if k not in SERIES_MEMBERS})
        series_out.append(entry)
    doc["music"] = {"series": series_out}
    doc.update({k: v for k, v in (model.get("extra") or {}).items() if k not in MOD_MEMBERS})
    return doc


def manifest_bytes(document: dict) -> bytes:
    return (json.dumps(document, indent=2, ensure_ascii=False) + "\n").encode("utf-8")


def model_from_manifest(document: dict, info: Optional[Callable[[str], Optional[dict]]] = None) -> dict:
    """The editor's model for a manifest (as far as it can be read). Loops in
    seconds become samples when the file's rate is known."""
    doc = document if isinstance(document, dict) else {}

    def text(obj, name):
        value = obj.get(name)
        return value if isinstance(value, str) else ""

    model = {name: text(doc, name) for name in ("id", "name", "version", "author", "description")}
    model["extra"] = {k: v for k, v in doc.items() if k not in MOD_MEMBERS}
    model["series"] = []
    music = doc.get("music") if isinstance(doc.get("music"), dict) else {}
    for entry in music.get("series", []) if isinstance(music.get("series"), list) else []:
        if not isinstance(entry, dict):
            continue
        order = whole_number(entry.get("order", 0))
        series = {"key": text(entry, "key"), "name": text(entry, "name"), "order": order or 0,
                  "extra": {k: v for k, v in entry.items() if k not in SERIES_MEMBERS}, "tracks": []}
        for item in entry.get("tracks", []) if isinstance(entry.get("tracks"), list) else []:
            if not isinstance(item, dict):
                continue
            track = {"key": text(item, "key"), "title": text(item, "title"), "file": text(item, "file"),
                     "extra": {k: v for k, v in item.items() if k not in TRACK_MEMBERS}}
            loop = item.get("loop", "auto")
            if isinstance(loop, str) and loop in ("auto", "whole", "none"):
                track["loop"] = {"mode": loop}
            elif isinstance(loop, dict) and ("start_seconds" in loop or "end_seconds" in loop):
                details = info(track["file"]) if info is not None and track["file"] else None
                rate = int(details.get("rate") or 0) if details else 0
                start, end = loop.get("start_seconds"), loop.get("end_seconds")
                if rate and _is_number(start):
                    track["loop"] = {"mode": "custom", "start": seconds_to_samples(start, rate),
                                     "end": seconds_to_samples(end, rate) if _is_number(end) else None,
                                     "converted_from_seconds": True}
                else:
                    track["loop"] = {"mode": "custom", "start_seconds": start, "end_seconds": end}
            elif isinstance(loop, dict):
                start, end = whole_number(loop.get("start")), whole_number(loop.get("end"))
                track["loop"] = {"mode": "custom", "start": start or 0, "end": end}
            else:
                track["loop"] = {"mode": "auto"}
            series["tracks"].append(track)
        model["series"].append(series)
    return model


def mod_summary(folder: str) -> dict:
    """{"folder", "id", "name", "version", "tracks": {number: id}, "error"} of a mod folder, read quickly."""
    name = os.path.basename(folder)
    result = {"folder": name, "id": None, "name": None, "version": None, "tracks": {}, "error": None}
    path = os.path.join(folder, "mod.json")
    try:
        with open(path, "rb") as handle:
            data = handle.read(MAX_MANIFEST_BYTES + 1)
    except OSError:
        result["error"] = "there is no mod.json"
        return result
    document, problems = parse_manifest_text(data)
    if document is None:
        result["error"] = problems[0]["message"]
        return result
    if not isinstance(document, dict):
        result["error"] = "the manifest must be an object"
        return result
    for key in ("id", "name", "version"):
        if isinstance(document.get(key), str):
            result[key] = document[key]
    music = document.get("music") if isinstance(document.get("music"), dict) else {}
    for entry in music.get("series", []) if isinstance(music.get("series"), list) else []:
        for item in (entry.get("tracks", []) if isinstance(entry, dict) and isinstance(entry.get("tracks"), list) else []):
            if isinstance(item, dict) and isinstance(item.get("key"), str) and result["id"]:
                track_id = f"{result['id']}:{item['key']}"
                result["tracks"][id_number(track_id)] = track_id
    return result
