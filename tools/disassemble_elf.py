"""Disassemble an explicitly bounded, file-backed PSP ELF address range.

This does not infer function boundaries. Output is local analysis, not source.
"""

import argparse
import hashlib
import struct
from pathlib import Path

import rabbitizer
from elftools.elf.elffile import ELFFile


def disassemble(path, start, size):
    if start < 0 or size <= 0 or start % 4 or size % 4 or start + size > 0x100000000:
        raise ValueError("Range must contain aligned PSP instructions")
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        if elf["e_machine"] != "EM_MIPS" or elf.elfclass != 32 or not elf.little_endian:
            raise ValueError("Expected a little-endian 32-bit MIPS ELF")
        data = None
        for section in elf.iter_sections():
            base = section["sh_addr"]
            if section["sh_type"] != "SHT_NOBITS" and base <= start and start + size <= base + section["sh_size"]:
                data = section.data()[start - base:start - base + size]
                break
        if data is None:
            for segment in elf.iter_segments():
                base = segment["p_vaddr"]
                if segment["p_type"] == "PT_LOAD" and base <= start and start + size <= base + segment["p_filesz"]:
                    data = segment.data()[start - base:start - base + size]
                    break
        if data is None:
            raise ValueError("Requested range is not fully file-backed in one ELF section or load segment")
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    lines = [f"# Input SHA-256: {digest}", f"# Explicit range: 0x{start:08X}..0x{start + size:08X} (end exclusive)"]
    for offset in range(0, size, 4):
        word = struct.unpack_from("<I", data, offset)[0]
        instruction = rabbitizer.Instruction(word, start + offset, rabbitizer.InstrCategory.R4000ALLEGREX)
        lines.append(f"{start + offset:08X}: {word:08X} {instruction.disassemble()}")
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("elf", type=Path)
    parser.add_argument("start", type=lambda text: int(text, 0))
    parser.add_argument("size", type=lambda text: int(text, 0))
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    if args.output and args.output.resolve() == args.elf.resolve():
        parser.error("Output must not overwrite the input ELF")
    result = disassemble(args.elf, args.start, args.size)
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(result, encoding="utf-8")
        print(f"Disassembled {args.size // 4} instructions to {args.output}")
    else:
        print(result, end="")


if __name__ == "__main__":
    main()
