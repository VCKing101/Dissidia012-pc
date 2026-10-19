"""Exercise relocation comparison against the actual compiled/retail wrappers.

Run after tools/build_extra.py. No original or build inputs are modified.
"""

import json
import unittest
from pathlib import Path

from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object
from verify_extra import verify

ROOT = Path(__file__).resolve().parents[1]


class MatchingChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.matches = json.loads((ROOT / "config/extra.matches.json").read_text())
        cls.addresses = configured_addresses(cls.matches)
        cls.function = functions_from_object(ROOT / "build/ULUS10566/data_install_callback_wrappers.raw.o")["func_089DE69C"]
        reference = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF").read_bytes()
        cls.expected = reference[0x1230:0x125C]

    def test_resolved_compiler_output_matches_retail(self):
        self.assertEqual(resolve_function(self.function, self.addresses), self.expected)

    def test_wrong_call_address_is_not_masked(self):
        wrong = dict(self.addresses)
        wrong["func_089DD5F8"] += 4
        self.assertNotEqual(resolve_function(self.function, wrong), self.expected)

    def test_same_low_jump_bits_in_wrong_region_are_rejected(self):
        wrong = dict(self.addresses)
        wrong["func_089DD5F8"] += 0x10000000
        with self.assertRaisesRegex(ValueError, "256 MiB region"):
            resolve_function(self.function, wrong)

    def test_wrong_global_address_is_not_masked(self):
        wrong = dict(self.addresses)
        wrong["D_089DE79C"] += 4
        self.assertNotEqual(resolve_function(self.function, wrong), self.expected)

    def test_missing_global_address_is_rejected(self):
        wrong = dict(self.addresses)
        del wrong["D_089DE79C"]
        with self.assertRaisesRegex(ValueError, "Unknown relocation target"):
            resolve_function(self.function, wrong)

    def test_reused_high_half_matches_independent_linker_and_retail(self):
        name = "func_089DDCE4"
        raw = functions_from_object(ROOT / "build/ULUS10566/data_install_status.raw.o")[name]
        # This real compiler sequence has one HI16 and two LO16 references to
        # the context. The second low reference reuses a saved high register.
        context = [r["type"] for r in raw["relocations"] if r["symbol"] == "D_089D5FB4"]
        self.assertEqual(context, [5, 6, 6])
        linked = functions_from_object(ROOT / "build/ULUS10566/extra.linked.elf")[name]
        resolved = resolve_function(raw, self.addresses)
        self.assertEqual(resolved, linked["bytes"])
        reference = (ROOT / "orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF").read_bytes()
        offset = self.addresses[name] - 0x089DD4C0 + 0x54
        self.assertEqual(resolved, reference[offset:offset + 132])

    def test_high_half_without_low_is_rejected(self):
        incomplete = dict(self.function)
        incomplete["relocations"] = [r for r in self.function["relocations"] if r["type"] != 6]
        with self.assertRaisesRegex(ValueError, "HI16 relocation has no paired LO16"):
            resolve_function(incomplete, self.addresses)

    def test_grouped_high_relocations_follow_table_order(self):
        name = "relocation_order"
        raw = functions_from_object(ROOT / "build/ULUS10566/relocation_order.o")[name]
        self.assertEqual([r["offset"] for r in raw["relocations"]], [4, 20, 8, 16])
        addresses = {name: 0x08801000, "relocation_target": 0x12348000}
        linked = functions_from_object(ROOT / "build/ULUS10566/relocation_order.linked.elf")[name]
        self.assertEqual(resolve_function(raw, addresses), linked["bytes"])
        # Sorting this valid table loses the high/low association and leaves
        # the second HI16 pending. Keep this guard against the original bug.
        sorted_raw = dict(raw)
        sorted_raw["relocations"] = sorted(raw["relocations"], key=lambda r: r["offset"])
        with self.assertRaisesRegex(ValueError, "HI16 relocation has no paired LO16"):
            resolve_function(sorted_raw, addresses)

    def test_all_integrated_functions_and_overlay_match(self):
        report = verify(ROOT / "build/ULUS10566/OVL_EXTRA_APP.ELF")
        self.assertEqual(report["recovered_c_functions"], 24)
        self.assertEqual(report["recovered_c_bytes"], 2116)


if __name__ == "__main__":
    unittest.main()
