"""Check the fixed local denominator and reject drift in callback evidence."""
import copy
import json
import unittest
from audit_overlay_dispatch import FileImage
from menu_controller_scope import ROOT, inspect_controller_scope


class ControllerScopeTests(unittest.TestCase):
    def setUp(self):
        self.scope = json.loads((ROOT / "config/menu-controller-scope.json").read_text())
        self.review = json.loads((ROOT / "config/menu-member-table-review.json").read_text())
        self.functions = json.loads((ROOT / "config/inventory/menu.json").read_text())["functions"]
        self.image = FileImage(ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF", self.scope["reference_sha256"])
        # No source proof is claimed by this boundary-audit test fixture.
        self.verified = {"sha256": self.scope["reference_sha256"], "byte_identical": True, "c_functions": []}

    def inspect(self):
        return inspect_controller_scope(self.image, self.functions, self.review, self.scope, self.verified)

    def test_fixed_table_and_extent_denominator(self):
        report = self.inspect()
        self.assertEqual(report["handler_count"], 20)
        self.assertEqual(report["reviewed_handler_bytes"], 8188)
        self.assertEqual(report["matching_handlers"], 0)

    def test_empty_callback_has_eight_bytes_of_source_credit(self):
        self.verified["c_functions"] = [{"address": "0x089DE3A4", "size": 8}]
        report = self.inspect()
        self.assertEqual(report["matching_handlers"], 1)
        self.assertEqual(report["matching_handler_bytes"], 8)
        self.assertEqual(report["handler_count_percent"], 5)
        self.assertLess(report["handler_byte_percent"], 0.1)

    def test_changed_target_rejected(self):
        self.scope["entries"][0]["address"] = "0x089DD70C"
        with self.assertRaises(ValueError): self.inspect()

    def test_shortened_extent_rejected(self):
        self.scope["entries"][0]["body_bytes"] -= 4
        with self.assertRaisesRegex(ValueError, "boundary"): self.inspect()

    def test_wrong_source_extent_rejected(self):
        self.verified["c_functions"] = [{"address": "0x089DE3A4", "size": 12}]
        with self.assertRaisesRegex(ValueError, "source proof"): self.inspect()

    def test_different_build_identity_rejected(self):
        self.verified["sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "pinned exact"): self.inspect()


if __name__ == "__main__": unittest.main()
