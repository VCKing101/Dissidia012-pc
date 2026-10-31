#pragma once

// The Loops step's views, drawn with ImGui: the waveform (zoom with the wheel,
// pan by dragging, click for the cursor, drag a marker), the overview strip of
// the whole file under it, and the seam view, which shows both sides of the
// loop's jump with what lies beyond each side faintly. The Python tool's
// (port/tools/bgm_maker/web/waveform.js), with the same rules and numbers.
// MIT licence; see port/README.md.

#include "decoder.hpp"

#include <imgui.h>

#include <cstdint>
#include <optional>
#include <string>

namespace d012::bgm_maker {

// m:ss.mmm (h:mm:ss.mmm past an hour) of `samples` at `rate`.
[[nodiscard]] std::string format_time(std::int64_t samples, int rate);
// Seconds from "83.5", "1:23.5" or "1:01:23.5"; nothing when it is not a time.
[[nodiscard]] std::optional<double> parse_time(const std::string &text);

enum class Marker { Start, End };

// What the views show of the loop.
struct LoopShown {
    enum class Kind { None, Whole, File, Custom };  // File: the file's own (auto), not editable
    Kind kind{Kind::None};
    std::int64_t start{};
    std::int64_t end{};
};

class WaveView {
public:
    // What the player did in the view this frame.
    struct Events {
        std::optional<std::int64_t> cursor;                 // a click
        std::optional<std::pair<Marker, std::int64_t>> set; // shift-click (start), alt- or right-click (end)
        std::optional<Marker> picked;                       // a marker grabbed
        std::optional<std::pair<Marker, std::int64_t>> drag;
    };

    // A new track: the view shows all of it again.
    void reset(const DecodedTrack &track);
    Events draw(const char *id, const DecodedTrack &track, const LoopShown &loop, std::optional<std::int64_t> playhead,
                Marker active, const ImVec2 &size);
    void draw_overview(const char *id, const DecodedTrack &track, const LoopShown &loop,
                       std::optional<std::int64_t> playhead, const ImVec2 &size);

    void zoom_fit();
    void zoom_to(double a, double b);
    void zoom_around(double centre, double span) { zoom_to(centre - span / 2.0, centre + span / 2.0); }
    void zoom_by(double factor, std::optional<double> anchor = std::nullopt);
    void ensure_visible(std::int64_t sample);
    // Keeps the playhead in view while playing, as the Python tool does.
    void follow(std::int64_t playhead);

    std::int64_t cursor{};

private:
    void clamp();
    [[nodiscard]] double x_of(double sample) const { return (sample - start_) / spp_; }
    [[nodiscard]] std::int64_t sample_at(float x) const;

    std::int64_t frames_{};
    double start_{};
    double spp_{1.0};  // samples per pixel
    float width_{1.0f};
    struct Drag {
        bool marker{};
        Marker which{};
        float x0{};
        double start0{};
        bool moved{};
    };
    std::optional<Drag> drag_;
};

// How well the two sides of a seam match: the step across it against the
// signal's usual step (as parts of full scale), and the difference of the
// audio before both points and after them, in dB below the signal.
struct SeamStats {
    double step{};
    double usual{};
    std::optional<double> before;
    std::optional<double> after;  // nothing when the loop ends at the file's end
};

// The seam view: `width` frames each side. Returns its numbers, or nothing
// when there is no seam.
std::optional<SeamStats> draw_seam(const char *id, const DecodedTrack &track, std::int64_t start, std::int64_t end,
                                   std::int64_t width, const ImVec2 &size);

} // namespace d012::bgm_maker
