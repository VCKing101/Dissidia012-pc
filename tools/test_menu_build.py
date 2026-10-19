"""Check the third overlay build's source, module identity and provenance."""

import copy
import json
import unittest
from pathlib import Path
from unittest.mock import patch

from menu_build_provenance import fingerprint_menu_build, require_current_menu_build, layout_digest
from prepare_menu import compile_units
from probe_compilers import functions_from_object
from progress import collect
from match_config import configured_totals
from verify_menu import verify

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = ROOT / "build/ULUS10566/OVL_MENU_APP.ELF"


class MenuBuildChecks(unittest.TestCase):
    def test_complete_file_and_every_configured_source_function_match(self):
        report = verify(CANDIDATE)
        self.assertTrue(report["byte_identical"])
        self.assertEqual((report["recovered_c_functions"], report["recovered_c_bytes"]), configured_totals("menu"))
        self.assertEqual(report["reconstructed_payload_bytes"], 1555864)
        self.assertEqual(report["file_bytes"], 1556096)
        self.assertFalse(report["runtime_verified"])

    def test_whole_file_mutation_is_rejected(self):
        temporary = ROOT / "build/ULUS10566/menu-negative-test.elf"
        try:
            data = bytearray(CANDIDATE.read_bytes())
            data[0x54] ^= 1
            temporary.write_bytes(data)
            with self.assertRaisesRegex(ValueError, "overlay mismatch"):
                verify(temporary)
        finally:
            temporary.unlink(missing_ok=True)

    def test_battle_binary_cannot_rescue_menu_identity(self):
        with self.assertRaisesRegex(ValueError, "Menu overlay mismatch"):
            verify(ROOT / "build/ULUS10566/OVL_BATTLE_APP.ELF")

    def test_discarded_inline_copies_receive_no_source_credit(self):
        report = verify(CANDIDATE)
        self.assertEqual(len(report["discarded_inline_definitions"]), 2)
        self.assertEqual(sum(h["bytes"] for h in report["discarded_inline_definitions"]), 32)
        self.assertEqual(sum(f["size"] for f in report["c_functions"]), configured_totals("menu")[1])
        self.assertFalse({h["name"] for h in report["discarded_inline_definitions"]} &
                         {f["name"] for f in report["c_functions"]})

    def test_changed_inline_evidence_cannot_hide_behind_exact_game_bytes(self):
        loader = json.loads
        def altered(text, *args, **kwargs):
            value = loader(text, *args, **kwargs)
            if isinstance(value, dict) and value.get("module") == "menu" and "parts" in value:
                value["discarded_inline_definitions"][0]["bytes"] += 4
            return value
        with patch("verify_menu.json.loads", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "inline definition evidence"):
                verify(CANDIDATE)

    def test_bad_raw_c_cannot_hide_behind_original_bytes(self):
        def altered(path):
            symbols = functions_from_object(path)
            if Path(path).name == "menu_menu_configuration_initializer.raw.o":
                symbols = copy.deepcopy(symbols)
                f = symbols["func_08A93DF4"]
                f["bytes"] = bytes([f["bytes"][0] ^ 1])+f["bytes"][1:]
            return symbols
        with patch("verify_menu.functions_from_object", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "raw C object does not match"):
                verify(CANDIDATE)

    def test_source_and_shared_header_provenance_is_current(self):
        proof = json.loads((ROOT / "build/ULUS10566/menu.verify.json").read_text())
        require_current_menu_build(proof["build_inputs"])
        changed = fingerprint_menu_build()
        self.assertIn("include/menu/configuration.h", changed)
        self.assertIn("include/main/callback_runtime.h", changed)
        self.assertIn("include/main/resource_descriptor.h", changed)
        self.assertIn("include/main/drawing_record.h", changed)
        self.assertIn("include/main/presentation_channel.h", changed)
        self.assertIn("tools/compiler_inline_sections.py", changed)
        self.assertIn("tools/audit_overlay_dispatch.py", changed)
        self.assertIn("review:menu-overrides", changed)
        changed["include/menu/configuration.h"] = "0"*64
        with self.assertRaisesRegex(ValueError, "Menu build inputs changed"):
            require_current_menu_build(changed)

    def test_entry_review_provenance_cannot_be_changed_behind_same_bytes(self):
        changed = fingerprint_menu_build()
        changed["review:menu-overrides"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "Menu build inputs changed"):
            require_current_menu_build(changed)

    def test_confidence_updates_preserve_layout_but_code_changes_do_not(self):
        inventory = json.loads((ROOT / "config/inventory/menu.json").read_text())
        altered = copy.deepcopy(inventory)
        altered["functions"][0]["primary_owner"] = "another_category"
        self.assertEqual(layout_digest(inventory), layout_digest(altered))
        altered["instruction_ranges"][0]["start"] = "0x089DD4D4"
        self.assertNotEqual(layout_digest(inventory), layout_digest(altered))

    def test_duplicate_units_are_rejected_before_compiler_runs(self):
        matches = json.loads((ROOT / "config/menu.matches.json").read_text())
        matches["units"][1]["name"] = matches["units"][0]["name"]
        with patch("prepare_menu.compile_object") as compiler:
            with self.assertRaisesRegex(ValueError, "Duplicate menu C unit names"):
                compile_units(matches, b"")
            compiler.assert_not_called()

    def test_preparation_c_parts_must_name_the_actual_unit(self):
        original_loader = json.loads
        def altered(text, *args, **kwargs):
            value = original_loader(text, *args, **kwargs)
            if isinstance(value, dict) and value.get("module") == "menu" and "parts" in value:
                next(p for p in value["parts"] if p["kind"] == "matching-c")["unit"] = "forged_unit"
            return value
        with patch("verify_menu.json.loads", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "matching-C parts differ"):
                verify(CANDIDATE)

    def test_three_module_builds_leave_game_percentage_unknown(self):
        report = collect()
        self.assertEqual(report["matched_c_functions"], sum(m["matched_c_functions"] for m in report["modules"]))
        self.assertEqual(report["matched_c_bytes"], sum(m["matched_c_bytes"] for m in report["modules"]))
        self.assertTrue(all(m["module_rebuild_verified"] for m in report["modules"] if m["id"] in ("extra", "menu", "battle")))
        menu = next(m for m in report["modules"] if m["id"] == "menu")
        self.assertEqual(menu["reconstruction_breakdown"]["raw_instruction_fallback_bytes"], 64)
        self.assertIsNone(menu["audited_code_completion_percent"])
        self.assertIsNone(report["whole_game_audited_code_percent"])
        controller = report["local_source_scopes"][0]
        self.assertEqual(controller["matching_handlers"], 14)
        self.assertEqual(controller["handler_count_percent"], 70)
        self.assertEqual(controller["matching_handler_bytes"], 2688)
        self.assertEqual(controller["reviewed_handler_bytes"], 8188)


if __name__ == "__main__":
    unittest.main()
