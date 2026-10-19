"""Totals declared by the maintained match configuration.

Tests compare these with what the verifiers actually prove, so a new batch
needs no hand-edited counts and a missing or unverified unit still fails.
"""

import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MODULES = ("main", "extra", "menu", "battle")


def configured_totals(module):
    """Return (function count, code bytes) declared for one module."""
    units = json.loads((ROOT / f"config/{module}.matches.json").read_text())["units"]
    sizes = [function["size"] for unit in units for function in unit["functions"]]
    return len(sizes), sum(sizes)


def configured_grand_totals():
    totals = [configured_totals(module) for module in MODULES]
    return sum(t[0] for t in totals), sum(t[1] for t in totals)
