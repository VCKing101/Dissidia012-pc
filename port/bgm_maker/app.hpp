#pragma once

// The Battle BGM Maker's interface: four step-tabs (Mod, Music, Loops,
// Check & save) that can be taken in order with Back and Next or clicked at
// any time, each showing whether its step is done. MIT licence; see
// port/README.md.

#include "audio_file.hpp"
#include "checks.hpp"
#include "decoder.hpp"
#include "importer.hpp"
#include "player.hpp"
#include "suggest.hpp"
#include "waveform.hpp"
#include "model.hpp"
#include "mods_folder.hpp"
#include "undo.hpp"
#include "window.hpp"

#include <imgui.h>

#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace d012::bgm_maker {

class App {
public:
    App(Window &window, ModsFolder mods);
    ~App();

    // Builds one frame; false once the window should close.
    bool frame();

    // Test switches (main.cpp): open a mod by its folder ("+new": a new one),
    // start on a step (1 to 4).
    void test_open(const std::string &folder);
    void test_tab(int step);

private:
    enum class Tab { Mod, Music, Loops, Check };
    static constexpr int kTabs = 4;

    // The mod being edited.
    struct Open {
        std::string folder;  // its folder's name in the mods folder
        bool is_new{};       // not on disc yet: saving makes the folder
        bool folder_follows_id{true};
        ModModel model;
        ModModel saved;      // as on disc (empty for a new mod)
        std::string saved_text;
        std::string open_problem;  // why mod.json could not be read, when it could not
    };

    // What the Music step has selected: a series, or a track in it.
    struct Selection {
        int series{-1};
        int track{-1};
    };

    void refresh_list();
    void open_mod(const ModSummary &summary);
    void new_mod();
    void set_mods_folder(const std::filesystem::path &folder);
    // Runs `action` now, or once the player has said what to do with unsaved
    // changes; either way the mod open now is left first.
    void after_unsaved_check(std::function<void()> action);
    // Leaves the open mod: files copied in for it that its saved manifest
    // does not use are deleted again (a new mod's, all of them).
    void leave_mod();
    [[nodiscard]] bool dirty() const;
    [[nodiscard]] std::filesystem::path mod_folder() const;
    // Where the open mod's files are: its folder, or for a new mod, until it
    // is saved, a folder of its own in the temporary folder.
    [[nodiscard]] std::filesystem::path files_folder();
    void update_checks();
    void undo();
    void redo();
    void say(std::string text, bool error = false);

    void draw_top_bar();
    void draw_tabs();
    void draw_navigation();
    void draw_unsaved_prompt();
    void tab_mod();
    void tab_music();
    void tab_loops();
    void tab_check();
    void mod_list();
    void mod_form();
    void choose_mods_folder();

    // The Music step (tab_music.cpp).
    void music_tree();
    void series_form(int series);
    void track_form(int series, int track);
    void handle_drops();
    void take_imports();
    void add_files(const std::vector<std::filesystem::path> &files, int series);
    void choose_files(int series);
    int add_series();
    void move_series(int from, int to);
    void move_track(int series, int track, int to_series, int to_index);
    void remove_series(int series);
    void remove_track(int series, int track);
    void sort_series();
    void clamp_selection();
    [[nodiscard]] std::string track_key_for(const std::string &title, int series, int track) const;
    [[nodiscard]] std::string series_key_for(const std::string &name, int series) const;
    const AudioInfo *info_for(const std::string &file);

    // The Loops step (tab_loops.cpp).
    void loops_list();
    void loop_editor(int series, int track);
    void loop_mode_row(TrackModel &track, const DecodedTrack &audio, const AudioInfo *info);
    void loop_transport(TrackModel &track, const DecodedTrack &audio);
    void loop_markers(TrackModel &track, const DecodedTrack &audio);
    void loop_seam(const TrackModel &track, const DecodedTrack &audio);
    void loop_suggestions(TrackModel &track, std::shared_ptr<const DecodedTrack> audio);
    void loop_keys(TrackModel &track, const DecodedTrack &audio);
    [[nodiscard]] LoopShown shown_loop(const TrackModel &track, const DecodedTrack &audio);
    void set_loop(TrackModel &track, std::int64_t start, std::int64_t end, std::int64_t frames);
    void set_marker(TrackModel &track, const DecodedTrack &audio, Marker which, std::int64_t sample);
    void ensure_custom(TrackModel &track, const DecodedTrack &audio);
    void nudge(TrackModel &track, const DecodedTrack &audio, Marker which, int direction, int times);
    void snap_zero(TrackModel &track, const DecodedTrack &audio, Marker which);
    void toggle_play(const TrackModel &track, std::shared_ptr<const DecodedTrack> audio);
    void play_from(const TrackModel &track, std::shared_ptr<const DecodedTrack> audio, std::int64_t from);
    // The seam of the track's loop, or of `points` (a suggestion's start and end).
    void audition(const TrackModel &track, std::shared_ptr<const DecodedTrack> audio,
                  std::optional<std::pair<std::int64_t, std::int64_t>> points = std::nullopt);
    [[nodiscard]] std::int64_t here();
    [[nodiscard]] std::int64_t step_samples(int rate) const;
    [[nodiscard]] std::filesystem::path track_path_on_disk(const TrackModel &track);

    [[nodiscard]] int tab_problems(Tab tab) const;
    [[nodiscard]] bool tab_done(Tab tab) const;
    [[nodiscard]] static Tab tab_of(const Problem &problem);
    [[nodiscard]] bool has_errors_below(const std::string &path) const;
    void update_title();

    Window &window_;
    ModsFolder mods_;
    std::vector<ModSummary> list_;
    std::optional<Open> open_;
    UndoStack<ModModel> undo_;
    std::vector<Problem> problems_;
    std::optional<ModModel> checked_;  // the model problems_ are for
    std::string checked_folder_;

    Tab tab_{Tab::Mod};
    std::optional<Tab> switch_to_;
    std::function<void()> pending_;  // waiting on the unsaved-changes prompt
    bool prompt_{};
    bool quit_{};
    bool focus_id_{};
    // A new mod's fields show their problems once left, or once the player
    // has looked at Check & save: a form just opened is not full of errors.
    std::vector<std::string> touched_;
    bool show_all_problems_{};
    std::string title_;

    // The Music step.
    Selection selected_;
    Importer importer_;
    int import_generation_{};  // results of an earlier mod's imports are dropped
    std::map<std::string, AudioInfo> infos_;  // by the file's text in the manifest
    std::vector<std::filesystem::path> added_files_;  // copied in since the mod was opened
    std::filesystem::path staging_;
    struct SeriesArea {
        int series{};
        ImVec2 min;
        ImVec2 max;
    };
    std::vector<SeriesArea> series_areas_;  // where each series is in the tree, for drops
    struct PendingMove {
        int series{}, track{}, to_series{}, to_index{};
    };
    std::optional<PendingMove> pending_move_;  // moves and removals wait for the frame's end
    struct PendingRemove {
        int series{}, track{-1};  // track -1: the series
    };
    std::optional<PendingRemove> pending_remove_;
    bool focus_series_name_{};

    // The Loops step.
    Decoder decoder_;
    Player player_;
    WaveView wave_;
    std::filesystem::path wave_file_;  // the track the view was last reset for
    Marker active_marker_{Marker::Start};
    int step_choice_{2};     // 1 ms
    double bpm_{120.0};
    int pre_choice_{2};      // 3 s
    int post_choice_{1};     // 4 s
    bool repeat_{};
    int seam_choice_{1};     // 10 ms
    std::string start_samples_, start_time_, end_samples_, end_time_;
    Suggester suggester_;
    std::vector<LoopCandidate> candidates_;  // for wave_file_
    std::string suggest_status_;
    double min_loop_{8.0};

    std::string status_;
    bool status_error_{};
    std::chrono::steady_clock::time_point status_until_{};

    // The file dialogs answer on their own thread.
    std::mutex chosen_mutex_;
    std::optional<std::filesystem::path> chosen_folder_;
    std::vector<std::filesystem::path> chosen_files_;
    int chosen_files_series_{-1};
    bool chosen_files_ready_{};
};

} // namespace d012::bgm_maker
