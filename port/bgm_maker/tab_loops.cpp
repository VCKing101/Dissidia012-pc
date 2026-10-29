// The Battle BGM Maker's Loops step: each track's loop, set on its waveform
// and heard as the game plays it. The Python tool's editor
// (port/tools/bgm_maker/web/app.js), with its rules, keys and numbers:
//
//   Space play from the cursor (following the loop) or pause; Home from the start
//   A audition the seam (the seconds before the loop end, then the loop start)
//   S, E set the start or end at the playhead or cursor (a custom loop starts if there is none)
//   [ ] pick the start or end marker; Left, Right nudge it by the step (Shift: ten steps)
//   Z the picked marker to the nearest zero crossing
//   + - zoom; F all of the file; Esc stop
//
// MIT licence; see port/README.md.

#include "app.hpp"

#include "d012_mod_manifest.hpp"
#include "theme.hpp"
#include "widgets.hpp"

#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

constexpr const char *kDot = "\xE2\x97\x8F";      // ●
constexpr const char *kPlay = "\xE2\x96\xB6";     // ▶
constexpr const char *kMiddle = "\xC2\xB7";       // ·

float em() { return ImGui::GetFontSize(); }

const char *const kSteps[] = {"1 sample", "10 samples", "1 ms", "10 ms", "1 beat", "1 bar (4 beats)"};
const double kPre[] = {1, 2, 3, 5, 8};
const char *const kPreNames[] = {"1 s", "2 s", "3 s", "5 s", "8 s"};
const double kPost[] = {2, 4, 8};
const char *const kPostNames[] = {"2 s", "4 s", "8 s"};
const double kSeam[] = {0.002, 0.01, 0.05, 0.25, 1.0};
const char *const kSeamNames[] = {"2 ms", "10 ms", "50 ms", "250 ms", "1 s"};

std::string with_commas(long long value) {
    const bool negative = value < 0;
    std::string digits = std::to_string(negative ? -value : value);
    for (int at = static_cast<int>(digits.size()) - 3; at > 0; at -= 3) digits.insert(static_cast<std::size_t>(at), ",");
    return negative ? "-" + digits : digits;
}

std::string fixed(double value, int decimals) {
    char text[64];
    std::snprintf(text, sizeof text, "%.*f", decimals, value);
    return text;
}

const char *badge(const LoopModel &loop) {
    switch (loop.mode) {
    case LoopModel::Mode::Auto: return "auto";
    case LoopModel::Mode::Whole: return "whole";
    case LoopModel::Mode::None: return "once";
    case LoopModel::Mode::Custom: return "loop";
    }
    return "";
}

} // namespace

// --- The loop ------------------------------------------------------------------

std::filesystem::path App::track_path_on_disk(const TrackModel &track) {
    return files_folder() / manifest::path_from_utf8(track.file);
}

LoopShown App::shown_loop(const TrackModel &track, const DecodedTrack &audio) {
    const std::int64_t n = audio.audio.frames;
    const LoopModel &loop = track.loop;
    switch (loop.mode) {
    case LoopModel::Mode::Custom: {
        std::int64_t start = loop.start;
        std::int64_t end = loop.end.value_or(n);
        if (loop.in_seconds) {
            // A loop the manifest gives in seconds, in samples of the file.
            start = std::llround(loop.start_seconds * audio.audio.rate);
            end = loop.end_seconds ? std::llround(*loop.end_seconds * audio.audio.rate) : n;
        }
        return {LoopShown::Kind::Custom, start, end};
    }
    case LoopModel::Mode::Auto:
        if (const AudioInfo *info = info_for(track.file); info != nullptr && info->loop)
            return {LoopShown::Kind::File, info->loop->start, info->loop->end.value_or(n)};
        return {LoopShown::Kind::Whole, 0, n};
    case LoopModel::Mode::Whole: return {LoopShown::Kind::Whole, 0, n};
    case LoopModel::Mode::None: break;
    }
    return {LoopShown::Kind::None, 0, n};
}

void App::set_loop(TrackModel &track, std::int64_t start, std::int64_t end, std::int64_t frames) {
    // At least two samples long, inside the file (the Python tool's setLoop).
    start = std::clamp<std::int64_t>(start, 0, std::max<std::int64_t>(frames - 2, 0));
    end = std::clamp<std::int64_t>(end, start + 2, std::max(frames, start + 2));
    track.loop.mode = LoopModel::Mode::Custom;
    track.loop.in_seconds = false;
    track.loop.start = start;
    track.loop.end = end;
}

void App::set_marker(TrackModel &track, const DecodedTrack &audio, Marker which, std::int64_t sample) {
    const LoopShown loop = shown_loop(track, audio);
    if (loop.kind != LoopShown::Kind::Custom) return;
    active_marker_ = which;
    if (which == Marker::Start) set_loop(track, std::min(sample, loop.end - 2), loop.end, audio.audio.frames);
    else set_loop(track, loop.start, std::max(sample, loop.start + 2), audio.audio.frames);
}

void App::ensure_custom(TrackModel &track, const DecodedTrack &audio) {
    if (track.loop.mode == LoopModel::Mode::Custom) return;
    set_loop(track, 0, audio.audio.frames, audio.audio.frames);
}

std::int64_t App::step_samples(int rate) const {
    switch (step_choice_) {
    case 0: return 1;
    case 1: return 10;
    case 2: return std::llround(rate / 1000.0);
    case 3: return std::llround(rate / 100.0);
    case 4: return std::llround(rate * 60.0 / bpm_);
    default: return std::llround(rate * 240.0 / bpm_);
    }
}

void App::nudge(TrackModel &track, const DecodedTrack &audio, Marker which, int direction, int times) {
    const LoopShown loop = shown_loop(track, audio);
    if (loop.kind != LoopShown::Kind::Custom) return;
    const std::int64_t at = which == Marker::Start ? loop.start : loop.end;
    set_marker(track, audio, which, at + direction * step_samples(audio.audio.rate) * times);
    const LoopShown moved = shown_loop(track, audio);
    wave_.ensure_visible(which == Marker::Start ? moved.start : moved.end);
}

// The nearest zero crossing of the summed channels within 20 ms: the sample
// nearer zero of the two either side of the sign change, preferring a crossing
// in the same direction as the signal at the other marker (the Python tool's).
void App::snap_zero(TrackModel &track, const DecodedTrack &audio, Marker which) {
    const LoopShown loop = shown_loop(track, audio);
    if (loop.kind != LoopShown::Kind::Custom) return;
    const std::int64_t n = audio.audio.frames;
    const std::int64_t at = which == Marker::Start ? loop.start : loop.end;
    const std::int64_t other = which == Marker::Start ? loop.end : loop.start;
    const auto direction = [&](std::int64_t i) {
        if (i <= 0 || i >= n) return 0;
        const int d = audio.mono(i) - audio.mono(i - 1);
        return (d > 0) - (d < 0);
    };
    const auto crossing = [&](std::int64_t i) { return i > 0 && i < n && (audio.mono(i - 1) < 0) != (audio.mono(i) < 0); };
    const int want = direction(other);
    const std::int64_t radius = std::llround(audio.audio.rate * 0.02);
    std::optional<std::int64_t> any;
    std::optional<std::int64_t> same;
    for (std::int64_t d = 0; d <= radius && !same; ++d) {
        for (const std::int64_t i : {at - d, at + d}) {
            if (!crossing(i)) continue;
            const std::int64_t j = std::abs(audio.mono(i - 1)) < std::abs(audio.mono(i)) ? i - 1 : i;
            if (!any) any = j;
            if (!same && (want == 0 || direction(i) == want)) same = j;
        }
    }
    const auto target = same ? same : any;
    if (!target) {
        say("No zero crossing within 20 ms.", true);
        return;
    }
    set_marker(track, audio, which, *target);
}

// --- Playing -------------------------------------------------------------------

std::int64_t App::here() {
    if (const auto at = player_.position()) return *at;
    return wave_.cursor;
}

void App::play_from(const TrackModel &track, std::shared_ptr<const DecodedTrack> audio, std::int64_t from) {
    const LoopShown loop = shown_loop(track, *audio);
    if (loop.kind == LoopShown::Kind::None) player_.play(audio, from, std::nullopt, std::nullopt);
    else player_.play(audio, from, loop.start, loop.end);
    if (!player_.error().empty()) say(player_.error(), true);
}

void App::toggle_play(const TrackModel &track, std::shared_ptr<const DecodedTrack> audio) {
    if (player_.playing()) {
        if (const auto at = player_.position()) wave_.cursor = *at;
        player_.stop();
        return;
    }
    play_from(track, std::move(audio), wave_.cursor);
}

void App::audition(const TrackModel &track, std::shared_ptr<const DecodedTrack> audio,
                   std::optional<std::pair<std::int64_t, std::int64_t>> points) {
    LoopShown loop = shown_loop(track, *audio);
    if (points) loop = {LoopShown::Kind::Custom, points->first, points->second};
    if (loop.kind == LoopShown::Kind::None) {
        say("No loop to audition: pick Custom loop or Whole file.", true);
        return;
    }
    const int rate = audio->audio.rate;
    const std::int64_t pre = std::llround(kPre[pre_choice_] * rate);
    const std::int64_t post = std::llround(kPost[post_choice_] * rate);
    wave_.ensure_visible(std::max<std::int64_t>(loop.end - pre, 0));
    player_.audition(std::move(audio), loop.start, loop.end, pre, post, repeat_);
    if (!player_.error().empty()) say(player_.error(), true);
}

void App::loop_keys(TrackModel &track, const DecodedTrack &audio) {
    const ImGuiIO &io = ImGui::GetIO();
    if (io.WantTextInput || io.KeyCtrl || io.KeyAlt || prompt_ || ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId))
        return;
    const auto pressed = [](ImGuiKey key, bool repeat = false) { return ImGui::IsKeyPressed(key, repeat); };
    auto shared = decoder_.get(track_path_on_disk(track));
    if (!shared) return;
    if (pressed(ImGuiKey_Space)) toggle_play(track, shared);
    if (pressed(ImGuiKey_Home)) play_from(track, shared, 0);
    if (pressed(ImGuiKey_Escape)) player_.stop();
    if (pressed(ImGuiKey_A)) audition(track, shared);
    if (pressed(ImGuiKey_S)) {
        ensure_custom(track, audio);
        set_marker(track, audio, Marker::Start, here());
    }
    if (pressed(ImGuiKey_E)) {
        ensure_custom(track, audio);
        set_marker(track, audio, Marker::End, here());
    }
    if (pressed(ImGuiKey_Z)) snap_zero(track, audio, active_marker_);
    if (pressed(ImGuiKey_LeftBracket)) active_marker_ = Marker::Start;
    if (pressed(ImGuiKey_RightBracket)) active_marker_ = Marker::End;
    if (pressed(ImGuiKey_LeftArrow, true)) nudge(track, audio, active_marker_, -1, io.KeyShift ? 10 : 1);
    if (pressed(ImGuiKey_RightArrow, true)) nudge(track, audio, active_marker_, 1, io.KeyShift ? 10 : 1);
    if (pressed(ImGuiKey_Equal, true) || pressed(ImGuiKey_KeypadAdd, true)) wave_.zoom_by(0.5, static_cast<double>(here()));
    if (pressed(ImGuiKey_Minus, true) || pressed(ImGuiKey_KeypadSubtract, true)) wave_.zoom_by(2.0, static_cast<double>(here()));
    if (pressed(ImGuiKey_F)) wave_.zoom_fit();
}

// --- The step ------------------------------------------------------------------

void App::tab_loops() {
    if (!open_) {
        widgets::centred_note("Pick or make a mod on the Mod step first.");
        return;
    }
    clamp_selection();
    auto &list = open_->model.series;
    // A track is what this step edits: the selected one, or the first.
    if (selected_.series < 0 || selected_.track < 0) {
        for (int s = 0; s < static_cast<int>(list.size()) && selected_.track < 0; ++s)
            if (!list[s].tracks.empty()) selected_ = {s, 0};
    }
    ImGui::BeginChild("##tracks", ImVec2(em() * 16.0f, 0.0f), ImGuiChildFlags_Borders);
    loops_list();
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##editor", ImVec2(0.0f, 0.0f), ImGuiChildFlags_None);
    if (selected_.series >= 0 && selected_.track >= 0) loop_editor(selected_.series, selected_.track);
    else widgets::centred_note("Add tracks on the Music step first.");
    ImGui::EndChild();
}

void App::loops_list() {
    const theme::Palette &colours = theme::palette();
    auto &list = open_->model.series;
    for (int s = 0; s < static_cast<int>(list.size()); ++s) {
        if (list[s].tracks.empty()) continue;
        ImGui::PushID(s);
        widgets::muted(list[s].name.empty() ? std::string("(no name)") : list[s].name);
        for (int t = 0; t < static_cast<int>(list[s].tracks.size()); ++t) {
            const TrackModel &track = list[s].tracks[t];
            ImGui::PushID(t);
            const bool selected = selected_.series == s && selected_.track == t;
            const ImVec2 row = ImGui::GetCursorPos();
            if (ImGui::Selectable("##t", selected, ImGuiSelectableFlags_AllowOverlap) && !selected) {
                player_.stop();
                selected_ = {s, t};
            }
            const float right = ImGui::GetContentRegionMax().x;
            const float width = ImGui::CalcTextSize("whole").x + em() * 0.8f;
            ImGui::SetCursorPos(ImVec2(row.x + ImGui::GetStyle().FramePadding.x, row.y));
            const std::string path = "series." + std::to_string(s) + ".tracks." + std::to_string(t) + ".loop";
            if (has_errors_below(path)) {
                ImGui::PushStyleColor(ImGuiCol_Text, colours.error);
                ImGui::TextUnformatted(kDot);
                ImGui::PopStyleColor();
                ImGui::SameLine();
            }
            ImGui::PushClipRect(ImGui::GetCursorScreenPos(),
                                ImVec2(ImGui::GetWindowPos().x + right - width, ImGui::GetCursorScreenPos().y + em() * 2.0f),
                                true);
            ImGui::TextUnformatted(track.title.empty() ? "(no title)" : track.title.c_str());
            ImGui::PopClipRect();
            ImGui::SameLine(right - width);
            widgets::muted(badge(track.loop));
            ImGui::PopID();
        }
        ImGui::Spacing();
        ImGui::PopID();
    }
}

void App::loop_editor(int s, int t) {
    TrackModel &track = open_->model.series[s].tracks[t];
    const std::string path = "series." + std::to_string(s) + ".tracks." + std::to_string(t);
    widgets::heading(track.title.empty() ? std::string("(no title)") : track.title);
    const AudioInfo *info = info_for(track.file);
    const std::filesystem::path file = track_path_on_disk(track);
    auto audio = decoder_.get(file);
    if (audio && !audio->audio.error.empty()) {
        const Problem problem{Problem::Level::Error, path + ".file", track.file + " cannot be decoded: " + audio->audio.error, 0};
        widgets::problem_lines({&problem});
        return;
    }
    if (!audio) {
        widgets::muted(track.file + "  " + kMiddle + "  decoding" + "\xE2\x80\xA6");
        return;
    }
    if (wave_file_ != file) {
        wave_.reset(*audio);
        wave_file_ = file;
        // Suggestions were for the track before.
        suggester_.cancel();
        candidates_.clear();
        suggest_status_.clear();
    }
    // Its exact length and rate, for the checks and for saving.
    if (auto found = infos_.find(track.file); found != infos_.end() && !found->second.samples) {
        found->second.samples = audio->audio.frames;
        found->second.decoded_rate = audio->audio.rate;
    }
    const int rate = audio->audio.rate;
    std::string line = track.file;
    if (info != nullptr && info->error.empty()) line += "  " + std::string(kMiddle) + "  " + describe(*info);
    line += "  " + std::string(kMiddle) + "  " + with_commas(audio->audio.frames) + " samples at " + with_commas(rate) +
            " Hz";
    line += rate == 44100 ? "; the port plays it at its own rate"
                          : "; the port resamples it to 44,100 Hz (loop points to the nearest frame)";
    ImGui::PushTextWrapPos(0.0f);
    widgets::muted(line);
    ImGui::PopTextWrapPos();
    widgets::problem_lines(problems_at(problems_, path + ".loop", true));

    loop_mode_row(track, *audio, info);
    loop_transport(track, *audio);

    // The waveform and the overview strip.
    const LoopShown loop = shown_loop(track, *audio);
    const auto playhead = player_.track() == audio.get() ? player_.position() : std::nullopt;
    if (playhead && player_.playing()) wave_.follow(*playhead);
    const float wave_height = std::max(em() * 9.0f, ImGui::GetContentRegionAvail().y - em() * 20.0f);
    const WaveView::Events events =
        wave_.draw("##wave", *audio, loop, playhead, active_marker_, ImVec2(ImGui::GetContentRegionAvail().x, wave_height));
    if (events.cursor) {
        wave_.cursor = *events.cursor;
        if (player_.playing()) play_from(track, audio, wave_.cursor);
    }
    if (events.set) set_marker(track, *audio, events.set->first, events.set->second);
    if (events.picked) active_marker_ = *events.picked;
    if (events.drag) set_marker(track, *audio, events.drag->first, events.drag->second);
    wave_.draw_overview("##overview", *audio, loop, playhead, ImVec2(ImGui::GetContentRegionAvail().x, em() * 2.2f));

    loop_markers(track, *audio);
    // The seam on the left, the suggestions beside it.
    ImGui::Spacing();
    const float rest = ImGui::GetContentRegionAvail().y;
    const float seam_width = std::floor(ImGui::GetContentRegionAvail().x * 0.56f);
    ImGui::BeginChild("##seampart", ImVec2(seam_width, rest), ImGuiChildFlags_None);
    loop_seam(track, *audio);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##suggest", ImVec2(0.0f, rest), ImGuiChildFlags_Borders);
    loop_suggestions(track, audio);
    ImGui::EndChild();
    loop_keys(track, *audio);
}

void App::loop_suggestions(TrackModel &track, std::shared_ptr<const DecodedTrack> audio) {
    if (auto done = suggester_.take(); done && done->file == wave_file_) {
        candidates_ = std::move(done->result.candidates);
        suggest_status_ = !candidates_.empty()
                              ? std::to_string(candidates_.size()) + " found in " +
                                    fixed(done->seconds, done->seconds < 1.0 ? 2 : 1) + " s"
                              : (!done->result.note.empty() ? done->result.note : std::string("No repeat found"));
    }
    const int rate = audio->audio.rate;
    const bool busy = suggester_.busy();
    ImGui::TextUnformatted("Suggested loops");
    ImGui::BeginDisabled(busy);
    if (ImGui::Button("Suggest loops")) suggester_.start(audio, -1, min_loop_);
    widgets::tooltip("Search the whole file for a start and an end whose audio matches");
    ImGui::SameLine();
    if (ImGui::Button("Starts for my end")) {
        const LoopShown loop = shown_loop(track, *audio);
        suggester_.start(audio, loop.end, min_loop_);
    }
    widgets::tooltip("Keep the loop end; find starts that match it");
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(em() * 3.0f);
    ImGui::InputDouble("##min", &min_loop_, 0.0, 0.0, "%.0f");
    if (!(min_loop_ > 0.0)) min_loop_ = 8.0;
    widgets::tooltip("The shortest loop to look for, in seconds");
    ImGui::SameLine();
    widgets::muted("s at least");
    if (busy) {
        widgets::muted("Searching" + std::string("\xE2\x80\xA6") + " " + std::to_string(static_cast<int>(suggester_.progress() * 100.0)) + "%");
        return;
    }
    ImGui::PushTextWrapPos(0.0f);
    widgets::muted(suggest_status_.empty() ? std::string("Suggestions only: audition each seam and choose by ear. The file "
                                                         "must hold the repeat (most rips play the loop twice).")
                                           : suggest_status_ + ". Suggestions only: audition each and choose by ear.");
    ImGui::PopTextWrapPos();
    const theme::Palette &colours = theme::palette();
    for (std::size_t i = 0; i < candidates_.size(); ++i) {
        const LoopCandidate &c = candidates_[i];
        ImGui::PushID(static_cast<int>(i));
        ImGui::Separator();
        const bool weak = c.score < 0.9;
        ImGui::PushFont(theme::mono_font(), theme::mono_size());
        ImGui::PushStyleColor(ImGuiCol_Text, weak ? colours.text_muted : ImGui::GetStyleColorVec4(ImGuiCol_Text));
        ImGui::TextUnformatted((format_time(c.start, rate) + " \xE2\x86\x92 " + format_time(c.end, rate) + "  loop " +
                                format_time(c.length, rate))
                                   .c_str());
        ImGui::PopStyleColor();
        ImGui::PopFont();
        if (ImGui::SmallButton("Audition")) audition(track, audio, std::pair{c.start, c.end});
        widgets::tooltip("Hear this seam");
        ImGui::SameLine();
        if (ImGui::SmallButton("Use")) {
            set_loop(track, c.start, c.end, audio->audio.frames);
            wave_.zoom_around(static_cast<double>(c.end), rate * 4.0);
        }
        widgets::tooltip("Make it the track's loop");
        std::string score = "match " + fixed(c.score * 100.0, 1) + "%";
        if (std::fabs(c.level_db) > 1.0) score += ", level " + std::string(c.level_db > 0 ? "+" : "") + fixed(c.level_db, 1) + " dB";
        ImGui::SameLine();
        widgets::muted(score + "  " + kMiddle + "  " + with_commas(c.start) + " to " + with_commas(c.end));
        ImGui::PopID();
    }
}

void App::loop_mode_row(TrackModel &track, const DecodedTrack &audio, const AudioInfo *info) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Loop");
    ImGui::SameLine();
    const LoopShown before = shown_loop(track, audio);
    const auto choice = [&](const char *label, LoopModel::Mode mode, const char *tip) {
        if (ImGui::RadioButton(label, track.loop.mode == mode) && track.loop.mode != mode) {
            if (mode == LoopModel::Mode::Custom) {
                // A custom loop starts from what the track played: the file's own
                // loop, else the whole file, unless it had custom points before.
                const bool had = track.loop.end.has_value() || track.loop.start > 0 || track.loop.in_seconds;
                if (had) track.loop.mode = LoopModel::Mode::Custom;
                else if (before.kind == LoopShown::Kind::File) set_loop(track, before.start, before.end, audio.audio.frames);
                else set_loop(track, 0, audio.audio.frames, audio.audio.frames);
            } else {
                track.loop.mode = mode;
            }
        }
        widgets::tooltip(tip);
        ImGui::SameLine();
    };
    choice("Custom loop", LoopModel::Mode::Custom, "From the start marker to the end marker, again and again");
    choice("Whole file", LoopModel::Mode::Whole, "The whole file, again and again");
    choice("None", LoopModel::Mode::None, "Plays once");
    choice("Auto", LoopModel::Mode::Auto, "The file's own loop (loop tags, or a BRSTM, BCSTM or BFSTM header), else the whole file");
    std::string note;
    if (info != nullptr && info->loop) {
        note = "(" + info->loop->source + ": " + with_commas(info->loop->start) + " to " +
               (info->loop->end ? with_commas(*info->loop->end) : std::string("the end")) + ")";
    } else if (info != nullptr && info->stream) {
        note = "(the " + info->stream->format + " header has " + (info->stream_problem.empty() ? "no loop" : "a broken loop") +
               ", so the whole file)";
    } else {
        note = "(no loop tags in the file, so the whole file)";
    }
    widgets::muted(note);
}

void App::loop_transport(TrackModel &track, const DecodedTrack &audio) {
    auto shared = decoder_.get(track_path_on_disk(track));
    if (!shared) return;
    const bool playing = player_.playing() && player_.track() == shared.get();
    if (ImGui::Button(playing ? "Pause" : (std::string(kPlay) + " Play").c_str(), ImVec2(em() * 5.0f, 0.0f)))
        toggle_play(track, shared);
    widgets::tooltip("Plays from the cursor as the game would, round the loop (Space)");
    ImGui::SameLine();
    if (ImGui::Button("From start")) play_from(track, shared, 0);
    widgets::tooltip("Plays from the beginning of the file (Home)");
    ImGui::SameLine();
    if (ImGui::Button("Audition seam")) audition(track, shared);
    widgets::tooltip("The seconds before the loop end, then the loop start, sample-exact (A)");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(em() * 3.6f);
    ImGui::Combo("##pre", &pre_choice_, kPreNames, IM_ARRAYSIZE(kPreNames));
    widgets::tooltip("Seconds before the loop end");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(em() * 3.6f);
    ImGui::Combo("##post", &post_choice_, kPostNames, IM_ARRAYSIZE(kPostNames));
    widgets::tooltip("Seconds after the jump");
    ImGui::SameLine();
    ImGui::Checkbox("repeat", &repeat_);
    ImGui::SameLine();
    ImGui::TextUnformatted("  ");
    ImGui::SameLine();
    if (ImGui::Button("Fit")) wave_.zoom_fit();
    widgets::tooltip("All of the file (F)");
    ImGui::SameLine();
    const LoopShown loop = shown_loop(track, audio);
    if (ImGui::Button("Loop")) {
        const double pad = (loop.end - loop.start) * 0.05;
        wave_.zoom_to(loop.start - pad, loop.end + pad);
    }
    ImGui::SameLine();
    if (ImGui::Button("At S")) wave_.zoom_around(static_cast<double>(loop.start), audio.audio.rate * 2.0);
    ImGui::SameLine();
    if (ImGui::Button("At E")) wave_.zoom_around(static_cast<double>(loop.end), audio.audio.rate * 2.0);
    ImGui::SameLine();
    if (ImGui::Button("+")) wave_.zoom_by(0.5, static_cast<double>(here()));
    ImGui::SameLine();
    if (ImGui::Button("-")) wave_.zoom_by(2.0, static_cast<double>(here()));
    ImGui::SameLine();
    const std::int64_t at = here();
    ImGui::AlignTextToFramePadding();
    ImGui::PushFont(theme::mono_font(), theme::mono_size());
    widgets::muted(format_time(at, audio.audio.rate) + "  " + kMiddle + "  " + with_commas(at));
    ImGui::PopFont();
}

void App::loop_markers(TrackModel &track, const DecodedTrack &audio) {
    const LoopShown loop = shown_loop(track, audio);
    const bool custom = loop.kind == LoopShown::Kind::Custom;
    const int rate = audio.audio.rate;
    const theme::Palette &colours = theme::palette();
    ImGui::Spacing();
    const auto box = [&](Marker which, std::string &samples_text, std::string &time_text) {
        const bool is_start = which == Marker::Start;
        const std::int64_t value = is_start ? loop.start : loop.end;
        ImGui::PushID(is_start ? "start" : "end");
        ImGui::BeginGroup();
        const bool picked = custom && active_marker_ == which;
        ImGui::PushStyleColor(ImGuiCol_Text, is_start ? colours.start : colours.end);
        ImGui::TextUnformatted(is_start ? (picked ? "Loop start  [" : "Loop start") : (picked ? "Loop end  ]" : "Loop end"));
        ImGui::PopStyleColor();
        if (ImGui::IsItemClicked() && custom) active_marker_ = which;
        ImGui::BeginDisabled(!custom);
        ImGui::SetNextItemWidth(em() * 7.0f);
        widgets::input_text("##samples", samples_text, ImGuiInputTextFlags_CharsDecimal);
        if (ImGui::IsItemActivated()) active_marker_ = which;
        if (!ImGui::IsItemActive()) samples_text = std::to_string(value);
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            std::string digits;
            for (const char c : samples_text)
                if (c >= '0' && c <= '9') digits += c;
            if (!digits.empty()) set_marker(track, audio, which, std::stoll(digits));
        }
        widgets::tooltip("In samples of the file");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(em() * 6.0f);
        widgets::input_text("##time", time_text);
        if (ImGui::IsItemActivated()) active_marker_ = which;
        if (!ImGui::IsItemActive()) time_text = format_time(value, rate);
        if (ImGui::IsItemDeactivatedAfterEdit())
            if (const auto seconds = parse_time(time_text)) set_marker(track, audio, which, std::llround(*seconds * rate));
        widgets::tooltip("m:ss.mmm");
        if (ImGui::Button(is_start ? "Set (S)" : "Set (E)")) set_marker(track, audio, which, here());
        widgets::tooltip("At the playhead, or the cursor");
        ImGui::SameLine();
        if (ImGui::ArrowButton("##earlier", ImGuiDir_Left)) nudge(track, audio, which, -1, 1);
        widgets::tooltip("Earlier by the step (Left)");
        ImGui::SameLine();
        if (ImGui::ArrowButton("##later", ImGuiDir_Right)) nudge(track, audio, which, 1, 1);
        widgets::tooltip("Later by the step (Right)");
        ImGui::SameLine();
        if (ImGui::Button("Zero")) snap_zero(track, audio, which);
        widgets::tooltip("The nearest zero crossing (Z)");
        ImGui::SameLine();
        if (ImGui::Button("Go")) {
            wave_.cursor = value;
            wave_.ensure_visible(value);
        }
        widgets::tooltip("The cursor to it");
        ImGui::EndDisabled();
        ImGui::EndGroup();
        ImGui::PopID();
    };
    box(Marker::Start, start_samples_, start_time_);
    ImGui::SameLine(0.0f, em() * 2.0f);
    box(Marker::End, end_samples_, end_time_);
    ImGui::SameLine(0.0f, em() * 2.0f);
    ImGui::BeginGroup();
    ImGui::TextUnformatted("Step");
    ImGui::SetNextItemWidth(em() * 8.0f);
    ImGui::Combo("##step", &step_choice_, kSteps, IM_ARRAYSIZE(kSteps));
    widgets::tooltip("How far Left and Right move the marker (Shift: ten steps)");
    ImGui::SetNextItemWidth(em() * 4.0f);
    ImGui::InputDouble("BPM", &bpm_, 0.0, 0.0, "%.4g");
    if (!(bpm_ > 0.0 && bpm_ < 1000.0)) bpm_ = 120.0;
    ImGui::EndGroup();
    ImGui::SameLine(0.0f, em() * 2.0f);
    ImGui::BeginGroup();
    if (loop.kind == LoopShown::Kind::None) {
        widgets::muted("Plays once, no loop.");
    } else {
        const std::int64_t length = loop.end - loop.start;
        widgets::muted("loop " + with_commas(length) + " samples");
        widgets::muted(format_time(length, rate) + " long, " + fixed(length / static_cast<double>(rate) * bpm_ / 60.0, 2) +
                       " beats at " + fixed(bpm_, 0) + " BPM");
        widgets::muted("intro " + format_time(loop.start, rate));
    }
    ImGui::EndGroup();
}

void App::loop_seam(const TrackModel &track, const DecodedTrack &audio) {
    const LoopShown loop = shown_loop(track, audio);
    ImGui::Spacing();
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Seam");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(em() * 5.0f);
    ImGui::Combo("##seam", &seam_choice_, kSeamNames, IM_ARRAYSIZE(kSeamNames));
    widgets::tooltip("How much of each side of the jump to show");
    if (loop.kind == LoopShown::Kind::None) {
        ImGui::SameLine();
        widgets::muted("no loop, no seam");
        return;
    }
    const std::int64_t width = std::llround(kSeam[seam_choice_] * audio.audio.rate);
    // Room under it for its numbers, which wrap onto two lines.
    const float height = std::max(em() * 4.0f, ImGui::GetContentRegionAvail().y - ImGui::GetTextLineHeightWithSpacing() * 2.6f);
    const auto stats = draw_seam("##seamview", audio, loop.start, loop.end, width,
                                 ImVec2(ImGui::GetContentRegionAvail().x, height));
    if (!stats) return;
    const auto db = [](const std::optional<double> &v) { return v ? fixed(*v, 1) + " dB" : std::string("-"); };
    ImGui::PushTextWrapPos(0.0f);
    widgets::muted("step across the seam " + fixed(stats->step * 100.0, 2) + "% of full scale (neighbouring samples " +
                   fixed(stats->usual * 100.0, 2) + "%)  " + kMiddle + "  audio before end vs before start: " +
                   db(stats->before) + "  " + kMiddle + "  after end vs after start: " +
                   (stats->after ? db(stats->after) : std::string("(end of file)")) + " (lower is closer)");
    ImGui::PopTextWrapPos();
}

} // namespace d012::bgm_maker
