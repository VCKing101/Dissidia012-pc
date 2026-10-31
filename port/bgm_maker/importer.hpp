#pragma once

// Adding audio files to a mod: each file is copied into the mod's music/
// folder under a plain name (the original is never touched) and read, on a
// worker thread, so a large file does not stop the window. The window takes
// the results each frame and makes tracks of them. MIT licence; see
// port/README.md.

#include "audio_file.hpp"

#include <condition_variable>
#include <deque>
#include <filesystem>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <vector>

namespace d012::bgm_maker {

struct ImportResult {
    int tag{};               // as given to add()
    int series{-1};          // the series the file was added to, as asked
    std::string source;      // the original's name, for messages
    std::string file;        // in the mod: "music/<plain name>"; empty on error
    std::filesystem::path copy;  // where the copy is
    std::string title;       // its title tag, or one made from its name
    AudioInfo info;
    std::string error;       // why it was not added
};

class Importer {
public:
    Importer();
    ~Importer();
    Importer(const Importer &) = delete;
    Importer &operator=(const Importer &) = delete;

    // Copies `files` into `folder`/music for series `series`; `tag` comes
    // back with each result (which mod it was for).
    void add(const std::vector<std::filesystem::path> &files, const std::filesystem::path &folder, int series, int tag);
    // What has finished since the last call.
    std::vector<ImportResult> take();
    // "Copying x.flac (2 of 5)", or empty when idle.
    [[nodiscard]] std::string status();
    [[nodiscard]] bool busy();

private:
    struct Job {
        std::filesystem::path source;
        std::filesystem::path folder;
        int series{};
        int tag{};
    };
    void run();
    ImportResult import(const Job &job);

    std::mutex mutex_;
    std::condition_variable wake_;
    std::deque<Job> jobs_;
    std::vector<ImportResult> done_;
    std::set<std::filesystem::path> reserved_;  // names being copied to
    std::string current_;
    int batch_total_{};
    int batch_done_{};
    bool stop_{};
    std::thread worker_;
};

} // namespace d012::bgm_maker
