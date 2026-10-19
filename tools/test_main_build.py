"""Check initialized-load reconstruction, source attribution and full scope."""

import copy
import json
import unittest
from pathlib import Path
from unittest.mock import patch

from main_build_provenance import fingerprint_main_build, require_current_main_build, layout_digest
from probe_compilers import functions_from_object
from progress import collect
from match_config import configured_grand_totals, configured_totals
from verify_main import geometry, reference_at, verify

ROOT = Path(__file__).resolve().parents[1]
CANDIDATE = ROOT / "build/ULUS10566/EBOOT.ELF"


class MainBuildChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.spec = json.loads((ROOT / "config/main.reference.json").read_text())
        cls.reference = (ROOT / cls.spec["path"]).read_bytes()

    def test_entire_decrypted_main_matches_with_real_c(self):
        report = verify(CANDIDATE)
        self.assertEqual(report["file_bytes"], 4640948)
        self.assertEqual((report["recovered_c_functions"], report["recovered_c_bytes"]), configured_totals("main"))
        self.assertTrue(report["byte_identical"])
        self.assertFalse(report["runtime_verified"])

    def test_bss_is_memory_only_and_never_added_to_output(self):
        report = verify(CANDIDATE)
        self.assertEqual(report["reconstructed_load_bytes"], 1938560)
        self.assertEqual(report["memory_only_bytes"], 16833168)
        self.assertEqual(report["preserved_nonload_file_bytes"], 2702388)
        self.assertEqual(report["file_bytes"], report["reconstructed_load_bytes"]+report["preserved_nonload_file_bytes"])
        with self.assertRaisesRegex(ValueError, "not wholly initialized"):
            reference_at(self.reference, self.spec["load_segments"], 0x08B63280, 4)

    def test_vma_gap_cannot_be_read_as_original_loaded_data(self):
        with self.assertRaisesRegex(ValueError, "not wholly initialized"):
            reference_at(self.reference, self.spec["load_segments"], 0x089DD4C0, 4)

    def test_crossing_segment_end_is_rejected(self):
        first = self.spec["load_segments"][0]
        end = int(first["address"], 16)+first["file_bytes"]
        with self.assertRaisesRegex(ValueError, "not wholly initialized"):
            reference_at(self.reference, self.spec["load_segments"], end-4, 8)

    def test_pinned_noncongruent_alignment_geometry_is_preserved(self):
        self.assertEqual(geometry(self.reference), self.spec["load_segments"])
        for segment in self.spec["load_segments"]:
            self.assertNotEqual(segment["file_offset"] % 4096, int(segment["address"], 16) % 4096)

    def test_header_or_tail_changes_fail_even_when_code_is_unchanged(self):
        path = ROOT / "build/ULUS10566/main-negative-test.elf"
        try:
            altered = bytearray(CANDIDATE.read_bytes())
            altered[-1] ^= 1
            path.write_bytes(altered)
            with self.assertRaisesRegex(ValueError, "Main ELF mismatch"):
                verify(path)
        finally:
            path.unlink(missing_ok=True)

    def test_original_file_cannot_hide_incorrect_compiled_c(self):
        matches = json.loads((ROOT / "config/main.matches.json").read_text())
        unit = matches["units"][0]
        symbol = unit["functions"][0]["name"]
        def altered(path):
            functions = functions_from_object(path)
            if Path(path).name == f"main_{unit['name']}.raw.o":
                functions = copy.deepcopy(functions)
                data = functions[symbol]["bytes"]
                functions[symbol]["bytes"] = bytes([data[0] ^ 1])+data[1:]
            return functions
        with patch("verify_main.functions_from_object", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "raw C object does not match"):
                verify(CANDIDATE)

    def test_import_stub_cannot_be_relabelled_as_recovered_c(self):
        loader = json.loads
        def altered(text, *args, **kwargs):
            value = loader(text, *args, **kwargs)
            if isinstance(value, dict) and "units" in value and "compiler" in value:
                function = value["units"][0]["functions"][0]
                function["address"], function["size"] = "0x089C5770", 8
            return value
        with patch("verify_main.json.loads", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "SDK import stubs"):
                verify(CANDIDATE)

    def test_unknown_prepared_kind_is_rejected(self):
        loader = json.loads
        def altered(text, *args, **kwargs):
            value = loader(text, *args, **kwargs)
            if isinstance(value, dict) and value.get("module") == "main" and "segments" in value:
                part = next(p for p in value["segments"][0]["parts"] if p["kind"] == "opaque-original-bytes")
                part["kind"] = "pretend-source"
            return value
        with patch("verify_main.json.loads", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "unknown part kind"):
                verify(CANDIDATE)

    def test_stale_main_source_and_segment_layout_are_rejected(self):
        proof = json.loads((ROOT / "build/ULUS10566/main.verify.json").read_text())
        require_current_main_build(proof["build_inputs"])
        changed = fingerprint_main_build()
        changed["include/main/state_prefixes.h"] = "0"*64
        with self.assertRaisesRegex(ValueError, "Main build inputs changed"):
            require_current_main_build(changed)
        inventory = json.loads((ROOT / "config/inventory/main.json").read_text())
        segments = copy.deepcopy(self.spec["load_segments"])
        segments[1]["memory_bytes"] += 16
        self.assertNotEqual(layout_digest(inventory, self.spec["load_segments"]), layout_digest(inventory, segments))

    def test_all_four_builds_are_verified_without_claiming_source_completion(self):
        report = collect()
        self.assertEqual((report["matched_c_functions"], report["matched_c_bytes"]), configured_grand_totals())
        self.assertTrue(all(m["module_rebuild_verified"] for m in report["modules"]))
        self.assertIsNone(report["whole_game_audited_code_percent"])
        self.assertEqual(report["executable_section_bytes"], 5018140)


if __name__ == "__main__":
    unittest.main()
