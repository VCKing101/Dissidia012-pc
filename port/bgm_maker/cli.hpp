#pragma once

// The Battle BGM Maker without its window: the window's own code (adding
// files, the checks, saving) for scripts and the end-to-end checks
// (port/tools/bgm_maker/tests/port_check.py and stream_port_check.py).
// MIT licence; see port/README.md.
//
//   BgmMaker --check <mod folder>
//       Reads every track's file to the end and prints the mod's problems,
//       and a line for each track: "track <key>: <file>, <n> samples at <rate> Hz (<kind>)".
//   BgmMaker --import <mod folder> <series key> <file>...
//       Copies the files into music/ as tracks of that series (made when it is
//       not there), titles from their tags or names, keys from their titles,
//       and saves. A folder without mod.json becomes a mod named after it.
//       Prints "added <track key>: <file in the mod>" for each.
//   BgmMaker --set-loop <mod folder> <track key> auto|whole|none|<start> [<end>]
//       Sets a track's loop (samples of the file) and saves.
//
// Exit status: 0 done (warnings allowed), 1 problems (nothing saved), 2 wrong use.

namespace d012::bgm_maker {

// For one of the switches above: its exit status. -1: not a switch of these.
[[nodiscard]] int run_command_line(int argc, char **argv);

} // namespace d012::bgm_maker
