#!/usr/bin/env bash
# Run from Ubuntu-24.04 WSL. Packages are installed in that distribution only.
set -euo pipefail
cd "$(dirname "$0")/.."
sudo apt-get update
sudo apt-get install -y --no-install-recommends binutils-mipsel-linux-gnu gcc g++ make libssl-dev zlib1g-dev curl ca-certificates git
mkdir -p tools/downloads tools/local/allegrex-binutils
archive=tools/downloads/allegrex-as.tar.gz
if [ ! -f "$archive" ]; then
    curl --fail --location 'https://github.com/Xeeynamo/sotn-decomp/releases/download/cc1-psx-26/allegrex-as.tar.gz' -o "$archive"
fi
echo '7408cc6c02664497a5058f61c275d60d208a0e9492d295330daf59fa1ea5b80f  tools/downloads/allegrex-as.tar.gz' | sha256sum --check
tar -xzf "$archive" -C tools/local/allegrex-binutils
if [ ! -d tools/local/pspdecrypt ]; then
    git clone https://github.com/hrydgard/pspdecrypt.git tools/local/pspdecrypt
fi
git -C tools/local/pspdecrypt fetch origin c156627db7634d395c380c0a9589130f603307fc
git -C tools/local/pspdecrypt checkout --detach c156627db7634d395c380c0a9589130f603307fc
make -C tools/local/pspdecrypt -j2 CC=gcc CXX=g++
