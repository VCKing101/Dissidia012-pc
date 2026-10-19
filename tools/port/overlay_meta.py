"""Describe Dissidia 012's overlays for PortableKit: meta.txt, and optionally the corpus.

    python tools/port/overlay_meta.py [--out DIR] [--code-size N] [--check DUMP] [--recompile]
                                      [--recompiler EXE] [--prx-base ADDR] ELF...

PortableKit installs a recompiled overlay when the bytes in the overlay window
match the image the corpus was built from. Its own tool, add_overlay.py, reads
Capcom's "MWo3" overlay header for that. Dissidia's overlays are plain ELFs
(type 0xFFA8, one PT_LOAD at 0x089DD4C0, a 16-byte descriptor and then code
and data together, no BSS), so this writes the same meta.txt from the ELF:

    base=      load address of the segment (the overlay window)
    name=      menu, battle, extra (or suppreacc, below)
    size=      bytes the segment takes in memory
    code_size= bytes after the first 64 that the identity hash covers
    hash=      FNV-1a 64 of the first 64 + code_size bytes, as host/overlays.cpp computes it

The game writes into a loaded overlay (code and data share one segment), so the
hashed range must stop before the first byte the game changes. --check DUMP
compares the ELF with a dump of the overlay window taken while that overlay was
loaded and running (the port's D012_DUMP_SLOT, see port/README.md), reports the
first changed offsets and, unless --code-size is given, uses the unchanged
prefix. Without either, code_size defaults to 4,032 bytes (a 4 KiB hash
window), which already differs between the three overlays.

Measured windows (docs/port/recompiled-build.md): menu first changes at
+0x3500, battle at +0x455F0; extra was not seen loaded, and its first 4,096
bytes are the 16-byte descriptor and function bodies. The default window is
inside all three and tells them apart.

The disc's own LIBSUPPREACC.PRX is described the same way. The profile loads
it at a fixed address (0x09B81000, the first block of the user partition), so a
corpus for it can be an overlay library too. A PRX is relocatable, so its hash
is taken over the image relocated to --prx-base (default 0x09B81000), as
PortableKit's Elf32Image::load_and_relocate leaves it in memory. Installing a
corpus outside the profile's overlay slots needs the framework change on the
dissidia-recomp branch (port/portablekit-patches/recomp/).

The directory written is <out>/ovl<BASE>_<name>_<HASH>/ (default out:
build/port/recomp/overlays, the recompiled build's PROFILE_DIR/overlays; the
interpreter build's build/port/overlays is left alone, because a meta.txt there
would add overlay targets to that build). --recompile also runs psp_recomp on
the ELF into that directory with the matching symbol prefix; --recompiler names
the psp_recomp to use (default: the recompiled build's, else the interpreter
build's), and passes it --dispatch-call for that overlay's rows of
port/config/dispatch-calls.txt (functions with a native override that must
take the calls from their own unit too).
"""

import argparse
import os
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
HEADER_BYTES = 64  # host/overlay_module.hpp kOverlayHeaderBytes
DEFAULT_CODE_SIZE = 4096 - HEADER_BYTES
NAMES = {"OVL_MENU_APP.ELF": "menu", "OVL_BATTLE_APP.ELF": "battle", "OVL_EXTRA_APP.ELF": "extra",
         "LIBSUPPREACC.PRX": "suppreacc"}
RECOMPILERS = [os.path.join(ROOT, "tools", "local", "port", build, "portablekit", "psp_recomp.exe")
               for build in ("build-d012-recomp", "build-d012")]
PRX_BASE = 0x09B81000  # where port/host/d012_prx.cpp's block allocation places the PRX
SHT_PSP_REL = 0x700000A0
R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6


def dispatch_calls(name):
    """--dispatch-call arguments for an overlay (or "main") from port/config/dispatch-calls.txt."""
    args = []
    path = os.path.join(ROOT, "port", "config", "dispatch-calls.txt")
    if os.path.isfile(path):
        for line in open(path, encoding="utf-8"):
            words = line.split("#", 1)[0].split()
            if len(words) >= 2 and words[0] == name:
                args += ["--dispatch-call", words[1]]
    return args


def fnv1a64(data):
    value = 0xCBF29CE484222325
    for byte in data:
        value = ((value ^ byte) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return value


def signed16(value):
    return value - 0x10000 if value & 0x8000 else value


def load_prx(data, base):
    """(base, image) of a PSP PRX loaded at base and relocated as PortableKit does it
    (src/elf32.cpp, Elf32Image::apply_relocations). Each section of type 0x700000A0 holds
    {offset, info} pairs: info's low nibble is the type, bits 8-15 the segment patched and
    bits 16-23 the segment whose load address is added. A HI16 pairs with the next LO16 of
    the same segment pair, and every patch starts from the original words."""
    phoff, shoff = struct.unpack_from("<II", data, 28)
    phentsize, phnum, shentsize, shnum = struct.unpack_from("<HHHH", data, 42)
    headers = [struct.unpack_from("<8I", data, phoff + i * phentsize) for i in range(phnum)]
    segments = [base + header[2] for header in headers]
    size = max(header[2] + header[5] for header in headers if header[0] == 1)
    image = bytearray(size)
    for p_type, offset, vaddr, _paddr, filesz, _memsz, _flags, _align in headers:
        if p_type == 1:
            image[vaddr:vaddr + filesz] = data[offset:offset + filesz]

    def word(address):
        return struct.unpack_from("<I", image, address - base)[0]

    for i in range(shnum):
        _name, sh_type, _flags, _addr, offset, sh_size = struct.unpack_from("<6I", data, shoff + i * shentsize)
        if sh_type != SHT_PSP_REL:
            continue
        rels = [struct.unpack_from("<II", data, offset + j * 8) for j in range(sh_size // 8)]
        valid = [((info >> 8) & 0xFF) < len(segments) for _offset, info in rels]
        original = [word(segments[(info >> 8) & 0xFF] + rel_offset) if ok else 0
                    for (rel_offset, info), ok in zip(rels, valid)]
        for j, (rel_offset, info) in enumerate(rels):
            kind, patch, target = info & 0xF, (info >> 8) & 0xFF, (info >> 16) & 0xFF
            if not valid[j] or target >= len(segments):
                continue
            to, op = segments[target], original[j]
            if kind == R_MIPS_32:
                op = (op + to) & 0xFFFFFFFF
            elif kind == R_MIPS_26:
                op = (op & 0xFC000000) | (((op & 0x03FFFFFF) + (to >> 2)) & 0x03FFFFFF)
            elif kind == R_MIPS_HI16:
                for k in range(j + 1, len(rels)):
                    other = rels[k][1]
                    if other & 0xF == R_MIPS_HI16:
                        continue
                    if other & 0xF != R_MIPS_LO16 or other >> 8 != info >> 8 or not valid[k]:
                        continue
                    value = (((op & 0xFFFF) << 16) + signed16(original[k] & 0xFFFF) + to) & 0xFFFFFFFF
                    value = (value - signed16(value & 0xFFFF)) & 0xFFFFFFFF
                    op = (op & 0xFFFF0000) | (value >> 16)
                    break
                else:
                    continue
            elif kind == R_MIPS_LO16:
                op = (op & 0xFFFF0000) | ((op + to) & 0xFFFF)
            else:
                continue
            struct.pack_into("<I", image, segments[patch] + rel_offset - base, op)
    return base, bytes(image)


def load_image(path, prx_base=PRX_BASE):
    """(base, image as loaded) of an overlay ELF's single PT_LOAD segment, or of a PRX
    relocated to prx_base."""
    data = open(path, "rb").read()
    if data[:4] != b"\x7fELF":
        raise SystemExit(f"{path}: not an ELF")
    e_type, = struct.unpack_from("<H", data, 16)
    if e_type == 0xFFA0:
        return load_prx(data, prx_base)
    phoff, = struct.unpack_from("<I", data, 28)
    phentsize, phnum = struct.unpack_from("<HH", data, 42)
    loads = []
    for i in range(phnum):
        p_type, offset, vaddr, _paddr, filesz, memsz, _flags, _align = struct.unpack_from(
            "<8I", data, phoff + i * phentsize)
        if p_type == 1:
            loads.append((vaddr, data[offset:offset + filesz] + bytes(memsz - filesz)))
    if e_type != 0xFFA8 or len(loads) != 1:
        raise SystemExit(f"{path}: expected a type 0xFFA8 ELF with one PT_LOAD or a PRX "
                         f"(type {e_type:#x}, {len(loads)} loads)")
    return loads[0]


def first_differences(image, dump, limit=8):
    found = []
    for offset in range(min(len(image), len(dump))):
        if image[offset] != dump[offset]:
            found.append(offset)
            if len(found) == limit:
                break
    return found


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("elves", nargs="+", metavar="ELF")
    parser.add_argument("--out", default=os.path.join(ROOT, "build", "port", "recomp", "overlays"))
    parser.add_argument("--code-size", type=lambda t: int(t, 0))
    parser.add_argument("--check", metavar="DUMP", help="overlay window dump to compare against (one ELF only)")
    parser.add_argument("--recompile", action="store_true")
    parser.add_argument("--recompiler", help="psp_recomp executable for --recompile")
    parser.add_argument("--prx-base", type=lambda t: int(t, 0), default=PRX_BASE,
                        help=f"load address of a PRX (default {PRX_BASE:#010x})")
    options = parser.parse_args()
    recompiler = options.recompiler or next((p for p in RECOMPILERS if os.path.isfile(p)), RECOMPILERS[0])
    if options.check and len(options.elves) != 1:
        raise SystemExit("--check takes one ELF")

    for path in options.elves:
        base, image = load_image(path, options.prx_base)
        name = NAMES.get(os.path.basename(path).upper(), os.path.splitext(os.path.basename(path))[0].lower())
        code_size = options.code_size
        if options.check:
            dump = open(options.check, "rb").read()
            changed = first_differences(image, dump)
            if not changed:
                print(f"{name}: the dump matches the whole image ({len(image)} bytes)")
                unchanged = len(image)
            else:
                print(f"{name}: first changed offsets " + ", ".join(f"+{o:#x} ({base + o:#010x})" for o in changed))
                unchanged = changed[0] & ~3
            if code_size is None:
                code_size = max(0, unchanged - HEADER_BYTES)
        if code_size is None:
            code_size = DEFAULT_CODE_SIZE
        code_size = min(code_size, len(image) - HEADER_BYTES)
        digest = fnv1a64(image[:HEADER_BYTES + code_size])
        prefix = f"ovl{base:08X}_{name}_{digest:016X}"
        target = os.path.join(options.out, prefix)
        os.makedirs(target, exist_ok=True)
        with open(os.path.join(target, "meta.txt"), "w", newline="\n") as out:
            out.write(f"base=0x{base:08X}\nname={name}\nsize={len(image)}\ncode_size={code_size}\n"
                      f"hash=0x{digest:016X}\nsource={os.path.basename(path)}\n")
        print(f"{name}: base {base:#010x}, size {len(image)}, code_size {code_size}, hash {digest:#018x} -> {target}")
        if options.recompile:
            subprocess.run([recompiler, path, "--auto", target, f"0x{base:08X}", "--prefix", prefix]
                           + dispatch_calls(name), check=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
