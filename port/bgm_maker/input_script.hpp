#pragma once

// Scripted input for the Battle BGM Maker's own checks (D012_BGM_MAKER_SCRIPT):
// a text file of steps fed to Dear ImGui as if typed and clicked, with
// captures of the window between them. One step per line, # for comments:
//
//   frames N          wait N frames
//   move X Y          the pointer to X, Y (window pixels)
//   click X Y         a left click there (pressed one frame, released the next)
//   type TEXT         TEXT typed, as characters
//   key CHORD         a key, with modifiers: "ctrl+z", "shift+tab", "enter", "escape", "backspace"
//   close             the window's close button
//   capture FILE.png  the window as it is after this frame
//   quit              ends the run
//
// MIT licence; see port/README.md.

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace d012::bgm_maker {

class InputScript {
public:
    // False with `error` when the file cannot be read or has a step it does not know.
    bool load(const std::filesystem::path &file, std::string &error);

    // Feeds this frame's input to ImGui; before ImGui::NewFrame.
    void before_frame();

    // After the frame: a file to capture it to, if the script asks.
    std::optional<std::filesystem::path> take_capture();
    [[nodiscard]] bool close_requested();
    [[nodiscard]] bool finished() const noexcept { return finished_; }
    [[nodiscard]] bool active() const noexcept { return !steps_.empty() && !finished_; }

private:
    struct Step {
        std::string verb;
        std::string text;
        float x{};
        float y{};
        int count{};
    };
    std::vector<Step> steps_;
    std::size_t next_{};
    int wait_{};
    bool release_{};
    bool close_{};
    bool finished_{};
    std::optional<std::filesystem::path> capture_;
};

} // namespace d012::bgm_maker
