#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p build/ULUS10566
mapfile -t c_objects < build/ULUS10566/extra.objects.txt
tools/local/allegrex-binutils/allegrex-as -EL -G0 -march=allegrex -Iinclude \
    -o build/ULUS10566/extra.o build/ULUS10566/extra.remaining.s
mipsel-linux-gnu-ld -m elf32ltsmip -T config/extra.link.ld \
    -Map build/ULUS10566/extra.map -o build/ULUS10566/extra.linked.elf \
    build/ULUS10566/extra.o "${c_objects[@]}"
tools/local/allegrex-binutils/allegrex-objcopy -O binary --only-section=.text \
    build/ULUS10566/extra.linked.elf build/ULUS10566/extra.payload.bin

# Produce the independent linker fixture used by tools/test_matching.py.
tools/local/allegrex-binutils/allegrex-as -EL -G0 -march=allegrex \
    -o build/ULUS10566/relocation_order.o tools/probes/mips_relocation_order.s
mipsel-linux-gnu-ld -m elf32ltsmip -Ttext 0x08801000 \
    --defsym=relocation_target=0x12348000 -e relocation_order \
    -o build/ULUS10566/relocation_order.linked.elf build/ULUS10566/relocation_order.o
