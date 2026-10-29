#pragma once

// Decoding tracks for the Loops step on a worker thread, and keeping the last
// few: the samples (audio_file.hpp decode_audio) and their peaks for drawing,
// the minimum and maximum of each channel in blocks of 256 and 8192 samples,
// as the Python tool's waveform view keeps them. MIT licence; see
// port/README.md.

#include "audio_file.hpp"

#include <array>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

namespace d012::bgm_maker {

struct Peaks {
    std::int64_t block{};
    // Per channel: minimum, maximum of each block, one after the other.
    std::vector<std::vector<std::int16_t>> levels;
};

struct DecodedTrack {
    std::filesystem::path file;
    DecodedAudio audio;
    std::array<Peaks, 2> peaks;  // blocks of 256, then 8192

    [[nodiscard]] std::int16_t sample(int channel, std::int64_t frame) const {
        return audio.samples[static_cast<std::size_t>(frame) * static_cast<std::size_t>(audio.channels) +
                             static_cast<std::size_t>(channel)];
    }
    // The minimum and maximum of `channel` over frames [a, b); nothing when empty.
    [[nodiscard]] std::optional<std::pair<int, int>> range(int channel, std::int64_t a, std::int64_t b) const;
    // The channels summed, at `frame` (for zero crossings).
    [[nodiscard]] int mono(std::int64_t frame) const;
};

class Decoder {
public:
    Decoder();
    ~Decoder();
    Decoder(const Decoder &) = delete;
    Decoder &operator=(const Decoder &) = delete;

    // The track decoded, or null while it decodes (it starts now if it has not).
    std::shared_ptr<const DecodedTrack> get(const std::filesystem::path &file);
    [[nodiscard]] bool busy();
    // Forgets a file (it changed, or the mod was left).
    void clear();

private:
    void run();

    std::mutex mutex_;
    std::condition_variable wake_;
    std::deque<std::filesystem::path> queue_;
    std::vector<std::shared_ptr<const DecodedTrack>> done_;  // the most recent last
    std::filesystem::path working_;
    bool stop_{};
    std::thread worker_;
};

} // namespace d012::bgm_maker
