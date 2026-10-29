// Playing a decoded track; see player.hpp. MIT licence; see port/README.md.

#include "player.hpp"

#include <SDL3/SDL.h>

#include <algorithm>

namespace d012::bgm_maker {
namespace {

// A short silence between repeats of an audition (the Python tool's 0.6 s).
constexpr double kRepeatGapSeconds = 0.6;

} // namespace

Player::~Player() {
    stop();
    if (stream_ != nullptr) SDL_DestroyAudioStream(stream_);
}

void Player::play(std::shared_ptr<const DecodedTrack> track, std::int64_t from, std::optional<std::int64_t> start,
                  std::optional<std::int64_t> end) {
    if (!track || track->audio.frames <= 0) return;
    const std::int64_t frames = track->audio.frames;
    from = std::clamp<std::int64_t>(from, 0, frames - 1);
    std::vector<Segment> segments;
    int repeat_from = -1;
    if (start && end && *end > *start && from < *end) {
        segments.push_back({from, *end - from});
        segments.push_back({*start, *end - *start});
        repeat_from = 1;
    } else {
        segments.push_back({from, frames - from});
    }
    this->start(std::move(track), std::move(segments), repeat_from);
}

void Player::audition(std::shared_ptr<const DecodedTrack> track, std::int64_t start, std::int64_t end, std::int64_t pre,
                      std::int64_t post, bool repeat) {
    if (!track || end <= start) return;
    pre = std::min(pre, end);
    std::vector<Segment> segments{{end - pre, pre}};
    for (std::int64_t left = post, at = start; left > 0;) {
        const std::int64_t take = std::min(left, end - at);
        segments.push_back({at, take});
        left -= take;
        at = start;
    }
    if (repeat) segments.push_back({-1, static_cast<std::int64_t>(kRepeatGapSeconds * track->audio.rate)});
    this->start(std::move(track), std::move(segments), repeat ? 0 : -1);
}

// The device stream is opened once and kept: opening a Windows audio device
// takes long enough to be heard as a late start. Each new thing to play only
// changes the stream's input format.
bool Player::start(std::shared_ptr<const DecodedTrack> track, std::vector<Segment> segments, int repeat_from) {
    stop();
    const SDL_AudioSpec spec{SDL_AUDIO_S16, track->audio.channels, track->audio.rate};
    if (stream_ == nullptr) {
        if ((SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO) == 0 && !SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            error_ = std::string("no sound: ") + SDL_GetError();
            return false;
        }
        stream_ = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, &Player::feed, this);
        if (stream_ == nullptr) {
            error_ = std::string("no sound: ") + SDL_GetError();
            return false;
        }
    } else if (!SDL_SetAudioStreamFormat(stream_, &spec, nullptr)) {
        error_ = std::string("no sound: ") + SDL_GetError();
        return false;
    }
    {
        const std::lock_guard lock(mutex_);
        track_ = std::move(track);
        segments_ = std::move(segments);
        repeat_from_ = repeat_from;
        segment_ = 0;
        offset_ = 0;
        written_ = 0;
        finished_ = false;
    }
    error_.clear();
    SDL_ResumeAudioStreamDevice(stream_);
    return true;
}

void Player::stop() {
    {
        const std::lock_guard lock(mutex_);
        track_.reset();
        segments_.clear();
    }
    // Paused, the device asks for nothing more; what was queued goes.
    if (stream_ != nullptr) {
        SDL_PauseAudioStreamDevice(stream_);
        SDL_ClearAudioStream(stream_);
    }
}

// SDL calls feed() with the stream's own lock held, and feed() takes mutex_:
// SDL is never asked about the stream while mutex_ is held, or the two could
// wait for each other.
bool Player::playing() {
    {
        const std::lock_guard lock(mutex_);
        if (stream_ == nullptr || track_ == nullptr) return false;
        if (!finished_) return true;
    }
    return SDL_GetAudioStreamQueued(stream_) > 0;  // the end still sounding
}

void Player::feed(void *userdata, SDL_AudioStream *stream, int additional, int) {
    auto *self = static_cast<Player *>(userdata);
    const std::lock_guard lock(self->mutex_);
    if (self->track_ == nullptr || additional <= 0) return;
    const int frame_bytes = self->track_->audio.channels * 2;
    self->fill(stream, (additional + frame_bytes - 1) / frame_bytes);
}

void Player::fill(SDL_AudioStream *stream, int frames) {
    const DecodedAudio &audio = track_->audio;
    const std::size_t channels = static_cast<std::size_t>(audio.channels);
    std::vector<std::int16_t> silence;
    while (frames > 0 && !finished_) {
        if (segment_ >= segments_.size()) {
            if (repeat_from_ < 0) {
                finished_ = true;
                break;
            }
            segment_ = static_cast<std::size_t>(repeat_from_);
            offset_ = 0;
        }
        const Segment &segment = segments_[segment_];
        const std::int64_t take = std::min<std::int64_t>(frames, segment.count - offset_);
        if (take > 0) {
            if (segment.from < 0) {
                silence.assign(static_cast<std::size_t>(take) * channels, 0);
                SDL_PutAudioStreamData(stream, silence.data(), static_cast<int>(silence.size() * 2u));
            } else {
                const std::int16_t *data = audio.samples.data() + static_cast<std::size_t>(segment.from + offset_) * channels;
                SDL_PutAudioStreamData(stream, data, static_cast<int>(static_cast<std::size_t>(take) * channels * 2u));
            }
            offset_ += take;
            written_ += take;
            frames -= static_cast<int>(take);
        }
        if (offset_ >= segment.count) {
            ++segment_;
            offset_ = 0;
        }
    }
}

std::optional<std::int64_t> Player::frame_at(std::int64_t played) const {
    // Walk the program: once through, then the repeated part in cycles.
    std::int64_t at = played;
    for (std::size_t i = 0; i < segments_.size(); ++i) {
        if (repeat_from_ >= 0 && i == static_cast<std::size_t>(repeat_from_)) {
            std::int64_t cycle = 0;
            for (std::size_t j = i; j < segments_.size(); ++j) cycle += segments_[j].count;
            if (cycle > 0) at %= cycle;
            for (std::size_t j = i; j < segments_.size(); ++j) {
                if (at < segments_[j].count)
                    return segments_[j].from < 0 ? std::nullopt : std::optional<std::int64_t>(segments_[j].from + at);
                at -= segments_[j].count;
            }
            return std::nullopt;
        }
        if (at < segments_[i].count)
            return segments_[i].from < 0 ? std::nullopt : std::optional<std::int64_t>(segments_[i].from + at);
        at -= segments_[i].count;
    }
    return std::nullopt;
}

std::optional<std::int64_t> Player::position() {
    if (stream_ == nullptr) return std::nullopt;
    // What SDL holds has not sounded yet.
    const int queued = SDL_GetAudioStreamQueued(stream_);
    const std::lock_guard lock(mutex_);
    if (track_ == nullptr) return std::nullopt;
    const std::int64_t played = written_ - std::max(queued, 0) / (track_->audio.channels * 2);
    return frame_at(std::max<std::int64_t>(played, 0));
}

} // namespace d012::bgm_maker
