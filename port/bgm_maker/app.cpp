// The Battle BGM Maker's interface; see app.hpp. MIT licence; see
// port/README.md.

#include "app.hpp"

#include "theme.hpp"
#include "widgets.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <algorithm>
#include <fstream>
#include <iterator>

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

constexpr const char *kTick = "\xE2\x9C\x93";     // ✓
constexpr const char *kDot = "\xE2\x97\x8F";      // ●
constexpr const char *kEllipsis = "\xE2\x80\xA6"; // …

float em() { return ImGui::GetFontSize(); }

bool has_tracks(const ModModel &model) {
    return std::any_of(model.series.begin(), model.series.end(), [](const SeriesModel &s) { return !s.tracks.empty(); });
}

void folder_chosen(void *userdata, const char *const *files, int) {
    if (files == nullptr || files[0] == nullptr) return;  // cancelled, or failed
    auto *slot = static_cast<std::pair<std::mutex *, std::optional<std::filesystem::path> *> *>(userdata);
    const std::lock_guard lock(*slot->first);
    *slot->second = manifest::path_from_utf8(files[0]);
}

} // namespace

App::App(Window &window, ModsFolder mods) : window_(window), mods_(std::move(mods)) {
    refresh_list();
    update_title();
}

App::~App() { leave_mod(); }

void App::refresh_list() { list_ = list_mods(mods_.path); }

void App::test_open(const std::string &folder) {
    if (folder == "+new") {
        new_mod();
        return;
    }
    for (const ModSummary &summary : list_)
        if (summary.folder == folder) open_mod(summary);
}

void App::test_tab(int step) {
    if (step >= 1 && step <= kTabs) switch_to_ = static_cast<Tab>(step - 1);
}

std::filesystem::path App::mod_folder() const {
    return open_ ? mods_.path / manifest::path_from_utf8(open_->folder) : std::filesystem::path();
}

bool App::dirty() const { return open_ && (open_->is_new || open_->model != open_->saved); }

std::filesystem::path App::files_folder() {
    if (!open_) return {};
    if (!open_->is_new) return mod_folder();
    if (staging_.empty()) {
        // A new mod's files wait here until it is saved, so the mods folder
        // has no half-made mod in it that the game would try to read.
        std::error_code ec;
        const auto stamp = std::chrono::system_clock::now().time_since_epoch().count();
        staging_ = std::filesystem::temp_directory_path(ec) / "d012-bgm-maker" / ("new-mod-" + std::to_string(stamp));
        std::filesystem::create_directories(staging_, ec);
    }
    return staging_;
}

void App::leave_mod() {
    ++import_generation_;
    player_.stop();
    suggester_.cancel();
    candidates_.clear();
    suggest_status_.clear();
    decoder_.clear();
    wave_file_.clear();
    std::error_code ec;
    if (open_ && !open_->is_new) {
        std::vector<std::string> kept;
        for (const SeriesModel &series : open_->saved.series)
            for (const TrackModel &track : series.tracks) kept.push_back(track.file);
        for (const auto &file : added_files_) {
            const std::u8string generic = file.lexically_relative(mod_folder()).generic_u8string();
            const std::string relative(generic.begin(), generic.end());
            if (std::find(kept.begin(), kept.end(), relative) == kept.end()) std::filesystem::remove(file, ec);
        }
    }
    if (!staging_.empty()) std::filesystem::remove_all(staging_, ec);
    staging_.clear();
    added_files_.clear();
    infos_.clear();
    open_.reset();
    checked_.reset();
    problems_.clear();
    selected_ = {};
}

void App::say(std::string text, bool error) {
    status_ = std::move(text);
    status_error_ = error;
    status_until_ = std::chrono::steady_clock::now() + std::chrono::seconds(error ? 8 : 4);
}

void App::open_mod(const ModSummary &summary) {
    Open open;
    open.folder = summary.folder;
    const std::filesystem::path file = mods_.path / manifest::path_from_utf8(summary.folder) / "mod.json";
    std::error_code ec;
    if (!std::filesystem::is_regular_file(file, ec)) {
        // A folder with no manifest yet: saving makes one, as the Python tool does.
        if (manifest::valid_key(summary.folder)) open.model.id = summary.folder;
        open.model.version = "1.0.0";
        open.open_problem = "there is no mod.json yet; saving makes one";
    } else {
        std::ifstream in(file, std::ios::binary);
        const std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        std::string error;
        const auto document = json::parse(text, error);
        if (document) open.model = model_from_document(*document);
        else open.open_problem = "mod.json could not be read (" + error + "); saving replaces it, keeping a backup";
        open.saved_text = text;
    }
    // In the game's order (by order, then key), as the Python tool shows them;
    // saving writes them so.
    std::stable_sort(open.model.series.begin(), open.model.series.end(), [](const SeriesModel &a, const SeriesModel &b) {
        return a.order != b.order ? a.order < b.order : a.key < b.key;
    });
    open.saved = open.model;
    open_ = std::move(open);
    undo_.reset(open_->model);
    checked_.reset();
    touched_.clear();
    show_all_problems_ = false;
    selected_ = {};
    if (!open_->model.series.empty()) selected_.series = 0;
    update_title();
}

void App::new_mod() {
    Open open;
    open.is_new = true;
    open.model.version = "1.0.0";
    open_ = std::move(open);
    undo_.reset(open_->model);
    checked_.reset();
    touched_.clear();
    show_all_problems_ = false;
    focus_id_ = true;
    switch_to_ = Tab::Mod;
    update_title();
}

void App::set_mods_folder(const std::filesystem::path &folder) {
    mods_ = {folder, "chosen in this window"};
    refresh_list();
    update_title();
}

void App::after_unsaved_check(std::function<void()> action) {
    if (!dirty()) {
        leave_mod();
        action();
        return;
    }
    pending_ = std::move(action);
    prompt_ = true;
}

void App::update_checks() {
    if (!open_) {
        problems_.clear();
        checked_.reset();
        return;
    }
    if (checked_ && *checked_ == open_->model && checked_folder_ == open_->folder) return;
    std::vector<ModSummary> others;
    for (const ModSummary &summary : list_)
        if (open_->is_new || summary.folder != open_->folder) others.push_back(summary);
    // Until the first change, the file's own problems, with their lines: some
    // (a loop in samples and seconds, a series that is not an object) cannot
    // be in the editor's model, and saving would quietly change them.
    const bool as_on_disc = !open_->is_new && open_->model == open_->saved && !open_->saved_text.empty();
    const std::filesystem::path files = open_->is_new ? staging_ : mod_folder();
    problems_ = as_on_disc ? check_text(open_->saved_text, open_->model.id, files, others)
                           : check_mod(open_->model, files, others);
    if (open_->is_new) {
        if (const std::string why = folder_name_problem(open_->folder, mods_.path); !why.empty())
            problems_.push_back({Problem::Level::Error, "mod.folder", why, 0});
    }
    checked_ = open_->model;
    checked_folder_ = open_->folder;
}

void App::undo() {
    if (!open_) return;
    if (auto state = undo_.undo()) open_->model = *state;
    clamp_selection();
}

void App::redo() {
    if (!open_) return;
    if (auto state = undo_.redo()) open_->model = *state;
    clamp_selection();
}

bool App::has_errors_below(const std::string &path) const {
    return std::any_of(problems_.begin(), problems_.end(), [&](const Problem &p) {
        return p.level == Problem::Level::Error &&
               (p.path == path || (p.path.size() > path.size() && p.path.starts_with(path) && p.path[path.size()] == '.'));
    });
}

App::Tab App::tab_of(const Problem &problem) {
    const std::string &path = problem.path;
    if (path == "mod" || path.starts_with("mod.")) return Tab::Mod;
    if (path.find(".loop") != std::string::npos) return Tab::Loops;
    return Tab::Music;
}

int App::tab_problems(Tab tab) const {
    if (tab == Tab::Check) return 0;
    return static_cast<int>(std::count_if(problems_.begin(), problems_.end(), [&](const Problem &p) {
        return p.level == Problem::Level::Error && tab_of(p) == tab;
    }));
}

bool App::tab_done(Tab tab) const {
    if (!open_) return false;
    switch (tab) {
    case Tab::Mod: return tab_problems(Tab::Mod) == 0;
    case Tab::Music: return has_tracks(open_->model) && tab_problems(Tab::Music) == 0;
    case Tab::Loops: return has_tracks(open_->model) && tab_problems(Tab::Loops) == 0;
    case Tab::Check:
        return !dirty() && std::none_of(problems_.begin(), problems_.end(),
                                        [](const Problem &p) { return p.level == Problem::Level::Error; });
    }
    return false;
}

void App::update_title() {
    std::string title = "Battle BGM Maker";
    if (open_) {
        const ModModel &model = open_->model;
        title += " \xE2\x80\x93 " + (!model.name.empty() ? model.name : !model.id.empty() ? model.id : std::string("new mod"));
        if (dirty()) title += " *";
    }
    if (title != title_) {
        title_ = title;
        window_.set_title(title_);
    }
}

bool App::frame() {
    {
        const std::lock_guard lock(chosen_mutex_);
        if (chosen_folder_) {
            const std::filesystem::path folder = *chosen_folder_;
            chosen_folder_.reset();
            after_unsaved_check([this, folder] { set_mods_folder(folder); });
        }
    }
    if (window_.close_requested()) {
        window_.clear_close_request();
        after_unsaved_check([this] { quit_ = true; });
    }
    {
        std::vector<std::filesystem::path> files;
        int series = -1;
        {
            const std::lock_guard lock(chosen_mutex_);
            if (chosen_files_ready_) {
                files.swap(chosen_files_);
                series = chosen_files_series_;
                chosen_files_ready_ = false;
            }
        }
        if (!files.empty()) add_files(files, series);
    }
    handle_drops();
    take_imports();
    window_.set_animating(importer_.busy() || decoder_.busy() || player_.playing() || suggester_.busy() ||
                          std::chrono::steady_clock::now() < status_until_);

    const ImGuiIO &io = ImGui::GetIO();
    if (!io.WantTextInput && !prompt_) {
        if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_Z)) undo();
        if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_Y) ||
            ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Z))
            redo();
    }
    update_checks();

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::Begin("##main", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);
    draw_top_bar();
    draw_tabs();
    draw_unsaved_prompt();
    ImGui::End();

    if (pending_move_) {
        const PendingMove move = *pending_move_;
        pending_move_.reset();
        move_track(move.series, move.track, move.to_series, move.to_index);
    }
    if (pending_remove_ && open_) {
        const PendingRemove remove = *pending_remove_;
        if (remove.track < 0) remove_series(remove.series);
        else remove_track(remove.series, remove.track);
    }
    pending_remove_.reset();
    // An edit is over when no field is being typed in or dragged: one undo step.
    if (open_ && !ImGui::IsAnyItemActive()) undo_.commit(open_->model);
    update_title();
    return !quit_;
}

void App::draw_top_bar() {
    widgets::heading("Battle BGM Maker");
    const float right = ImGui::GetContentRegionMax().x;
    const char *undo_label = "Undo";
    const char *redo_label = "Redo";
    const float buttons = ImGui::CalcTextSize(undo_label).x + ImGui::CalcTextSize(redo_label).x +
                          ImGui::GetStyle().FramePadding.x * 4.0f + ImGui::GetStyle().ItemSpacing.x;
    if (open_) {
        ImGui::SameLine();
        const std::string name = !open_->model.name.empty() ? open_->model.name : !open_->model.id.empty()
                                                                                       ? open_->model.id
                                                                                       : std::string("New mod");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + em());
        ImGui::AlignTextToFramePadding();
        widgets::muted(name);
        if (dirty()) {
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Text, theme::palette().warning);
            ImGui::TextUnformatted((std::string(kDot) + " not saved").c_str());
            ImGui::PopStyleColor();
        }
    }
    ImGui::SameLine(right - buttons);
    ImGui::BeginDisabled(!undo_.can_undo() || !open_);
    if (ImGui::Button(undo_label)) undo();
    widgets::tooltip("Undo the last change (Ctrl+Z)");
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::BeginDisabled(!undo_.can_redo() || !open_);
    if (ImGui::Button(redo_label)) redo();
    widgets::tooltip("Redo (Ctrl+Y)");
    ImGui::EndDisabled();
    ImGui::Spacing();
}

void App::draw_tabs() {
    static constexpr const char *kNames[kTabs] = {"Mod", "Music", "Loops", "Check & save"};
    static constexpr const char *kIds[kTabs] = {"mod", "music", "loops", "check"};
    // A switch asked for now (a "Go to" button, Back, Next) happens next frame.
    const std::optional<Tab> request = switch_to_;
    switch_to_.reset();
    if (ImGui::BeginTabBar("##steps", ImGuiTabBarFlags_NoTooltip)) {
        for (int i = 0; i < kTabs; ++i) {
            const Tab tab = static_cast<Tab>(i);
            std::string label = std::to_string(i + 1) + "  " + kNames[i];
            const int problems = tab_problems(tab);
            if (problems > 0) label += "  (" + std::to_string(problems) + ")";
            else if (tab_done(tab)) label += std::string("  ") + kTick;
            label += std::string("###") + kIds[i];
            const ImGuiTabItemFlags flags = request == tab ? ImGuiTabItemFlags_SetSelected : 0;
            const bool coloured = problems > 0 || tab_done(tab);
            if (coloured) ImGui::PushStyleColor(ImGuiCol_Text, problems > 0 ? theme::palette().error : theme::palette().good);
            const bool selected = ImGui::BeginTabItem(label.c_str(), nullptr, flags);
            if (coloured) ImGui::PopStyleColor();
            if (selected) {
                tab_ = tab;
                const float navigation = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y * 2.0f;
                ImGui::BeginChild("##page", ImVec2(0.0f, -navigation), ImGuiChildFlags_None);
                switch (tab) {
                case Tab::Mod: tab_mod(); break;
                case Tab::Music: tab_music(); break;
                case Tab::Loops: tab_loops(); break;
                case Tab::Check: tab_check(); break;
                }
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    draw_navigation();
}

void App::draw_navigation() {
    ImGui::Separator();
    const int index = static_cast<int>(tab_);
    ImGui::BeginDisabled(index == 0);
    if (ImGui::Button("\xE2\x86\x90 Back")) switch_to_ = static_cast<Tab>(index - 1);
    ImGui::EndDisabled();
    static constexpr const char *kHints[kTabs] = {
        "Pick a mod, or make a new one, and fill in its details.",
        "Add series and their tracks.",
        "Set where each track loops.",
        "Check the mod and save it.",
    };
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    if (std::chrono::steady_clock::now() < status_until_) {
        ImGui::PushStyleColor(ImGuiCol_Text, status_error_ ? theme::palette().error : theme::palette().good);
        ImGui::TextUnformatted(status_.c_str());
        ImGui::PopStyleColor();
    } else {
        widgets::muted(kHints[index]);
    }
    if (index + 1 < kTabs) {
        const char *next = "Next \xE2\x86\x92";
        ImGui::SameLine(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize(next).x - ImGui::GetStyle().FramePadding.x * 2.0f);
        ImGui::BeginDisabled(!open_);
        if (ImGui::Button(next)) switch_to_ = static_cast<Tab>(index + 1);
        ImGui::EndDisabled();
    }
}

void App::draw_unsaved_prompt() {
    if (prompt_) {
        ImGui::OpenPopup("Unsaved changes");
        prompt_ = false;
    }
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (!ImGui::BeginPopupModal("Unsaved changes", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) return;
    ImGui::PushTextWrapPos(em() * 26.0f);
    ImGui::TextUnformatted("This mod has changes that are not saved. Mods are saved on the Check & save step, once "
                           "they have no problems left.");
    ImGui::PopTextWrapPos();
    ImGui::Spacing();
    if (ImGui::Button("Review and save")) {
        pending_ = nullptr;
        switch_to_ = Tab::Check;
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Discard changes")) {
        auto action = std::move(pending_);
        pending_ = nullptr;
        ImGui::CloseCurrentPopup();
        leave_mod();  // the changes go, whatever the action is
        if (action) action();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
        pending_ = nullptr;
        ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
}

// --- 1. Mod ------------------------------------------------------------------

void App::tab_mod() {
    const float list_width = em() * 19.0f;
    ImGui::BeginChild("##mods", ImVec2(list_width, 0.0f), ImGuiChildFlags_Borders);
    mod_list();
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##details", ImVec2(0.0f, 0.0f), ImGuiChildFlags_None);
    mod_form();
    ImGui::EndChild();
}

void App::choose_mods_folder() {
    static std::pair<std::mutex *, std::optional<std::filesystem::path> *> slot;
    slot = {&chosen_mutex_, &chosen_folder_};
    const std::string start = manifest::utf8(mods_.path);
    SDL_ShowOpenFolderDialog(folder_chosen, &slot, window_.sdl_window(), start.c_str(), false);
}

void App::mod_list() {
    widgets::muted("Mods folder");
    ImGui::PushTextWrapPos(0.0f);
    ImGui::TextUnformatted(manifest::utf8(mods_.path).c_str());
    ImGui::PopTextWrapPos();
    widgets::muted("(" + mods_.found_by + ")");
    if (ImGui::Button((std::string("Open another") + kEllipsis).c_str())) choose_mods_folder();
    widgets::tooltip("Work on the mods in another folder, such as a copy");
    ImGui::SameLine();
    if (ImGui::Button("Refresh")) refresh_list();
    widgets::tooltip("Read the mods folder again");
    ImGui::Separator();
    if (ImGui::Button("+ New mod", ImVec2(-1.0f, 0.0f))) after_unsaved_check([this] { new_mod(); });
    ImGui::Spacing();

    const float row = ImGui::GetTextLineHeight() * 2.0f + ImGui::GetStyle().ItemSpacing.y;
    const auto entry = [&](const std::string &id, bool selected, const std::string &first, const std::string &second,
                           bool second_is_error) {
        const ImVec2 start = ImGui::GetCursorPos();
        const bool clicked = ImGui::Selectable(("##" + id).c_str(), selected, ImGuiSelectableFlags_None, ImVec2(0.0f, row));
        const ImVec2 end = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(start.x + ImGui::GetStyle().FramePadding.x, start.y));
        ImGui::TextUnformatted(first.c_str());
        ImGui::SetCursorPosX(start.x + ImGui::GetStyle().FramePadding.x);
        if (second_is_error) {
            ImGui::PushStyleColor(ImGuiCol_Text, theme::palette().error);
            ImGui::TextUnformatted(second.c_str());
            ImGui::PopStyleColor();
        } else {
            widgets::muted(second);
        }
        ImGui::SetCursorPos(end);
        ImGui::Dummy(ImVec2(0.0f, 0.0f));
        return clicked;
    };
    if (open_ && open_->is_new)
        entry("new", true, !open_->model.name.empty() ? open_->model.name : std::string("New mod"), "not saved yet", false);
    if (list_.empty()) widgets::muted("No mods here yet.");
    for (const ModSummary &summary : list_) {
        const bool selected = open_ && !open_->is_new && open_->folder == summary.folder;
        const std::string first = !summary.name.empty() ? summary.name : summary.folder;
        std::string second = summary.error;
        if (second.empty()) {
            second = summary.id.empty() ? "no id" : summary.id;
            if (!summary.version.empty()) second += "  " + summary.version;
            if (summary.folder != summary.id) second += "  (folder " + summary.folder + ")";
        }
        if (entry(summary.folder, selected, first, second, !summary.error.empty()) && !selected) {
            const ModSummary chosen = summary;
            after_unsaved_check([this, chosen] { open_mod(chosen); });
        }
    }
}

void App::mod_form() {
    if (!open_) {
        widgets::centred_note("Pick a mod on the left to change it, or make a new one with + New mod.");
        return;
    }
    Open &open = *open_;
    ModModel &model = open.model;
    widgets::heading(open.is_new ? "New mod" : "Mod details");
    if (!open.is_new) {
        ImGui::PushTextWrapPos(0.0f);
        widgets::muted("Folder: " + manifest::utf8(mod_folder()));
        ImGui::PopTextWrapPos();
    }
    if (!open.open_problem.empty()) {
        const Problem problem{Problem::Level::Warning, "mod", open.open_problem, 0};
        widgets::problem_lines({&problem});
    }
    widgets::problem_lines(problems_at(problems_, "mod"));
    widgets::problem_lines(problems_at(problems_, "mod.format"));
    ImGui::Spacing();

    const float width = std::min(ImGui::GetContentRegionAvail().x, em() * 34.0f);
    ImGui::PushItemWidth(width);
    ImGui::BeginGroup();
    ImGui::PushID("mod");

    if (focus_id_) {
        ImGui::SetKeyboardFocusHere();  // the next field: the id
        focus_id_ = false;
    }
    // A field's problems, unless it is a new mod's and not left yet.
    const auto shown = [&](const char *path) {
        const bool hidden = open.is_new && !show_all_problems_ &&
                            std::find(touched_.begin(), touched_.end(), path) == touched_.end();
        return hidden ? std::vector<const Problem *>{} : problems_at(problems_, path);
    };
    const auto leave = [&](bool left, const char *path) {
        if (left && std::find(touched_.begin(), touched_.end(), path) == touched_.end()) touched_.emplace_back(path);
    };
    bool left = false;
    if (widgets::text_field("Id", "##id", model.id, shown("mod.id"), "lower-case letters, digits, . _ -", "my-music",
                            widgets::kKeyFlags, &left) &&
        open.is_new && open.folder_follows_id)
        open.folder = model.id;
    leave(left, "mod.id");
    if (left && open.is_new && open.folder_follows_id) leave(true, "mod.folder");
    if (!open.is_new && !open.saved.id.empty() && model.id != open.saved.id) {
        const Problem note{Problem::Level::Warning, "mod.id",
                           "the game remembers players' choices of this mod's tracks by its id; with a new id they "
                           "are new tracks to it",
                           0};
        widgets::problem_lines({&note});
    }
    if (open.is_new) {
        if (widgets::text_field("Folder", "##folder", open.folder, shown("mod.folder"),
                                "in the mods folder; the id unless you change it", nullptr, 0, &left))
            open.folder_follows_id = open.folder == model.id;
        leave(left, "mod.folder");
    }
    widgets::text_field("Name", "##name", model.name, shown("mod.name"), "shown in the game's mod list",
                        "My battle music", 0, &left);
    leave(left, "mod.name");
    widgets::text_field("Version", "##version", model.version, shown("mod.version"), nullptr, "1.0.0", 0, &left);
    leave(left, "mod.version");
    widgets::text_field("Author", "##author", model.author, shown("mod.author"), "optional", nullptr, 0, &left);
    leave(left, "mod.author");
    ImGui::TextUnformatted("Description");
    ImGui::SameLine();
    widgets::muted("optional");
    widgets::input_multiline("##description", model.description, ImVec2(width, ImGui::GetTextLineHeight() * 5.0f));
    widgets::problem_lines(problems_at(problems_, "mod.description"));

    ImGui::PopID();
    ImGui::EndGroup();
    ImGui::PopItemWidth();
}

// --- 4. Check & save ---------------------------------------------------------

void App::tab_check() {
    if (!open_) {
        widgets::centred_note("Pick or make a mod on the Mod step first.");
        return;
    }
    show_all_problems_ = true;
    widgets::heading("Check");
    const auto errors = std::count_if(problems_.begin(), problems_.end(),
                                      [](const Problem &p) { return p.level == Problem::Level::Error; });
    if (problems_.empty()) {
        ImGui::PushStyleColor(ImGuiCol_Text, theme::palette().good);
        ImGui::TextUnformatted((std::string(kTick) + " The game would load this mod as it is.").c_str());
        ImGui::PopStyleColor();
    } else {
        const auto others = problems_.size() - static_cast<std::size_t>(errors);
        widgets::muted(std::to_string(errors) + " problem(s) to fix before saving" +
                       (others > 0 ? ", " + std::to_string(others) + " warning(s)" : std::string()) + ".");
    }
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
    widgets::heading("Save");
    widgets::muted("Saving comes in a later stage of the C++ tool.");
}

} // namespace d012::bgm_maker
