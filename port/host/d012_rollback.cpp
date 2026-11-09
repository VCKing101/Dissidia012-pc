#include "d012_rollback.hpp"

#include "snapshot.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

namespace d012::rollback {

namespace {

using Clock = std::chrono::steady_clock;

struct Test {
    bool on = false;
    std::uint64_t back = 0;    // K: reads run twice
    std::uint64_t every = 60;  // a save at every this many reads
    bool wrong = false;        // the first run after a save gets player 2's input wrong
    std::shared_ptr<const portablekit::snapshot::State> saved;
    std::uint64_t saved_at = 0;     // the read the state was saved after
    std::uint64_t replayed_to = 0;  // the last read already run twice
    bool want_save = false;
    bool want_restore = false;
    std::uint64_t pending_read = 0;
    std::uint64_t restores = 0;
    Clock::duration save_time{};
    Clock::duration restore_time{};
};

Test &test() {
    static Test t;
    return t;
}

} // namespace

void install() {
    Test &t = test();
    const char *text = std::getenv("D012_ROLLBACK_TEST");
    if (text == nullptr || *text == '\0') return;
    char *end = nullptr;
    t.back = std::strtoull(text, &end, 10);
    if (end != nullptr && *end == ',') t.every = std::max<std::uint64_t>(1u, std::strtoull(end + 1, &end, 10));
    t.wrong = end != nullptr && std::string(end) == ",wrong";
    if (t.back == 0u) return;
    if (t.every <= t.back) t.every = t.back + 1u;
    t.on = true;
    std::cout << "[rollback] test: in battle, every " << t.every << " reads, back " << t.back << " reads and again"
              << (t.wrong ? ", player 2's input wrong the first time" : "") << std::endl;
}

void note_pad_read(std::uint64_t read, bool battle) {
    Test &t = test();
    // D012_ROLLBACK_LOG=1: every pad read in the log, to line up traces.
    static const bool log = std::getenv("D012_ROLLBACK_LOG") != nullptr;
    if (log) std::cout << "[rollback] read " << read << (battle ? " (battle)" : "") << std::endl;
    if (!t.on || !battle) return;
    if (t.saved && read == t.saved_at + t.back && read > t.replayed_to) {
        t.want_restore = true;
        t.pending_read = read;
    } else if (!t.saved && read % t.every == 0u && read > t.replayed_to) {
        t.want_save = true;
        t.pending_read = read;
    }
}

bool wrong_input() {
    const Test &t = test();
    return t.on && t.wrong && t.saved != nullptr;
}

void after_dispatch(psprecomp::Runtime &rt) {
    Test &t = test();
    if (!t.want_save && !t.want_restore) return;
    if (t.want_save) {
        t.want_save = false;
        const auto began = Clock::now();
        t.saved = portablekit::snapshot::save(rt);
        t.save_time += Clock::now() - began;
        t.saved_at = t.pending_read;
        return;
    }
    t.want_restore = false;
    const auto began = Clock::now();
    portablekit::snapshot::restore(rt, *t.saved);
    t.restore_time += Clock::now() - began;
    t.replayed_to = t.pending_read;
    t.saved.reset();
    if (++t.restores % 20u == 1u) {
        const auto ms = [](Clock::duration d, std::uint64_t n) {
            return std::chrono::duration<double, std::milli>(d).count() / static_cast<double>(n);
        };
        std::cout << "[rollback] restore " << t.restores << ": back from read " << t.pending_read << " to read "
                  << t.saved_at << "; a save takes " << ms(t.save_time, t.restores) << " ms, a restore "
                  << ms(t.restore_time, t.restores) << " ms" << std::endl;
    }
}

} // namespace d012::rollback
