#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p build/ULUS10566
mapfile -t c_objects < build/ULUS10566/battle.objects.txt
# Ignore the empty manifest's trailing blank line.
objects=()
for object in "${c_objects[@]}"; do
    [[ -z "$object" ]] || objects+=("$object")
done
# The installed 2.42 Allegrex gas lacks VFPU opcodes. The pinned historical
# PSP gas supports them; numeric registers and prefix lists are emitted for it.
tools/compilers/psp-gcc-1.3.1/bin/psp-as -EL -G0 -march=allegrex -Iinclude \
    -o build/ULUS10566/battle.raw.o build/ULUS10566/battle.remaining.s
# Old Sony gas uses the same historical machine identifier as SNC. Preserve
# raw output and change only that identifier in the modern-linker copy.
python3 - <<'PY'
import struct
from pathlib import Path
raw = Path('build/ULUS10566/battle.raw.o').read_bytes()
data = bytearray(raw)
flags = struct.unpack_from('<I', data, 36)[0]
if flags & 0x00FF0000 != 0x00A20000:
    raise SystemExit('Unexpected historical PSP assembler machine identifier')
struct.pack_into('<I', data, 36, (flags & ~0x00FF0000) | 0x00840000)
Path('build/ULUS10566/battle.o').write_bytes(data)
PY
mipsel-linux-gnu-ld -m elf32ltsmip -T config/battle.link.ld \
    -Map build/ULUS10566/battle.map -o build/ULUS10566/battle.linked.elf \
    build/ULUS10566/battle.o "${objects[@]}"
tools/local/allegrex-binutils/allegrex-objcopy -O binary --only-section=.text \
    build/ULUS10566/battle.linked.elf build/ULUS10566/battle.payload.bin
