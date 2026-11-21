"""Units built from C with VFPU inline assembly are verified but counted apart from C/C++."""

import json
import unittest
from pathlib import Path
from unittest.mock import patch

from match_config import (MODULES, configured_inline_asm_totals, configured_totals,
                          inline_asm_kind)
from progress import collect
from verify_battle import verify as verify_battle

ROOT = Path(__file__).resolve().parents[1]


class InlineAsmUnitChecks(unittest.TestCase):
    def test_configured_totals_split_every_unit_once(self):
        for module in MODULES:
            units = json.loads((ROOT / f"config/{module}.matches.json").read_text())["units"]
            sizes = [f["size"] for u in units for f in u["functions"]]
            plain, inline = configured_totals(module), configured_inline_asm_totals(module)
            self.assertEqual(plain[0] + inline[0], len(sizes))
            self.assertEqual(plain[1] + inline[1], sum(sizes))

    def test_unknown_inline_asm_kind_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "Unknown inline_asm kind"):
            inline_asm_kind({"name": "unit", "inline_asm": "anything"})

    def test_verified_modules_report_inline_asm_apart(self):
        for module in ("main", "battle", "menu"):
            proof = json.loads((ROOT / f"build/ULUS10566/{module}.verify.json").read_text())
            self.assertEqual((proof["recovered_c_functions"], proof["recovered_c_bytes"]), configured_totals(module))
            self.assertEqual((proof["inline_asm_functions"], proof["inline_asm_bytes"]),
                             configured_inline_asm_totals(module))
            plain = {f["name"] for f in proof["c_functions"]}
            self.assertFalse(plain & {f["name"] for f in proof["inline_asm_c_functions"]})

    def test_progress_headline_excludes_inline_asm(self):
        report = collect()
        self.assertEqual(report["inline_asm_bytes"], sum(m["inline_asm_bytes"] for m in report["modules"]))
        self.assertEqual(report["matched_c_bytes"], sum(configured_totals(m)[1] for m in MODULES))
        self.assertGreater(report["inline_asm_functions"], 0)

    def test_undeclared_inline_asm_source_is_rejected(self):
        loader = json.loads
        def altered(text, *args, **kwargs):
            value = loader(text, *args, **kwargs)
            if isinstance(value, dict) and "units" in value and "compiler" in value:
                for unit in value["units"]:
                    unit.pop("inline_asm", None)
            return value
        with patch("verify_battle.json.loads", side_effect=altered):
            with self.assertRaisesRegex(ValueError, "inline-assembly declaration differs"):
                verify_battle(ROOT / "build/ULUS10566/OVL_BATTLE_APP.ELF")


if __name__ == "__main__":
    unittest.main()
