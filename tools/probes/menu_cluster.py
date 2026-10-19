"""Compare the menu source expansion against pinned bytes, without patching code."""
import hashlib
import json
import struct
import sys
from pathlib import Path
import rabbitizer

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
from compile_psp import compile_object
from mips_relocations import configured_addresses, resolve_function
from probe_compilers import functions_from_object


UNITS = [
    ('runtime_lifecycle', [('func_089DD4D0', 0x089DD4D0, 152), ('func_089DD568', 0x089DD568, 116)]),
    ('configuration_accessors', [('func_08A93D84', 0x08A93D84, 40), ('func_08A93DAC', 0x08A93DAC, 24)]),
    ('initializer_runtime', [('func_08AFFF54', 0x08AFFF54, 36)]),
    ('initializer_float', [('func_08B44E70', 0x08B44E70, 32)]),
]
SYMBOLS = {
    'func_08864FF0': 0x08864FF0, 'func_0889F63C': 0x0889F63C,
    'func_0889F674': 0x0889F674, 'func_08992F04': 0x08992F04,
    'func_08866AA8': 0x08866AA8, 'D_089C8E70': 0x089C8E70,
    'D_089C8E88': 0x089C8E88, 'D_089C8EA0': 0x089C8EA0,
    'D_08A9426C': 0x08A9426C, 'D_09B6CDA0': 0x09B6CDA0,
    'func_088CB9E8': 0x088CB9E8, 'D_08B4BFD8': 0x08B4BFD8,
    'D_09B6DCE0': 0x09B6DCE0,
}


def main():
    matches = json.loads((ROOT / 'config/menu.matches.json').read_text())
    reference = (ROOT / 'orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_MENU_APP.ELF').read_bytes()
    if hashlib.sha256(reference).hexdigest() != 'f021f96fa05da5907563a28c232966aa8a344526d6b8bdee728053ec75224d0a':
        raise ValueError('Menu reference changed')
    addresses = configured_addresses(matches)
    addresses.update(SYMBOLS)
    addresses.update({name: address for _, fs in UNITS for name, address, size in fs})
    output = ROOT / 'analysis/menu-source-recovery'
    output.mkdir(parents=True, exist_ok=True)
    rows = []
    for unit, functions in UNITS:
        obj = output / (unit + '.raw.o')
        process = compile_object(matches['compiler'], matches['flags'], f'src/menu/{unit}.c', obj.relative_to(ROOT))
        obj.with_suffix('.log').write_text(process.stdout + process.stderr)
        if process.returncode:
            raise RuntimeError(f'Compilation failed: {unit}; see {obj.with_suffix(".log")}')
        compiled_functions = functions_from_object(obj)
        for name, address, size in functions:
            actual = resolve_function(compiled_functions[name], addresses)
            offset = address - 0x089DD4C0 + 0x54
            expected = reference[offset:offset + size]
            differences = [i for i in range(0, max(len(actual), size), 4) if actual[i:i+4] != expected[i:i+4]]
            rows.append({'name': name, 'address': f'0x{address:08X}', 'size': size, 'compiled_size': len(actual), 'match': actual == expected, 'different_words': len(differences)})
            print(f'{name}: {len(actual)}/{size} bytes; {len(differences)} different words', flush=True)
            for i in differences[:16]:
                decoded = []
                for data in [actual[i:i+4], expected[i:i+4]]:
                    decoded.append(rabbitizer.Instruction(struct.unpack('<I', data)[0], address+i, rabbitizer.InstrCategory.R4000ALLEGREX).disassemble() if len(data)==4 else '<absent>')
                print(f'  +0x{i:03X}: {decoded[0]} | retail: {decoded[1]}')
    (output / 'results.json').write_text(json.dumps(rows, indent=2) + '\n')
    if not all(row['match'] for row in rows):
        raise ValueError('At least one menu source-expansion function does not match')
    return rows


if __name__ == '__main__':
    main()
