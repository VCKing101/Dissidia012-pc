// Decoding tracks for the Loops step; see decoder.hpp. MIT licence; see
// port/README.md.

#include "decoder.hpp"

#include <algorithm>
#include <limits>

namespace d012::bgm_maker {
namespace {

constexpr std::size_t kKept = 3;  // tracks kept decoded, as the Python tool's page keeps them
constexpr std::int64_t kSmallBlock = 256;
constexpr std::int64_t kLargeBlock = 8192;

Peaks peaks_of(const DecodedAudio &audio, std::int64_t block) {
    Peaks peaks;
    peaks.block = block;
    const std::int64_t count = (audio.frames + block - 1) / block;
    peaks.levels.assign(static_cast<std::size_t>(audio.channels), {});
    for (int c = 0; c < audio.channels; ++c) {
        auto &out = peaks.levels[static_cast<std::size_t>(c)];
        out.resize(static_cast<std::size_t>(count) * 2u);
        for (std::int64_t i = 0; i < count; ++i) {
            int lo = std::numeric_limits<std::int16_t>::max();
            int hi = std::numeric_limits<std::int16_t>::min();
            const std::int64_t end = std::min(audio.frames, (i + 1) * block);
            for (std::int64_t j = i * block; j < end; ++j) {
                const int v = audio.samples[static_cast<std::size_t>(j * audio.channels + c)];
                lo = std::min(lo, v);
                hi = std::max(hi, v);
            }
            out[static_cast<std::size_t>(2 * i)] = static_cast<std::int16_t>(lo);
            out[static_cast<std::size_t>(2 * i + 1)] = static_cast<std::int16_t>(hi);
        }
    }
    return peaks;
}

} // namespace

std::optional<std::pair<int, int>> DecodedTrack::range(int channel, std::int64_t a, std::int64_t b) const {
    a = std::max<std::int64_t>(a, 0);
    b = std::min(b, audio.frames);
    if (b <= a) return std::nullopt;
    int lo = std::numeric_limits<std::int16_t>::max();
    int hi = std::numeric_limits<std::int16_t>::min();
    const auto direct = [&](std::int64_t from, std::int64_t to) {
        for (std::int64_t j = from; j < to; ++j) {
            const int v = sample(channel, j);
            lo = std::min(lo, v);
            hi = std::max(hi, v);
        }
    };
    const Peaks *level = nullptr;
    for (const Peaks &p : peaks)
        if (b - a >= p.block * 4) level = &p;
    if (level == nullptr) {
        direct(a, b);
        return std::pair{lo, hi};
    }
    const std::int64_t block = level->block;
    const auto &values = level->levels[static_cast<std::size_t>(channel)];
    const std::int64_t first = (a + block - 1) / block;
    const std::int64_t last = b / block;
    direct(a, std::min(b, first * block));
    for (std::int64_t i = first; i < last; ++i) {
        lo = std::min<int>(lo, values[static_cast<std::size_t>(2 * i)]);
        hi = std::max<int>(hi, values[static_cast<std::size_t>(2 * i + 1)]);
    }
    direct(std::max(a, last * block), b);
    return std::pair{lo, hi};
}

int DecodedTrack::mono(std::int64_t frame) const {
    int sum = 0;
    for (int c = 0; c < audio.channels; ++c) sum += sample(c, frame);
    return sum;
}

Decoder::Decoder() : worker_([this] { run(); }) {}

Decoder::~Decoder() {
    {
        const std::lock_guard lock(mutex_);
        stop_ = true;
        queue_.clear();
    }
    wake_.notify_all();
    worker_.join();
}

std::shared_ptr<const DecodedTrack> Decoder::get(const std::filesystem::path &file) {
    {
        const std::lock_guard lock(mutex_);
        for (auto it = done_.begin(); it != done_.end(); ++it) {
            if ((*it)->file != file) continue;
            auto track = *it;
            done_.erase(it);
            done_.push_back(track);  // the most recently used, kept longest
            return track;
        }
        if (working_ != file && std::find(queue_.begin(), queue_.end(), file) == queue_.end()) {
            // Only the track wanted now is worth decoding: an older request goes.
            queue_.clear();
            queue_.push_back(file);
        }
    }
    wake_.notify_one();
    return nullptr;
}

bool Decoder::busy() {
    const std::lock_guard lock(mutex_);
    return !working_.empty() || !queue_.empty();
}

void Decoder::clear() {
    const std::lock_guard lock(mutex_);
    done_.clear();
    queue_.clear();
}

void Decoder::run() {
    for (;;) {
        std::filesystem::path file;
        {
            std::unique_lock lock(mutex_);
            wake_.wait(lock, [this] { return stop_ || !queue_.empty(); });
            if (stop_) return;
            file = queue_.front();
            queue_.pop_front();
            working_ = file;
        }
        auto track = std::make_shared<DecodedTrack>();
        track->file = file;
        track->audio = decode_audio(file);
        if (track->audio.error.empty()) {
            track->peaks[0] = peaks_of(track->audio, kSmallBlock);
            track->peaks[1] = peaks_of(track->audio, kLargeBlock);
        }
        const std::lock_guard lock(mutex_);
        working_.clear();
        done_.push_back(std::move(track));
        while (done_.size() > kKept) done_.erase(done_.begin());
    }
}

} // namespace d012::bgm_maker
