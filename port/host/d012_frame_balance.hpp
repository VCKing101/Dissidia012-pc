// Keeping the two fighters' frames level (docs/port/communication-mode.md,
// section 17.1). Each copy holds its frames to its own clock, so where they fall
// against each other is set by when each started and by every stall since. A
// copy's slack on a frame is how long before it ran that frame the other copy's
// input for it came, less than 0 when the input came after. The copy with less
// is ahead of the other, and holds its frames back a little until both have the
// same. No game and no network in here: port/tests/d012_tests.cpp plays it out.
//
// MIT licence; see port/README.md.

#pragma once

#include <algorithm>
#include <chrono>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

namespace d012::net {

// In an INPUT: this copy does not know its slack yet.
inline constexpr std::int16_t kNoSlack = INT16_MIN;

class FrameBalance {
public:
    using Clock = std::chrono::steady_clock;

    // How many frames the slack is averaged over, the least difference acted
    // on, and the most a frame is held back.
    static constexpr std::size_t kFrames = 16u;
    static constexpr double kMargin = 2.0;  // ms
    static constexpr std::chrono::microseconds kMostHold{1000};

    // One frame's slack.
    void note(Clock::duration slack) {
        slacks_.push_back(std::chrono::duration<double, std::milli>(slack).count());
        if (slacks_.size() > kFrames) slacks_.erase(slacks_.begin());
        if (slacks_.size() < kFrames) return;
        double total = 0.0;
        for (const double s : slacks_) total += s;
        slack_ = total / static_cast<double>(slacks_.size());
        known_ = true;
    }
    // The other copy's average, from its latest INPUT.
    void hear(std::int16_t their_slack) { their_slack_ = their_slack; }

    [[nodiscard]] bool known() const { return known_; }
    [[nodiscard]] double slack() const { return slack_; }  // ms
    [[nodiscard]] std::int16_t their_slack() const { return their_slack_; }
    // This copy's average as an INPUT carries it.
    [[nodiscard]] std::int16_t sent_slack() const {
        return known_ ? static_cast<std::int16_t>(std::clamp(slack_, -30000.0, 30000.0)) : kNoSlack;
    }

    // How long to hold this frame back: a share of the difference, so the two
    // copies meet in the middle over a second or two. Nothing for the copy
    // that is behind, or while either slack is not known.
    [[nodiscard]] std::chrono::microseconds hold() const {
        if (!known_ || their_slack_ == kNoSlack) return std::chrono::microseconds(0);
        const double ahead = static_cast<double>(their_slack_) - slack_;  // ms this copy is ahead by, twice over
        if (ahead <= kMargin) return std::chrono::microseconds(0);
        return std::min(kMostHold,
                        std::chrono::microseconds(static_cast<long long>(ahead * 1000.0 / 2.0 / (2.0 * kFrames))));
    }

private:
    std::vector<double> slacks_;  // ms, the latest kFrames frames'
    double slack_ = 0.0;          // their average, once there are kFrames
    bool known_ = false;
    std::int16_t their_slack_ = kNoSlack;
};

// With rollback a frame does not wait for the other copy's input, so at any
// real distance that input comes after the frame has run. The slacks now known
// as frame `upto` runs: of every frame up to it whose input has come, when it
// ran (`ran`) less when its input came (`came`). Those frames leave both maps;
// frames still waiting for their input stay in `ran`.
inline std::vector<FrameBalance::Clock::duration> take_slacks(std::map<std::uint64_t, FrameBalance::Clock::time_point> &ran,
                                                              std::map<std::uint64_t, FrameBalance::Clock::time_point> &came,
                                                              std::uint64_t upto) {
    std::vector<FrameBalance::Clock::duration> slacks;
    for (auto it = came.begin(); it != came.end() && it->first <= upto; it = came.erase(it)) {
        const auto frame = ran.find(it->first);
        if (frame == ran.end()) continue;
        slacks.push_back(frame->second - it->second);
        ran.erase(frame);
    }
    return slacks;
}

} // namespace d012::net
