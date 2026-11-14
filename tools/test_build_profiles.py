"""Per-unit build profiles: main's SDK libraries are compiled with Sony psp-gcc, the rest with SNC."""

import json
import struct
import tempfile
import unittest
from pathlib import Path

import c_rodata
import compile_psp
import dc
from mips_relocations import resolve_function

ROOT = Path(__file__).resolve().parents[1]
GCC = "tools/compilers/psp-gcc-1.3.1/bin/psp-gcc"


def symtab_object(bindings, first_global):
    """A minimal ELF32 object holding one .symtab; bindings are STB values per symbol."""
    entries = b"".join(struct.pack("<IIIBBH", 0, 0, 0, binding << 4, 0, 0) for binding in bindings)
    shoff = 52 + len(entries)
    header = bytearray(52)
    header[:4] = b"\x7fELF"
    struct.pack_into("<I", header, 32, shoff)
    struct.pack_into("<HH", header, 46, 40, 2)
    null = bytes(40)
    symtab = struct.pack("<IIIIIIIIII", 0, 2, 0, 0, 52, len(entries), 0, first_global, 4, 16)
    return bytes(header) + entries + null + symtab


def first_global(data):
    shoff, = struct.unpack_from("<I", data, 32)
    return struct.unpack_from("<I", data, shoff + 40 + 28)[0]


class SymbolTableRepair(unittest.TestCase):
    def test_old_gnu_as_off_by_one_is_corrected(self):
        # The FILE symbol at index 3 is local, but sh_info says globals start there.
        data = symtab_object([0, 0, 0, 0, 1, 1], first_global=3)
        self.assertEqual(first_global(compile_psp.repair_local_symbol_count(data)), 4)

    def test_a_correct_table_is_unchanged(self):
        data = symtab_object([0, 0, 1, 1], first_global=2)
        self.assertEqual(compile_psp.repair_local_symbol_count(data), data)

    def test_locals_after_globals_are_refused(self):
        with self.assertRaises(ValueError):
            compile_psp.repair_local_symbol_count(symtab_object([0, 1, 0, 1], first_global=1))


class ProfileSelection(unittest.TestCase):
    MATCHES = {"compiler": "tools/compilers/pspsnc_1.2.7503.0/pspsnc.exe", "flags": ["-O2", "-g"],
               "profiles": {"sdk_gcc": {"compiler": GCC, "flags": ["-O2", "-G0"]}}}

    def test_units_without_a_profile_use_the_pinned_compiler(self):
        self.assertEqual(compile_psp.unit_compiler(self.MATCHES, {"name": "a"}),
                         ("tools/compilers/pspsnc_1.2.7503.0/pspsnc.exe", ["-O2", "-g"]))

    def test_named_profiles_select_their_compiler(self):
        self.assertEqual(compile_psp.unit_compiler(self.MATCHES, {"name": "a", "profile": "sdk_gcc"}), (GCC, ["-O2", "-G0"]))

    def test_unknown_profiles_are_errors(self):
        with self.assertRaises(ValueError):
            compile_psp.unit_compiler(self.MATCHES, {"name": "a", "profile": "missing"})

    def test_gcc_toolchain_layout(self):
        self.assertEqual(compile_psp.gcc_toolchain(GCC),
                         ("tools/compilers/psp-gcc-1.3.1", "bin/psp-gcc",
                          "lib/gcc-lib/psp/3.3.3+allegrex-2.2.1-psp-1.3.1", "bin/psp-as"))
        with self.assertRaises(ValueError):
            compile_psp.gcc_toolchain("tools/compilers/psp-gcc-9.9.9/bin/psp-gcc")

    def test_compiler_probes_keep_their_own_gcc_path(self):
        # tools/probe_compilers.py passes only an optimisation level; build profiles pass -G0.
        self.assertTrue(compile_psp._probe_gcc(GCC, ["-O2"]))
        self.assertFalse(compile_psp._probe_gcc(GCC, ["-O2", "-G0"]))
        self.assertFalse(compile_psp._probe_gcc("tools/compilers/pspsnc_1.2.7503.0/pspsnc.exe", ["-O2"]))

    def test_provenance_covers_the_gcc_driver_cc1_and_assembler(self):
        files = compile_psp.compiler_files(GCC)
        self.assertIn("tools/compilers/psp-gcc-1.3.1/bin/psp-as", files)
        self.assertTrue(any(f.endswith("/cc1") for f in files))

    def test_address_in_the_file_name_selects_the_profile(self):
        self.assertEqual(dc.source_profile("main", "src/main/sdk/vec_add_089a38a8.c"), "sdk_gcc")
        self.assertEqual(dc.source_profile("main", "src/main/sdk/gu_089a2678.c"), "sdk_gcc_o3")
        self.assertIsNone(dc.source_profile("main", "src/main/auto/main_08804040.c"))
        self.assertIsNone(dc.source_profile("main", "src/main/sdk/vec_add_089a38a8.c", "default"))
        self.assertIsNone(dc.source_profile("battle", "src/battle/x_089a2678.c"))

    def test_func_names_select_the_profile_when_the_name_has_no_address(self):
        with tempfile.TemporaryDirectory() as directory:
            inside = Path(directory) / "library.c"
            inside.write_text("int func_089A38A8(void) { return func_089A38C4(); }\n")
            mixed = Path(directory) / "mixed.c"
            mixed.write_text("int func_089A38A8(void) { return func_08804040(); }\n")
            self.assertEqual(dc.source_profile("main", str(inside)), "sdk_gcc")
            self.assertIsNone(dc.source_profile("main", str(mixed)))


class InUnitJumps(unittest.TestCase):
    def test_a_jump_to_a_label_resolves_against_the_units_text(self):
        # j .L+0x10 assembled as R_MIPS_26 against .text with the offset as addend.
        function = {"name": "func_089A0000", "bytes": struct.pack("<I", 0x08000000 | (0x10 >> 2)),
                    "relocations": [{"offset": 0, "type": 4, "symbol": c_rodata.TEXT}]}
        resolved = resolve_function(function, {"func_089A0000": 0x089A0000, c_rodata.TEXT: 0x089A0000})
        self.assertEqual(struct.unpack("<I", resolved)[0], 0x08000000 | (0x089A0010 >> 2) & 0x03FFFFFF)


class MainConfiguration(unittest.TestCase):
    def test_profile_units_name_a_profile_and_lie_in_its_range(self):
        matches = json.loads((ROOT / "config/main.matches.json").read_text())
        for unit in matches["units"]:
            if "profile" not in unit:
                continue
            self.assertIn(unit["profile"], matches["profiles"], unit["name"])
            for function in unit["functions"]:
                self.assertEqual(dc.range_profile("main", int(function["address"], 16)), unit["profile"], unit["name"])

    def test_the_sdk_profiles_are_sony_gcc_without_small_data(self):
        profiles = json.loads((ROOT / "config/main.matches.json").read_text())["profiles"]
        self.assertEqual(profiles["sdk_gcc"], {**profiles["sdk_gcc"], "compiler": GCC, "flags": ["-O2", "-G0"]})
        self.assertEqual(profiles["sdk_gcc_o3"], {**profiles["sdk_gcc_o3"], "compiler": GCC, "flags": ["-O3", "-G0"]})

    def test_the_graphics_span_takes_the_o3_profile(self):
        self.assertEqual(dc.range_profile("main", 0x0899F3F8), "sdk_gcc")
        self.assertEqual(dc.range_profile("main", 0x0899F3FC), "sdk_gcc_o3")
        self.assertEqual(dc.range_profile("main", 0x089A2C20), "sdk_gcc_o3")
        self.assertEqual(dc.range_profile("main", 0x089A2C24), "sdk_gcc")


if __name__ == "__main__":
    unittest.main()
