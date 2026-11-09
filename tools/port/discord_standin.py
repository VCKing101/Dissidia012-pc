r"""A stand-in for the Discord app's local connection on Windows, for testing the port's Discord status
(docs/port/discord-status.md): a named pipe that answers the handshake with READY and every SET_ACTIVITY with its
own data, and logs each message as one JSON line. Point the port at it with D012_DISCORD_IPC.

    python tools/port/discord_standin.py \\.\pipe\d012-fake-discord log.jsonl [--refuse] [--ping] [--accept N]

--refuse closes the handshake as Discord does for a wrong application ID (code 4000); --ping sends a PING after
the first SET_ACTIVITY; --accept N serves N connections, one after another, then exits.
"""
import ctypes
import json
import struct
import sys
import time
from ctypes import wintypes

k32 = ctypes.WinDLL("kernel32", use_last_error=True)
k32.CreateNamedPipeW.restype = wintypes.HANDLE
k32.CreateNamedPipeW.argtypes = [wintypes.LPCWSTR, wintypes.DWORD, wintypes.DWORD, wintypes.DWORD,
                                 wintypes.DWORD, wintypes.DWORD, wintypes.DWORD, wintypes.LPVOID]
k32.ConnectNamedPipe.argtypes = [wintypes.HANDLE, wintypes.LPVOID]
k32.ReadFile.argtypes = [wintypes.HANDLE, wintypes.LPVOID, wintypes.DWORD, ctypes.POINTER(wintypes.DWORD), wintypes.LPVOID]
k32.WriteFile.argtypes = [wintypes.HANDLE, wintypes.LPCVOID, wintypes.DWORD, ctypes.POINTER(wintypes.DWORD), wintypes.LPVOID]
k32.DisconnectNamedPipe.argtypes = [wintypes.HANDLE]
k32.CloseHandle.argtypes = [wintypes.HANDLE]
INVALID = wintypes.HANDLE(-1).value

name, log_path = sys.argv[1], sys.argv[2]
refuse = "--refuse" in sys.argv
ping = "--ping" in sys.argv
accept = int(sys.argv[sys.argv.index("--accept") + 1]) if "--accept" in sys.argv else 1
log = open(log_path, "a", encoding="utf-8")


def note(**what):
    what["t"] = round(time.time(), 2)
    log.write(json.dumps(what) + "\n")
    log.flush()


def read_exact(pipe, n):
    out = b""
    while len(out) < n:
        buf = ctypes.create_string_buffer(n - len(out))
        got = wintypes.DWORD()
        if not k32.ReadFile(pipe, buf, n - len(out), ctypes.byref(got), None) or got.value == 0:
            return None
        out += buf.raw[:got.value]
    return out


def send(pipe, op, body):
    data = json.dumps(body).encode()
    blob = struct.pack("<II", op, len(data)) + data
    wrote = wintypes.DWORD()
    k32.WriteFile(pipe, blob, len(blob), ctypes.byref(wrote), None)


for session in range(accept):
    pipe = k32.CreateNamedPipeW(name, 3, 0, 1, 65536, 65536, 0, None)  # duplex, byte mode, one instance
    if pipe == INVALID:
        note(event="create failed", error=ctypes.get_last_error())
        sys.exit(1)
    note(event="listening", session=session)
    k32.ConnectNamedPipe(pipe, None)
    note(event="client connected", session=session)
    while True:
        head = read_exact(pipe, 8)
        if head is None:
            note(event="client gone", session=session)
            break
        op, length = struct.unpack("<II", head)
        body = json.loads(read_exact(pipe, length) or b"null")
        note(event="message", op=op, body=body)
        if op == 0:
            if refuse:
                send(pipe, 2, {"code": 4000, "message": "Invalid Client ID"})
                note(event="refused")
                time.sleep(0.5)
                break
            send(pipe, 1, {"cmd": "DISPATCH", "evt": "READY", "data": {"v": 1, "user": {"username": "tester"}}})
        elif op == 1:
            if ping and body.get("nonce") == "1":
                send(pipe, 3, {"ping": 1})
                note(event="sent ping")
            send(pipe, 1, {"cmd": body.get("cmd"), "evt": None, "data": body.get("args", {}).get("activity"),
                           "nonce": body.get("nonce")})
    k32.DisconnectNamedPipe(pipe)
    k32.CloseHandle(pipe)
note(event="done")
