#pragma once

// An audio file as the Battle BGM Maker sees it: what FFmpeg says of it
// (codec, rate, channels, length, tags), its loop tags, and for a BRSTM,
// BCSTM or BFSTM its header. Read with the same FFmpeg as the game, through
// the host's own file I/O as PortableKit's host_audio.cpp does, so a file
// reads the same in both. MIT licence; see port/README.md.

#include "d012_nintendo_stream.hpp"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace d012::bgm_maker {

// A loop the file carries: LOOPSTART with LOOPLENGTH or LOOPEND tags, or a
// Nintendo stream header's (to the end of the stream). What "loop": "auto" plays.
struct FileLoop {
    std::int64_t start{};
    std::optional<std::int64_t> end;  // samples of the file; none: its end
    std::string source;               // "the file's loop tags", "the BRSTM header"
};

struct AudioInfo {
    std::string error;  // why it cannot be read; empty when it can
    std::string codec;  // FFmpeg's name ("mp3", "vorbis", "adpcm_thp")
    int rate{};
    int channels{};
    double seconds{};   // from the container, as FFmpeg estimates it
    std::string title;  // the title tag, when there is one
    std::optional<FileLoop> loop;
    std::optional<nintendo_stream::StreamInfo> stream;
    std::string stream_problem;  // a header loop that starts past the end
    std::string stream_description;  // "BRSTM (Wii, big-endian), DSP-ADPCM"
    // Once decoded: its exact length, and the rate it decodes at (Opus is
    // always 48 kHz), which loop samples count in.
    std::optional<std::int64_t> samples;
    int decoded_rate{};
};

[[nodiscard]] AudioInfo probe_audio(const std::filesystem::path &file);

// A whole file decoded at its own rate, as the game decodes it (the packet
// time base set, so encoder delay and padding are dropped the same way), to
// 16-bit samples, interleaved; more than two channels are mixed to two. A
// sample here is the sample of the file a loop point counts.
struct DecodedAudio {
    std::string error;
    int rate{};
    int channels{};        // 1 or 2
    int source_channels{};
    std::int64_t frames{};
    std::vector<std::int16_t> samples;  // frames * channels
};

[[nodiscard]] DecodedAudio decode_audio(const std::filesystem::path &file);

// One line for the editor: "MP3, 44,100 Hz, stereo, 3:21".
[[nodiscard]] std::string describe(const AudioInfo &info);

} // namespace d012::bgm_maker
