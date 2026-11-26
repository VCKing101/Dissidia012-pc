#pragma once

// Bartz's moves from the first Dissidia (2008), as eight more abilities of
// his in Dissidia 012, named "(OG)". Nothing of either game is in the port:
// the moves are built from the player's own Dissidia 012 disc, by a recipe
// (port/features/bartz-original-moves.json) that says which records to add
// to his moveset and which of the disc's own animations, effects and sounds
// each move is made of. The first game's disc is not needed.
//
// This file is the builder: it turns the disc's files into the changed ones
// (his moveset, his four models, the ability table and its texts). It reads
// no files itself and needs nothing of PortableKit, so a test tool can run
// it (port/tests/check_original_moves.cpp); d012_original_moves_feature.cpp
// gives it the disc and hands the result to the game.
//
// It follows tools/original_moves.py step for step (`bring`, `abilities`,
// `Recipe`), and must come to the same bytes on every machine: online play
// needs both players' files the same. The recipe carries the size and CRC-32
// of every file the build reads and of every file it must come to; check()
// compares them. Research: docs/research/original-bartz.md. MIT licence; see
// port/README.md.

#include "d012_json.hpp"

#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace d012::original_moves {

// The data is not as the build expects. The message says where.
struct Error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

using Bytes = std::vector<std::uint8_t>;

// A file of the game's package by its path ("obj/p_fiv100.objx"); nothing
// when the package has none.
using Reader = std::function<std::optional<Bytes>(std::string_view path)>;

struct File {
    std::string path;  // package path
    Bytes bytes;
};

struct Built {
    std::vector<File> files;
    // (ability id, the level it is learned at; 0: the game gives it itself).
    std::vector<std::pair<std::uint16_t, int>> abilities;
    std::vector<std::string> notes;  // what was copied and left out, for the log
};

// What the recipe says of a file: its size and CRC-32.
struct Mark {
    std::string path;
    std::uint64_t size{};
    std::uint32_t crc{};
};

// The files a build reads, and the files it must come to, as the recipe's
// "check" block has them. Throws Error when the recipe has no such block.
[[nodiscard]] std::vector<Mark> inputs(const json::Value &recipe);
[[nodiscard]] std::vector<Mark> outputs(const json::Value &recipe);
// The abilities the recipe adds: (id, learn level), as Built::abilities.
[[nodiscard]] std::vector<std::pair<std::uint16_t, int>> abilities(const json::Value &recipe);

// CRC-32 as zlib's (polynomial 0xEDB88320, reflected).
[[nodiscard]] std::uint32_t crc32(std::span<const std::uint8_t> bytes);

// Builds the changed files from the disc's. Throws Error.
[[nodiscard]] Built build(const json::Value &recipe, const Reader &read);

// Whether `built` is what the recipe says a build comes to: the same paths,
// sizes and CRCs. `problem` says the first difference.
[[nodiscard]] bool check(const json::Value &recipe, const Built &built, std::string &problem);

// --- His looks from DLC ------------------------------------------------------
// The build gives the moves' animations to his four looks on the disc. A DLC
// look (the official DLC's fourth, or a costume pack) is a model file of its
// own on the memory stick, which the build never sees: in it the moves found
// no animation and he held still in his bind pose, and online the other copy,
// drawing the look it has, ran the move, so the two battles went apart
// (docs/port/original-moves.md, "Looks from DLC").

// The model a DLC costume row (dlc/obj/dlc_NNNoe.bin: {u32 0x0C, u32 count}
// and 0x44-byte rows) gives Bartz as a fighter, "P_FIV100_4P"; nothing when
// the file is no such row, or the row is another character's, an assist's or
// an attachment's.
[[nodiscard]] std::optional<std::string> fighter_look_model(std::span<const std::uint8_t> row_file);

enum class LookFit {
    has_moves,    // it has the moves' animations as the build made them
    moves_added,  // it is his model with his own animations: the moves' are added
    cannot,       // the moves cannot go in; `why` says what is in the way
};
struct FittedLook {
    LookFit fit{};
    Bytes bytes;      // with moves_added: the model with the moves' animations
    std::string why;  // with cannot
};
// How a DLC look of his can have the moves. `built` is his first look as the
// build made it (obj/P_FIV100_1P.gmo), whose last `added` animations are the
// moves'. A look takes them when it has the same bones and the same
// animations, by name and in order, as that model before them: they are
// added after its own, the same bytes the build adds to the disc's looks, so
// every look of his runs the moves alike. `largest` is the most bytes a model
// may come to (the game's staging block for model files).
[[nodiscard]] FittedLook fit_look(std::span<const std::uint8_t> model, std::span<const std::uint8_t> built, std::size_t added,
                                  std::uint64_t largest);
// How many animations the recipe's build adds to each of his looks.
[[nodiscard]] std::size_t added_animations(const json::Value &recipe);

} // namespace d012::original_moves
