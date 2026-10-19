"""Read a PSP ISO9660 image and emit metadata; never modify or extract game files.

Usage: python tools/inspect_iso.py ULES-01505.ISO --output docs/decomp/iso-inventory.json
Standard library only. Supports ordinary single-extent ISO9660 files, not UDF/CSO.
"""

import argparse
import hashlib
import json
import struct
from pathlib import Path


class Iso:
    def __init__(self, path):
        self.path = Path(path)
        self.size = self.path.stat().st_size
        self.stream = self.path.open("rb")
        self.block = 2048
        for sector in range(16, 256):
            descriptor = self.read(sector * 2048, 2048)
            if descriptor[1:6] != b"CD001":
                raise ValueError("Not an ISO9660 image")
            if descriptor[0] == 1:
                self.block = struct.unpack_from("<H", descriptor, 128)[0]
                self.volume = descriptor[40:72].decode("ascii").strip()
                self.volume_blocks = struct.unpack_from("<I", descriptor, 80)[0]
                self.root = self.record(descriptor[156:])
                break
            if descriptor[0] == 255:
                raise ValueError("No primary volume descriptor")
        else:
            raise ValueError("No primary volume descriptor within scan limit")

    def read(self, offset, size):
        if offset < 0 or size < 0 or offset + size > self.size:
            raise ValueError("Extent outside image")
        self.stream.seek(offset)
        data = self.stream.read(size)
        if len(data) != size:
            raise ValueError("Short read")
        return data

    @staticmethod
    def record(data):
        if len(data) < 34 or data[0] < 34 or data[0] > len(data):
            raise ValueError("Invalid directory record")
        name = data[33:33 + data[32]]
        return {"extent_lba": struct.unpack_from("<I", data, 2)[0],
                "size": struct.unpack_from("<I", data, 10)[0],
                "flags": data[25], "name": name}

    def files(self, directory=None, prefix="", visited=None):
        directory = directory or self.root
        visited = set() if visited is None else visited
        key = (directory["extent_lba"], directory["size"])
        if key in visited:
            raise ValueError("Directory cycle or duplicate extent")
        visited.add(key)
        data = self.read(directory["extent_lba"] * self.block, directory["size"])
        position = 0
        while position < len(data):
            length = data[position]
            if not length:
                position = (position // self.block + 1) * self.block
                continue
            record = self.record(data[position:position + length])
            position += length
            if record["name"] in (b"\x00", b"\x01"):
                continue
            if record["flags"] & 0x80:
                raise ValueError("Multi-extent file unsupported")
            name = record.pop("name").decode("ascii").split(";")[0]
            path = prefix + "/" + name
            if record["flags"] & 2:
                yield from self.files(record, path, visited)
            else:
                record.pop("flags")
                yield {"path": path, **record}

    def content(self, entry, limit=None):
        size = entry["size"] if limit is None else min(limit, entry["size"])
        return self.read(entry["extent_lba"] * self.block, size)


def parse_sfo(data):
    if data[:4] != b"\x00PSF":
        raise ValueError("Invalid PARAM.SFO")
    _, keys, values, count = struct.unpack_from("<4I", data, 4)
    result = {}
    for i in range(count):
        key_off, fmt, length, _, value_off = struct.unpack_from("<HHIII", data, 20 + i * 16)
        key_start = keys + key_off
        key = data[key_start:data.index(b"\0", key_start)].decode("utf-8")
        value = data[values + value_off:values + value_off + length]
        result[key] = (struct.unpack("<I", value)[0] if fmt == 0x404
                       else value.rstrip(b"\0").decode("utf-8", errors="replace"))
    return result


def executable_info(data):
    if data and not any(data):
        return {"format": "all-zero placeholder"}
    if data.startswith(b"~SCE"):
        return {"format": "~SCE wrapped module (requires decoding before ELF analysis)"}
    if data.startswith(b"~PSP"):
        return {"format": "~PSP container (requires decoding before ELF analysis)",
                "module_name": data[10:38].split(b"\0")[0].decode("ascii", errors="replace"),
                "elf_size_from_header": struct.unpack_from("<I", data, 40)[0],
                "container_size_from_header": struct.unpack_from("<I", data, 44)[0],
                "tag": f"0x{struct.unpack_from('<I', data, 208)[0]:08x}"}
    if not data.startswith(b"\x7fELF"):
        return {"format": "unknown", "first_16_bytes_hex": data[:16].hex()}
    if data[4:6] != b"\x01\x01":
        return {"format": "ELF (not ELF32 little endian)"}
    header = struct.unpack_from("<HHIIIIIHHHHHH", data, 16)
    e_type, machine, _, entry, phoff, shoff, flags, _, phsize, phnum, shsize, shnum, shstr = header
    programs = []
    for i in range(phnum):
        ptype, offset, vaddr, paddr, filesz, memsz, pflags, align = struct.unpack_from("<8I", data, phoff + i * phsize)
        programs.append({"type": ptype, "offset": offset, "virtual_address": f"0x{vaddr:08x}",
                         "physical_address": f"0x{paddr:08x}", "file_size": filesz,
                         "memory_size": memsz, "flags": pflags, "alignment": align})
    sections = [struct.unpack_from("<10I", data, shoff + i * shsize) for i in range(shnum)]
    names = b""
    if sections and shstr < len(sections):
        section = sections[shstr]
        names = data[section[4]:section[4] + section[5]]
    out = []
    comments = []
    module_name = None
    for section in sections:
        name_off = section[0]
        name = names[name_off:].split(b"\0")[0].decode("ascii", errors="replace")
        out.append({"name": name, "type": f"0x{section[1]:08x}",
                    "address": f"0x{section[3]:08x}", "offset": section[4], "size": section[5]})
        if name == ".comment":
            comments = [s.decode("ascii", errors="replace") for s in
                        data[section[4]:section[4] + section[5]].split(b"\0") if s]
        if name == ".rodata.sceModuleInfo":
            module_name = data[section[4] + 4:section[4] + 32].split(b"\0")[0].decode("ascii", errors="replace")
    return {"format": "ELF32 little endian", "type": f"0x{e_type:04x}",
            "machine": machine, "entry": f"0x{entry:08x}",
            "flags": f"0x{flags:08x}", "module_name": module_name,
            "program_headers": programs, "sections": out, "compiler_comments": comments}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("iso", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    if args.output.resolve() == args.iso.resolve():
        parser.error("Output must not overwrite the ISO")
    iso = Iso(args.iso)
    try:
        entries = list(iso.files())
        outside_image = []
        for entry in entries:
            end = entry["extent_lba"] * iso.block + entry["size"]
            if end > iso.size:
                outside_image.append({"path": entry["path"], "declared_end": end,
                                      "bytes_missing": end - iso.size})
        hashes = {name: hashlib.new(name) for name in ("sha1", "sha256")}
        iso.stream.seek(0)
        while chunk := iso.stream.read(8 * 1024 * 1024):
            for digest in hashes.values():
                digest.update(chunk)
        report = {"image": {"filename": args.iso.name, "size": iso.size,
                            "volume_id": iso.volume, "logical_block_size": iso.block,
                            "volume_blocks": iso.volume_blocks,
                            **{name: digest.hexdigest() for name, digest in hashes.items()}},
                  "validation": {"declared_volume_size": iso.volume_blocks * iso.block,
                                 "image_size_matches_volume": iso.size == iso.volume_blocks * iso.block,
                                 "files_outside_image": outside_image},
                  "files": entries, "metadata": {}, "executables": {}}
        for entry in entries:
            path = entry["path"]
            if path.endswith("/PARAM.SFO"):
                report["metadata"][path] = parse_sfo(iso.content(entry))
            if path == "/UMD_DATA.BIN":
                report["metadata"][path] = iso.content(entry).rstrip(b"\0").decode("ascii", errors="replace")
            if path.upper().endswith(("/EBOOT.BIN", "/EBOOT.OLD", "/BOOT.BIN", ".PRX", ".ELF")):
                data = iso.content(entry)
                report["executables"][path] = {"size": len(data),
                    "sha256": hashlib.sha256(data).hexdigest(), **executable_info(data)}
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        print(json.dumps({"image": report["image"], "validation": report["validation"],
                          "file_count": len(entries), "metadata": report["metadata"],
                          "executables": {path: {k: v for k, v in info.items() if k != "sections"}
                                          for path, info in report["executables"].items()}}, indent=2))
    finally:
        iso.stream.close()


if __name__ == "__main__":
    main()
