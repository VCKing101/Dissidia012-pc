"""Extract PSP NID -> function name tables from a local PPSSPP Windows build.

PPSSPP compiles its HLE tables (``HLEFunction`` arrays and the ``HLEModule``
list that names them) into the executable. This script reads them out of the
PE image so the port survey can name imported NIDs without a network download.

Only PPSSPP data is read. Output: ``build/scratch/port/ppsspp_nids.json`` as
``{module: {"0xNID": name}}``.
"""

import argparse
import json
import re
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DEFAULT_EXE = ROOT / "tools/local/ppsspp/PPSSPPWindows64.exe"
DEFAULT_OUT = ROOT / "build/scratch/port/ppsspp_nids.json"

# HLEFunction on x64: u32 ID (+pad), func*, const char* name, char retmask
# (+pad), const char* argmask, u32 flags, u32 stackBytesToClear -> 48 bytes.
STRIDE = 48
# REX.W(/R) 8D /r with mod=00, rm=101: lea r64, [rip+disp32]
LEA_RIP = re.compile(b"[\\x48\\x4C]\\x8D[\\x05\\x0D\\x15\\x1D\\x25\\x2D\\x35\\x3D]", re.DOTALL)


class Image:
    def __init__(self, data):
        self.data = data
        pe = struct.unpack_from("<I", data, 0x3C)[0]
        if data[pe:pe + 4] != b"PE\0\0":
            raise ValueError("not a PE image")
        sections, opt_size = struct.unpack_from("<H12xH", data, pe + 6)
        opt = pe + 24
        if struct.unpack_from("<H", data, opt)[0] != 0x20B:
            raise ValueError("expected PE32+")
        self.base = struct.unpack_from("<Q", data, opt + 24)[0]
        self.sections = []
        table = opt + opt_size
        for index in range(sections):
            name, vsize, rva, rsize, raw = struct.unpack_from("<8sIIII", data, table + index * 40)
            self.sections.append((name.rstrip(b"\0").decode(), rva, max(vsize, rsize), raw, rsize))

    def offset(self, va):
        rva = va - self.base
        for _, start, size, raw, rsize in self.sections:
            if start <= rva < start + min(size, rsize):
                return raw + rva - start
        return None

    def cstring(self, va, limit=96):
        off = self.offset(va)
        if off is None:
            return None
        end = self.data.find(b"\0", off, off + limit)
        if end < 0:
            return None
        raw = self.data[off:end]
        if not raw or any(c < 0x20 or c > 0x7E for c in raw):
            return None
        return raw.decode("ascii")


def looks_like_name(text):
    return bool(text) and (text[0].isalpha() or text[0] == "_") and all(c.isalnum() or c in "_" for c in text)


def entry_at(img, off):
    nid, pad, func, name_ptr = struct.unpack_from("<IIQQ", img.data, off)
    if pad != 0:
        return None
    name = img.cstring(name_ptr)
    if not looks_like_name(name):
        return None
    if func and img.offset(func) is None:
        return None
    return nid, name


def scan(img):
    tables = {}
    for name, rva, size, raw, rsize in img.sections:
        if name not in (".rdata", ".data"):
            continue
        off = raw
        end = raw + rsize - STRIDE
        while off < end:
            first = entry_at(img, off)
            if first is None:
                off += 8
                continue
            entries = [first]
            cursor = off + STRIDE
            while cursor < end:
                item = entry_at(img, cursor)
                if item is None:
                    break
                entries.append(item)
                cursor += STRIDE
            if len(entries) >= 2 or (first[1].startswith("sce") and first[0] > 0xFFFF):
                va = img.base + rva + (off - raw)
                tables[va] = entries
                off = cursor
            else:
                off += 8
    return tables


def lea_targets(img):
    """Map every RIP-relative LEA target in .text to the code offsets using it."""
    refs = {}
    for name, rva, size, raw, rsize in img.sections:
        if name != ".text":
            continue
        blob = img.data[raw:raw + rsize]
        for match in LEA_RIP.finditer(blob):
            pos = match.start()
            if pos + 7 > len(blob):
                continue
            disp = struct.unpack_from("<i", blob, pos + 3)[0]
            target = img.base + rva + pos + 7 + disp
            refs.setdefault(target, []).append(raw + pos)
    return refs


def split_and_name(img, tables):
    """PPSSPP registers each table with RegisterHLEModule(name, count, table);
    the call site loads the table and the module-name string with nearby LEAs.
    Split scanned runs where a table start is referenced, and take the nearest
    name string loaded within 64 bytes of the table reference."""
    refs = lea_targets(img)
    named = {}
    for va, entries in tables.items():
        starts = [i for i in range(len(entries)) if va + i * STRIDE in refs] or [0]
        if starts[0] != 0:
            starts.insert(0, 0)
        for n, first in enumerate(starts):
            last = starts[n + 1] if n + 1 < len(starts) else len(entries)
            sub_va = va + first * STRIDE
            best = None
            for site in refs.get(sub_va, []):
                for probe in range(site - 64, site + 64):
                    b = img.data
                    if b[probe] in (0x48, 0x4C) and b[probe + 1] == 0x8D and (b[probe + 2] & 0xC7) == 5 and probe != site:
                        sec = next(s for s in img.sections if s[0] == ".text")
                        target = img.base + sec[1] + (probe - sec[3]) + 7 + struct.unpack_from("<i", b, probe + 3)[0]
                        text = img.cstring(target)
                        if text and looks_like_name(text) and len(text) > 3:
                            dist = abs(probe - site)
                            if best is None or dist < best[0]:
                                best = (dist, text)
            named[sub_va] = (best[1] if best else None, entries[first:last])
    return named


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--exe", type=Path, default=DEFAULT_EXE)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUT)
    args = parser.parse_args()
    img = Image(args.exe.read_bytes())
    tables = split_and_name(img, scan(img))
    result = {}
    unnamed = 0
    for va, (module, entries) in sorted(tables.items()):
        if not any(name.startswith(("sce", "_sce", "Kernel", "sys", "__", "ku")) for _, name in entries[:3]):
            continue
        if module is None:
            unnamed += 1
            module = f"?table_{va:X}"
        bucket = result.setdefault(module, {})
        for nid, name in entries:
            bucket[f"0x{nid:08X}"] = name
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=1, sort_keys=True) + "\n", encoding="utf-8")
    total = sum(len(v) for v in result.values())
    print(f"{len(result)} modules ({unnamed} unnamed tables), {total} NIDs -> {args.output}")


if __name__ == "__main__":
    main()
