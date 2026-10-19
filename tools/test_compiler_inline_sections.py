"""Reject source credit or retained references for discarded inline bodies."""
import unittest
from compiler_inline_sections import audit_inline_sections, validate_inline_manifest


class Section(dict):
    def __init__(self, name, **values):
        super().__init__(sh_type="SHT_PROGBITS", sh_flags=0, sh_size=0, **values)
        self.name = name
    def data(self):
        return b"\0" * self["sh_size"]


class Symbol(dict):
    def __init__(self, name, section, size, bind="STB_GLOBAL"):
        super().__init__(st_shndx=section, st_value=0, st_size=size,
                         st_info={"type": "STT_FUNC", "bind": bind})
        self.name = name


class Elf:
    def __init__(self):
        self.sections = [Section(""), Section(".text"), Section(".gnu.linkonce.t.helper"), Section(".symtab")]
        self.sections[1].update(sh_flags=6, sh_size=8)
        self.sections[2].update(sh_flags=6, sh_size=16)
        self.symbols = [Symbol("func_089DDDA0", 1, 8), Symbol("helper", 2, 16, "STB_WEAK")]
        self.sections[3].iter_symbols = lambda: iter(self.symbols)
        self.sections[3].get_symbol = lambda i: self.symbols[i]
    def iter_sections(self):
        return iter(self.sections)
    def get_section_by_name(self, name):
        return next((s for s in self.sections if s.name == name), None)
    def relocation(self, origin, target):
        s = Section(".rel.text")
        s.update(sh_type="SHT_REL", sh_info=origin, sh_link=3)
        s.iter_relocations = lambda: iter([{"r_info_sym": target}])
        self.sections.append(s)


class InlineSectionTests(unittest.TestCase):
    def setUp(self):
        self.elf = Elf()
        self.unit = {"functions": [{"name": "func_089DDDA0", "size": 8}], "unused_inline_symbols": ["helper"]}

    def test_explicit_unused_weak_body_has_separate_evidence(self):
        rows = audit_inline_sections(self.elf, self.unit)
        self.assertEqual([(r["name"], r["bytes"]) for r in rows], [("helper", 16)])
        self.assertEqual(self.elf.sections[1]["sh_size"], 8)

    def test_unlisted_inline_body_rejected(self):
        self.unit["unused_inline_symbols"] = []
        with self.assertRaises(ValueError): audit_inline_sections(self.elf, self.unit)

    def test_live_code_reference_rejected(self):
        self.elf.relocation(1, 1)
        with self.assertRaisesRegex(ValueError, "Live allocated"): audit_inline_sections(self.elf, self.unit)

    def test_live_data_reference_rejected(self):
        self.elf.sections.append(Section(".reginfo"))
        self.elf.sections[-1].update(sh_flags=2, sh_size=24)
        self.elf.relocation(4, 1)
        with self.assertRaisesRegex(ValueError, "Live allocated"): audit_inline_sections(self.elf, self.unit)

    def test_debug_reference_allowed(self):
        self.elf.sections.append(Section(".debug_info"))
        self.elf.relocation(4, 1)
        self.assertEqual(len(audit_inline_sections(self.elf, self.unit)), 1)

    def test_strong_helper_rejected(self):
        self.elf.symbols[1]["st_info"]["bind"] = "STB_GLOBAL"
        with self.assertRaisesRegex(ValueError, "complete weak"): audit_inline_sections(self.elf, self.unit)

    def test_incomplete_helper_extent_rejected(self):
        self.elf.symbols[1]["st_size"] = 12
        with self.assertRaises(ValueError): audit_inline_sections(self.elf, self.unit)

    def test_writable_helper_section_rejected(self):
        self.elf.sections[2]["sh_flags"] = 7
        with self.assertRaises(ValueError): audit_inline_sections(self.elf, self.unit)

    def test_extra_text_rejected(self):
        self.elf.sections[1]["sh_size"] = 24
        with self.assertRaisesRegex(ValueError, "unexpected text"): audit_inline_sections(self.elf, self.unit)

    def test_duplicate_or_credit_overlap_rejected(self):
        for names in (["helper", "helper"], ["func_089DDDA0"]):
            self.unit["unused_inline_symbols"] = names
            with self.assertRaises(ValueError): audit_inline_sections(self.elf, self.unit)

    def test_other_unit_undefined_helper_reference_rejected(self):
        self.elf.symbols.append(Symbol("another_units_helper", "SHN_UNDEF", 0))
        self.elf.relocation(1, 2)
        with self.assertRaisesRegex(ValueError, "Live allocated"):
            audit_inline_sections(self.elf, self.unit, {"another_units_helper"})

    def test_helper_cannot_be_assigned_an_external_original_address(self):
        manifest = {"units": [self.unit], "symbols": {"helper": "0x088D9D24"}}
        with self.assertRaisesRegex(ValueError, "external address"):
            validate_inline_manifest(manifest)


if __name__ == "__main__": unittest.main()
