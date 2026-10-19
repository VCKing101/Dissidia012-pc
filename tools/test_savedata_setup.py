"""Reject drift in SDK bindings and native save-slot request observations."""
import copy
import json
import unittest
from audit_savedata_setup import ROOT, inspect_setup
from audit_overlay_dispatch import FileImage
from inspect_imports import inspect as inspect_imports


class SavedataSetupChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.original = json.loads((ROOT / "config/menu-savedata-setup-review.json").read_text())
        cls.image = FileImage(ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF",
                             cls.original["menu_reference_sha256"])
        cls.imports = inspect_imports(ROOT / "analysis/ULUS10566/main/EBOOT.ELF")

    def setUp(self): self.review = copy.deepcopy(self.original)
    def inspect(self): return inspect_setup(self.image, self.imports, self.review)

    def test_pinned_requests_and_buffer_bounds(self):
        report = self.inspect()
        self.assertEqual(report["slot_capacity"], 20)
        self.assertEqual(report["name_list_records"], 21)
        self.assertEqual(report["payload_offset"] + report["payload_capacity"], 0x3FA98)
        self.assertLessEqual(report["cached_timestamp_offset"] + 16, report["observed_allocation_bytes"])
        self.assertTrue(report["source_matching_not_assessed"])

    def test_changed_import_binding_rejected(self):
        self.review["imports"][0]["nid"] = "0x8874DBE0"
        with self.assertRaisesRegex(ValueError, "binding differs"): self.inspect()

    def test_redirected_sdk_call_rejected(self):
        self.review["calls"][0]["target"] = "0x089C5868"
        with self.assertRaisesRegex(ValueError, "redirected"): self.inspect()

    def test_changed_allocation_extent_rejected(self):
        self.review["materialized_constants"][1]["value"] -= 16
        with self.assertRaisesRegex(ValueError, "constant differs"): self.inspect()

    def test_missing_mode_or_loop_check_rejected(self):
        self.review["instruction_checks"].pop()
        with self.assertRaisesRegex(ValueError, "incomplete"): self.inspect()

    def test_changed_name_string_rejected(self):
        self.review["strings"][1]["value"] = "GameData00"
        with self.assertRaisesRegex(ValueError, "string differs"): self.inspect()


if __name__ == "__main__": unittest.main()
