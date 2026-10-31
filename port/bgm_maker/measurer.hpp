#pragma once

// Reading a mod's track files to the end, on a worker thread, for the checks
// that need their exact length (the loop inside the file, 30 minutes at most)
// and for "end" left out of a loop that ends where the file does. Check &
// save asks for every file it has not measured yet; saving waits for them.
// MIT licence; see port/README.md.

#include "audio_file.hpp"
#include "checks.hpp"

#include <condition_variable>
#include <deque>
#include <filesystem>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace d012::bgm_maker {

// `info` probed and counted: samples and decoded_rate set, or error.
[[nodiscard]] AudioInfo measure_audio(const std::filesystem::path &file);

// What the checks take of a file's info; samples only once measured.
[[nodiscard]] FileFacts facts_of(const AudioInfo &info);

// Whether an info has the file's exact length, or says why it cannot be read.
[[nodiscard]] inline bool measured(const AudioInfo &info) { return !info.error.empty() || info.samples.has_value(); }

struct Measured {
    int tag{};
    std::string file;  // as in the manifest
    AudioInfo info;
};

class Measurer {
public:
    Measurer();
    ~Measurer();
    Measurer(const Measurer &) = delete;
    Measurer &operator=(const Measurer &) = delete;

    // Measures `full` (the manifest's `file`) unless it is already waiting.
    void add(const std::string &file, const std::filesystem::path &full, int tag);
    std::vector<Measured> take();
    // Drops what is waiting (another mod was opened).
    void clear();
    // "Reading x.flac (2 of 5)", or empty when idle.
    [[nodiscard]] std::string status();
    [[nodiscard]] bool busy();

private:
    struct Job {
        std::string file;
        std::filesystem::path full;
        int tag{};
    };
    void run();

    std::mutex mutex_;
    std::condition_variable wake_;
    std::deque<Job> jobs_;
    std::vector<Measured> done_;
    std::string current_;
    int batch_total_{};
    int batch_done_{};
    bool stop_{};
    std::thread worker_;
};

} // namespace d012::bgm_maker
