#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p analysis/ULUS10566/main
echo '8fbf3541ab121aff85aae72795760736dfe37706996b2b0f9b81d36fc98f3394  orig/ULUS10566/PSP_GAME/SYSDIR/EBOOT.BIN' | sha256sum --check
tools/local/pspdecrypt/pspdecrypt -o analysis/ULUS10566/main/EBOOT.ELF orig/ULUS10566/PSP_GAME/SYSDIR/EBOOT.BIN
echo '0bc8e779aad2814a3d1bd77346f69ca5e16665845ed053cd0ad44a9cbfaefe6d  analysis/ULUS10566/main/EBOOT.ELF' | sha256sum --check
