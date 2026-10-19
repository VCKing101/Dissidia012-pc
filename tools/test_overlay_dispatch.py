"""Keep overlay call roots bound to the selected binary and real lifecycle."""

import copy
import json
import struct
import unittest
from pathlib import Path

from audit_overlay_dispatch import FileImage, inspect_dispatch, validate_dispatch_entries, validate_shutdown_bodies, trace_dispatch_calls

ROOT = Path(__file__).resolve().parents[1]


class PatchedImage:
    def __init__(self, image, changes):
        self.image, self.changes = image, changes

    def word(self, address, executable=False):
        return self.changes.get(address, self.image.word(address, executable))

    def read(self, address, size, executable=False):
        blob = bytearray(self.image.read(address, size, executable))
        for pc, word in self.changes.items():
            if address <= pc and pc+4 <= address+size:
                struct.pack_into("<I", blob, pc-address, word)
        return bytes(blob)


class DispatchChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        scope = json.loads((ROOT / "config/progress.json").read_text())
        cls.specs = {s["id"]: s for s in scope["modules"]}
        cls.review = json.loads((ROOT / "config/overlay-dispatch.json").read_text())["review"]
        cls.inventories = [json.loads((ROOT / s["function_inventory"]).read_text()) for s in scope["modules"]]
        cls.main = next(i for i in cls.inventories if i["module"] == "main")
        spec = cls.specs["main"]
        cls.image = FileImage(ROOT / spec["path"], spec["sha256"])

    def inspect(self, image=None, review=None):
        return inspect_dispatch(self.image if image is None else image, self.main["functions"],
                                self.review if review is None else review)

    def test_real_lifecycle_selects_three_distinct_modules(self):
        report = validate_dispatch_entries(self.inspect(), self.inventories)
        self.assertEqual([s["module"] for s in report["slots"]], ["menu", "battle", "extra"])
        shutdowns = validate_shutdown_bodies(self.image, self.main["functions"], self.review)
        self.assertEqual([len(s["cleared_word_addresses"]) for s in shutdowns], [42, 39, 2])
        self.assertEqual([s["body_bytes"] for s in shutdowns], [340, 316, 20])
        menu = next(i for i in self.inventories if i["module"] == "menu")
        prefix = next(f for f in menu["functions"] if f["address"] == "0x089DD4D0")
        self.assertEqual(prefix["body_bytes"], 152)
        self.assertTrue(prefix["matched_c"])
        self.assertEqual(prefix["primary_owner"], "menu_helpers")

    def test_filename_alias_cannot_change_active_module(self):
        table = int(self.review["filename_table"], 16)
        image = PatchedImage(self.image, {table: self.image.word(table+4)})
        with self.assertRaisesRegex(ValueError, "filename does not identify"):
            self.inspect(image=image)

    def test_one_mode_cannot_be_dropped_from_dispatch_scope(self):
        review = copy.deepcopy(self.review)
        review["slots"].pop()
        with self.assertRaisesRegex(ValueError, "three distinct mode bindings"):
            self.inspect(review=review)

    def test_initializer_selector_cannot_use_shutdown_table(self):
        pc = int(self.review["selectors"]["initialize"]["low_pc"], 16)
        low = (self.image.word(pc) & ~0xFFFF) | 0x85DC
        with self.assertRaisesRegex(ValueError, "different table"):
            self.inspect(image=PatchedImage(self.image, {pc: low}))

    def test_lifecycle_call_cannot_skip_the_overlay_loader(self):
        pc = 0x08933B0C
        word = 0x0C000000 | ((0x0893335C >> 2) & 0x3FFFFFF)
        with self.assertRaisesRegex(ValueError, "lifecycle call target"):
            self.inspect(image=PatchedImage(self.image, {pc: word}))

    def test_shutdown_store_of_an_argument_is_not_a_zero_clear(self):
        pc = 0x08879750
        image = PatchedImage(self.image, {pc: self.image.word(pc) | (5 << 16)})
        with self.assertRaisesRegex(ValueError, "absolute zero-store form"):
            validate_shutdown_bodies(image, self.main["functions"], self.review)

    def test_another_overlay_cannot_rescue_a_missing_menu_entry(self):
        inventories = copy.deepcopy(self.inventories)
        menu = next(i for i in inventories if i["module"] == "menu")
        extra = next(i for i in inventories if i["module"] == "extra")
        self.assertTrue(any(f["address"] == "0x089DD4D0" for f in extra["functions"]))
        menu["functions"] = [f for f in menu["functions"] if f["address"] != "0x089DD4D0"]
        with self.assertRaisesRegex(ValueError, "not a menu function entry"):
            validate_dispatch_entries(self.inspect(), inventories)

    def test_followup_callee_requires_a_real_local_call(self):
        menu = next(i for i in self.inventories if i["module"] == "menu")
        helper = next(f for f in menu["functions"] if f["address"] == "0x08A50118")
        pc = int(helper["dispatch_reachability"]["call_pc"], 16)
        spec = self.specs["menu"]
        image = FileImage(ROOT / spec["path"], spec["sha256"])
        roots = self.inspect()["roots"]["menu"]
        before = trace_dispatch_calls(image, menu["functions"], roots, "menu")
        self.assertIn(helper["id"], {r["id"] for r in before})
        after = trace_dispatch_calls(PatchedImage(image, {pc: 0}), menu["functions"], roots, "menu")
        self.assertNotIn(helper["id"], {r["id"] for r in after})

    def test_reported_additions_have_call_paths_and_no_matched_c_credit(self):
        report = json.loads((ROOT / "docs/decomp/overlay-dispatch-changes.json").read_text())
        self.assertEqual(report["functions_added"], 63)
        self.assertEqual(report["function_bytes_added"], 5072)
        by_id = {f["id"]: f for i in self.inventories for f in i["functions"]}
        for module in report["modules"]:
            for entry in module["added_entries"]:
                # A later root can shorten the canonical path. Check the
                # historical direct call itself rather than requiring the
                # earlier BFS predecessor to remain the preferred route.
                path = entry["reachability"]
                self.assertIn(path["root"], by_id)
                if "call_pc" in path:
                    spec = self.specs[module["module"]]
                    image = FileImage(ROOT / spec["path"], spec["sha256"])
                    pc = int(path["call_pc"], 16)
                    word = image.word(pc, True)
                    self.assertEqual(word >> 26, 3)
                    self.assertEqual(((pc+4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2), int(entry["id"].split(":")[1], 16))
                # Later source integration may match these original mapping
                # additions; their historical call evidence stays valid.
                self.assertEqual(by_id[entry["id"]]["body_bytes"], entry["body_bytes"])


if __name__ == "__main__":
    unittest.main()
