#!/usr/bin/env bash
# The key that signs releases, so a copy of the game installs only an update
# that came from this project (port/host/d012_update_install.cpp). Run it in WSL,
# in a terminal: OpenSSL asks for the key's password there.
#
#   release_signing.sh new               makes the key once, and writes its public
#                                        half to tools/port/update-signing-key.txt
#   release_signing.sh sign <folder>     signs <folder>/SHA256SUMS: SHA256SUMS.sig
#   release_signing.sh verify <folder>   checks that signature with the public half
#
# The private key stays in ~/d012-android/keys (D012_UPDATE_SIGNING_KEY names
# another file). It never goes into the repository or a release. Back it up off
# this machine, with its password kept apart.
#
# MIT licence; see README.md at the repository root.
set -euo pipefail

root="$(cd "$(dirname "$0")/../.." && pwd)"
key="${D012_UPDATE_SIGNING_KEY:-$HOME/d012-android/keys/update-signing.pem}"
public_file="${D012_UPDATE_PUBLIC_KEY:-$root/tools/port/update-signing-key.txt}"

usage() {
    sed -n '6,8p' "$0" | sed 's/^# *//' >&2
    exit 2
}

public_hex() {
    tr -d '[:space:]' < "$public_file"
}

# The public half as the PEM file OpenSSL reads: the fixed DER header of an
# Ed25519 public key, then the key's 32 bytes.
public_pem() {
    local hex
    hex="$(public_hex)"
    [[ "$hex" =~ ^[0-9a-f]{64}$ ]] || { echo "error: $public_file does not hold a public key" >&2; exit 1; }
    {
        echo "-----BEGIN PUBLIC KEY-----"
        printf '302a300506032b6570032100%s' "$hex" | sed 's/../\\x&/g' | xargs -0 printf '%b' | base64 -w 64
        echo "-----END PUBLIC KEY-----"
    } > "$1"
}

verify() {
    local folder="$1" pem
    pem="$(mktemp)"
    public_pem "$pem"
    if ! openssl pkeyutl -verify -pubin -inkey "$pem" -rawin -in "$folder/SHA256SUMS" \
        -sigfile "$folder/SHA256SUMS.sig" > /dev/null; then
        rm -f "$pem"
        echo "error: $folder/SHA256SUMS.sig is not this key's signature of SHA256SUMS" >&2
        exit 1
    fi
    rm -f "$pem"
    echo "SHA256SUMS.sig is good"
}

case "${1:-}" in
    new)
        [[ $# -eq 1 ]] || usage
        [[ ! -e "$key" ]] || { echo "error: $key is already there; a new key would orphan every copy that trusts it" >&2; exit 1; }
        mkdir -p "$(dirname "$key")"
        (umask 077 && openssl genpkey -algorithm ed25519 -aes-256-cbc -out "$key")
        openssl pkey -in "$key" -pubout -outform DER | tail -c 32 | od -An -v -tx1 | tr -d ' \n' > "$public_file"
        echo >> "$public_file"
        echo "The private key is $key. Back it up off this machine."
        echo "Its public half is in $public_file: commit that file."
        ;;
    sign)
        [[ $# -eq 2 ]] || usage
        [[ -f "$2/SHA256SUMS" ]] || { echo "error: no SHA256SUMS in $2" >&2; exit 1; }
        openssl pkeyutl -sign -inkey "$key" -rawin -in "$2/SHA256SUMS" -out "$2/SHA256SUMS.sig"
        [[ "$(stat -c %s "$2/SHA256SUMS.sig")" -eq 64 ]] || { echo "error: the signature is not 64 bytes" >&2; exit 1; }
        verify "$2"
        ;;
    verify)
        [[ $# -eq 2 ]] || usage
        verify "$2"
        ;;
    *)
        usage
        ;;
esac
