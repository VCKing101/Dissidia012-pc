#!/usr/bin/env bash
# Old 32-bit GCC cannot stat some /mnt/g files. Use temporary Linux files.
set -euo pipefail
cd "$(dirname "$0")/.."
project_root=$PWD
compiler_name=$1
optimization=$2
source=$3
output=$4
case "$compiler_name" in
    psp-gcc-1.3.1) driver=bin/psp-gcc; internal=lib/gcc-lib/psp/3.3.3+allegrex-2.2.1-psp-1.3.1 ;;
    psp-gcc-1.7.1) driver=psp/bin/gcc; internal=lib/gcc-lib/psp/3.3.3+allegrex-2.2.2-psp-1.7.1 ;;
    *) echo 'Unknown compiler' >&2; exit 1 ;;
esac
compiler_root=$(mktemp -d /tmp/dissidia-gcc-probe.XXXXXX)
tar -xzf "tools/downloads/$compiler_name.tar.gz" -C "$compiler_root"
chmod +x "$compiler_root/$driver" "$compiler_root/$internal/cc1"
mkdir -p "$compiler_root/work/include/extra" "$compiler_root/work/src/extra" "$compiler_root/work/tools/probes"
cp include/extra/data_install.h "$compiler_root/work/include/extra/"
cp src/extra/data_install.c src/extra/data_install_notifications.c "$compiler_root/work/src/extra/"
cp tools/probes/extra_probe.c tools/probes/extra_callbacks.c "$compiler_root/work/tools/probes/"
cd "$compiler_root/work"
"$compiler_root/$driver" "-B$compiler_root/$internal/" \
    "$optimization" -G0 -nostdinc -Iinclude -S "$source" -o candidate.s
"$project_root/tools/local/allegrex-binutils/allegrex-as" -EL -G0 -march=allegrex \
    -o candidate.o candidate.s
cp candidate.o "$project_root/$output"
# Leave the small, task-specific temporary directory for OS cleanup.
