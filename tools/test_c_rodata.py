"""Switch jump tables: placed from C objects at retail addresses, never credited as code."""

import contextlib
import copy
import io
import json
import struct
import unittest
from pathlib import Path
from unittest.mock import patch

import c_rodata
import dc
from match_config import configured_inline_asm_totals, configured_totals
from verify_battle import verify as verify_battle

ROOT = Path(__file__).resolve().parents[1]
BATTLE = ROOT / "build/ULUS10566/OVL_BATTLE_APP.ELF"
TABLES_UNIT = "kind_groups_08a64fb0"


def words(*values):
    return struct.pack(f"<{len(values)}I", *values)


def table(symbols, entries):
    """Synthetic .rodata: entries are .text offsets resolved through R_MIPS_32."""
    return {"bytes": words(*entries), "size": 4 * len(entries), "align": 8, "symbols": symbols,
            "relocations": [{"offset": 4 * i, "type": c_rodata.R_MIPS_32, "symbol": c_rodata.TEXT}
                            for i in range(len(entries))]}


class SyntheticTables(unittest.TestCase):
    # lui $at, 0 / addu $at, $at, $a0 / lw $at, 0($at) with HI16/LO16 against $LSW_2.
    FUNCTION = {"address": 0x08A65000, "bytes": words(0x3C010000, 0x00240821, 0x8C210000),
                "relocations": [{"offset": 0, "type": c_rodata.R_MIPS_HI16, "symbol": "$LSW_2"},
                                {"offset": 8, "type": c_rodata.R_MIPS_LO16, "symbol": "$LSW_2"}]}
    RETAIL = {0x08A65000: 0x3C0108A6, 0x08A65004: 0x00240821, 0x08A65008: 0x8C215688}

    def test_entries_resolve_against_the_units_text_address(self):
        rodata = table({"$LSW_1": 0}, [0x3C, 0x44])
        self.assertEqual(c_rodata.resolve_rodata(rodata, {c_rodata.TEXT: 0x08A64FB0}),
                         words(0x08A64FEC, 0x08A64FF4))
        with self.assertRaisesRegex(ValueError, "Unknown .rodata relocation target"):
            c_rodata.resolve_rodata(rodata, {})

    def test_address_comes_from_the_retail_pair_at_the_same_instructions(self):
        rodata = table({"$LSW_1": 0, "$LSW_2": 0x28}, [0] * 20)
        self.assertEqual(c_rodata.derive_address(rodata, [self.FUNCTION], self.RETAIL.get), [0x08A65660])

    def test_a_different_retail_instruction_gives_no_address(self):
        rodata = table({"$LSW_2": 0x28}, [0] * 20)
        retail = dict(self.RETAIL)
        retail[0x08A65008] = 0x8C245688  # lw into another register: not the same instruction
        self.assertEqual(c_rodata.derive_address(rodata, [self.FUNCTION], retail.get), [])
        self.assertEqual(c_rodata.derive_address(rodata, [self.FUNCTION], {}.get), [])

    def test_declarations_are_strict(self):
        unit = {"name": "u", "functions": []}
        self.assertIsNone(c_rodata.unit_rodata(unit))
        self.assertEqual(c_rodata.unit_rodata({**unit, "rodata": {"address": "0x08A65660", "size": 120}}), (0x08A65660, 120))
        for bad in ({"address": "0x08A65662", "size": 120}, {"address": "0x08A65660", "size": 6},
                    {"address": "0x08A65660", "size": True}, {"address": "0x08A65660", "size": 120, "credit": 1},
                    [{"address": "0x08A65660", "size": 120}]):
            with self.assertRaises(ValueError):
                c_rodata.unit_rodata({**unit, "rodata": bad})

    def test_dc_check_places_only_an_exact_table(self):
        with contextlib.redirect_stdout(io.StringIO()):
            self.check_placement()

    def check_placement(self):
        base = 0x08A65000
        payload = bytearray(0x800)
        struct.pack_into("<2I", payload, 0x660, 0x08A64FEC, 0x08A64FF4)
        state = {"rodata": table({"$LSW_1": 0}, [0x3C, 0x44]), "address": 0x08A65660,
                 "candidates": [0x08A65660], "text_start": 0x08A64FB0, "symbols": {}}
        self.assertEqual(dc.compare_rodata(state, {}, base, bytes(payload), verbose=False),
                         {"address": "0x08A65660", "size": 8})
        payload[0x664] ^= 4
        self.assertIsNone(dc.compare_rodata(state, {}, base, bytes(payload), verbose=False))
        self.assertIsNone(dc.compare_rodata({**state, "address": None}, {}, base, bytes(payload), verbose=False))
        self.assertIsNone(dc.compare_rodata({**state, "text_start": None}, {}, base, bytes(payload), verbose=False))
        self.assertIsNone(dc.compare_rodata({"error": "unsupported", "symbols": {}}, {}, base, bytes(payload), verbose=False))


class BuiltTables(unittest.TestCase):
    """Uses the verified battle build (tools/build_battle.py)."""

    @classmethod
    def setUpClass(cls):
        matches = json.loads((ROOT / "config/battle.matches.json").read_text())
        cls.matches = matches
        cls.unit = next(u for u in matches["units"] if u["name"] == TABLES_UNIT)
        cls.raw = ROOT / f"build/ULUS10566/battle_{TABLES_UNIT}.raw.o"
        spec = json.loads((ROOT / "config/battle.reference.json").read_text())
        reference = (ROOT / spec["path"]).read_bytes()
        base, offset = int(spec["payload_address"], 16), spec["payload_file_offset"]
        cls.retail = staticmethod(lambda address, size: reference[offset + address - base:offset + address - base + size])

    def test_snc_emits_named_local_tables_with_text_relative_entries(self):
        rodata = c_rodata.read_rodata(self.raw)
        self.assertEqual(rodata["size"], 120)
        self.assertEqual({k: v for k, v in rodata["symbols"].items() if k.startswith("$LSW")},
                         {"$LSW_1": 0, "$LSW_2": 0x28, "$LSW_3": 0x50})
        self.assertTrue(rodata["relocations"])
        self.assertTrue(all(r["type"] == c_rodata.R_MIPS_32 and r["symbol"] == c_rodata.TEXT for r in rodata["relocations"]))

    def test_declared_tables_match_retail_and_resolve_the_function_pairs(self):
        local, placed = c_rodata.check_unit(self.raw, self.unit, {}, self.retail)
        self.assertEqual(placed, {"start": 0x08A65660, "end": 0x08A656D8})
        self.assertEqual(local["$LSW_2"], 0x08A65688)

    def test_wrong_or_missing_declarations_are_rejected(self):
        moved = copy.deepcopy(self.unit)
        moved["rodata"]["address"] = "0x08A65668"
        with self.assertRaisesRegex(ValueError, "read-only data does not match"):
            c_rodata.check_unit(self.raw, moved, {}, self.retail)
        resized = copy.deepcopy(self.unit)
        resized["rodata"]["size"] = 112
        with self.assertRaisesRegex(ValueError, "declared 112"):
            c_rodata.check_unit(self.raw, resized, {}, self.retail)
        undeclared = {k: v for k, v in self.unit.items() if k != "rodata"}
        with self.assertRaisesRegex(ValueError, "disagree"):
            c_rodata.check_unit(self.raw, undeclared, {}, self.retail)

    def test_table_bytes_are_data_not_c_credit(self):
        report = verify_battle(BATTLE)
        declared = sum(u["rodata"]["size"] for u in self.matches["units"] if u.get("rodata"))
        self.assertGreater(declared, 0)
        self.assertEqual(report["c_rodata_bytes"], declared)
        self.assertEqual((report["recovered_c_functions"], report["recovered_c_bytes"]), configured_totals("battle"))
        self.assertEqual(sum(report[k] for k in ("recovered_c_bytes", "inline_asm_bytes", "c_rodata_bytes", "assembly_instruction_bytes",
                                                 "raw_instruction_fallback_bytes", "opaque_original_bytes")),
                         report["reconstructed_payload_bytes"])

    def test_altered_raw_table_cannot_hide_behind_exact_game_bytes(self):
        original = c_rodata.read_rodata

        def altered(path):
            rodata = original(path)
            if rodata and Path(path).name == self.raw.name:
                rodata = {**rodata, "bytes": bytes([rodata["bytes"][0] ^ 4]) + rodata["bytes"][1:]}
            return rodata
        with patch("c_rodata.read_rodata", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "read-only data does not match"):
                verify_battle(BATTLE)

    def test_every_module_places_declared_tables_outside_c_credit(self):
        for module in ("battle", "menu", "main"):
            matches = json.loads((ROOT / f"config/{module}.matches.json").read_text())
            prepared = json.loads((ROOT / f"build/ULUS10566/{module}.prepare.json").read_text())
            declared = sorted((int(u["rodata"]["address"], 16), u["rodata"]["size"], u["name"])
                              for u in matches["units"] if u.get("rodata"))
            placed = sorted((int(p["start"], 16), int(p["end"], 16) - int(p["start"], 16), p["unit"])
                            for p in prepared["parts"] if p["kind"] == "matching-c-rodata")
            self.assertTrue(declared, module)
            self.assertEqual(placed, declared, module)
            self.assertEqual(prepared["matching_c_rodata_bytes"], sum(size for _, size, _ in declared))
            self.assertEqual(prepared["matching_c_bytes"],
                             configured_totals(module)[1] + configured_inline_asm_totals(module)[1])

    def test_table_parts_cannot_be_relabelled(self):
        loader = json.loads

        def altered(text, *args, **kwargs):
            value = loader(text, *args, **kwargs)
            if isinstance(value, dict) and value.get("module") == "battle" and "parts" in value:
                next(p for p in value["parts"] if p["kind"] == "matching-c-rodata")["kind"] = "opaque-original-bytes"
            return value
        with patch("verify_battle.json.loads", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "read-only data parts differ"):
                verify_battle(BATTLE)


if __name__ == "__main__":
    unittest.main()
