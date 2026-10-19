# Notice

## What the MIT licence covers

The [MIT licence](LICENSE) covers the original work in this repository: the port's host code and build files
(`port/`), the mod maker (`port/tools/mod_maker/`), the example mod, the tools and scripts (`tools/`), the
configuration (`config/`) and the documentation (`docs/`, `README.md`).

## What it does not cover

- **The game.** *Dissidia 012 Duodecim Final Fantasy*, its code, data, art, music, text and other assets belong to
  Square Enix. None of them are included in this repository or distributed with it. To build or run anything here
  you need your own legally obtained copy of the game (USA ULUS10566).
- **Decompiled code.** The C and C++ in `src/`, `include/` and `port/decomp/` is reconstructed from the original
  game's machine code for interoperability and research. It describes how the original program works; the program it
  describes remains Square Enix's. The MIT licence applies only to whatever original contribution these files
  contain, not to the game.
- **Facts about the game.** Addresses, sizes, hashes, format descriptions and short identifiers in the
  documentation and configuration are recorded as facts for interoperability.
- **PortableKit.** The patches in `port/portablekit-patches/` modify [PortableKit](https://github.com/TeamGDB/PortableKit)
  (MIT, copyright PSPRecomp contributors and PortableKit contributors). The patches are offered under the same MIT
  licence; PortableKit itself is not included and keeps its own licence and notices.
- **Other third-party software** fetched or linked at build time (FFmpeg under the LGPL, SDL3, glslang, the Vulkan
  headers, and the libraries bundled by PortableKit) keeps its own licence.

## Trademarks

This project is not affiliated with or endorsed by Square Enix or Sony. *Dissidia*, *Final Fantasy*, PlayStation and
PlayStation Portable are trademarks of their respective owners.
