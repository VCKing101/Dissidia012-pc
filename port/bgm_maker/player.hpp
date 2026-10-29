#pragma once

// Playing a decoded track through SDL3 audio, as the game plays it: on a
// loop, the loop's end runs straight into its start. What plays is a list of
// segments of the file (and of silence), the part from some index on repeated,
// so the sample under the playhead is always known exactly: playing through
// the loop, or the seam on its own, sample-exact (the Python tool's audition).
// MIT licence; see port/README.md.

#include "decoder.hpp"

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

struct SDL_AudioStream;

namespace d012::bgm_maker {

class Player {
public:
    struct Segment {
        std::int64_t from{};   // a frame of the file; -1: silence
        std::int64_t count{};
    };

    Player() = default;
    ~Player();
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;

    // Plays from `from`: with a loop (`end` > `start`) it runs from `end` into
    // `start` for ever, as the game does when `from` is before the end; without
    // one it stops at the end of the file.
    void play(std::shared_ptr<const DecodedTrack> track, std::int64_t from, std::optional<std::int64_t> start,
              std::optional<std::int64_t> end);
    // The seam: `pre` frames before `end`, then `post` from `start` on (round
    // the loop again if it is shorter); with `repeat`, a moment's silence and
    // again, for ever.
    void audition(std::shared_ptr<const DecodedTrack> track, std::int64_t start, std::int64_t end, std::int64_t pre,
                  std::int64_t post, bool repeat);
    void stop();

    [[nodiscard]] bool playing();
    // The frame of the file sounding now; nothing in a silence or when stopped.
    [[nodiscard]] std::optional<std::int64_t> position();
    [[nodiscard]] const DecodedTrack *track() const noexcept { return track_.get(); }
    // Why there is no sound, when there is none.
    [[nodiscard]] const std::string &error() const noexcept { return error_; }

private:
    bool start(std::shared_ptr<const DecodedTrack> track, std::vector<Segment> segments, int repeat_from);
    static void feed(void *userdata, SDL_AudioStream *stream, int additional, int total);
    void fill(SDL_AudioStream *stream, int frames);
    [[nodiscard]] std::optional<std::int64_t> frame_at(std::int64_t played) const;

    std::mutex mutex_;
    SDL_AudioStream *stream_{};
    std::shared_ptr<const DecodedTrack> track_;
    std::vector<Segment> segments_;
    int repeat_from_{-1};  // segments from here on play again and again; -1: once
    std::size_t segment_{};
    std::int64_t offset_{};   // into the current segment
    std::int64_t written_{};  // frames given to SDL so far
    bool finished_{};
    std::string error_;
};

} // namespace d012::bgm_maker
