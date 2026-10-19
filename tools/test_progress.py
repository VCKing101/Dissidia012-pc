"""Check that local progress preserves unknown totals and excludes assembly."""

import unittest

from build_provenance import fingerprint_extra_build, require_current_build
from progress import collect
from match_config import configured_totals


class ProgressChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = collect()

    def test_full_binary_rebuild_does_not_mean_full_c_recovery(self):
        extra = next(m for m in self.report["modules"] if m["id"] == "extra")
        self.assertTrue(extra["module_rebuild_verified"])
        self.assertEqual(extra["matched_c_functions"], 24)
        self.assertEqual(extra["matched_c_bytes"], 2116)
        self.assertLess(extra["payload_c_coverage_percent"], 10)

    def test_unknown_denominators_are_not_reported_as_zero_or_complete(self):
        self.assertIsNone(self.report["whole_game_audited_code_percent"])
        for feature in self.report["features"]:
            self.assertIsNone(feature["completion_percent"])

    def test_overlays_with_shared_addresses_remain_separate_modules(self):
        self.assertEqual({m["id"] for m in self.report["modules"]}, {"main", "extra", "menu", "battle"})
        self.assertEqual(self.report["executable_section_bytes"], 5018140)

    def test_mismatched_source_fingerprint_rejects_reporting(self):
        changed = fingerprint_extra_build()
        changed["src/extra/data_install_transfer.c"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "Build inputs changed"):
            require_current_build(changed)

    def test_savedata_handlers_have_one_primary_owner(self):
        features = {f["id"]: f for f in self.report["features"]}
        self.assertEqual(features["save"]["matched_functions"], 4)
        self.assertEqual(features["save"]["matched_c_bytes"], 1216)
        # Every menu match outside the four savedata handlers stays with menu helpers.
        self.assertEqual(features["menu_helpers"]["matched_functions"], configured_totals("menu")[0] - 4)
        self.assertFalse(features["save"]["scope_complete"])
        self.assertIsNone(features["save"]["completion_percent"])


if __name__ == "__main__":
    unittest.main()
