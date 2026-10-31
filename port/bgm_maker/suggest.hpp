#pragma once

// Loop suggestions: a start and an end whose audio repeats. Only suggestions:
// the modder auditions them and chooses. The Python tool's method
// (port/tools/bgm_maker/web/suggest.js), ported as it is, down to which values
// it keeps as 32-bit floats and how JavaScript rounds, so both give the same
// candidates for the same file:
//
// 1. Fingerprints: the file (mono, decimated to about 11 kHz) is cut into
//    frames; each frame's spectrum is summed into 32 log-spaced bands, logged,
//    centred on the file's mean and normalised.
// 2. Matches: for every lag L (a loop length), frame t is compared with frame
//    t - L. A real loop is a long run of frames matching their copy one loop
//    earlier. "Suggest loops" ranks lags by how long and how well they match;
//    "Starts for my end" keeps the loop end and ranks lags by how well the
//    seconds before it match the seconds before end - L.
// 3. Refinement: the length is fixed to the sample by normalised
//    cross-correlation of the waveform (decimated, then at full rate), and the
//    end moved, within a quarter of a second, to where the samples either side
//    of the seam match best.
//
// MIT licence; see port/README.md.

#include "decoder.hpp"

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

namespace d012::bgm_maker {

struct LoopCandidate {
    std::int64_t start{};
    std::int64_t end{};
    std::int64_t length{};
    double score{};     // the waveform's match across the seam, 1 for the same
    double level_db{};  // the audio before the end against the audio before the start
    double coarse{};    // the fingerprints' score
};

struct SuggestResult {
    std::vector<LoopCandidate> candidates;  // best first, at most five
    std::string note;                       // why there are none, when there are none
    double hop_seconds{};
};

// The channels averaged, as the Python tool's page makes its mono signal.
[[nodiscard]] std::vector<float> mono_signal(const DecodedTrack &track);

// `end`: the loop end to keep ("Starts for my end"), or -1 to search all of it.
// `progress` is told how far it has got (0 to 1); `cancel` stops it early.
[[nodiscard]] SuggestResult suggest_loops(const std::vector<float> &mono, int rate, std::int64_t end, double min_loop_seconds,
                                          const std::function<void(double)> &progress,
                                          const std::atomic<bool> *cancel = nullptr);

// suggest_loops() on a worker thread, for the window: one search at a time,
// a new one (or cancel()) stopping the last.
class Suggester {
public:
    ~Suggester();
    void start(std::shared_ptr<const DecodedTrack> track, std::int64_t end, double min_loop_seconds);
    void cancel();
    [[nodiscard]] bool busy() const noexcept { return busy_.load(); }
    [[nodiscard]] double progress() const noexcept { return progress_.load(); }
    // The finished search's result, once, and which track and mode it was for.
    struct Done {
        std::filesystem::path file;
        bool for_end{};
        SuggestResult result;
        double seconds{};
    };
    std::optional<Done> take();

private:
    std::thread worker_;
    std::atomic<bool> cancel_{};
    std::atomic<bool> busy_{};
    std::atomic<double> progress_{};
    std::mutex mutex_;
    std::optional<Done> done_;
};

} // namespace d012::bgm_maker
