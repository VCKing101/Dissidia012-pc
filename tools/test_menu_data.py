"""Reject unsafe menu data classifications and consumer/schema drift."""

import copy
import json
import struct
import unittest
from pathlib import Path

from audit_menu_data import validate_menu_data, elf_word_reader
from build_inventory import build_module, byte_count, decode, intersection

ROOT = Path(__file__).resolve().parents[1]


class MenuDataChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        scope = json.loads((ROOT / "config/progress.json").read_text())
        cls.specs = {m["id"]: m for m in scope["modules"]}
        cls.override = json.loads((ROOT / "config/inventory-overrides.json").read_text())["modules"]["menu"]
        cls.review = cls.override["data_island_audit"]
        cls.inventory = json.loads((ROOT / "config/inventory/menu.json").read_text())
        main = json.loads((ROOT / "config/inventory/main.json").read_text())
        cls.main_starts = {int(f["address"], 16) for f in main["functions"]}
        cls.local_starts = {int(f["address"], 16) for f in cls.inventory["functions"]}
        cls.base = 0x089DD4C0
        cls.payload = (ROOT / cls.specs["menu"]["path"]).read_bytes()[0x54:0x54+1555864]
        cls.read_main = staticmethod(elf_word_reader(ROOT / cls.specs["main"]["path"], cls.specs["main"]["sha256"]))

    def validate(self, payload=None, review=None, read_main=None):
        return validate_menu_data(self.payload if payload is None else payload, self.base,
                                  self.review if review is None else review,
                                  self.read_main if read_main is None else read_main,
                                  self.main_starts, self.local_starts)

    def test_real_table_readers_partition_two_islands_without_source_credit(self):
        report = self.validate()
        self.assertEqual(len(report["tables"]), 60)
        self.assertEqual(byte_count(report["data"]), 8265)
        self.assertEqual(byte_count(report["padding"]), 5)
        self.assertFalse(intersection(report["data"]+report["padding"], decode(self.inventory["instruction_ranges"])))
        self.assertFalse(self.inventory["audited"])
        # Source recovery can subsequently use these data references. The
        # partial data audit never makes the entire module audited.
        self.assertEqual(sum(f["matched_c"] for f in self.inventory["functions"]), self.inventory["summary"]["matched_c_functions"])

    def test_early_sentinel_changes_table_boundary_and_is_rejected(self):
        payload = bytearray(self.payload)
        struct.pack_into("<H", payload, 0x08A08FDA-self.base, 0xFFFF)
        with self.assertRaisesRegex(ValueError, "first consumer-visible sentinel"):
            self.validate(payload=payload)

    def test_removed_terminal_record_is_rejected(self):
        review = copy.deepcopy(self.review)
        review["data_regions"][1]["end"] = "0x08A090FE"
        with self.assertRaisesRegex(ValueError, "first consumer-visible sentinel"):
            self.validate(review=review)

    def test_offset_four_sentinel_cannot_be_reported_as_offset_zero(self):
        review = copy.deepcopy(self.review)
        region = next(r for r in review["data_regions"] if r.get("sentinel_offset") == 4)
        region["sentinel_offset"] = 0
        with self.assertRaisesRegex(ValueError, "schema differs"):
            self.validate(review=review)

    def test_changed_main_reader_stride_is_rejected(self):
        address = int(self.review["readers"]["main:0x088D3910"]["stride_pc"], 16)
        def altered(pc):
            return (self.read_main(pc) & ~0xFFFF) | 8 if pc == address else self.read_main(pc)
        with self.assertRaisesRegex(ValueError, "stride/sentinel"):
            self.validate(read_main=altered)

    def test_call_to_different_reader_is_rejected(self):
        payload = bytearray(self.payload)
        region = self.review["data_regions"][1]
        pc = int(region["evidence"]["call_pc"], 16)
        struct.pack_into("<I", payload, pc-self.base, 0x0C000000 | ((0x088D3B88 >> 2) & 0x3FFFFFF))
        with self.assertRaisesRegex(ValueError, "call no longer reaches"):
            self.validate(payload=payload)

    def test_embedded_code_pointer_must_still_resolve_to_initializer(self):
        payload = bytearray(self.payload)
        struct.pack_into("<I", payload, 0x08A94668-self.base, 0x08A93DF8)
        with self.assertRaisesRegex(ValueError, "Embedded initializer pointer"):
            self.validate(payload=payload)

    def test_reviewed_data_cannot_swallow_the_following_function(self):
        override = copy.deepcopy(self.override)
        override["data_island_audit"]["data_regions"][-1]["end"] = "0x08A946F8"
        with self.assertRaisesRegex(ValueError, "intersects code"):
            build_module(self.specs["menu"], {}, override, self.main_starts, self.specs["main"])

    def test_wrong_main_reference_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "reference hash mismatch"):
            elf_word_reader(ROOT / self.specs["main"]["path"], "0"*64)


if __name__ == "__main__":
    unittest.main()
