#pragma once

// Story scenes with Japanese voices (d012_voices.cpp): the scenes whose
// script differs between the USA and Japanese discs, played from the
// Japanese disc's file with the game's own text.
//
// Each line of a story scene waits a fixed number of frames, tuned to the
// recordings of its own release, and the English release rewrote 302 voiced
// scenes: lines added, cut, moved and re-timed. With the English script and
// the Japanese voice files, lines ran into each other and the 37 narrator
// lines recorded only in English stayed English (issue 18). So with Japanese
// voices the game reads each of those scenes as the Japanese disc has it,
// with the game's text in it, and the text files it shows lines from fixed
// where the Japanese script shows a line the English text lacks.
// docs/port/japanese-voices.md ("Story scenes") has the findings. Pure:
// nothing here reads a disc. MIT licence; see port/README.md.

#include <cstdint>
#include <functional>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace d012::voice_scenes {

// A voice line of a Japanese script that the English game has no file name
// for, and the English name it is served under: a name no scene plays with
// Japanese voices on. Both as a scene names them ("voice/jp/event/...",
// which the game rewrites to its language).
struct VoiceName {
    const char *japanese;
    const char *english;
};

// The scenes (talkevent archives, by name hash), sorted.
[[nodiscard]] std::span<const std::uint32_t> scenes();
[[nodiscard]] std::span<const VoiceName> voice_names();
// The text files whose lines change for the Japanese scripts, by name.
[[nodiscard]] std::span<const std::string_view> text_files();

// A voice line of a merged scene: the name the game asks for (as the scene
// has it) and the Japanese disc's file it is.
struct Voice {
    std::string asked;
    std::string japanese;
};

// The scene `scene` as the game plays it with Japanese voices: `japanese`
// (the Japanese disc's archive) with the text chunk of `own` (the game's),
// its lines fixed for the Japanese script, and its voice table naming the
// lines the game has no name for as voice_names() says. `voices` gets the
// table's lines. Empty when either archive is not laid out as expected (the
// game's own is played then).
[[nodiscard]] std::vector<std::uint8_t> merge_scene(std::uint32_t scene, std::span<const std::uint8_t> japanese,
                                                    std::span<const std::uint8_t> own, std::vector<Voice> &voices);

// The game's text file `name` (one of text_files()) with its lines fixed for
// the Japanese scripts. Empty when it is not laid out as expected.
[[nodiscard]] std::vector<std::uint8_t> fix_text_file(std::string_view name, std::span<const std::uint8_t> own);

// A story movie (PSMF) with the sound of another copy of it: the game's file,
// its picture and all, with the Japanese disc's sound. Some movies' pictures
// differ between the discs (the end credits, for one), so the Japanese file
// cannot play as it is. A movie is an MPEG program stream from the offset at
// byte 8 of its header (big endian): 2,048-byte packs, each a pack header and
// whole PES packets. The sound is private stream 1 (0xBD): after the PES
// header, 4 bytes (the stream number, and where in the packet the first
// frame starts) and then ATRAC3plus frames, 752 bytes each on both discs
// from the first sound byte on. So the Japanese sound's byte n goes where the
// game's sound byte n is, and each packet's 4 bytes stay right; where the
// Japanese sound is shorter the game's stays. Both files are scanned as the
// game reads, so nothing is read ahead.
class MovieSplice {
public:
    // Up to out.size() bytes of a file at an offset; how many were read.
    using Reader = std::function<std::size_t(std::uint64_t, std::span<std::uint8_t>)>;
    // Null when either is not a movie as expected.
    [[nodiscard]] static std::unique_ptr<MovieSplice> open(Reader own, Reader theirs);
    ~MovieSplice();
    // Up to out.size() bytes of the game's movie at `offset`, with their
    // sound: how many.
    std::size_t read(std::uint64_t offset, std::span<std::uint8_t> out);
    // A pack was not as expected: from there on the sound stays the game's.
    [[nodiscard]] bool broken() const;

    struct Scan;

private:
    MovieSplice(Reader own, Reader theirs, std::uint64_t own_start, std::uint64_t their_start);
    Reader read_own_, read_theirs_;
    std::unique_ptr<Scan> own_, theirs_;
};

// For the tests: a language container's tables, each string's 16-bit units
// as shown (the terminator included), an absent string empty.
[[nodiscard]] std::vector<std::vector<std::u16string>> read_container(std::span<const std::uint8_t> container);
[[nodiscard]] std::vector<std::uint8_t> write_container(const std::vector<std::vector<std::u16string>> &tables);

} // namespace d012::voice_scenes
