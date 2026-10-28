#pragma once

// The Battle BGM Maker's interface: four step-tabs (Mod, Music, Loops,
// Check & save) that can be taken in order with Back and Next or clicked at
// any time, each showing whether its step is done. MIT licence; see
// port/README.md.

#include "checks.hpp"
#include "model.hpp"
#include "mods_folder.hpp"
#include "undo.hpp"
#include "window.hpp"

#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace d012::bgm_maker {

class App {
public:
    App(Window &window, ModsFolder mods);

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

    void refresh_list();
    void open_mod(const ModSummary &summary);
    void new_mod();
    void set_mods_folder(const std::filesystem::path &folder);
    // Runs `action` now, or once the player has said what to do with unsaved changes.
    void after_unsaved_check(std::function<void()> action);
    [[nodiscard]] bool dirty() const;
    [[nodiscard]] std::filesystem::path mod_folder() const;
    void update_checks();
    void undo();
    void redo();

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

    [[nodiscard]] int tab_problems(Tab tab) const;
    [[nodiscard]] bool tab_done(Tab tab) const;
    [[nodiscard]] static Tab tab_of(const Problem &problem);
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

    // The folder dialog answers on its own thread.
    std::mutex chosen_mutex_;
    std::optional<std::filesystem::path> chosen_folder_;
};

} // namespace d012::bgm_maker
