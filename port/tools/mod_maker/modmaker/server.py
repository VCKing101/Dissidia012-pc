"""The mod maker's local server: the page, and a small JSON API over one mods folder.

Bound to 127.0.0.1 only. Every API call needs the session's random token (in
the X-Mod-Maker-Token header), the Host header must be this server's, and
every path is a mod folder's name or a path inside that folder, checked after
resolving links, so nothing outside the mods folder is read or written.
MIT licence; see port/README.md.
"""

from __future__ import annotations

import concurrent.futures
import datetime
import hmac
import http.server
import json
import os
import re
import secrets
import shutil
import socketserver
import threading
import unicodedata
import urllib.parse
from typing import Optional

from . import manifest as mf
from .audio import REPO_ROOT, TOOL_DIR, Decoder

WEB_DIR = os.path.join(TOOL_DIR, "web")
STATIC = {"index.html": "text/html; charset=utf-8", "style.css": "text/css; charset=utf-8",
          "app.js": "text/javascript; charset=utf-8", "suggest.js": "text/javascript; charset=utf-8",
          "waveform.js": "text/javascript; charset=utf-8"}
MAX_UPLOAD = 1 << 30
MAX_REQUEST = 4 << 20
KEPT_BACKUPS = 20
TOKEN_HEADER = "X-Mod-Maker-Token"


class ApiError(Exception):
    def __init__(self, message: str, status: int = 400):
        super().__init__(message)
        self.status = status


def safe_file_name(name: str) -> str:
    """A plain file name for an imported file: its base name, ASCII letters,
    digits, space, . _ - ( ), with an audio extension."""
    base = re.split(r"[\\/]", name or "")[-1]
    stem, extension = os.path.splitext(base)
    extension = extension.lower()
    if extension not in mf.AUDIO_EXTENSIONS:
        raise ApiError(f"\"{base}\" is not .mp3, .ogg, .oga, .opus, .flac or .wav")
    stem = unicodedata.normalize("NFKD", stem).encode("ascii", "ignore").decode("ascii")
    stem = re.sub(r"[^A-Za-z0-9 ._()-]+", "_", stem).strip(" ._") or "track"
    return stem[:80] + extension


def title_from_file(name: str) -> str:
    stem = os.path.splitext(re.split(r"[\\/]", name)[-1])[0]
    stem = re.sub(r"^\d{1,3}[ ._-]+", "", stem)  # a leading track number
    return " ".join(stem.replace("_", " ").split()) or "Track"


class App:
    """One session: a mods folder, a decoder, a token."""

    def __init__(self, mods_dir: str, decoder: Decoder, token: Optional[str] = None):
        self.mods_dir = os.path.realpath(mods_dir)
        self.decoder = decoder
        self.token = token or secrets.token_urlsafe(24)
        self.port = 0
        self._pool = concurrent.futures.ThreadPoolExecutor(max_workers=2)
        self._write_lock = threading.Lock()

    # --- Paths -----------------------------------------------------------

    def mod_path(self, folder, must_exist: bool = True) -> str:
        if not isinstance(folder, str) or not folder or folder in (".", "..") or len(folder) > 128 or \
                re.search(r"[\\/:\x00*?\"<>|]", folder) or folder != folder.strip() or folder.endswith("."):
            raise ApiError("a mod folder is a plain folder name inside the mods folder")
        path = os.path.join(self.mods_dir, folder)
        if not mf.inside(self.mods_dir, path) or os.path.realpath(path) == self.mods_dir:
            raise ApiError("that folder is not inside the mods folder", 403)
        if must_exist and not os.path.isdir(path):
            raise ApiError(f"there is no mod folder \"{folder}\"", 404)
        return path

    def file_path(self, mod: str, relative) -> str:
        if not isinstance(relative, str):
            raise ApiError("a file is a path inside the mod's folder")
        why = mf.check_file_text(relative)
        if why is not None:
            raise ApiError(why.replace("\"file\" ", "the file "), 403 if ".." in relative or relative[:1] in "/\\" or
                           ":" in relative else 400)
        path = os.path.join(mod, *relative.split("/"))
        if not mf.inside(mod, path):
            raise ApiError("that file is not inside the mod's folder", 403)
        return path

    # --- Mods ------------------------------------------------------------

    def summaries(self, skip: Optional[str] = None) -> list:
        out = []
        try:
            names = sorted(os.listdir(self.mods_dir))
        except OSError:
            return out
        for name in names:
            path = os.path.join(self.mods_dir, name)
            if name == skip or not os.path.isdir(path) or name.startswith("."):
                continue
            out.append(mf.mod_summary(path))
        return out

    def track_info(self, mod: str, relative: str, decode: bool) -> Optional[dict]:
        try:
            path = self.file_path(mod, relative)
        except ApiError:
            return None
        if not os.path.isfile(path):
            return None
        try:
            return self.decoder.info(path, decode=decode)
        except (RuntimeError, OSError, ValueError) as error:
            return {"error": str(error)}

    def state(self) -> dict:
        return {"mods_dir": self.mods_dir, "ffmpeg": self.decoder.ff.describe(), "mods": [
            {k: v for k, v in s.items() if k != "tracks"} for s in self.summaries()], "commands": self.commands()}

    def create(self, request: dict) -> dict:
        folder = (request.get("folder") or request.get("id") or "").strip()
        document = {"format": mf.FORMAT, "id": request.get("id", ""), "name": request.get("name", ""),
                    "version": request.get("version", "") or "1.0.0"}
        for name in ("author", "description"):
            if (request.get(name) or "").strip():
                document[name] = request[name].strip()
        document["music"] = {"series": []}
        problems = mf.validate(document, others=self.summaries())
        if mf.errors(problems):
            return {"ok": False, "problems": problems}
        os.makedirs(self.mods_dir, exist_ok=True)
        path = self.mod_path(folder, must_exist=False)
        if os.path.exists(path):
            raise ApiError(f"the mods folder already has \"{folder}\"")
        os.makedirs(os.path.join(path, "music"))
        with open(os.path.join(path, "mod.json"), "wb") as handle:
            handle.write(mf.manifest_bytes(document))
        return {"ok": True, "folder": folder}

    def open(self, request: dict) -> dict:
        path = self.mod_path(request.get("folder"))
        manifest_path = os.path.join(path, "mod.json")
        problems: list = []
        document = None
        if os.path.isfile(manifest_path):
            with open(manifest_path, "rb") as handle:
                document, problems = mf.parse_manifest_text(handle.read(mf.MAX_MANIFEST_BYTES + 1))
        else:
            problems = [mf.problem("warning", "mod", "there is no mod.json yet; saving makes one")]
        model = mf.model_from_manifest(document or {}, info=lambda rel: self.track_info(path, rel, decode=False))
        if not model["id"] and document is None:
            model["id"] = request.get("folder") if mf.valid_key(request.get("folder")) else ""
            model["version"] = "1.0.0"
        if document is not None:
            problems = mf.validate(document, path, lambda rel: self.track_info(path, rel, decode=False),
                                   self.summaries(skip=request.get("folder")))
        for series in model["series"]:
            for track in series["tracks"]:
                if track.get("file"):
                    self._pool.submit(self.track_info, path, track["file"], True)
        return {"folder": request.get("folder"), "model": model, "problems": problems,
                "unreadable": document is None and os.path.isfile(manifest_path)}

    def check(self, request: dict, decode: bool = False) -> tuple:
        folder = request.get("folder")
        path = self.mod_path(folder)
        model = request.get("model")
        if not isinstance(model, dict):
            raise ApiError("no model")
        info = lambda rel: self.track_info(path, rel, decode=decode)  # noqa: E731
        document = mf.build_manifest(model, info)
        problems = mf.validate(document, path, info, self.summaries(skip=folder))
        data = mf.manifest_bytes(document)
        if len(data) > mf.MAX_MANIFEST_BYTES:
            problems.append(mf.problem("error", "mod", "the manifest would be larger than 1 MiB"))
        return path, document, problems, data

    def validate(self, request: dict) -> dict:
        _, document, problems, _ = self.check(request)
        return {"problems": problems, "manifest": document}

    def save(self, request: dict) -> dict:
        with self._write_lock:
            path, document, problems, data = self.check(request, decode=True)
            if mf.errors(problems):
                return {"ok": False, "problems": problems, "manifest": document}
            target = os.path.join(path, "mod.json")
            backup = None
            if os.path.isfile(target):
                with open(target, "rb") as handle:
                    old = handle.read()
                if old == data:
                    return {"ok": True, "problems": problems, "manifest": document, "backup": None,
                            "unchanged": True}
                folder = os.path.join(path, ".backups")
                os.makedirs(folder, exist_ok=True)
                stamp = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
                backup = os.path.join(folder, f"mod-{stamp}.json")
                counter = 1
                while os.path.exists(backup):
                    counter += 1
                    backup = os.path.join(folder, f"mod-{stamp}-{counter}.json")
                with open(backup, "wb") as handle:
                    handle.write(old)
                kept = sorted(n for n in os.listdir(folder) if n.startswith("mod-") and n.endswith(".json"))
                for name in kept[:-KEPT_BACKUPS]:
                    os.remove(os.path.join(folder, name))
            partial = target + ".part"
            with open(partial, "wb") as handle:
                handle.write(data)
            os.replace(partial, target)
            return {"ok": True, "problems": problems, "manifest": document,
                    "backup": os.path.relpath(backup, path).replace("\\", "/") if backup else None}

    def files(self, request: dict) -> dict:
        path = self.mod_path(request.get("folder"))
        out = []
        for base, dirs, names in os.walk(path):
            dirs[:] = [d for d in dirs if not d.startswith(".")]
            for name in sorted(names):
                if os.path.splitext(name)[1].lower() in mf.AUDIO_EXTENSIONS:
                    out.append(os.path.relpath(os.path.join(base, name), path).replace("\\", "/"))
        return {"files": sorted(out)}

    def import_file(self, folder: str, name: str, source, length: int) -> dict:
        """Copies an uploaded file into the mod's music/ folder under a free name."""
        path = self.mod_path(folder)
        if length <= 0 or length > MAX_UPLOAD:
            raise ApiError("the file is empty or larger than 1 GiB")
        file_name = safe_file_name(name)
        music = os.path.join(path, "music")
        os.makedirs(music, exist_ok=True)
        stem, extension = os.path.splitext(file_name)
        target = os.path.join(music, file_name)
        counter = 1
        while os.path.exists(target):
            counter += 1
            target = os.path.join(music, f"{stem}-{counter}{extension}")
        if not mf.inside(path, target):
            raise ApiError("that file is not inside the mod's folder", 403)
        partial = target + ".part"
        remaining = length
        with open(partial, "wb") as handle:
            while remaining > 0:
                chunk = source.read(min(remaining, 1 << 20))
                if not chunk:
                    break
                handle.write(chunk)
                remaining -= len(chunk)
        if remaining:
            os.remove(partial)
            raise ApiError("the upload stopped early")
        os.replace(partial, target)
        relative = os.path.relpath(target, path).replace("\\", "/")
        info = self.track_info(path, relative, decode=False) or {}
        if info.get("error"):
            os.remove(target)
            raise ApiError(f"\"{name}\" cannot be read: {info['error']}")
        title = (info.get("tags") or {}).get("title", "").strip() or title_from_file(name)
        return {"file": relative, "title": mf.clip_bytes(title, 64), "info": info}

    def info(self, request: dict) -> dict:
        path = self.mod_path(request.get("folder"))
        full = self.file_path(path, request.get("file"))
        if not os.path.isfile(full):
            raise ApiError(f"{request.get('file')} is not in the mod's folder", 404)
        details = self.track_info(path, request.get("file"), decode=True)
        if details and details.get("error"):
            raise ApiError(details["error"])
        return details or {}

    def key(self, request: dict) -> dict:
        kind = request.get("kind", "track")
        base = mf.key_from_text(request.get("text", ""), fallback="series" if kind == "series" else "track")
        taken = [k for k in request.get("taken", []) if isinstance(k, str)]
        if kind == "series":
            return {"key": mf.unique_key(base, taken)}
        mod_id = request.get("mod_id") if mf.valid_key(request.get("mod_id")) else None
        numbers = []
        if mod_id:
            numbers = [mf.id_number(f"{mod_id}:{k}") for k in taken]
            for other in self.summaries(skip=request.get("folder")):
                numbers.extend(other["tracks"].keys())
        return {"key": mf.unique_key(base, taken, numbers, mod_id)}

    def commands(self, request: Optional[dict] = None) -> dict:
        """How to try the mods in the port (shown, never run)."""
        request = request or {}
        track_id = request.get("track_id") if isinstance(request.get("track_id"), str) else None
        script = os.path.join(REPO_ROOT, "tools", "port", "play_d012.ps1")
        data_dir = os.path.dirname(self.mods_dir) if os.path.basename(self.mods_dir).lower() == "mods" else None
        lines = []
        env = []
        if data_dir is None:
            env.append(f"$env:D012_MODS_DIR = '{self.mods_dir}'")
        if track_id:
            env.append(f"$env:D012_TEST_BGM_START = '{track_id}'")
        if os.path.isfile(script):
            play = ".\\tools\\port\\play_d012.ps1"
            if data_dir is not None:
                try:
                    relative = os.path.relpath(data_dir, REPO_ROOT)
                except ValueError:  # another drive
                    relative = data_dir
                inside_repo = not relative.startswith("..") and not os.path.isabs(relative)
                play += f" -DataDir '{relative if inside_repo else data_dir}'"
            lines = env + [play]
            where = f"in {REPO_ROOT}"
        else:
            if data_dir is not None:
                env.insert(0, f"$env:D012_DATA_DIR = '{data_dir}'")
            lines = env + [".\\D012Native.exe"]
            where = "in the port's folder"
        cleanup = "Remove-Item Env:D012_TEST_BGM_START" if track_id else None
        return {"powershell": "\n".join(lines), "where": where, "cleanup": cleanup}


def make_handler(app: App):
    class Handler(http.server.BaseHTTPRequestHandler):
        server_version = "ModMaker/1"
        protocol_version = "HTTP/1.1"

        def log_message(self, fmt, *args):  # quiet
            pass

        # --- Plumbing -----------------------------------------------------

        def _send(self, status: int, body: bytes, content_type: str, extra: Optional[dict] = None):
            self.send_response(status)
            self.send_header("Content-Type", content_type)
            self.send_header("Content-Length", str(len(body)))
            self.send_header("Cache-Control", "no-store")
            self.send_header("X-Content-Type-Options", "nosniff")
            self.send_header("Referrer-Policy", "no-referrer")
            for name, value in (extra or {}).items():
                self.send_header(name, value)
            self.end_headers()
            if self.command != "HEAD":
                self.wfile.write(body)

        def _json(self, status: int, value):
            self._send(status, json.dumps(value).encode("utf-8"), "application/json")

        def _allowed_host(self) -> bool:
            host = (self.headers.get("Host") or "").lower()
            if host not in (f"127.0.0.1:{app.port}", f"localhost:{app.port}"):
                return False
            origin = self.headers.get("Origin")
            return origin is None or origin.lower() in (f"http://127.0.0.1:{app.port}", f"http://localhost:{app.port}")

        def _authorised(self) -> bool:
            given = self.headers.get(TOKEN_HEADER) or ""
            return hmac.compare_digest(given.encode("utf-8"), app.token.encode("utf-8"))

        def _guard(self) -> bool:
            # A refused request's body is not read, so the connection cannot be kept.
            self.close_connection = not (self._allowed_host() and self._authorised())
            if not self._allowed_host():
                self._json(403, {"error": "this server only answers its own page"})
                return False
            if not self._authorised():
                self._json(403, {"error": "missing or wrong session token; open the address the mod maker printed"})
                return False
            return True

        def _body(self) -> dict:
            length = int(self.headers.get("Content-Length") or 0)
            if length > MAX_REQUEST:
                raise ApiError("request too large", 413)
            data = self.rfile.read(length) if length else b"{}"
            try:
                value = json.loads(data.decode("utf-8"))
            except ValueError as error:
                raise ApiError(f"not JSON: {error}") from error
            if not isinstance(value, dict):
                raise ApiError("the request must be an object")
            return value

        # --- Routes -------------------------------------------------------

        def do_GET(self):
            url = urllib.parse.urlsplit(self.path)
            if not self._allowed_host():
                self._json(403, {"error": "this server only answers its own page"})
                return
            if url.path in ("/", "/index.html"):
                self._static("index.html")
            elif url.path.startswith("/static/"):
                self._static(url.path[len("/static/"):])
            elif url.path == "/api/track/audio":
                if not self._guard():
                    return
                query = urllib.parse.parse_qs(url.query)
                try:
                    mod = app.mod_path((query.get("folder") or [""])[0])
                    full = app.file_path(mod, (query.get("file") or [""])[0])
                    if not os.path.isfile(full):
                        raise ApiError("no such file", 404)
                    wav = app.decoder.wav(full)
                except ApiError as error:
                    self._json(error.status, {"error": str(error)})
                    return
                except (RuntimeError, OSError) as error:
                    self._json(500, {"error": str(error)})
                    return
                self._file(wav, "audio/wav")
            else:
                self._json(404, {"error": "not found"})

        do_HEAD = do_GET

        def _static(self, name: str):
            if name not in STATIC:
                self._json(404, {"error": "not found"})
                return
            with open(os.path.join(WEB_DIR, name), "rb") as handle:
                body = handle.read()
            policy = ("default-src 'self'; script-src 'self'; worker-src 'self'; style-src 'self' 'unsafe-inline'; "
                      "img-src 'self' data:; media-src 'self' blob:; connect-src 'self'; object-src 'none'; "
                      "base-uri 'none'; frame-ancestors 'none'")
            self._send(200, body, STATIC[name], {"Content-Security-Policy": policy})

        def _file(self, path: str, content_type: str):
            size = os.path.getsize(path)
            self.send_response(200)
            self.send_header("Content-Type", content_type)
            self.send_header("Content-Length", str(size))
            self.send_header("Cache-Control", "no-store")
            self.end_headers()
            if self.command == "HEAD":
                return
            with open(path, "rb") as handle:
                shutil.copyfileobj(handle, self.wfile, 1 << 20)

        def do_POST(self):
            url = urllib.parse.urlsplit(self.path)
            if not self._guard():
                return
            routes = {"/api/state": lambda r: app.state(), "/api/mod/create": app.create, "/api/mod/open": app.open,
                      "/api/mod/validate": app.validate, "/api/mod/save": app.save, "/api/mod/files": app.files,
                      "/api/track/info": app.info, "/api/key": app.key, "/api/port/commands": app.commands}
            try:
                if url.path == "/api/track/import":
                    query = urllib.parse.parse_qs(url.query)
                    result = app.import_file((query.get("folder") or [""])[0], (query.get("name") or [""])[0],
                                             self.rfile, int(self.headers.get("Content-Length") or 0))
                elif url.path in routes:
                    result = routes[url.path](self._body())
                else:
                    self._json(404, {"error": "not found"})
                    return
            except ApiError as error:
                self.close_connection = True  # the body may not have been read
                self._json(error.status, {"error": str(error)})
                return
            except Exception as error:  # report, keep serving
                self.close_connection = True
                self._json(500, {"error": f"{type(error).__name__}: {error}"})
                return
            self._json(200, result)

    return Handler


class Server(socketserver.ThreadingMixIn, http.server.HTTPServer):
    daemon_threads = True
    allow_reuse_address = False


def serve(app: App, port: int = 0) -> Server:
    """A server for `app` on 127.0.0.1 (a free port when 0), not yet serving."""
    server = Server(("127.0.0.1", port), make_handler(app))
    app.port = server.server_address[1]
    return server
