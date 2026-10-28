# Contributing

Thanks for wanting to help. Bug reports, fixes, port features, decompiled functions, tools, documentation and mods
are all welcome. This is a one-person, spare-time project, so replies may take a few days.

## Before you start

- **Small fixes** (a typo, a build step, a one-function bug) can go straight to a pull request.
- **Anything bigger** (a new feature, a new tool, a change to how mods work, work from another project of yours):
  open an issue first and describe what you have in mind. That way we can agree on where it fits before you spend
  time on it, and you won't be building against something that is about to change.
- **Questions** are fine as issues too.

## The one hard rule: no game data

This repository holds no Square Enix game data, and it has to stay that way for the project to stay up. See
[NOTICE.md](NOTICE.md). Please don't commit, attach or link to:

- anything from *Dissidia 012*: the ISO, the EBOOT, extracted or decrypted files, models, textures, animations,
  sounds, music, movies or text;
- anything from **other games** either, such as *Dissidia NT*, the *Kingdom Hearts* collections or *Final Fantasy
  XV*. Their assets belong to their owners just as 012's do;
- converted, re-encoded, upscaled or edited versions of any of the above. A changed file is still made from the
  original;
- save files, which hold game data and personal data.

What **is** welcome is the code that works with that data: a tool that reads files from the player's **own** copy of
a game and builds what's needed on their PC. That is how the port itself works: `setup` builds everything from the
player's ISO, and nothing made from it leaves their machine. Content from other games fits the same way, as a
converter the player runs on their own files, producing a mod.

Mods only **add** to the game and never replace what's there: a mod adds a series of tracks, it doesn't overwrite
the original ones. See [the mod folder](docs/port/mod-folder.md).

Facts about the game (addresses, sizes, hashes, format descriptions) are fine in code and docs.

## Building and testing

- **Building the port from source:** [Building from source](README.md#building-from-source), with more detail in
  [docs/port/setup.md](docs/port/setup.md) and the manual steps in [port/README.md](port/README.md).
- **The matching decompilation:** [getting started](docs/decomp/getting-started.md). It has its own toolchain (the
  PSP SNC compiler under WSL).
- **Tests:**

  ```powershell
  .\.venv\Scripts\python.exe -m unittest discover -s tools -p "test_*.py"
  python -m unittest discover -s port/tools/bgm_maker/tests
  ```

  Many tool tests need the locally extracted game files and skip or fail without them. Say in your pull request which
  tests you ran.

## Making changes

- **Match the code around you:** its naming, comment style and layout. There is no separate style guide.
- **PortableKit changes** are kept as numbered patches in `port/portablekit-patches/`, applied in order on top of
  PortableKit. Make your change as a commit on a PortableKit clone with the existing patches applied, export it with
  `git format-patch` as the next number, and add a row for it to the table in [port/README.md](port/README.md).
- **Matching decompilation** must still rebuild byte-for-byte: the verify step has to pass.
- **Visible changes** (graphics, menus, controls): a screenshot or short clip in the pull request helps a lot.
- **Docs:** if you change how something is built, run or modded, update its page in `docs/` or the README.

## AI use

AI assistants are allowed: this project is built with a lot of AI help itself. You are responsible for what you
submit, so understand it and test it.

Every pull request must include **your own testing notes**, written by you after testing the change yourself: what
you built, what you ran or played, on what system, and what you saw. AI-generated test reports or summaries don't
count on their own, and a pull request without human testing notes won't be merged.

## Pull requests

1. Fork the repository and make a branch for your change.
2. Keep one change per pull request; several small ones are easier to review than one large one.
3. Open the pull request against `main` and fill in the template: what it changes, why, and your own notes on how
   you tested it (see [AI use](#ai-use)).
4. I may ask for changes. Once it's merged it ships in the next release, and you'll be thanked in its notes unless
   you'd rather not be.

## Reporting bugs

Open an issue with the bug report form. Say which version you're on (a release such as v0.2.0, or a commit), your
Windows version and GPU, what you did, what you expected and what happened. Error text and logs help, as long as they
contain no game files.

## Licence

By submitting a contribution you agree that it is offered under this repository's [MIT licence](LICENSE), and that
you have the right to offer it. If it includes someone else's code, say where it comes from and under which licence,
so that it can be credited and checked as compatible.
