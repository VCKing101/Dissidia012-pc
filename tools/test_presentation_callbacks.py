"""Reject callback-boundary, import, and invocation evidence drift."""
import copy
import json
import unittest
from audit_presentation_callbacks import ROOT, inspect_callbacks
from audit_overlay_dispatch import FileImage
from inspect_imports import inspect


class PresentationCallbackChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.original = json.loads((ROOT / "config/presentation-callback-review.json").read_text())
        scope = json.loads((ROOT / "config/progress.json").read_text())
        specs = {m["id"]: m for m in scope["modules"]}
        cls.images = {m: FileImage(ROOT / specs[m]["path"], cls.original["references"][m]) for m in ("main", "menu")}
        cls.functions = {m: json.loads((ROOT / specs[m]["function_inventory"]).read_text())["functions"] for m in ("main", "menu")}
        cls.imports = inspect(ROOT / specs["main"]["path"])

    def setUp(self): self.review = copy.deepcopy(self.original)
    def check(self): return inspect_callbacks(self.images, self.functions, self.review, self.imports)

    def test_native_entries_and_callback_argument(self):
        rows = self.check()
        self.assertEqual([(r["module"], r["size"]) for r in rows], [("main", 8), ("menu", 24)])
        self.assertEqual(rows[1]["callback_interface"], "void(int)")
        self.assertTrue(all(not r["runtime_verified"] for r in rows))

    def test_changed_callback_address_rejected(self):
        self.review["callbacks"][0]["address"] = "0x08873F80"
        with self.assertRaisesRegex(ValueError, "identity/boundaries"): self.check()

    def test_redirected_menu_address_evidence_rejected(self):
        self.review["callbacks"][1]["instruction_checks"]["0x089DE158"] = "0x24A5E9EC"
        with self.assertRaisesRegex(ValueError, "redirected"): self.check()

    def test_different_callback_argument_rejected(self):
        self.review["event_invocation"]["0x08874074"] = "0x8E250018"
        with self.assertRaisesRegex(ValueError, "argument evidence"): self.check()

    def test_overlapping_neighbor_rejected(self):
        functions = copy.deepcopy(self.functions)
        previous = next(f for f in functions["main"] if f["address"] == "0x08873F54")
        previous["ranges"][0]["end"] = "0x08873F80"
        with self.assertRaisesRegex(ValueError, "neighbor extent"):
            inspect_callbacks(self.images, functions, self.review, self.imports)

    def test_import_stub_cannot_receive_callback_credit(self):
        imports = copy.deepcopy(self.imports)
        imports["libraries"][0]["functions"].append({"address": "0x08873F78"})
        with self.assertRaisesRegex(ValueError, "SDK import"):
            inspect_callbacks(self.images, self.functions, self.review, imports)


if __name__ == "__main__": unittest.main()
