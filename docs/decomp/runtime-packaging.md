# Local runtime test packages

`tools/prepare_runtime_packages.py` creates two disposable ISO images beneath
`build/ULUS10566/runtime/packages/`:

- `ULUS10566-original-decrypted.iso`: the pinned decrypted main ELF and original overlays.
- `ULUS10566-rebuilt.iso`: the four verified outputs produced by the reconstruction builds.

Run from the project root:

```powershell
.venv/Scripts/python.exe tools/prepare_runtime_packages.py
```

The user's USA ISO remains the retail baseline and is opened read-only. Its full
SHA256 must match `config/ulus10566.json` before preparation and again afterward.
All four build proofs must be current, and each rebuilt file is fully verified.
Packaging does not change source-recovery progress or establish runtime success.

The encrypted EBOOT.BIN is 4,641,296 bytes. The pinned decrypted EBOOT.ELF is
4,640,948 bytes, so it fits the same allocation. The tool replaces its payload,
updates the ISO9660 directory length in both endian encodings, and zeroes the
348 retired bytes and existing sector padding. Overlay lengths remain unchanged.
File LBAs, boot placeholder, assets, update files, volume descriptors and all
other bytes remain unchanged.

The parser rejects directory aliases/cycles, duplicate paths, unsupported
interleaving or multi-extent files, endian disagreements, overlapping allocations,
nonzero original padding and replacements larger than their original file. An
independent streamed verification compares every output byte with the source plus
the explicit patch ranges. The finished directory tree is parsed again and each
replacement's exact size, LBA and SHA256 are checked.

Output writes use a temporary file and atomic directory-entry replacement. A
same-file/hard-link alias of the source is rejected, and a link to another image
cannot cause that image to be truncated.

`manifest.json` records source and output hashes, per-file geometry, separate
input provenance and all four build fingerprints. The two packages are generated
independently. Their bytes should be identical because the current rebuilt files
are byte-identical to their references. This comparison checks packaging and build
provenance; it does not create a new behavior difference to test.

These are **local decrypted test packages**, not retail-encrypted or signed
releases. Their compatibility must be established by actual PPSSPP runs.
The manifest initially records `runtime_verified: false`; runtime results belong
in separate run evidence rather than overwriting packaging provenance.

Safety checks use synthetic data:

```powershell
.venv/Scripts/python.exe -m unittest discover -s tools -p test_runtime_packages.py -v
```
