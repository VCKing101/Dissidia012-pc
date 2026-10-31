// The Check & save step: the mod's problems, each with the step to fix it on;
// saving (once every track's file has been read to the end, so the loop
// checks know its exact length); Try in game. MIT licence; see
// port/README.md.

#include "app.hpp"

#include "psp_export.hpp"
#include "saving.hpp"
#include "theme.hpp"
#include "widgets.hpp"

#include <imgui.h>

#include <SDL3/SDL.h>

#include <algorithm>
#include <mutex>

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

// A path the save or open dialog answers with, on its own thread.
struct PathSlot {
    std::mutex *mutex{};
    std::optional<std::filesystem::path> *target{};
};

void SDLCALL path_chosen(void *data, const char *const *files, int) {
    const auto *slot = static_cast<const PathSlot *>(data);
    if (files == nullptr || files[0] == nullptr) return;  // an error, or cancelled
    const std::lock_guard lock(*slot->mutex);
    *slot->target = manifest::path_from_utf8(files[0]);
}

std::string megabytes(std::uint64_t bytes) { return std::to_string((bytes + (1u << 19u)) >> 20u) + " MB"; }

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
    ImGui::Spacing();
    ImGui::Separator();
    psp_section();
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

void App::choose_iso(bool original) {
    static PathSlot slot;
    slot = {&chosen_mutex_, original ? &chosen_iso_ : &chosen_out_};
    static const SDL_DialogFileFilter filters[] = {{"ISO images", "iso"}, {"All files", "*"}};
    const std::filesystem::path &now = original ? psp_original_ : psp_out_;
    const std::string where = manifest::utf8(original ? now.parent_path() : now);
    if (original)
        SDL_ShowOpenFileDialog(path_chosen, &slot, window_.sdl_window(), filters, 2, where.empty() ? nullptr : where.c_str(), false);
    else
        SDL_ShowSaveFileDialog(path_chosen, &slot, window_.sdl_window(), filters, 2, where.empty() ? nullptr : where.c_str());
}

void App::psp_section() {
    using Phase = PspExport::Phase;
    if (!ImGui::CollapsingHeader("Make a PSP / PPSSPP ISO (optional)")) return;
    {
        const std::lock_guard lock(chosen_mutex_);
        if (chosen_iso_) psp_original_ = *chosen_iso_, chosen_iso_.reset(), psp_defaults_ = false;
        if (chosen_out_) psp_out_ = *chosen_out_, chosen_out_.reset();
    }
    if (!psp_defaults_) {
        psp_defaults_ = true;
        if (psp_original_.empty()) psp_original_ = game_iso(mods_);
        if (!psp_original_.empty() && psp_out_.empty())
            psp_out_ = psp_original_.parent_path() /
                       manifest::path_from_utf8(manifest::utf8(psp_original_.stem()) + " (Battle BGM).iso");
    }
    const auto now = std::chrono::steady_clock::now();
    const bool busy = psp_.busy();
    if (!busy && (!psp_plan_ || now - psp_plan_at_ > std::chrono::seconds(2))) {
        psp_plan_ = psp_plan(mods_.path);
        psp_plan_at_ = now;
    }
    if (psp_disc_of_ != psp_original_) {
        psp_disc_of_ = psp_original_;
        psp_disc_.reset();
        psp_disc_error_.clear();
        std::error_code ec;
        if (!psp_original_.empty() && std::filesystem::is_regular_file(psp_original_, ec))
            psp_disc_ = psp_iso::read_disc_music(psp_original_, psp_disc_error_);
    }
    const PspPlan &plan = *psp_plan_;

    ImGui::PushTextWrapPos(0.0f);
    widgets::muted("Makes a copy of your Dissidia 012 ISO (USA) with the battle music of every mod that is on in the mods "
                   "folder added to the game's own list, for PPSSPP or a PSP with custom firmware. Your ISO is not changed. "
                   "The game's list has no series, so each track is named \"<short name> - <title>\" (the short name is "
                   "set on the Music step).");
    ImGui::PopTextWrapPos();
    if (open_ && (dirty() || open_->is_new))
        coloured("It uses the mods as saved: save this one first to include your changes.", theme::palette().warning);
    ImGui::Spacing();

    const float label = em() * 6.0f;
    const auto path_row = [&](const char *name, const char *id, const std::filesystem::path &path, bool original) {
        ImGui::PushID(id);
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(name);
        ImGui::SameLine(label);
        std::string text = manifest::utf8(path);
        ImGui::SetNextItemWidth(std::max(em() * 10.0f, ImGui::GetContentRegionAvail().x - em() * 7.0f));
        ImGui::BeginDisabled(busy);
        if (widgets::input_text("##path", text, 0, original ? "your original ISO" : "where the new ISO goes")) {
            (original ? psp_original_ : psp_out_) = manifest::path_from_utf8(text);
            if (original) psp_defaults_ = true;
        }
        ImGui::SameLine();
        if (ImGui::Button((std::string("Choose") + kEllipsis).c_str())) choose_iso(original);
        ImGui::EndDisabled();
        ImGui::PopID();
    };
    path_row("Your ISO", "original", psp_original_, true);
    if (!psp_disc_error_.empty()) coloured("This ISO: " + psp_disc_error_ + ".", theme::palette().error);
    path_row("New ISO", "out", psp_out_, false);
    std::error_code ec;
    const bool replaces = !psp_out_.empty() && std::filesystem::exists(psp_out_, ec);
    const bool same = replaces && !psp_original_.empty() && std::filesystem::equivalent(psp_original_, psp_out_, ec);
    if (same) coloured("The new ISO cannot replace your original; pick another name.", theme::palette().error);
    else if (replaces) widgets::muted("A file of that name is there; making the ISO replaces it.");
    ImGui::Spacing();

    // What goes in.
    const int room = psp_disc_ ? psp_disc_->room() : psp_iso::kTableRows - 131;
    const int count = static_cast<int>(plan.tracks.size());
    std::vector<const PspTrack *> long_names;
    for (const PspTrack &track : plan.tracks)
        if (track.name.size() > kLongestDiscName) long_names.push_back(&track);
    if (count == 0) widgets::muted("No mod in the mods folder has a track to add yet.");
    else if (count > room)
        coloured(std::to_string(count) + " tracks, but the game's list has room for " + std::to_string(room) +
                     " more. Turn some mods off (mods/enabled.json) to make room.",
                 theme::palette().error);
    else
        widgets::muted(std::to_string(count) + " track(s) to add (room for " + std::to_string(room) + "), about " +
                       megabytes(static_cast<std::uint64_t>(count) * (3u << 20u)) + " more than your ISO.");
    if (!long_names.empty())
        coloured(std::to_string(long_names.size()) + " name(s) are longer than the game's longest (" +
                     std::to_string(kLongestDiscName) + " characters) and may not fit its box; a shorter short name helps.",
                 theme::palette().warning);
    if (count > 0 && ImGui::TreeNode("psp-tracks", "The tracks, in the game's order")) {
        for (const PspTrack &track : plan.tracks) {
            const bool too_long = track.name.size() > kLongestDiscName;
            if (too_long) ImGui::PushStyleColor(ImGuiCol_Text, theme::palette().warning);
            ImGui::BulletText("%s", track.name.c_str());
            if (too_long) ImGui::PopStyleColor();
            widgets::tooltip(track.id.c_str());
        }
        for (const std::string &line : plan.left_out) widgets::muted("Left out: " + line);
        ImGui::TreePop();
    }
    const std::filesystem::path encoder = encoder_program(mods_);
    if (encoder.empty()) coloured("The encoder (atracdenc) is not beside the Battle BGM Maker; reinstall it.", theme::palette().error);
    ImGui::Spacing();

    const bool ready = !busy && count > 0 && count <= room && psp_disc_ && !encoder.empty() && !psp_out_.empty() && !same;
    ImGui::BeginDisabled(!ready);
    if (ImGui::Button("Make ISO")) {
        std::string error;
        if (!psp_.start(psp_original_, psp_out_, mods_, error)) say("Not started: " + error + ".", true);
    }
    ImGui::EndDisabled();
    widgets::tooltip("Checks your ISO, encodes the tracks (ATRAC3) and writes the new ISO");
    const PspExport::Status status = psp_.status();
    if (busy) {
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) psp_.cancel();
        const char *what = status.phase == Phase::Checking   ? "Checking your ISO"
                           : status.phase == Phase::Encoding ? "Encoding"
                                                             : "Writing the new ISO";
        const float fraction = status.total == 0 ? 0.0f : static_cast<float>(static_cast<double>(status.done) / static_cast<double>(status.total));
        const std::string detail = status.phase == Phase::Encoding
                                       ? std::to_string(status.done) + " of " + std::to_string(status.total) + " tracks"
                                       : megabytes(status.done) + " of " + megabytes(status.total);
        ImGui::ProgressBar(fraction, ImVec2(std::min(ImGui::GetContentRegionAvail().x, em() * 30.0f), 0.0f),
                           (std::string(what) + kEllipsis + "  " + detail).c_str());
    } else if (status.phase == Phase::Done) {
        coloured(std::string(kTick) + " " + status.message + " Load it in PPSSPP, or copy it to the PSP's ISO folder.",
                 theme::palette().good);
    } else if (status.phase == Phase::Failed) {
        coloured(status.message, theme::palette().error);
    } else if (status.phase == Phase::Cancelled) {
        widgets::muted(status.message);
    }
}

} // namespace d012::bgm_maker
