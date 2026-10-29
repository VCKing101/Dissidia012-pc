// Reading track files to the end; see measurer.hpp. MIT licence; see
// port/README.md.

#include "measurer.hpp"

#include "d012_mod_manifest.hpp"

#include <algorithm>

namespace d012::bgm_maker {

AudioInfo measure_audio(const std::filesystem::path &file) {
    AudioInfo info = probe_audio(file);
    if (!info.error.empty()) return info;
    const DecodedAudio decoded = decode_audio(file, false);
    if (!decoded.error.empty()) {
        info.error = decoded.error;
        return info;
    }
    info.samples = decoded.frames;
    info.decoded_rate = decoded.rate;
    return info;
}

FileFacts facts_of(const AudioInfo &info) {
    FileFacts facts;
    facts.error = info.error;
    facts.rate = info.decoded_rate > 0 ? info.decoded_rate : info.rate;
    facts.samples = info.samples;
    if (info.stream) {
        facts.header_samples = info.stream->samples;
        facts.header_format = info.stream->format;
    }
    return facts;
}

Measurer::Measurer() : worker_([this] { run(); }) {}

Measurer::~Measurer() {
    {
        const std::lock_guard lock(mutex_);
        stop_ = true;
        jobs_.clear();
    }
    wake_.notify_all();
    worker_.join();
}

void Measurer::add(const std::string &file, const std::filesystem::path &full, int tag) {
    {
        const std::lock_guard lock(mutex_);
        const bool waiting = std::any_of(jobs_.begin(), jobs_.end(),
                                         [&](const Job &job) { return job.file == file && job.tag == tag; });
        if (waiting) return;
        if (jobs_.empty() && current_.empty()) batch_total_ = batch_done_ = 0;
        jobs_.push_back({file, full, tag});
        ++batch_total_;
    }
    wake_.notify_one();
}

std::vector<Measured> Measurer::take() {
    const std::lock_guard lock(mutex_);
    std::vector<Measured> results;
    results.swap(done_);
    return results;
}

void Measurer::clear() {
    const std::lock_guard lock(mutex_);
    jobs_.clear();
}

std::string Measurer::status() {
    const std::lock_guard lock(mutex_);
    if (current_.empty()) return {};
    return "Reading " + current_ + " (" + std::to_string(batch_done_ + 1) + " of " + std::to_string(batch_total_) + ")";
}

bool Measurer::busy() {
    const std::lock_guard lock(mutex_);
    return !current_.empty() || !jobs_.empty() || !done_.empty();
}

void Measurer::run() {
    for (;;) {
        Job job;
        {
            std::unique_lock lock(mutex_);
            wake_.wait(lock, [this] { return stop_ || !jobs_.empty(); });
            if (stop_) return;
            job = std::move(jobs_.front());
            jobs_.pop_front();
            current_ = mod_manifest::utf8(job.full.filename());
        }
        Measured result{job.tag, job.file, {}};
        std::error_code ec;
        if (std::filesystem::is_regular_file(job.full, ec)) result.info = measure_audio(job.full);
        else result.info.error = "the file is not in the mod's folder";
        const std::lock_guard lock(mutex_);
        done_.push_back(std::move(result));
        current_.clear();
        ++batch_done_;
    }
}

} // namespace d012::bgm_maker
