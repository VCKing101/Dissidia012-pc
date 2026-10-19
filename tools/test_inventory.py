"""Check range accounting and the maintained inventories against real inputs."""

import json
import unittest
from pathlib import Path

from build_inventory import byte_count, decode, difference, intersection, merged
from audit_layout import checked_descriptor, validate_extra_layout, validate_overlay_hooks
import copy
import struct

ROOT = Path(__file__).resolve().parents[1]


class RangeChecks(unittest.TestCase):
    def test_overlaps_are_counted_once(self):
        ranges = [(0x1000, 0x1010), (0x1008, 0x1018), (0x1018, 0x1020)]
        self.assertEqual(merged(ranges), [(0x1000, 0x1020)])
        self.assertEqual(byte_count(ranges), 32)

    def test_discontiguous_function_does_not_credit_its_hole(self):
        self.assertEqual(byte_count([(0x1000, 0x1008), (0x1020, 0x1028)]), 16)

    def test_code_data_and_unknown_partition_exactly(self):
        whole, code, data = [(0x1000, 0x1020)], [(0x1000, 0x100C)], [(0x1010, 0x1014)]
        unknown = difference(whole, code + data)
        self.assertEqual(unknown, [(0x100C, 0x1010), (0x1014, 0x1020)])
        self.assertFalse(intersection(code, data))
        self.assertEqual(byte_count(code) + byte_count(data) + byte_count(unknown), byte_count(whole))

    def test_invalid_range_is_rejected(self):
        with self.assertRaises(ValueError):
            merged([(0x1010, 0x1000)])


class InventoryChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.modules = [json.loads((ROOT / f"config/inventory/{module}.json").read_text())
                       for module in ("main", "extra", "menu", "battle")]

    def test_module_qualified_function_identities_are_unique(self):
        ids = [f["id"] for m in self.modules for f in m["functions"]]
        self.assertEqual(len(ids), len(set(ids)))
        # All three overlays share the same address space, yet remain distinct.
        addresses = [{f["address"] for f in m["functions"]} for m in self.modules if m["module"] != "main"]
        self.assertTrue(addresses[0] & addresses[1] | addresses[1] & addresses[2])

    def test_all_real_sections_are_partitioned_without_omitting_unknown_bytes(self):
        for module in self.modules:
            s = module["summary"]
            self.assertEqual(s["decoded_instruction_bytes"] + s["defined_data_bytes_in_executable_sections"] +
                             s.get("padding_or_zero_fill_bytes", 0) +
                             s["unclassified_executable_bytes"], s["executable_section_bytes"])
            self.assertEqual(byte_count(decode(module["function_body_ranges"])), s["function_body_bytes_union"])
            self.assertEqual(module["audited"], module["module"] == "extra")

    def test_import_reconciliation_does_not_count_as_c_source(self):
        main = self.modules[0]
        self.assertEqual(main["summary"]["platform_import_functions"], 296)
        self.assertEqual(main["elf_declared_imports_added"], 34)
        self.assertFalse(any(f["matched_c"] for f in main["functions"] if f["primary_owner"] == "platform_imports"))

    def test_reviewed_extra_boundaries_do_not_inflate_matched_source(self):
        extra = self.modules[1]
        self.assertEqual(extra["function_count"], 100)
        self.assertEqual(extra["reviewed_functions_added"], 3)
        self.assertEqual(sum(f["matched_c"] for f in extra["functions"]), 24)


class LayoutAuditChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.base = 0x089DD4C0
        cls.payload = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF").read_bytes()[0x54:0x5F4C]
        cls.inventory = json.loads((ROOT / "config/inventory/extra.json").read_text())
        cls.review = json.loads((ROOT / "config/inventory-overrides.json").read_text())["modules"]["extra"]["layout_audit"]
        cls.code = decode(cls.inventory["function_body_ranges"])
        cls.starts = {int(f["address"], 16) for f in cls.inventory["functions"]}

    def validate(self, payload=None, review=None):
        return validate_extra_layout(payload if payload is not None else self.payload, self.base,
                                     self.code, self.starts, review if review is not None else self.review)

    def test_whole_extra_payload_has_an_exact_reviewed_partition(self):
        report = self.validate()
        self.assertEqual([byte_count(report[k]) for k in ("code", "data", "padding")], [22796, 1476, 40])
        self.assertEqual(self.inventory["summary"]["unclassified_executable_bytes"], 0)

    def test_nonzero_padding_is_rejected(self):
        payload = bytearray(self.payload)
        payload[0x089DE744-self.base] = 1
        with self.assertRaisesRegex(ValueError, "nonzero bytes"):
            self.validate(payload=payload)

    def test_data_address_is_not_accepted_as_a_jump_target(self):
        payload = bytearray(self.payload)
        struct.pack_into("<I", payload, 0x089E32A8-self.base, 0x089DE748)
        with self.assertRaisesRegex(ValueError, "Jump target"):
            self.validate(payload=payload)

    def test_interior_code_address_is_not_accepted_as_a_member_entry(self):
        payload = bytearray(self.payload)
        struct.pack_into("<I", payload, 0x089E3314-self.base, 0x089DFA20)
        with self.assertRaisesRegex(ValueError, "Dispatch record"):
            self.validate(payload=payload)

    def test_review_cannot_drop_a_byte_from_the_denominator(self):
        review = copy.deepcopy(self.review)
        review["data_regions"][1]["end"] = "0x089DE74C"
        with self.assertRaisesRegex(ValueError, "overlap or a gap"):
            self.validate(review=review)

    def test_nonempty_descriptor_table_must_stay_inside_the_payload(self):
        descriptor = struct.pack("<IHHII", 2, 1, 0, self.base+len(self.payload), 0)
        with self.assertRaisesRegex(ValueError, "table leaves"):
            checked_descriptor(descriptor, self.base)


class OverlayHookChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.base = 0x089DD4C0
        cls.payloads, cls.starts, cls.inventories = {}, {}, {}
        main = json.loads((ROOT / "config/inventory/main.json").read_text())
        cls.main_starts = {int(f["address"], 16) for f in main["functions"]}
        for module, size in (("menu", 1555864), ("battle", 1596080)):
            cls.payloads[module] = (ROOT / f"orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_{module.upper()}_APP.ELF").read_bytes()[0x54:0x54+size]
            cls.inventories[module] = json.loads((ROOT / f"config/inventory/{module}.json").read_text())
            cls.starts[module] = {int(f["address"], 16) for f in cls.inventories[module]["functions"]}

    def validate(self, module="battle", payload=None, main_starts=None):
        return validate_overlay_hooks(payload if payload is not None else self.payloads[module], self.base, module,
                                      self.starts[module], self.main_starts if main_starts is None else main_starts)

    def test_all_loader_hooks_resolve_and_data_is_outside_code(self):
        for module in ("menu", "battle"):
            report = self.validate(module)
            inventory = self.inventories[module]
            self.assertEqual(report, inventory["hook_audit"])
            self.assertFalse(intersection(decode(report["data_regions"]), decode(inventory["instruction_ranges"])))
            self.assertFalse(inventory["audited"])
            # Source credit may subsequently arrive through a verified build;
            # the table audit itself must remain a partial layout audit.
            self.assertEqual(sum(f["matched_c"] for f in inventory["functions"]), inventory["summary"]["matched_c_functions"])

    def test_cleanup_record_and_callback_have_distinct_module_identities(self):
        report = self.validate()
        self.assertEqual(report["cleanup"][-1]["callback"], "main:0x0887A644")
        self.assertEqual(report["cleanup"][-1]["a0"], "0x09B6C2B0")
        for record in report["cleanup"]:
            self.assertNotIn(int(record["record"], 16), self.starts["battle"])

    def test_interior_initializer_address_is_rejected(self):
        payload = bytearray(self.payloads["menu"])
        struct.pack_into("<I", payload, 0x08B59238-self.base, 0x08A08EEC)
        with self.assertRaisesRegex(ValueError, "Initializer"):
            self.validate("menu", payload)

    def test_cleanup_record_cannot_leave_payload(self):
        payload = bytearray(self.payloads["battle"])
        struct.pack_into("<I", payload, 0x08B62F5C-self.base, self.base+len(payload)-8)
        with self.assertRaisesRegex(ValueError, "Cleanup record leaves"):
            self.validate(payload=payload)

    def test_cleanup_callback_into_data_is_rejected(self):
        payload = bytearray(self.payloads["battle"])
        struct.pack_into("<I", payload, 0x08A4AAD8-self.base, 0x08A4AAD0)
        with self.assertRaisesRegex(ValueError, "Cleanup callback"):
            self.validate(payload=payload)

    def test_missing_main_callback_cannot_resolve_from_overlay(self):
        with self.assertRaisesRegex(ValueError, "Cleanup callback"):
            self.validate(main_starts=set())

    def test_overlapping_hook_data_is_rejected(self):
        payload = bytearray(self.payloads["battle"])
        struct.pack_into("<I", payload, 0x08B62F60-self.base, 0x08A4AAD0)
        with self.assertRaisesRegex(ValueError, "data regions overlap"):
            self.validate(payload=payload)

    def test_truncated_descriptor_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "truncated"):
            checked_descriptor(b"\0" * 15, self.base)

    def test_unaligned_nonempty_table_is_rejected(self):
        payload = bytearray(self.payloads["menu"])
        struct.pack_into("<I", payload, 8, 0x08B59237)
        with self.assertRaisesRegex(ValueError, "table leaves"):
            self.validate("menu", payload)


if __name__ == "__main__":
    unittest.main()
