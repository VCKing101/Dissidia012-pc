"""Bounded menu candidate comparison using fully resolved raw PSP relocations."""
import hashlib
import json
import re
import struct
import sys
from pathlib import Path
import rabbitizer

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
from compile_psp import compile_object
from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object
from audit_presentation_callbacks import collect_callbacks


def main():
    manifest = json.loads((ROOT / 'config/menu.matches.json').read_text())
    inventory = json.loads((ROOT / 'config/inventory/menu.json').read_text())
    functions = {int(f['address'], 16): f for f in inventory['functions']}
    # A newly reviewed callback can be raw-compared before the maintained
    # inventory is regenerated from its first full build. Only these pinned,
    # independently audited entries receive this temporary lookup extent.
    for reviewed in collect_callbacks():
        if reviewed['module'] == 'menu':
            functions.setdefault(int(reviewed['address'], 16),
                                 {'address': reviewed['address'], 'body_bytes': reviewed['size']})
    addresses = configured_addresses(manifest)
    for module in ('main', 'menu'):
        current = json.loads((ROOT / f'config/inventory/{module}.json').read_text())
        addresses.update({'func_' + f['address'][2:]: int(f['address'], 16) for f in current['functions']})
    reference = (ROOT / 'orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF').read_bytes()
    if hashlib.sha256(reference).hexdigest() != inventory['reference_sha256']:
        raise ValueError('Menu reference mismatch')
    sources = [s for s in sys.argv[1:] if s != '--details'] or [u['source'] for u in manifest['units']]
    output = ROOT / 'analysis/menu-source-continuation'
    output.mkdir(parents=True, exist_ok=True)
    rows = []
    for source in sources:
        path = ROOT / source
        addresses.update({'D_' + a: int(a, 16) for a in re.findall(r'\bD_([0-9A-F]{8})\b', path.read_text())})
        addresses.update({'func_' + a: int(a, 16) for a in re.findall(r'\bfunc_([0-9A-F]{8})\b', path.read_text())})
        obj = output / (path.stem + '.raw.o')
        result = compile_object(manifest['compiler'], manifest['flags'], source, obj.relative_to(ROOT))
        obj.with_suffix('.log').write_text(result.stdout + result.stderr)
        if result.returncode:
            raise RuntimeError(f'Compile failed: {source}; see {obj.with_suffix(".log")}')
        compiled = functions_from_object(obj)
        for symbol in compiled.values():
            for relocation in symbol["relocations"]:
                native = re.search(r"func_([0-9A-F]{8})", relocation["symbol"])
                if native:
                    addresses[relocation["symbol"]] = int(native.group(1), 16)
        for name, symbol in compiled.items():
            native = re.search(r"func_([0-9A-F]{8})", name)
            if not native:
                print(f"Uncredited compiler helper: {name} / {len(symbol['bytes'])} bytes", flush=True)
                continue
            address = int(native.group(1), 16)
            addresses[name] = address
            spec = functions[address]
            actual = resolve_function(symbol, addresses)
            offset = address - 0x089DD4C0 + 0x54
            expected = reference[offset:offset + spec['body_bytes']]
            diffs = [i for i in range(0, max(len(actual), len(expected)), 4) if actual[i:i+4] != expected[i:i+4]]
            rows.append({'name': name, 'address': spec['address'], 'size': len(expected), 'compiled_size': len(actual), 'match': actual == expected, 'source': source, 'different_words': len(diffs)})
            print(f'{name}: {len(actual)}/{len(expected)} bytes; {len(diffs)} different words', flush=True)
            if '--details' in sys.argv:
                for i in diffs[:30]:
                    decoded = []
                    for data in (actual[i:i+4], expected[i:i+4]):
                        decoded.append(rabbitizer.Instruction(struct.unpack('<I', data)[0], address+i, rabbitizer.InstrCategory.R4000ALLEGREX).disassemble() if len(data)==4 else '<absent>')
                    print(f'  +0x{i:03X}: {decoded[0]} | retail: {decoded[1]}')
    (output / 'results.json').write_text(json.dumps(rows, indent=2) + '\n')
    if not rows or not all(r['match'] for r in rows):
        raise ValueError('Menu candidate batch contains an empty result or a raw C mismatch')


if __name__ == '__main__':
    main()
