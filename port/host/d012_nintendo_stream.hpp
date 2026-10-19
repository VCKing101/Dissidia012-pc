#pragma once

// Nintendo stream files (BRSTM; BCSTM; BFSTM, Wii U big-endian and Switch
// little-endian): the loop in their header. These formats carry no tags, so
// a mod track's "loop": "auto" takes the header's loop flag, loop start and
// sample count (the loop runs from the start to the end of the stream).
// FFmpeg's brstm and bfstm demuxers decode the audio; their "loop_start"
// metadata is in microseconds, so it is not used. The Battle BGM Maker reads headers
// with the same rules (port/tools/bgm_maker/bgmmaker/nintendo.py). The format
// notes are docs/port/mod-folder.md, section 3. MIT licence; see
// port/README.md.

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace d012::nintendo_stream {

struct StreamInfo {
    std::string format;  // "BRSTM", "BCSTM" or "BFSTM"
    bool big_endian{};
    int codec{};         // 0 PCM8, 1 PCM16, 2 DSP-ADPCM, 3 IMA-ADPCM
    unsigned channels{};
    std::uint32_t rate{};
    bool loop_flag{};
    std::uint32_t loop_start{};  // a sample of the stream, counted from 0
    std::uint32_t samples{};     // the stream's length, which is also its loop end
};

// True for ".brstm", ".bcstm" and ".bfstm" (lower case).
[[nodiscard]] bool is_stream_extension(std::string_view extension);

// The stream info from the first bytes of a file (enough for its HEAD or
// INFO block); nullopt with `error` saying why when it is not such a file,
// or not one the port can decode (only DSP-ADPCM is: FFmpeg refuses IMA
// ADPCM and decodes PCM streams wrongly).
[[nodiscard]] std::optional<StreamInfo> parse_header(std::span<const std::uint8_t> data, std::string &error);

// parse_header() of a file, reading only its header.
[[nodiscard]] std::optional<StreamInfo> read_header(const std::filesystem::path &file, std::string &error);

[[nodiscard]] const char *codec_name(int codec);

} // namespace d012::nintendo_stream
