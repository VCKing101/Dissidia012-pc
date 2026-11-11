#pragma once

// Japanese voices: the voices of the player's own Japanese disc (NPJH50377)
// in place of the English ones, with the game's text left as it is.
//
// Every voice file is in a language folder of PACKAGE.BIN
// ("voice/EN/battle/li_battle_91.at3"), which the game finds by a hash of
// its name alone (docs/research/character-assets.md). The Japanese disc has
// the same files under "voice/JP/": the same names, clip ids and banks, in
// the same formats. The hash is a CRC, so the hash of the English name
// follows from the hash of the Japanese one and the name's length without
// knowing the name; that pairs the files of the two discs. The game then
// reads its own index of PACKAGE.BIN with each paired English record
// pointing at the Japanese file instead, served from the Japanese disc after
// the end of PACKAGE.BIN. Story scenes whose script differs between the
// discs play as the Japanese disc has them, with the game's text
// (d012_voice_scenes.hpp), and the story movies play the game's picture
// with the Japanese sound.
//
// The player chooses the disc and the voices in the game's own Options
// (d012_voices_menu.cpp): settings.ini's voices.japanese_disc and
// voices.language. <prefix>_JAPANESE_VOICES=<image> sets both for one run
// without saving them, and =0 keeps the voices English.
// docs/port/japanese-voices.md has the findings. MIT licence; see
// port/README.md.

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

namespace portablekit {
class HleRegistrar;
}
namespace psprecomp {
class GuestMemory;
}

namespace d012::voices {

void install(portablekit::HleRegistrar &hle);
// The rows in the game's Options (d012_voices_menu.cpp).
void install_menu(portablekit::HleRegistrar &hle);

// The Options rows' text, added to the game's text/EN/menu/option.bin as
// strings of these numbers (d012_voices_menu.cpp names them).
inline constexpr std::uint16_t kTextFirst = 200;
inline constexpr std::uint16_t kTextDiscTitle = 200;
inline constexpr std::uint16_t kTextDiscValue = 201;  // the disc's file name, or "Not set"
inline constexpr std::uint16_t kTextDiscHelp = 202;
inline constexpr std::uint16_t kTextLanguageTitle = 203;
inline constexpr std::uint16_t kTextLanguageHelp = 204;
inline constexpr std::uint16_t kTextCount = 5;
// The disc's name is kept in this many characters, the terminator included,
// so that it can change in the game's memory while the screen shows it.
inline constexpr std::size_t kDiscValueUnits = 40;

// A Japanese disc is chosen and can be read.
[[nodiscard]] bool disc_ready();
// The disc row's value: the image's file name, shortened, or "Not set".
[[nodiscard]] std::u16string disc_label();
// Whether `image` is the Japanese release, without using it: an error to
// show the player when it is not. Any thread.
std::optional<std::string> check_disc(const std::filesystem::path &image);
// Uses `image` as the Japanese disc and saves it in settings.ini; with
// Japanese voices on, the game's index in `memory` follows at once. Call on
// the emulation thread. An error to show the player when it is not one.
std::optional<std::string> choose_disc(psprecomp::GuestMemory &memory, const std::filesystem::path &image);
// Japanese voices are on (and a disc is ready).
[[nodiscard]] bool japanese();
// Turns them on or off, saves it, and moves the game's index records in its
// memory at once: files the game reads from now on are in that language.
// Call on the emulation thread.
void set_japanese(psprecomp::GuestMemory &memory, bool on);

} // namespace d012::voices
