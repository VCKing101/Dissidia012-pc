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
};

[[nodiscard]] AudioInfo probe_audio(const std::filesystem::path &file);

// One line for the editor: "MP3, 44,100 Hz, stereo, 3:21".
[[nodiscard]] std::string describe(const AudioInfo &info);

} // namespace d012::bgm_maker
