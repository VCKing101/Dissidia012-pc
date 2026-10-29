// Adding audio files to a mod; see importer.hpp. MIT licence; see
// port/README.md.

#include "importer.hpp"

#include "d012_mod_manifest.hpp"
#include "names.hpp"

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

// The port's limit on a track (PortableKit host_audio.cpp kMaxFrames is 30
// minutes); a file larger than 1 GiB is surely not a track.
constexpr std::uintmax_t kMaxFileBytes = 1ull << 30u;

} // namespace

Importer::Importer() : worker_([this] { run(); }) {}

Importer::~Importer() {
    {
        const std::lock_guard lock(mutex_);
        stop_ = true;
        jobs_.clear();
    }
    wake_.notify_all();
    worker_.join();
}

void Importer::add(const std::vector<std::filesystem::path> &files, const std::filesystem::path &folder, int series,
                   int tag) {
    {
        const std::lock_guard lock(mutex_);
        if (jobs_.empty() && current_.empty()) batch_total_ = batch_done_ = 0;
        for (const auto &file : files) jobs_.push_back({file, folder, series, tag});
        batch_total_ += static_cast<int>(files.size());
    }
    wake_.notify_one();
}

std::vector<ImportResult> Importer::take() {
    const std::lock_guard lock(mutex_);
    std::vector<ImportResult> results;
    results.swap(done_);
    return results;
}

std::string Importer::status() {
    const std::lock_guard lock(mutex_);
    if (current_.empty()) return {};
    return "Copying " + current_ + " (" + std::to_string(batch_done_ + 1) + " of " + std::to_string(batch_total_) + ")";
}

bool Importer::busy() {
    const std::lock_guard lock(mutex_);
    return !current_.empty() || !jobs_.empty() || !done_.empty();
}

void Importer::run() {
    for (;;) {
        Job job;
        {
            std::unique_lock lock(mutex_);
            wake_.wait(lock, [this] { return stop_ || !jobs_.empty(); });
            if (stop_) return;
            job = std::move(jobs_.front());
            jobs_.pop_front();
            current_ = manifest::utf8(job.source.filename());
        }
        ImportResult result = import(job);
        const std::lock_guard lock(mutex_);
        done_.push_back(std::move(result));
        current_.clear();
        ++batch_done_;
    }
}

ImportResult Importer::import(const Job &job) {
    ImportResult result;
    result.tag = job.tag;
    result.series = job.series;
    result.source = manifest::utf8(job.source.filename());
    std::error_code ec;
    if (!std::filesystem::is_regular_file(job.source, ec)) {
        result.error = "\"" + result.source + "\" is not a file";
        return result;
    }
    const std::string plain = names::safe_file_name(result.source, result.error);
    if (plain.empty()) return result;
    const auto size = std::filesystem::file_size(job.source, ec);
    if (ec || size == 0u || size > kMaxFileBytes) {
        result.error = "\"" + result.source + "\" is empty or larger than 1 GiB";
        return result;
    }
    const std::filesystem::path music = job.folder / "music";
    std::filesystem::create_directories(music, ec);
    // A free name: the plain one, else name-2, name-3...
    const auto [stem, extension] = [&] {
        const auto dot = plain.rfind('.');
        return std::pair{plain.substr(0u, dot), plain.substr(dot)};
    }();
    std::filesystem::path target;
    {
        const std::lock_guard lock(mutex_);
        for (int counter = 1;; ++counter) {
            const std::string name = counter == 1 ? plain : stem + "-" + std::to_string(counter) + extension;
            target = music / manifest::path_from_utf8(name);
            if (!std::filesystem::exists(target, ec) && !reserved_.contains(target)) break;
        }
        reserved_.insert(target);
    }
    const auto release = [&] {
        const std::lock_guard lock(mutex_);
        reserved_.erase(target);
    };
    std::filesystem::path partial = target;
    partial += ".part";
    if (!std::filesystem::copy_file(job.source, partial, std::filesystem::copy_options::overwrite_existing, ec)) {
        result.error = "\"" + result.source + "\" cannot be copied (" + ec.message() + ")";
        std::filesystem::remove(partial, ec);
        release();
        return result;
    }
    std::filesystem::rename(partial, target, ec);
    release();
    if (ec) {
        result.error = "\"" + result.source + "\" cannot be copied (" + ec.message() + ")";
        std::filesystem::remove(partial, ec);
        return result;
    }
    result.info = probe_audio(target);
    if (!result.info.error.empty()) {
        result.error = "\"" + result.source + "\" cannot be read: " + result.info.error;
        std::filesystem::remove(target, ec);
        return result;
    }
    result.copy = target;
    result.file = "music/" + manifest::utf8(target.filename());
    const std::string title = !result.info.title.empty() ? result.info.title : names::title_from_file(result.source);
    result.title = names::clip_bytes(title, manifest::kMaxKeyLength);
    return result;
}

} // namespace d012::bgm_maker
