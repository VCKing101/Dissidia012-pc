// The Battle BGM Maker's Music step: the mod's series and their tracks in
// one tree, files added by dropping them on a series or with Windows' Open
// dialog, and the selected series' or track's details beside it. The rules
// are the Python tool's (port/tools/bgm_maker/web/app.js): a new series' key
// follows its name and a new track's its title until typed or saved; moving
// a series renumbers the orders the game sorts by. MIT licence; see
// port/README.md.

#include "app.hpp"

#include "names.hpp"
#include "theme.hpp"
#include "widgets.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>

#include <algorithm>
#include <set>

namespace d012::bgm_maker {
namespace {

namespace manifest = d012::mod_manifest;

constexpr const char *kDot = "\xE2\x97\x8F";      // ●
constexpr const char *kUp = "\xE2\x96\xB2";       // ▲
constexpr const char *kDown = "\xE2\x96\xBC";     // ▼
constexpr const char *kEllipsis = "\xE2\x80\xA6"; // …
constexpr const char *kTrackPayload = "BGM_TRACK";

float em() { return ImGui::GetFontSize(); }

const char *loop_badge(const LoopModel &loop) {
    switch (loop.mode) {
    case LoopModel::Mode::Auto: return "auto";
    case LoopModel::Mode::Whole: return "whole";
    case LoopModel::Mode::None: return "once";
    case LoopModel::Mode::Custom: return "loop";
    }
    return "";
}

std::string series_label(const SeriesModel &series) {
    return series.name.empty() ? std::string("(no name)") : series.name;
}

std::string track_label(const TrackModel &track) {
    return track.title.empty() ? std::string("(no title)") : track.title;
}

std::string series_path(int s) { return "series." + std::to_string(s); }
std::string track_path(int s, int t) { return series_path(s) + ".tracks." + std::to_string(t); }

struct FilesSlot {
    std::mutex *mutex{};
    std::vector<std::filesystem::path> *files{};
    int *series{};
    bool *ready{};
    int asked_series{};
};

void files_chosen(void *userdata, const char *const *files, int) {
    auto *slot = static_cast<FilesSlot *>(userdata);
    if (files == nullptr || files[0] == nullptr) return;  // cancelled, or failed
    const std::lock_guard lock(*slot->mutex);
    for (const char *const *file = files; *file != nullptr; ++file)
        slot->files->push_back(manifest::path_from_utf8(*file));
    *slot->series = slot->asked_series;
    *slot->ready = true;
}

} // namespace

// --- Keys --------------------------------------------------------------------

std::string App::track_key_for(const std::string &title, int series, int track) const {
    // Unique in the mod (every series' tracks), and so is its number, also
    // against the other mods' tracks.
    const ModModel &model = open_->model;
    std::vector<std::string> taken;
    std::set<std::uint32_t> numbers;
    for (int s = 0; s < static_cast<int>(model.series.size()); ++s)
        for (int t = 0; t < static_cast<int>(model.series[s].tracks.size()); ++t) {
            if (s == series && t == track) continue;
            const std::string &key = model.series[s].tracks[t].key;
            taken.push_back(key);
            if (manifest::valid_key(model.id)) numbers.insert(manifest::id_number(model.id + ":" + key));
        }
    if (manifest::valid_key(model.id))
        for (const ModSummary &other : list_)
            if (open_->is_new || other.folder != open_->folder)
                for (const auto &[number, id] : other.tracks) numbers.insert(number);
    return names::unique_key(names::key_from_text(title, "track"), taken, numbers,
                             manifest::valid_key(model.id) ? std::string_view(model.id) : std::string_view());
}

std::string App::series_key_for(const std::string &name, int series) const {
    std::vector<std::string> taken;
    for (int s = 0; s < static_cast<int>(open_->model.series.size()); ++s)
        if (s != series) taken.push_back(open_->model.series[s].key);
    return names::unique_key(names::key_from_text(name, "series"), taken);
}

// --- Changing the tree -------------------------------------------------------

int App::add_series() {
    ModModel &model = open_->model;
    std::int64_t order = 0;
    for (const SeriesModel &series : model.series) order = std::max(order, series.order + 10);
    SeriesModel series;
    series.order = model.series.empty() ? 0 : order;
    series.auto_key = true;
    model.series.push_back(std::move(series));
    const int index = static_cast<int>(model.series.size()) - 1;
    model.series[index].key = series_key_for("series", index);
    selected_ = {index, -1};
    focus_series_name_ = true;
    return index;
}

void App::move_series(int from, int to) {
    auto &list = open_->model.series;
    if (from < 0 || to < 0 || from >= static_cast<int>(list.size()) || to >= static_cast<int>(list.size())) return;
    std::swap(list[from], list[to]);
    // The game sorts series by "order": renumber them as they now stand.
    for (std::size_t i = 0; i < list.size(); ++i) list[i].order = static_cast<std::int64_t>(i) * 10;
    if (selected_.series == from) selected_.series = to;
    else if (selected_.series == to) selected_.series = from;
}

void App::move_track(int series, int track, int to_series, int to_index) {
    auto &list = open_->model.series;
    if (series < 0 || series >= static_cast<int>(list.size()) || to_series < 0 ||
        to_series >= static_cast<int>(list.size()))
        return;
    auto &from = list[series].tracks;
    if (track < 0 || track >= static_cast<int>(from.size())) return;
    TrackModel moving = std::move(from[track]);
    from.erase(from.begin() + track);
    auto &to = list[to_series].tracks;
    if (to_series == series && to_index > track) --to_index;
    to_index = std::clamp(to_index, 0, static_cast<int>(to.size()));
    to.insert(to.begin() + to_index, std::move(moving));
    selected_ = {to_series, to_index};
}

void App::remove_series(int series) {
    auto &list = open_->model.series;
    if (series < 0 || series >= static_cast<int>(list.size())) return;
    const std::string name = series_label(list[series]);
    const std::size_t tracks = list[series].tracks.size();
    list.erase(list.begin() + series);
    selected_ = {list.empty() ? -1 : std::min(series, static_cast<int>(list.size()) - 1), -1};
    say("Removed the series " + name + (tracks > 0 ? " and its " + std::to_string(tracks) + " track(s)" : std::string()) +
        ". Ctrl+Z brings it back.");
}

void App::remove_track(int series, int track) {
    auto &tracks = open_->model.series[series].tracks;
    if (track < 0 || track >= static_cast<int>(tracks.size())) return;
    const std::string title = track_label(tracks[track]);
    tracks.erase(tracks.begin() + track);
    selected_ = tracks.empty() ? Selection{series, -1} : Selection{series, std::min(track, static_cast<int>(tracks.size()) - 1)};
    say("Removed " + title + " from the mod. Ctrl+Z brings it back.");
}

void App::sort_series() {
    auto &list = open_->model.series;
    const std::string selected_key = selected_.series >= 0 && selected_.series < static_cast<int>(list.size())
                                         ? list[selected_.series].key
                                         : std::string();
    std::stable_sort(list.begin(), list.end(), [](const SeriesModel &a, const SeriesModel &b) {
        return a.order != b.order ? a.order < b.order : a.key < b.key;
    });
    for (int s = 0; s < static_cast<int>(list.size()); ++s)
        if (list[s].key == selected_key && !selected_key.empty()) selected_.series = s;
}

void App::clamp_selection() {
    if (!open_) {
        selected_ = {};
        return;
    }
    const auto &list = open_->model.series;
    if (selected_.series >= static_cast<int>(list.size())) selected_ = {list.empty() ? -1 : static_cast<int>(list.size()) - 1, -1};
    if (selected_.series >= 0 && selected_.track >= static_cast<int>(list[selected_.series].tracks.size()))
        selected_.track = static_cast<int>(list[selected_.series].tracks.size()) - 1;
}

// --- Adding files ------------------------------------------------------------

void App::add_files(const std::vector<std::filesystem::path> &files, int series) {
    if (!open_) {
        say("Pick or make a mod on the Mod step first.", true);
        return;
    }
    ModModel &model = open_->model;
    if (series < 0 || series >= static_cast<int>(model.series.size())) {
        series = selected_.series >= 0 && selected_.series < static_cast<int>(model.series.size()) ? selected_.series : -1;
        if (series < 0) series = model.series.empty() ? add_series() : 0;
    }
    importer_.add(files, files_folder(), series, import_generation_);
}

void App::choose_files(int series) {
    static FilesSlot slot;
    slot = {&chosen_mutex_, &chosen_files_, &chosen_files_series_, &chosen_files_ready_, series};
    static const SDL_DialogFileFilter filters[] = {
        {"Audio files", "mp3;ogg;oga;opus;flac;wav;brstm;bcstm;bfstm"},
        {"All files", "*"},
    };
    SDL_ShowOpenFileDialog(files_chosen, &slot, window_.sdl_window(), filters, 2, nullptr, true);
}

void App::take_imports() {
    for (ImportResult &result : importer_.take()) {
        if (result.tag != import_generation_ || !open_) {
            // For a mod that is no longer open: its copy goes again.
            std::error_code ec;
            if (!result.copy.empty()) std::filesystem::remove(result.copy, ec);
            continue;
        }
        if (!result.error.empty()) {
            say("Not added: " + result.error, true);
            continue;
        }
        ModModel &model = open_->model;
        int series = result.series;
        if (series < 0 || series >= static_cast<int>(model.series.size()))
            series = model.series.empty() ? add_series() : static_cast<int>(model.series.size()) - 1;
        TrackModel track;
        track.title = result.title;
        track.file = result.file;
        track.auto_key = true;
        // A file with a loop of its own (loop tags, a stream header's loop)
        // plays it; any other loops whole until a loop is set.
        track.loop.mode = result.info.loop ? LoopModel::Mode::Auto : LoopModel::Mode::Whole;
        auto &tracks = model.series[series].tracks;
        tracks.push_back(std::move(track));
        const int index = static_cast<int>(tracks.size()) - 1;
        tracks[index].key = track_key_for(tracks[index].title, series, index);
        infos_[result.file] = result.info;
        ++infos_changed_;
        added_files_.push_back(result.copy);
        selected_ = {series, index};
        say("Added " + result.source + " as " + track_label(tracks[index]) + ".");
    }
}

void App::handle_drops() {
    for (Window::Drop &drop : window_.take_drops()) {
        int series = -1;
        if (tab_ == Tab::Music)
            for (const SeriesArea &area : series_areas_)
                if (drop.x >= area.min.x && drop.x < area.max.x && drop.y >= area.min.y && drop.y < area.max.y)
                    series = area.series;
        if (open_ && tab_ != Tab::Music) switch_to_ = Tab::Music;
        add_files(drop.files, series);
    }
}

const AudioInfo *App::info_for(const std::string &file) {
    if (file.empty() || !open_) return nullptr;
    auto found = infos_.find(file);
    if (found == infos_.end()) {
        const std::filesystem::path full = files_folder() / manifest::path_from_utf8(file);
        std::error_code ec;
        AudioInfo info;
        if (std::filesystem::is_regular_file(full, ec)) info = probe_audio(full);
        else info.error = "the file is not in the mod's folder";
        found = infos_.emplace(file, std::move(info)).first;
        ++infos_changed_;
    }
    return &found->second;
}

// --- The step ----------------------------------------------------------------

void App::tab_music() {
    if (!open_) {
        widgets::centred_note("Pick or make a mod on the Mod step first.");
        return;
    }
    clamp_selection();
    const float tree_width = em() * 24.0f;
    ImGui::BeginChild("##tree", ImVec2(tree_width, 0.0f), ImGuiChildFlags_Borders);
    if (ImGui::Button("+ Series")) add_series();
    widgets::tooltip("A new series: a source game, as the BGM choice lists it");
    ImGui::SameLine();
    if (ImGui::Button((std::string("Add files") + kEllipsis).c_str())) choose_files(selected_.series);
    widgets::tooltip("Add audio files to the selected series (they are copied into the mod's music folder)");
    ImGui::Separator();
    music_tree();
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("##item", ImVec2(0.0f, 0.0f), ImGuiChildFlags_None);
    const auto &list = open_->model.series;
    if (selected_.series >= 0 && selected_.series < static_cast<int>(list.size())) {
        if (selected_.track >= 0) track_form(selected_.series, selected_.track);
        else series_form(selected_.series);
    } else {
        widgets::centred_note(list.empty() ? "A mod's music comes in series, one for each game it is from. Add a series "
                                             "with + Series, then drop audio files on it."
                                           : "Pick a series or a track on the left.");
    }
    ImGui::EndChild();
}

void App::music_tree() {
    auto &list = open_->model.series;
    series_areas_.clear();
    const theme::Palette &colours = theme::palette();
    const bool dragging = window_.dragging_files();
    const ImVec2 drag(window_.drag_x(), window_.drag_y());
    if (list.empty()) widgets::muted("No series yet.");
    for (int s = 0; s < static_cast<int>(list.size()); ++s) {
        SeriesModel &series = list[s];
        ImGui::PushID(s);
        ImGui::BeginGroup();
        const ImVec2 group_start = ImGui::GetCursorScreenPos();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow |
                                   ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
        if (selected_.series == s && selected_.track < 0) flags |= ImGuiTreeNodeFlags_Selected;
        const bool open = ImGui::TreeNodeEx("##series", flags, "%s", series_label(series).c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) selected_ = {s, -1};
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(kTrackPayload)) {
                const int *from = static_cast<const int *>(payload->Data);
                pending_move_ = PendingMove{from[0], from[1], s, static_cast<int>(series.tracks.size())};
            }
            ImGui::EndDragDropTarget();
        }
        if (has_errors_below(series_path(s))) {
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Text, colours.error);
            ImGui::TextUnformatted(kDot);
            ImGui::PopStyleColor();
        }
        if (open) {
            for (int t = 0; t < static_cast<int>(series.tracks.size()); ++t) {
                const TrackModel &track = series.tracks[t];
                ImGui::PushID(t);
                const bool selected = selected_.series == s && selected_.track == t;
                const float badge = ImGui::CalcTextSize("whole").x + em() * 0.8f;
                const ImVec2 row = ImGui::GetCursorPos();
                if (ImGui::Selectable("##track", selected, ImGuiSelectableFlags_AllowOverlap)) selected_ = {s, t};
                if (ImGui::BeginDragDropSource()) {
                    const int payload[2] = {s, t};
                    ImGui::SetDragDropPayload(kTrackPayload, payload, sizeof payload);
                    ImGui::TextUnformatted(track_label(track).c_str());
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(kTrackPayload)) {
                        const int *from = static_cast<const int *>(payload->Data);
                        pending_move_ = PendingMove{from[0], from[1], s, t};
                    }
                    ImGui::EndDragDropTarget();
                }
                const float right = ImGui::GetContentRegionMax().x;
                ImGui::SetCursorPos(ImVec2(row.x + ImGui::GetStyle().FramePadding.x, row.y));
                const bool error = has_errors_below(track_path(s, t));
                if (error) {
                    ImGui::PushStyleColor(ImGuiCol_Text, colours.error);
                    ImGui::TextUnformatted(kDot);
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                }
                ImGui::PushClipRect(ImGui::GetCursorScreenPos(),
                                    ImVec2(ImGui::GetWindowPos().x + right - badge, ImGui::GetCursorScreenPos().y + em() * 2.0f),
                                    true);
                ImGui::TextUnformatted(track_label(track).c_str());
                ImGui::PopClipRect();
                ImGui::SameLine(right - badge);
                widgets::muted(loop_badge(track.loop));
                ImGui::PopID();
            }
            if (series.tracks.empty()) widgets::muted("   Drop audio files here");
            ImGui::TreePop();
        }
        ImGui::EndGroup();
        // The whole width of the tree, from the series' header to its last track.
        const SeriesArea area{s, group_start,
                              ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x,
                                     ImGui::GetItemRectMax().y)};
        series_areas_.push_back(area);
        if (dragging && drag.x >= area.min.x && drag.x < area.max.x && drag.y >= area.min.y && drag.y < area.max.y) {
            ImVec4 fill = colours.accent;
            fill.w = 0.15f;
            ImGui::GetWindowDrawList()->AddRectFilled(area.min, area.max, ImGui::GetColorU32(fill), 4.0f);
            ImGui::GetWindowDrawList()->AddRect(area.min, area.max, ImGui::GetColorU32(colours.accent), 4.0f, 0, 2.0f);
        }
        ImGui::PopID();
    }
    ImGui::Spacing();
    const std::string importing = importer_.status();
    if (!importing.empty()) {
        widgets::muted(importing + kEllipsis);
    } else if (!list.empty()) {
        ImGui::PushTextWrapPos(0.0f);
        widgets::muted(dragging ? "Drop on a series to add the files to it."
                                : "Drop audio files on a series, or use Add files. Drag tracks to reorder them or move "
                                  "them to another series.");
        ImGui::PopTextWrapPos();
    }
}

void App::series_form(int s) {
    SeriesModel &series = open_->model.series[s];
    const std::string path = series_path(s);
    widgets::heading("Series");
    widgets::problem_lines(problems_at(problems_, path));
    ImGui::Spacing();
    const float width = std::min(ImGui::GetContentRegionAvail().x, em() * 30.0f);
    ImGui::PushItemWidth(width);
    ImGui::PushID("series");

    if (focus_series_name_) {
        ImGui::SetKeyboardFocusHere();
        focus_series_name_ = false;
    }
    ImGui::TextUnformatted("Name");
    ImGui::SameLine();
    widgets::muted("as the BGM choice shows it, at most 48 bytes");
    ImGui::SetNextItemWidth(width);
    if (widgets::input_text("##name", series.name, 0, "FINAL FANTASY XV") && series.auto_key)
        series.key = series_key_for(series.name, s);
    if (series.name != names::upper_case(series.name)) {
        ImGui::SameLine();
        if (ImGui::Button("ABC")) {
            series.name = names::upper_case(series.name);
            if (series.auto_key) series.key = series_key_for(series.name, s);
        }
        widgets::tooltip("Upper case, as the game's own series names are");
    }
    widgets::problem_lines(problems_at(problems_, path + ".name"));

    if (widgets::text_field("Key", "##key", series.key, problems_at(problems_, path + ".key"),
                            series.auto_key ? "made from the name" : "players' saved choices refer to it",
                            nullptr, widgets::kKeyFlags))
        series.auto_key = false;

    ImGui::TextUnformatted("Short name (PSP)");
    ImGui::SameLine();
    widgets::muted("only for Make a PSP / PPSSPP ISO: tracks are named \"<short name> - <title>\"");
    ImGui::SetNextItemWidth(em() * 12.0f);
    widgets::input_text("##short", series.short_name, 0, default_short_name(series.name).c_str());
    widgets::problem_lines(problems_at(problems_, path + ".short"));

    ImGui::TextUnformatted("Order");
    ImGui::SameLine();
    widgets::muted("the game lists series by it, after its own");
    ImGui::SetNextItemWidth(em() * 8.0f);
    ImGui::InputScalar("##order", ImGuiDataType_S64, &series.order);
    if (ImGui::IsItemDeactivatedAfterEdit()) sort_series();
    widgets::problem_lines(problems_at(problems_, path + ".order"));
    ImGui::PopID();
    ImGui::PopItemWidth();

    // sort_series() may have moved it.
    s = selected_.series;
    ImGui::Spacing();
    const int count = static_cast<int>(open_->model.series.size());
    ImGui::BeginDisabled(s <= 0);
    if (ImGui::Button((std::string(kUp) + " Earlier").c_str())) move_series(s, s - 1);
    ImGui::EndDisabled();
    widgets::tooltip("Earlier in the game's list (renumbers the orders)");
    ImGui::SameLine();
    ImGui::BeginDisabled(s + 1 >= count);
    if (ImGui::Button((std::string(kDown) + " Later").c_str())) move_series(s, s + 1);
    ImGui::EndDisabled();
    widgets::tooltip("Later in the game's list (renumbers the orders)");
    ImGui::SameLine();
    if (ImGui::Button((std::string("Add files") + kEllipsis).c_str())) choose_files(s);
    ImGui::SameLine();
    if (ImGui::Button("Remove series")) pending_remove_ = PendingRemove{s, -1};
    widgets::tooltip("Takes the series and its tracks out of the mod (Ctrl+Z brings them back)");

    if (s >= 0 && s < count) {
        const std::size_t tracks = open_->model.series[s].tracks.size();
        ImGui::Spacing();
        widgets::muted(tracks == 0 ? std::string("No tracks yet: drop audio files on the series, or Add files.")
                                   : std::to_string(tracks) + " track(s)");
    }
}

void App::track_form(int s, int t) {
    ModModel &model = open_->model;
    TrackModel &track = model.series[s].tracks[t];
    const std::string path = track_path(s, t);
    widgets::heading("Track");
    widgets::problem_lines(problems_at(problems_, path));
    ImGui::Spacing();
    const float width = std::min(ImGui::GetContentRegionAvail().x, em() * 30.0f);
    ImGui::PushItemWidth(width);
    ImGui::PushID("track");

    if (widgets::text_field("Title", "##title", track.title, problems_at(problems_, path + ".title"),
                            "as the BGM choice shows it, at most 64 bytes") &&
        track.auto_key)
        track.key = track_key_for(track.title, s, t);
    if (widgets::text_field("Key", "##key", track.key, problems_at(problems_, path + ".key"),
                            track.auto_key ? "made from the title" : "players' saved choices refer to it", nullptr,
                            widgets::kKeyFlags))
        track.auto_key = false;

    ImGui::TextUnformatted("Series");
    ImGui::SetNextItemWidth(width);
    if (ImGui::BeginCombo("##series", series_label(model.series[s]).c_str())) {
        for (int other = 0; other < static_cast<int>(model.series.size()); ++other) {
            ImGui::PushID(other);
            if (ImGui::Selectable(series_label(model.series[other]).c_str(), other == s) && other != s)
                pending_move_ = PendingMove{s, t, other, static_cast<int>(model.series[other].tracks.size())};
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();
    ImGui::PopItemWidth();

    ImGui::Spacing();
    ImGui::TextUnformatted("File");
    ImGui::SameLine();
    widgets::muted(track.file.empty() ? std::string("none") : track.file);
    widgets::problem_lines(problems_at(problems_, path + ".file"));
    if (const AudioInfo *info = info_for(track.file)) {
        if (!info->error.empty()) {
            const Problem problem{Problem::Level::Error, path + ".file", "cannot be read: " + info->error, 0};
            widgets::problem_lines({&problem});
        } else {
            widgets::muted(describe(*info));
            std::string loop = std::string("Loop: ") + loop_badge(track.loop);
            if (track.loop.mode == LoopModel::Mode::Auto)
                loop += info->loop ? " (" + info->loop->source + ")" : std::string(" (no loop in the file: the whole file)");
            if (!info->stream_problem.empty()) loop += "; " + info->stream_problem;
            widgets::muted(loop + ". Set on the Loops step.");
        }
    }

    ImGui::Spacing();
    const int count = static_cast<int>(model.series[s].tracks.size());
    ImGui::BeginDisabled(t <= 0);
    if (ImGui::Button((std::string(kUp) + " Earlier").c_str())) pending_move_ = PendingMove{s, t, s, t - 1};
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::BeginDisabled(t + 1 >= count);
    if (ImGui::Button((std::string(kDown) + " Later").c_str())) pending_move_ = PendingMove{s, t, s, t + 2};
    ImGui::EndDisabled();
    widgets::tooltip("The order of the tracks in their series");
    ImGui::SameLine();
    if (ImGui::Button("Remove track")) pending_remove_ = PendingRemove{s, t};
    widgets::tooltip("Takes the track out of the mod (Ctrl+Z brings it back)");
}

} // namespace d012::bgm_maker
