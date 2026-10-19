"""Check battle source attribution independently of full-file build equality."""

import copy
import json
import unittest
from pathlib import Path
from unittest.mock import patch

from battle_build_provenance import fingerprint_battle_build, require_current_battle_build, layout_digest
from probe_compilers import functions_from_object
from verify_battle import verify
from progress import collect
from match_config import configured_totals

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = ROOT / "build/ULUS10566/OVL_BATTLE_APP.ELF"


class BattleBuildChecks(unittest.TestCase):
    def test_complete_file_rebuild_contains_every_configured_c_match(self):
        report = verify(CANDIDATE)
        self.assertTrue(report["byte_identical"])
        self.assertEqual((report["recovered_c_functions"], report["recovered_c_bytes"]), configured_totals("battle"))
        self.assertEqual(report["file_bytes"], 1596312)
        self.assertEqual(report["reconstructed_payload_bytes"], 1596080)
        self.assertFalse(report["runtime_verified"])

    def test_full_file_mismatch_is_rejected(self):
        path = ROOT / "build/ULUS10566/battle-negative-test.elf"
        try:
            data = bytearray(CANDIDATE.read_bytes())
            data[0x54] ^= 1
            path.write_bytes(data)
            with self.assertRaisesRegex(ValueError, "overlay mismatch"):
                verify(path)
        finally:
            path.unlink(missing_ok=True)

    def test_unchanged_file_cannot_hide_an_incorrect_raw_c_object(self):
        def altered(path):
            symbols = functions_from_object(path)
            if Path(path).name == "battle_cleanup_group_a.raw.o":
                symbols = copy.deepcopy(symbols)
                function = symbols["func_08A48E78"]
                function["bytes"] = bytes([function["bytes"][0] ^ 1])+function["bytes"][1:]
            return symbols
        with patch("verify_battle.functions_from_object", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "raw C object does not match"):
                verify(CANDIDATE)

    def test_linked_c_must_retain_original_address(self):
        def altered(path):
            symbols = functions_from_object(path)
            if Path(path).name == "battle.linked.elf":
                symbols = copy.deepcopy(symbols)
                symbols["func_08A48E78"]["address"] += 4
            return symbols
        with patch("verify_battle.functions_from_object", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "linked C address/bytes"):
                verify(CANDIDATE)

    def test_stale_battle_source_proof_is_rejected(self):
        fingerprints = fingerprint_battle_build()
        fingerprints["src/battle/slot_setter.c"] = "0"*64
        with self.assertRaisesRegex(ValueError, "Battle build inputs changed"):
            require_current_battle_build(fingerprints)

    def test_matching_metadata_updates_do_not_invalidate_the_layout_input(self):
        inventory = json.loads((ROOT / "config/inventory/battle.json").read_text())
        changed = copy.deepcopy(inventory)
        changed["functions"][0]["matched_c"] = not changed["functions"][0]["matched_c"]
        self.assertEqual(layout_digest(inventory), layout_digest(changed))
        changed["instruction_ranges"][0]["start"] = "0x089DD4D4"
        self.assertNotEqual(layout_digest(inventory), layout_digest(changed))

    def test_raw_and_opaque_bytes_receive_no_c_credit(self):
        report = verify(CANDIDATE)
        self.assertEqual(report["raw_instruction_fallback_bytes"], 24)
        self.assertEqual(report["opaque_original_bytes"], 87180)
        self.assertEqual(sum(report[k] for k in ("recovered_c_bytes", "assembly_instruction_bytes", "raw_instruction_fallback_bytes", "opaque_original_bytes")), 1596080)

    def test_progress_counts_both_modules_and_retains_unknown_headlines(self):
        report = collect()
        self.assertEqual(report["matched_c_functions"], sum(m["matched_c_functions"] for m in report["modules"]))
        self.assertEqual(report["matched_c_bytes"], sum(m["matched_c_bytes"] for m in report["modules"]))
        self.assertTrue(next(m for m in report["modules"] if m["id"] == "extra")["module_rebuild_verified"])
        battle = next(m for m in report["modules"] if m["id"] == "battle")
        self.assertEqual(battle["matched_c_functions"], configured_totals("battle")[0])
        self.assertIsNone(battle["audited_code_completion_percent"])
        self.assertIsNone(report["whole_game_audited_code_percent"])

    def assert_rejects_altered_preparation(self, mutate, message):
        original_loader = json.loads
        def altered(text, *args, **kwargs):
            value = original_loader(text, *args, **kwargs)
            if isinstance(value, dict) and value.get("module") == "battle" and "parts" in value:
                mutate(value)
            return value
        with patch("verify_battle.json.loads", side_effect=altered):
            with self.assertRaisesRegex(ValueError, message):
                verify(CANDIDATE)

    def test_matching_c_parts_must_name_the_actual_unit(self):
        def mutate(prepared):
            next(p for p in prepared["parts"] if p["kind"] == "matching-c")["unit"] = "forged_unit"
        self.assert_rejects_altered_preparation(mutate, "matching-C parts differ")

    def test_assembly_relabel_cannot_retain_c_credit(self):
        def mutate(prepared):
            next(p for p in prepared["parts"] if p["kind"] == "matching-c")["kind"] = "inventory-instructions"
        self.assert_rejects_altered_preparation(mutate, "matching-C parts differ")

    def test_unknown_part_kind_is_rejected(self):
        def mutate(prepared):
            next(p for p in prepared["parts"] if p["kind"] == "opaque-original-bytes")["kind"] = "unreviewed"
        self.assert_rejects_altered_preparation(mutate, "Unknown battle preparation part kind")


if __name__ == "__main__":
    unittest.main()
