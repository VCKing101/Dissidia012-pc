"""Reject changes that sever the deferred callback or its overlay barrier."""

import copy
import json
import unittest
from pathlib import Path

from audit_deferred_battle import inspect_deferred_battle
from audit_overlay_dispatch import FileImage
from test_overlay_dispatch import PatchedImage

ROOT = Path(__file__).resolve().parents[1]


class DeferredBattleChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        scope = json.loads((ROOT / "config/progress.json").read_text())
        spec = next(s for s in scope["modules"] if s["id"] == "main")
        cls.main = json.loads((ROOT / spec["function_inventory"]).read_text())
        cls.image = FileImage(ROOT / spec["path"], spec["sha256"])
        cls.review = json.loads((ROOT / "config/deferred-battle.json").read_text())
        cls.slots = json.loads((ROOT / "config/overlay-dispatch.json").read_text())["review"]["slots"]

    def inspect(self, image=None, review=None, slots=None):
        return inspect_deferred_battle(
            self.image if image is None else image, self.main["functions"],
            self.review if review is None else review,
            self.slots if slots is None else slots)

    def test_real_callbacks_and_battle_prefix_have_separate_namespaces(self):
        report = self.inspect()
        self.assertEqual([c["phase"] for c in report["callbacks"]], ["setup", "update", "teardown"])
        self.assertEqual([c["controller_field_offset"] for c in report["callbacks"]], [28, 32, 36])
        self.assertEqual(report["prefix"], "battle:0x089DD4D0")
        self.assertTrue(all(r["caller"] == "main:0x0888C4C8" for r in report["roots"]))
        self.assertEqual(len(report["roots"]), 60)
        self.assertEqual(len({r["address"] for r in report["roots"]}), 58)

    def test_registration_cannot_replace_setup_wrapper_with_update(self):
        pc = 0x088896C8
        image = PatchedImage(self.image, {pc: 0x24A590B8})
        with self.assertRaisesRegex(ValueError, "Registered callback pointer"):
            self.inspect(image=image)

    def test_setter_cannot_store_setup_callback_in_update_slot(self):
        with self.assertRaisesRegex(ValueError, "callback setter slots"):
            self.inspect(image=PatchedImage(self.image, {0x0886503C: 0xAC85001C}))

    def test_callback_runner_requires_a_null_guard(self):
        with self.assertRaisesRegex(ValueError, "runner null guard"):
            self.inspect(image=PatchedImage(self.image, {0x08864F84: 0}))

    def test_member_record_cannot_alias_another_runner(self):
        with self.assertRaisesRegex(ValueError, "member record"):
            self.inspect(image=PatchedImage(self.image, {0x089C9354: 0x08864FA0}))

    def test_constructor_cannot_install_another_member_table(self):
        with self.assertRaisesRegex(ValueError, "member table low address"):
            self.inspect(image=PatchedImage(self.image, {0x08864FF4: 0x25089350}))

    def test_ensure_cannot_skip_loading_battle_mode(self):
        with self.assertRaisesRegex(ValueError, "ensure-mode control flow"):
            self.inspect(image=PatchedImage(self.image, {0x08933C44: 0x34050000}))

    def test_setup_entry_cannot_branch_around_ensure(self):
        with self.assertRaisesRegex(ValueError, "setup entry before mode barrier"):
            self.inspect(image=PatchedImage(self.image, {0x0888C4CC: 0x1000000D}))

    def test_ensure_manager_argument_cannot_be_an_overlay_object(self):
        with self.assertRaisesRegex(ValueError, "ensure manager argument"):
            self.inspect(image=PatchedImage(self.image, {0x0888C500: 0x8C8485EC}))

    def test_mode_binding_cannot_resolve_shared_address_to_menu(self):
        slots = copy.deepcopy(self.slots)
        slots[1]["module"] = "menu"
        with self.assertRaisesRegex(ValueError, "another overlay"):
            self.inspect(slots=slots)

    def test_setup_cannot_switch_overlay_before_prefix(self):
        word = 0x0C000000 | ((0x08933ACC >> 2) & 0x3FFFFFF)
        with self.assertRaisesRegex(ValueError, "another direct overlay transition"):
            self.inspect(image=PatchedImage(self.image, {0x0888C558: word}))

    def test_phase_labels_cannot_reorder_callback_evidence(self):
        review = copy.deepcopy(self.review)
        review["callbacks"][0]["phase"] = "update"
        with self.assertRaisesRegex(ValueError, "mode/object offset"):
            self.inspect(review=review)


if __name__ == "__main__":
    unittest.main()
