"""Compile selected C functions and compare their unrelocated bytes to retail.

This probe only accepts functions without relocations. Relocated functions need
linked comparison; never hide a relocation mismatch by masking its operands.
"""

import hashlib
import json
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
from compile_psp import compile_object

ROOT = Path(__file__).resolve().parents[1]


def functions_from_object(path):
    functions = {}
    with Path(path).open("rb") as stream:
        elf = ELFFile(stream)
        symbols = elf.get_section_by_name(".symtab")
        if symbols is None:
            raise ValueError("Compiler object has no symbol table")
        for symbol in symbols.iter_symbols():
            if symbol["st_info"]["type"] != "STT_FUNC" or not isinstance(symbol["st_shndx"], int):
                continue
            index = symbol["st_shndx"]
            address, size = symbol["st_value"], symbol["st_size"]
            start = address - elf.get_section(index)["sh_addr"]
            relocations = []
            for section in elf.iter_sections():
                if not isinstance(section, RelocationSection) or section["sh_info"] != index:
                    continue
                if section.is_RELA():
                    raise ValueError("RELA objects are not supported by the PSP comparator")
                relocation_symbols = elf.get_section(section["sh_link"])
                for relocation in section.iter_relocations():
                    location = relocation["r_offset"]
                    base = address if elf["e_type"] != "ET_REL" else start
                    if base <= location < base + size:
                        relocations.append({"offset": location - base, "type": relocation["r_info_type"],
                                            "symbol": relocation_symbols.get_symbol(relocation["r_info_sym"]).name})
            functions[symbol.name] = {"bytes": elf.get_section(index).data()[start:start + size],
                                      "name": symbol.name,
                                      "address": address,
                                      "relocations": relocations,
                                      "has_relocations": bool(relocations)}
    return functions


def main():
    config = json.loads((ROOT / "config/compiler-probes.json").read_text(encoding="utf-8"))
    target = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF").read_bytes()
    expected = json.loads((ROOT / "config/ulus10566.json").read_text())["extra_sha256"]
    if hashlib.sha256(target).hexdigest() != expected:
        raise ValueError("Reference overlay hash mismatch")
    output = ROOT / "analysis/compiler-probes"
    output.mkdir(parents=True, exist_ok=True)
    rows = []
    for name, executable in config["candidates"].items():
        is_snc = name.startswith("pspsnc")
        is_gcc = name.startswith("psp-gcc-")
        flags = [] if is_snc or is_gcc else config["base_flags"]
        optimizations = (["-O2"] if is_gcc else ["-O0", "-O1", "-O2", "-O3", "-O4", "-O5"] if is_snc else config["optimizations"])
        profiles = [(optimization, False) for optimization in optimizations]
        if is_snc:
            profiles += [(optimization, True) for optimization in optimizations]
        for optimization, debug in profiles:
            tag = optimization.lstrip("-").replace(",", "_") + ("_debug" if debug else "")
            profile_flags = [*flags, optimization, *(["-g"] if debug else [])]
            object_path = output / f"{name}_{tag}.o"
            process = compile_object(executable, profile_flags,
                                     config["source"], object_path.relative_to(ROOT))
            (output / f"{name}_{tag}.log").write_text(process.stdout + process.stderr, encoding="utf-8")
            if process.returncode:
                raise RuntimeError(f"{name} {optimization} failed; see its compiler log")
            compiled = functions_from_object(object_path)
            row = {"compiler": name, "optimization": optimization, "debug": debug,
                   "flags": profile_flags, "functions": []}
            for specification in config["functions"]:
                function = compiled[specification["name"]]
                offset = int(specification["address"], 16) - 0x089DD4C0 + 0x54
                expected_bytes = target[offset:offset + specification["size"]]
                data = function["bytes"]
                match = not function["has_relocations"] and data == expected_bytes
                differing_words = sum(data[i:i+4] != expected_bytes[i:i+4] for i in range(0, max(len(data), len(expected_bytes)), 4))
                row["functions"].append({**specification, "compiled_size": len(data), "match": match,
                                         "has_relocations": function["has_relocations"], "differing_words": differing_words})
            matches = sum(function["match"] for function in row["functions"])
            print(f"{name} {optimization}{' -g' if debug else ''}: {matches}/{len(row['functions'])} exact functions", flush=True)
            rows.append(row)
    (output / "results.json").write_text(json.dumps(rows, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
