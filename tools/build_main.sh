#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p build/ULUS10566
mapfile -t c_objects < build/ULUS10566/main.objects.txt
objects=()
for object in "${c_objects[@]}"; do
    [[ -z "$object" ]] || objects+=("$object")
done
# Historical Sony gas supports the Allegrex VFPU instructions in main.
tools/compilers/psp-gcc-1.3.1/bin/psp-as -EL -G0 -march=allegrex -Iinclude \
    -o build/ULUS10566/main.raw.o build/ULUS10566/main.remaining.s
python3 - <<'PY'
import struct
from pathlib import Path
data = bytearray(Path('build/ULUS10566/main.raw.o').read_bytes())
flags = struct.unpack_from('<I', data, 36)[0]
if flags & 0x00FF0000 != 0x00A20000:
    raise SystemExit('Unexpected historical PSP assembler machine identifier')
struct.pack_into('<I', data, 36, (flags & ~0x00FF0000) | 0x00840000)
Path('build/ULUS10566/main.o').write_bytes(data)
PY
mipsel-linux-gnu-ld -m elf32ltsmip -T config/main.link.ld \
    -Map build/ULUS10566/main.map -o build/ULUS10566/main.linked.elf \
    build/ULUS10566/main.o "${objects[@]}"
for index in 0 1; do
    tools/local/allegrex-binutils/allegrex-objcopy -O binary --only-section=.segment${index} \
        build/ULUS10566/main.linked.elf build/ULUS10566/main.segment${index}.bin
done
