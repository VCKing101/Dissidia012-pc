// The Check & save step: the mod's problems, each with the step to fix it on;
// saving (once every track's file has been read to the end, so the loop
// checks know its exact length); Try in game. MIT licence; see
// port/README.md.

#include "app.hpp"

#include "saving.hpp"
#include "theme.hpp"
#include "widgets.hpp"

#include <imgui.h>

#include <algorithm>

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

constexpr const char *kTick = "\xE2\x9C\x93";     // ✓
constexpr const char *kMiddle = "\xC2\xB7";       // ·
constexpr const char *kEllipsis = "\xE2\x80\xA6"; // …

float em() { return ImGui::GetFontSize(); }

void coloured(const std::string &text, const ImVec4 &colour) {
    ImGui::PushStyleColor(ImGuiCol_Text, colour);
    ImGui::PushTextWrapPos(0.0f);
    ImGui::TextUnformatted(text.c_str());
    ImGui::PopTextWrapPos();
    ImGui::PopStyleColor();
}

} // namespace

// --- Reading the files -------------------------------------------------------

void App::measure_all() {
    if (!open_) return;
    const std::filesystem::path folder = files_folder();
    for (const SeriesModel &series : open_->model.series)
        for (const TrackModel &track : series.tracks) {
            if (track.file.empty() || measuring_.contains(track.file)) continue;
            if (const auto found = infos_.find(track.file); found != infos_.end() && measured(found->second)) continue;
            measuring_.insert(track.file);
            measurer_.add(track.file, folder / manifest::path_from_utf8(track.file), import_generation_);
        }
}

bool App::all_measured() const {
    if (!open_) return false;
    for (const SeriesModel &series : open_->model.series)
        for (const TrackModel &track : series.tracks) {
            if (track.file.empty()) continue;
            const auto found = infos_.find(track.file);
            if (found == infos_.end() || !measured(found->second)) return false;
        }
    return true;
}

void App::take_measured() {
    for (Measured &result : measurer_.take()) {
        if (result.tag != import_generation_ || !open_) continue;
        measuring_.erase(result.file);
        infos_[result.file] = std::move(result.info);
        ++infos_changed_;
    }
    // A file asked for and dropped (another mod), or changed since: ask again.
    if (save_wanted_) measure_all();
}

// --- Saving ------------------------------------------------------------------

void App::save() {
    if (!open_) return;
    SaveRequest request;
    request.mods = mods_.path;
    request.folder = open_->folder;
    request.is_new = open_->is_new;
    request.staging = open_->is_new ? staging_ : std::filesystem::path();
    request.model = open_->model;
    request.facts = known_facts();
    request.others = other_mods();
    request.added = added_files_;
    const SaveResult result = save_mod(request);
    problems_ = result.problems;
    checked_ = open_->model;
    checked_folder_ = open_->folder;
    checked_infos_ = infos_changed_;
    show_all_problems_ = true;
    if (!result.error.empty()) {
        save_note_ = "Not saved: " + result.error + ".";
        save_note_error_ = true;
        say(save_note_, true);
        return;
    }
    if (!result.saved) {
        save_note_ = "Not saved: fix the problems listed above first.";
        save_note_error_ = true;
        say(save_note_, true);
        return;
    }
    // Saved: keys stay as they are now (players' choices refer to them).
    for (SeriesModel &series : open_->model.series) {
        series.auto_key = false;
        for (TrackModel &track : series.tracks) track.auto_key = false;
    }
    const std::filesystem::path folder = mods_.path / manifest::path_from_utf8(open_->folder);
    std::vector<std::filesystem::path> added;
    for (const std::filesystem::path &file : added_files_) {
        const std::filesystem::path now = open_->is_new ? folder / file.lexically_relative(staging_) : file;
        if (std::find(result.deleted.begin(), result.deleted.end(), now) == result.deleted.end()) added.push_back(now);
    }
    added_files_ = std::move(added);
    if (open_->is_new) {
        open_->is_new = false;
        staging_.clear();  // moved into the mod's folder
    }
    open_->saved = open_->model;
    open_->saved_text = result.text;
    open_->open_problem.clear();
    undo_.reset(open_->model);
    refresh_list();
    checked_.reset();
    const std::string where = manifest::utf8(folder / "mod.json");
    if (result.unchanged) save_note_ = "Saved: " + where + " was already so.";
    else save_note_ = "Saved " + where + ".";
    if (!result.backup.empty()) save_note_ += "\nThe old one is in " + result.backup + ".";
    if (!result.deleted.empty())
        save_note_ += "\n" + std::to_string(result.deleted.size()) + " copied file(s) no track uses were deleted.";
    save_note_error_ = false;
    say(result.unchanged ? "Saved (no change)." : "Saved.");
}

// --- The step ----------------------------------------------------------------

void App::tab_check() {
    if (!open_) {
        widgets::centred_note("Pick or make a mod on the Mod step first.");
        return;
    }
    show_all_problems_ = true;
    // The loop and length checks need each file's exact length.
    measure_all();
    widgets::heading("Check");
    const auto count = [&](Problem::Level level) {
        return std::count_if(problems_.begin(), problems_.end(), [&](const Problem &p) { return p.level == level; });
    };
    const auto errors = count(Problem::Level::Error);
    const auto warnings = count(Problem::Level::Warning);
    const std::string reading = measurer_.status();
    if (errors == 0 && warnings == 0 && reading.empty()) {
        coloured(std::string(kTick) + " The game would load this mod as it is.", theme::palette().good);
    } else if (errors == 0 && reading.empty()) {
        coloured(std::string(kTick) + " The game would load this mod; " + std::to_string(warnings) +
                     " warning(s) below.",
                 theme::palette().good);
    } else if (errors > 0) {
        widgets::muted(std::to_string(errors) + " problem(s) to fix before saving" +
                       (warnings > 0 ? ", " + std::to_string(warnings) + " warning(s)" : std::string()) + ".");
    }
    if (!reading.empty()) widgets::muted(reading + kEllipsis + "  (each file is read to the end once, for its length)");
    ImGui::Spacing();
    static constexpr const char *kTabNames[kTabs] = {"Mod", "Music", "Loops", "Check & save"};
    int row = 0;
    for (const Problem &problem : problems_) {
        ImGui::PushID(row++);
        const Tab tab = tab_of(problem);
        if (ImGui::SmallButton((std::string("Go to ") + kTabNames[static_cast<int>(tab)]).c_str())) switch_to_ = tab;
        ImGui::SameLine();
        widgets::problem_lines({&problem});
        ImGui::PopID();
    }
    ImGui::Spacing();
    ImGui::Separator();
    save_section();
    ImGui::Spacing();
    ImGui::Separator();
    try_section();
}

void App::save_section() {
    widgets::heading("Save");
    const bool errors = has_errors(problems_);
    const std::filesystem::path folder = mods_.path / manifest::path_from_utf8(open_->folder);
    ImGui::PushTextWrapPos(0.0f);
    if (open_->is_new)
        widgets::muted("Saving makes the folder " + manifest::utf8(folder) + " with mod.json and the music in it.");
    else
        widgets::muted("Saving writes " + manifest::utf8(folder / "mod.json") +
                       "\nThe one there now is kept in the mod's .backups folder (the last 20).");
    ImGui::PopTextWrapPos();
    ImGui::BeginDisabled(errors || save_wanted_ || (!dirty() && !open_->is_new));
    if (ImGui::Button(save_wanted_ ? (std::string("Saving") + kEllipsis).c_str() : "Save")) {
        save_wanted_ = true;
        measure_all();
    }
    ImGui::EndDisabled();
    widgets::tooltip(errors ? "Fix the problems above first" : "Save the mod (Ctrl+S)");
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    if (save_wanted_) widgets::muted("reading the files first" + std::string(kEllipsis));
    else if (!dirty() && !open_->is_new) widgets::muted("no changes to save");
    if (!save_note_.empty())
        coloured(save_note_, save_note_error_ ? theme::palette().error : theme::palette().good);
}

void App::try_section() {
    widgets::heading("Try in game");
    // Tracks of the mod as saved; the one picked on the Music step first.
    const ModModel &saved = open_->saved;
    if (try_series_ < 0 && selected_.series >= 0 && selected_.track >= 0) {
        try_series_ = selected_.series;
        try_track_ = selected_.track;
    }
    const auto valid = [&](int s, int t) {
        return s >= 0 && s < static_cast<int>(saved.series.size()) && t >= 0 &&
               t < static_cast<int>(saved.series[s].tracks.size());
    };
    if (!valid(try_series_, try_track_)) {
        try_series_ = try_track_ = -1;
        for (int s = 0; s < static_cast<int>(saved.series.size()) && try_series_ < 0; ++s)
            if (!saved.series[s].tracks.empty()) try_series_ = s, try_track_ = 0;
    }
    const auto label = [&](int s, int t) {
        const SeriesModel &series = saved.series[s];
        const TrackModel &track = series.tracks[t];
        return (series.name.empty() ? series.key : series.name) + "  " + kMiddle + "  " +
               (track.title.empty() ? track.key : track.title);
    };
    const std::filesystem::path program = game_program();
    // Asking Windows for its processes is not for every frame.
    const auto now = std::chrono::steady_clock::now();
    if (now - game_checked_ > std::chrono::seconds(1)) {
        game_seen_running_ = launcher_.running() || game_running();
        game_checked_ = now;
    }
    const bool on_disc = !open_->is_new && !dirty() && !has_errors(problems_);
    ImGui::PushTextWrapPos(0.0f);
    widgets::muted("Starts the game with its battle music selector on the track; start a battle to hear it loop.");
    ImGui::PopTextWrapPos();
    if (try_series_ < 0) {
        widgets::muted("The saved mod has no tracks yet.");
        return;
    }
    ImGui::SetNextItemWidth(std::min(ImGui::GetContentRegionAvail().x, em() * 30.0f));
    if (ImGui::BeginCombo("##try", label(try_series_, try_track_).c_str())) {
        for (int s = 0; s < static_cast<int>(saved.series.size()); ++s)
            for (int t = 0; t < static_cast<int>(saved.series[s].tracks.size()); ++t) {
                ImGui::PushID(s * 10000 + t);
                if (ImGui::Selectable(label(s, t).c_str(), s == try_series_ && t == try_track_)) {
                    try_series_ = s;
                    try_track_ = t;
                }
                ImGui::PopID();
            }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::BeginDisabled(!on_disc || program.empty() || game_seen_running_);
    if (ImGui::Button("Try in game")) {
        std::string error;
        const std::string track_id = saved.id + ":" + saved.series[try_series_].tracks[try_track_].key;
        if (launcher_.start(mods_, track_id, error)) {
            game_seen_running_ = true;
            game_checked_ = now;
            say("The game is starting on " + track_id + ".");
        } else {
            say("Not started: " + error + ".", true);
        }
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    if (program.empty()) widgets::muted("the game is not beside this program");
    else if (game_seen_running_) widgets::muted("the game is running; close it to start it again");
    else if (!on_disc) widgets::muted("save the mod first");
}

} // namespace d012::bgm_maker
