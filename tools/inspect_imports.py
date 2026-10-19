"""Read PSP module information and function imports/exports from a decrypted ELF."""

import argparse
import json
import struct
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[1]


def inspect(path):
    with path.open("rb") as stream:
        elf = ELFFile(stream)
        segments = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]
        sections = [section for section in elf.iter_sections() if section["sh_type"] != "SHT_NOBITS"]
        def read(address, size):
            for section in sections:
                start = section["sh_addr"]
                if start <= address and address + size <= start + section["sh_size"]:
                    return section.data()[address - start:address - start + size]
            raise ValueError(f"Unmapped PSP address: 0x{address:08X}")
        def string(address):
            result = bytearray()
            for offset in range(256):
                value = read(address + offset, 1)[0]
                if not value:
                    return result.decode("ascii")
                result.append(value)
            raise ValueError("Unterminated library name")
        # PSP uses the first load header's p_paddr as the module-info file
        # offset, optionally with its kernel flag set.
        first = segments[0]
        offset = first["p_paddr"] & 0x7FFFFFFF
        module_address = first["p_vaddr"] + offset - first["p_offset"]
        attrs, minor, major, name, gp, export_start, export_end, import_start, import_end = struct.unpack("<HBB28s5I", read(module_address, 52))
        libraries = []
        position = import_start
        while position < import_end:
            name_address, version, attributes, words, variables, count, nids, stubs = struct.unpack("<IHHBBHII", read(position, 20))
            if words < 5 or position + words * 4 > import_end:
                raise ValueError("Invalid PSP import-library record")
            functions = []
            for index in range(count):
                nid = struct.unpack("<I", read(nids + index * 4, 4))[0]
                functions.append({"address": f"0x{stubs + index * 8:08X}", "nid": f"0x{nid:08X}"})
            libraries.append({"library": string(name_address), "version": version,
                              "attributes": attributes, "variable_count": variables, "functions": functions})
            position += words * 4
        exports = []
        position = export_start
        while position < export_end:
            name_address, version, attributes, words, variables, count, table = struct.unpack("<IHHBBHI", read(position, 16))
            if words < 4 or position + words * 4 > export_end:
                raise ValueError("Invalid PSP export-library record")
            total = count + variables
            functions = []
            for index in range(count):
                nid = struct.unpack("<I", read(table + index * 4, 4))[0]
                address = struct.unpack("<I", read(table + (total + index) * 4, 4))[0]
                functions.append({"address": f"0x{address:08X}", "nid": f"0x{nid:08X}"})
            exports.append({"library": string(name_address) if name_address else "syslib",
                            "version": version, "attributes": attributes,
                            "variable_count": variables, "functions": functions})
            position += words * 4
        return {"module": name.split(b"\0")[0].decode("ascii"),
                "module_info_address": f"0x{module_address:08X}",
                "library_count": len(libraries), "function_count": sum(len(lib["functions"]) for lib in libraries),
                "libraries": libraries, "export_libraries": exports}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("elf", nargs="?", type=Path, default=ROOT / "analysis/ULUS10566/main/EBOOT.ELF")
    parser.add_argument("--output", type=Path, default=ROOT / "analysis/ULUS10566/main/imports.json")
    args = parser.parse_args()
    if args.output.resolve() == args.elf.resolve():
        parser.error("Output must not overwrite the input ELF")
    report = inspect(args.elf)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{report['module']}: {report['library_count']} import libraries, {report['function_count']} function imports")
    addresses = {"0x089C5D10", "0x089C5D28", "0x089C5D40", "0x089C5F80",
                 "0x089C5F90", "0x089C57F8", "0x089C5810", "0x089C5E20"}
    for library in report["libraries"]:
        for function in library["functions"]:
            if function["address"] in addresses:
                print(library["library"], function)
    for library in report["export_libraries"]:
        print(f"Export library {library['library']}: {len(library['functions'])} functions")
        for function in library["functions"]:
            if function["nid"] in {"0xB03FF882", "0xA0EAF444", "0x110E318B", "0x2EC3F4D9"}:
                print(library["library"], function)


if __name__ == "__main__":
    main()
