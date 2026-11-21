"""Totals declared by the maintained match configuration.

Tests compare these with what the verifiers actually prove, so a new batch
needs no hand-edited counts and a missing or unverified unit still fails.
"""

import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MODULES = ("main", "extra", "menu", "battle")


# Units built from C that needs inline assembly the compiler cannot express
# otherwise (the PSP's VFPU has no C operators or intrinsics). They are built
# and byte-verified like any unit but reported apart from the C/C++ headline.
INLINE_ASM_KINDS = ("vfpu",)


def inline_asm_kind(unit):
    """The unit's declared inline-assembly kind, or None for plain C/C++."""
    kind = unit.get("inline_asm")
    if kind is not None and kind not in INLINE_ASM_KINDS:
        raise ValueError(f"Unknown inline_asm kind for unit {unit['name']}: {kind}")
    return kind


def _sizes(module, inline):
    units = json.loads((ROOT / f"config/{module}.matches.json").read_text())["units"]
    return [function["size"] for unit in units if bool(inline_asm_kind(unit)) == inline
            for function in unit["functions"]]


def configured_totals(module):
    """Return (function count, code bytes) of plain C/C++ declared for one module."""
    sizes = _sizes(module, False)
    return len(sizes), sum(sizes)


def configured_inline_asm_totals(module):
    """Return (function count, code bytes) declared as C with inline assembly."""
    sizes = _sizes(module, True)
    return len(sizes), sum(sizes)


def configured_grand_totals():
    totals = [configured_totals(module) for module in MODULES]
    return sum(t[0] for t in totals), sum(t[1] for t in totals)
