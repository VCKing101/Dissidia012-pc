// The Loops step's views; see waveform.hpp. MIT licence; see port/README.md.

#include "waveform.hpp"

#include "theme.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

namespace d012::bgm_maker {
namespace {

constexpr double kFull = 32768.0;  // full scale of a 16-bit sample
constexpr double kMinSpp = 1.0 / 32.0;  // the closest zoom: 32 pixels a sample

double nice_step(double seconds) {
    static constexpr double kSteps[] = {1e-4, 2e-4, 5e-4, 1e-3, 2e-3, 5e-3, 0.01, 0.02, 0.05, 0.1, 0.2,
                                        0.5,  1,    2,    5,    10,   15,   30,   60,   120,  300,  600};
    for (const double step : kSteps)
        if (step >= seconds) return step;
    return 1200.0;
}

ImU32 colour(const ImVec4 &c, float alpha = 1.0f) { return ImGui::GetColorU32(ImVec4(c.x, c.y, c.z, c.w * alpha)); }

float scale() { return ImGui::GetFontSize() / 17.0f; }

// A dashed vertical line.
void dashed(ImDrawList *dl, float x, float y0, float y1, ImU32 c) {
    const float on = 4.0f * scale();
    const float off = 3.0f * scale();
    for (float y = y0; y < y1; y += on + off) dl->AddLine(ImVec2(x, y), ImVec2(x, std::min(y + on, y1)), c);
}

} // namespace

std::string format_time(std::int64_t samples, int rate) {
    if (rate <= 0) return "-";
    const char *sign = samples < 0 ? "-" : "";
    long long ms = std::llround(std::fabs(static_cast<double>(samples)) * 1000.0 / rate);
    const long long h = ms / 3600000;
    ms -= h * 3600000;
    const long long m = ms / 60000;
    ms -= m * 60000;
    const long long s = ms / 1000;
    ms -= s * 1000;
    char text[48];
    if (h > 0) std::snprintf(text, sizeof text, "%s%lld:%02lld:%02lld.%03lld", sign, h, m, s, ms);
    else std::snprintf(text, sizeof text, "%s%lld:%02lld.%03lld", sign, m, s, ms);
    return text;
}

std::optional<double> parse_time(const std::string &input) {
    // [h:][m:]s[.fraction], as the Python tool's time fields take it.
    const auto first = input.find_first_not_of(" \t");
    if (first == std::string::npos) return std::nullopt;
    const std::string text = input.substr(first, input.find_last_not_of(" \t") - first + 1u);
    std::vector<std::string> parts;
    std::size_t from = 0;
    for (;;) {
        const auto colon = text.find(':', from);
        parts.push_back(text.substr(from, colon == std::string::npos ? std::string::npos : colon - from));
        if (colon == std::string::npos) break;
        from = colon + 1u;
    }
    if (parts.size() > 3u) return std::nullopt;
    double seconds = 0.0;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        const std::string &part = parts[i];
        const bool last = i + 1u == parts.size();
        if (part.empty()) return std::nullopt;
        bool dot = false;
        for (const char c : part) {
            if (c == '.' && last && !dot) dot = true;
            else if (c < '0' || c > '9') return std::nullopt;
        }
        if (part.front() == '.') return std::nullopt;
        seconds = seconds * 60.0 + std::strtod(part.c_str(), nullptr);
    }
    return seconds;
}

// --- The waveform -------------------------------------------------------------

void WaveView::reset(const DecodedTrack &track) {
    frames_ = track.audio.frames;
    cursor = 0;
    drag_.reset();
    zoom_fit();
}

void WaveView::clamp() {
    const double max_spp = std::max(static_cast<double>(frames_) / width_, kMinSpp);
    spp_ = std::clamp(spp_, kMinSpp, max_spp);
    const double span = spp_ * width_;
    start_ = std::clamp(start_, 0.0, std::max(0.0, static_cast<double>(frames_) - span));
}

void WaveView::zoom_fit() {
    start_ = 0.0;
    spp_ = static_cast<double>(frames_) / width_;
    clamp();
}

void WaveView::zoom_to(double a, double b) {
    spp_ = std::max(b - a, 32.0) / width_;
    start_ = a;
    clamp();
}

void WaveView::zoom_by(double factor, std::optional<double> anchor) {
    const double at = anchor.value_or(static_cast<double>(cursor));
    const double x = x_of(at);
    const bool in_view = x >= 0.0 && x <= width_;
    const double ax = in_view ? x : width_ / 2.0;
    const double as = in_view ? at : start_ + spp_ * ax;
    spp_ *= factor;
    clamp();
    start_ = as - ax * spp_;
    clamp();
}

void WaveView::ensure_visible(std::int64_t sample) {
    const double span = spp_ * width_;
    if (sample < start_ || sample > start_ + span) {
        start_ = static_cast<double>(sample) - span * 0.1;
        clamp();
    }
}

void WaveView::follow(std::int64_t playhead) {
    const double span = spp_ * width_;
    if (playhead > start_ + span * 0.97 || playhead < start_) {
        start_ = static_cast<double>(playhead) - span * 0.05;
        clamp();
    }
}

std::int64_t WaveView::sample_at(float x) const {
    return std::llround(std::clamp(start_ + x * spp_, 0.0, static_cast<double>(frames_)));
}

WaveView::Events WaveView::draw(const char *id, const DecodedTrack &track, const LoopShown &loop,
                                std::optional<std::int64_t> playhead, Marker active, const ImVec2 &size) {
    Events events;
    const theme::Palette &p = theme::palette();
    ImGui::InvisibleButton(id, size, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const ImVec2 p0 = ImGui::GetItemRectMin();
    const ImVec2 p1 = ImGui::GetItemRectMax();
    const float W = p1.x - p0.x;
    const float H = p1.y - p0.y;
    if (std::fabs(W - width_) > 0.5f || frames_ != track.audio.frames) {
        // A new width keeps the same span on screen.
        const double span = spp_ * width_;
        frames_ = track.audio.frames;
        width_ = std::max(W, 1.0f);
        spp_ = span / width_;
    }
    clamp();
    const ImGuiIO &io = ImGui::GetIO();
    const float mx = io.MousePos.x - p0.x;
    const bool hovered = ImGui::IsItemHovered();
    const bool editable = loop.kind == LoopShown::Kind::Custom;

    // Interaction.
    if (hovered) {
        ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelY);
        ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelX);
        float pan = io.MouseWheelH;
        if (io.KeyShift && io.MouseWheel != 0.0f) pan = io.MouseWheel;
        else if (io.MouseWheel != 0.0f) zoom_by(std::pow(0.8, io.MouseWheel), static_cast<double>(sample_at(mx)));
        if (pan != 0.0f) {
            start_ -= pan * 80.0 * scale() * spp_;
            clamp();
        }
    }
    const float grab = 6.0f * scale();
    std::optional<Marker> near;
    if (editable) {
        const float ds = std::fabs(mx - static_cast<float>(x_of(static_cast<double>(loop.start))));
        const float de = std::fabs(mx - static_cast<float>(x_of(static_cast<double>(loop.end))));
        if (std::min(ds, de) <= grab) near = ds <= de ? Marker::Start : Marker::End;
    }
    if (hovered && !drag_) ImGui::SetMouseCursor(near ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_Arrow);
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        if (io.KeyShift && editable) events.set = {Marker::Start, sample_at(mx)};
        else if (io.KeyAlt && editable) events.set = {Marker::End, sample_at(mx)};
        else if (near) {
            drag_ = Drag{true, *near};
            events.picked = *near;
        } else {
            drag_ = Drag{false, Marker::Start, mx, start_};
        }
    }
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && editable) events.set = {Marker::End, sample_at(mx)};
    if (drag_) {
        if (drag_->marker) {
            events.drag = {drag_->which, sample_at(mx)};
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        } else {
            const float dx = mx - drag_->x0;
            if (std::fabs(dx) > 3.0f) drag_->moved = true;
            if (drag_->moved) {
                start_ = drag_->start0 - dx * spp_;
                clamp();
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
            }
        }
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            if (!drag_->marker && !drag_->moved) events.cursor = sample_at(mx);
            drag_.reset();
        }
    }

    // Drawing.
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->PushClipRect(p0, p1, true);
    dl->AddRectFilled(p0, p1, colour(p.wave_bg));
    const int rate = track.audio.rate;
    const int channels = track.audio.channels;
    const float ruler = std::round(ImGui::GetFontSize() * 1.2f);
    std::optional<std::pair<std::int64_t, std::int64_t>> region;
    if (loop.kind == LoopShown::Kind::Whole) region = std::pair<std::int64_t, std::int64_t>{0, frames_};
    else if (loop.kind != LoopShown::Kind::None) region = std::pair{loop.start, loop.end};
    if (region) {
        const float x0 = p0.x + static_cast<float>(x_of(static_cast<double>(region->first)));
        const float x1 = p0.x + static_cast<float>(x_of(static_cast<double>(region->second)));
        if (x0 > p0.x) dl->AddRectFilled(ImVec2(p0.x, p0.y + ruler), ImVec2(std::min(x0, p1.x), p1.y), colour(p.outside));
        if (x1 < p1.x) dl->AddRectFilled(ImVec2(std::max(x1, p0.x), p0.y + ruler), p1, colour(p.outside));
        dl->AddRectFilled(ImVec2(std::max(x0, p0.x), p0.y + ruler), ImVec2(std::min(x1, p1.x), p1.y), colour(p.region));
    }
    // The ruler.
    dl->AddRectFilled(p0, ImVec2(p1.x, p0.y + ruler), colour(p.ruler_bg));
    ImFont *mono = theme::mono_font();
    const float label_size = ImGui::GetFontSize() * 0.78f;
    const double step = nice_step(80.0 * scale() * spp_ / rate);
    for (double t = std::floor(start_ / rate / step) * step;; t += step) {
        const float x = p0.x + static_cast<float>(x_of(t * rate));
        if (x > p1.x) break;
        if (x < p0.x - 80.0f) continue;
        dl->AddLine(ImVec2(std::round(x), p0.y + ruler), ImVec2(std::round(x), p1.y), colour(p.grid));
        dl->AddLine(ImVec2(std::round(x), p0.y + ruler - 5.0f), ImVec2(std::round(x), p0.y + ruler), colour(p.ruler_text));
        const std::int64_t at = std::llround(t * rate);
        const std::string label = step < 0.001 ? std::to_string(at) : format_time(at, rate);
        dl->AddText(mono, label_size, ImVec2(x + 3.0f, p0.y + (ruler - label_size) / 2.0f), colour(p.ruler_text),
                    label.c_str());
    }
    // A lane for each channel.
    const float lane = (H - ruler) / static_cast<float>(channels);
    for (int c = 0; c < channels; ++c) {
        const float top = p0.y + ruler + lane * static_cast<float>(c);
        const float mid = top + lane / 2.0f;
        const float amp = lane / 2.0f - 3.0f;
        dl->AddLine(ImVec2(p0.x, std::round(mid)), ImVec2(p1.x, std::round(mid)), colour(p.grid));
        if (c > 0) dl->AddLine(ImVec2(p0.x, std::round(top)), ImVec2(p1.x, std::round(top)), colour(p.grid));
        if (spp_ >= 1.0) {
            for (int x = 0; x < static_cast<int>(W); ++x) {
                const double a = start_ + x * spp_;
                const auto r = track.range(c, static_cast<std::int64_t>(std::floor(a)),
                                           static_cast<std::int64_t>(std::ceil(a + spp_)));
                if (!r) continue;
                const float y0 = mid - static_cast<float>(r->second / kFull) * amp;
                const float y1 = mid - static_cast<float>(r->first / kFull) * amp;
                dl->AddRectFilled(ImVec2(p0.x + x, y0), ImVec2(p0.x + x + 1.0f, std::max(y1, y0 + 1.0f)), colour(p.wave));
            }
        } else {
            const std::int64_t a = std::max<std::int64_t>(0, static_cast<std::int64_t>(std::floor(start_)) - 1);
            const std::int64_t b = std::min(frames_, static_cast<std::int64_t>(std::ceil(start_ + W * spp_)) + 2);
            std::vector<ImVec2> points;
            for (std::int64_t i = a; i < b; ++i)
                points.emplace_back(p0.x + static_cast<float>(x_of(static_cast<double>(i))),
                                    mid - static_cast<float>(track.sample(c, i) / kFull) * amp);
            if (points.size() >= 2u)
                dl->AddPolyline(points.data(), static_cast<int>(points.size()), colour(p.wave), 0, 1.25f * scale());
            if (1.0 / spp_ >= 6.0)
                for (const ImVec2 &pt : points)
                    dl->AddRectFilled(ImVec2(pt.x - 1.5f, pt.y - 1.5f), ImVec2(pt.x + 1.5f, pt.y + 1.5f), colour(p.wave));
        }
    }
    // The markers: each sits at the left edge of its sample.
    if (region && loop.kind != LoopShown::Kind::Whole) {
        const auto marker = [&](std::int64_t s, const ImVec4 &c, const char *label, bool is_start, bool bold) {
            const float x = std::round(p0.x + static_cast<float>(x_of(static_cast<double>(s)))) + 0.5f;
            if (x < p0.x - 20.0f || x > p1.x + 20.0f) return;
            const float alpha = editable ? 1.0f : 0.55f;
            dl->AddLine(ImVec2(x, p0.y), ImVec2(x, p1.y), colour(c, alpha), bold ? 2.0f * scale() : 1.25f * scale());
            const float fw = 16.0f * scale();
            const ImVec2 f0 = is_start ? ImVec2(x, p0.y) : ImVec2(x - fw, p0.y);
            dl->AddRectFilled(f0, ImVec2(f0.x + fw, p0.y + ruler - 2.0f), colour(c, alpha));
            dl->AddText(theme::heading_font(), label_size, ImVec2(f0.x + 4.0f * scale(), p0.y + (ruler - label_size) / 2.0f),
                        colour(p.wave_bg), label);
        };
        marker(region->first, p.start, "S", true, editable && active == Marker::Start);
        marker(region->second, p.end, "E", false, editable && active == Marker::End);
    }
    const float cx = std::round(p0.x + static_cast<float>(x_of(static_cast<double>(cursor)))) + 0.5f;
    if (cx >= p0.x && cx <= p1.x) dashed(dl, cx, p0.y + ruler, p1.y, colour(p.cursor));
    if (playhead) {
        const float px = std::round(p0.x + static_cast<float>(x_of(static_cast<double>(*playhead)))) + 0.5f;
        if (px >= p0.x && px <= p1.x) dl->AddLine(ImVec2(px, p0.y), ImVec2(px, p1.y), colour(p.playhead), 1.5f * scale());
    }
    dl->PopClipRect();
    return events;
}

void WaveView::draw_overview(const char *id, const DecodedTrack &track, const LoopShown &loop,
                             std::optional<std::int64_t> playhead, const ImVec2 &size) {
    const theme::Palette &p = theme::palette();
    ImGui::InvisibleButton(id, size);
    const ImVec2 p0 = ImGui::GetItemRectMin();
    const ImVec2 p1 = ImGui::GetItemRectMax();
    const float W = p1.x - p0.x;
    const float H = p1.y - p0.y;
    const double n = static_cast<double>(std::max<std::int64_t>(track.audio.frames, 1));
    if (ImGui::IsItemActive()) {
        // Dragging along the strip moves the view's middle there.
        const double s = (ImGui::GetIO().MousePos.x - p0.x) / W * n;
        start_ = s - spp_ * width_ / 2.0;
        clamp();
    }
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->PushClipRect(p0, p1, true);
    dl->AddRectFilled(p0, p1, colour(p.ruler_bg));
    const double spp = n / W;
    const float mid = p0.y + H / 2.0f;
    const float amp = H / 2.0f - 2.0f;
    for (int x = 0; x < static_cast<int>(W); ++x) {
        int lo = 32767;
        int hi = -32768;
        for (int c = 0; c < track.audio.channels; ++c) {
            if (const auto r = track.range(c, static_cast<std::int64_t>(x * spp), static_cast<std::int64_t>((x + 1) * spp))) {
                lo = std::min(lo, r->first);
                hi = std::max(hi, r->second);
            }
        }
        if (hi >= lo)
            dl->AddRectFilled(ImVec2(p0.x + x, mid - static_cast<float>(hi / kFull) * amp),
                              ImVec2(p0.x + x + 1.0f, std::max(mid - static_cast<float>(lo / kFull) * amp,
                                                               mid - static_cast<float>(hi / kFull) * amp + 1.0f)),
                              colour(p.wave_dim));
    }
    const float k = static_cast<float>(W / n);
    if (loop.kind != LoopShown::Kind::None) {
        const float a = p0.x + (loop.kind == LoopShown::Kind::Whole ? 0.0f : static_cast<float>(loop.start) * k);
        const float b = p0.x + (loop.kind == LoopShown::Kind::Whole ? W : static_cast<float>(loop.end) * k);
        ImVec4 tint = p.region;
        tint.w = 0.14f;
        dl->AddRectFilled(ImVec2(a, p0.y), ImVec2(b, p1.y), colour(tint));
        if (loop.kind != LoopShown::Kind::Whole) {
            dl->AddRectFilled(ImVec2(a, p0.y), ImVec2(a + 1.5f, p1.y), colour(p.start));
            dl->AddRectFilled(ImVec2(b - 1.0f, p0.y), ImVec2(b + 0.5f, p1.y), colour(p.end));
        }
    }
    const float v0 = p0.x + static_cast<float>(start_) * k;
    const float v1 = v0 + std::max(2.0f, static_cast<float>(spp_ * width_) * k);
    dl->AddRect(ImVec2(v0, p0.y), ImVec2(v1, p1.y), colour(p.playhead, 0.55f));
    if (playhead) {
        const float x = p0.x + static_cast<float>(*playhead) * k;
        dl->AddLine(ImVec2(x, p0.y), ImVec2(x, p1.y), colour(p.playhead));
    }
    dl->PopClipRect();
}

// --- The seam -----------------------------------------------------------------

namespace {

SeamStats seam_stats(const DecodedTrack &track, std::int64_t start, std::int64_t end, std::int64_t w) {
    const std::int64_t n = track.audio.frames;
    SeamStats stats;
    double diff_before = 0.0, signal_before = 0.0, diff_after = 0.0, signal_after = 0.0;
    std::int64_t after = 0;
    const auto at = [&](int c, std::int64_t i) { return track.sample(c, i) / kFull; };
    for (int c = 0; c < track.audio.channels; ++c) {
        if (end >= 2 && start < n) {
            stats.step = std::max(stats.step, std::fabs(at(c, start) - at(c, end - 1)));
            stats.usual = std::max({stats.usual, std::fabs(at(c, end - 1) - at(c, end - 2)),
                                    start + 1 < n ? std::fabs(at(c, start + 1) - at(c, start)) : 0.0});
        }
        for (std::int64_t i = 1; i <= w; ++i) {
            if (start - i < 0) break;
            const double a = at(c, end - i);
            const double b = at(c, start - i);
            diff_before += (a - b) * (a - b);
            signal_before += a * a;
        }
        for (std::int64_t i = 0; i < w && end + i < n; ++i) {
            const double a = at(c, end + i);
            const double b = at(c, start + i);
            diff_after += (a - b) * (a - b);
            signal_after += b * b;
            ++after;
        }
    }
    const auto db = [](double d, double s) -> std::optional<double> {
        if (s <= 0.0) return std::nullopt;
        return 10.0 * std::log10(std::max(d, 1e-20) / s);
    };
    stats.before = db(diff_before, signal_before);
    if (after > 0) stats.after = db(diff_after, signal_after);
    return stats;
}

} // namespace

std::optional<SeamStats> draw_seam(const char *id, const DecodedTrack &track, std::int64_t start, std::int64_t end,
                                   std::int64_t width, const ImVec2 &size) {
    const theme::Palette &p = theme::palette();
    ImGui::Dummy(size);
    ImGui::PushID(id);
    ImGui::PopID();
    const ImVec2 p0 = ImGui::GetItemRectMin();
    const ImVec2 p1 = ImGui::GetItemRectMax();
    const float W = p1.x - p0.x;
    const float H = p1.y - p0.y;
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->PushClipRect(p0, p1, true);
    dl->AddRectFilled(p0, p1, colour(p.wave_bg));
    if (!(end > start)) {
        dl->PopClipRect();
        return std::nullopt;
    }
    const std::int64_t n = track.audio.frames;
    const float half = W / 2.0f;
    const std::int64_t w = std::max<std::int64_t>(4, width);
    const int channels = track.audio.channels;
    const float lane = H / static_cast<float>(channels);
    dl->AddLine(ImVec2(p0.x + std::round(half), p0.y), ImVec2(p0.x + std::round(half), p1.y), colour(p.grid), 1.0f);
    const float px = half / static_cast<float>(w);  // pixels per sample
    for (int c = 0; c < channels; ++c) {
        const float mid = p0.y + lane * static_cast<float>(c) + lane / 2.0f;
        const float amp = lane / 2.0f - 3.0f;
        dl->AddLine(ImVec2(p0.x, std::round(mid)), ImVec2(p1.x, std::round(mid)), colour(p.grid));
        // Samples [from, from + w), the first at x0: a line, or a min/max band when dense.
        const auto trace = [&](std::int64_t from, float x0, const ImVec4 &col, float thickness) {
            if (px >= 1.0f) {
                std::vector<ImVec2> points;
                const auto flush = [&] {
                    if (points.size() >= 2u)
                        dl->AddPolyline(points.data(), static_cast<int>(points.size()), colour(col), 0, thickness * scale());
                    points.clear();
                };
                for (std::int64_t i = 0; i < w; ++i) {
                    const std::int64_t j = from + i;
                    if (j < 0 || j >= n) {
                        flush();
                        continue;
                    }
                    points.emplace_back(p0.x + x0 + (static_cast<float>(i) + 0.5f) * px,
                                        mid - static_cast<float>(track.sample(c, j) / kFull) * amp);
                }
                flush();
            } else {
                const double per = 1.0 / px;
                for (int x = 0; x < static_cast<int>(half); ++x) {
                    const auto r = track.range(c, static_cast<std::int64_t>(std::floor(from + x * per)),
                                               static_cast<std::int64_t>(std::floor(from + (x + 1) * per)));
                    if (!r) continue;
                    const float y0 = mid - static_cast<float>(r->second / kFull) * amp;
                    const float y1 = mid - static_cast<float>(r->first / kFull) * amp;
                    dl->AddRectFilled(ImVec2(p0.x + x0 + x, y0), ImVec2(p0.x + x0 + x + 1.0f, std::max(y1, y0 + 1.0f)),
                                      colour(col));
                }
            }
        };
        // Faintly, what comes before the loop start (left) and after the loop end (right).
        trace(start - w, 0.0f, p.ghost, 1.0f);
        trace(end, half, p.ghost, 1.0f);
        const ImVec4 &col = c == 0 ? p.wave : p.wave_second;
        trace(end - w, 0.0f, col, 1.5f);
        trace(start, half, col, 1.5f);
    }
    const float label_size = ImGui::GetFontSize() * 0.78f;
    dl->AddText(theme::mono_font(), label_size, ImVec2(p0.x + 6.0f, p0.y + 4.0f), colour(p.ruler_text), "... loop end");
    dl->AddText(theme::mono_font(), label_size, ImVec2(p0.x + half + 6.0f, p0.y + 4.0f), colour(p.ruler_text),
                "loop start ...");
    dl->PopClipRect();
    return seam_stats(track, start, end, w);
}

} // namespace d012::bgm_maker
