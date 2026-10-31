#pragma once

// The Battle BGM Maker's window: SDL3, a Vulkan swapchain and Dear ImGui with
// its SDL3 and Vulkan back ends (PortableKit's copy, third_party/imgui). It
// draws only while something changes, so an idle window costs nothing. MIT
// licence; see port/README.md.

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

struct SDL_Window;

namespace d012::bgm_maker {

class Window {
public:
    Window() = default;
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    ~Window();

    // Opens the window and ImGui on it; false with `error` when it cannot.
    bool open(const std::string &title, std::string &error);

    // Runs frames until `frame` returns false. `frame` builds the interface;
    // the window's close button only sets close_requested(), so the caller
    // decides (unsaved changes).
    void run(const std::function<bool()> &frame);

    [[nodiscard]] bool close_requested() const noexcept { return close_requested_; }
    void clear_close_request() noexcept { close_requested_ = false; }

    // Files dropped on the window since the last call, and where (window
    // coordinates, as ImGui's).
    struct Drop {
        std::vector<std::filesystem::path> files;
        float x{};
        float y{};
    };
    std::vector<Drop> take_drops();
    // Files being dragged over the window now, and where.
    [[nodiscard]] bool dragging_files() const noexcept { return dragging_; }
    [[nodiscard]] float drag_x() const noexcept { return drag_x_; }
    [[nodiscard]] float drag_y() const noexcept { return drag_y_; }

    // Keep drawing every frame (playback, a progress bar) rather than only on input.
    void set_animating(bool animating) noexcept { animating_ = animating; }

    void set_title(const std::string &title);

    // Test switches (main.cpp): the theme instead of the system's; the window
    // saved as a PNG after `frames` frames, then the run ends.
    void force_theme(bool dark) noexcept { forced_dark_ = dark ? 1 : 0; }
    void set_script(class InputScript *script) noexcept { script_ = script; }
    void capture_after(int frames, std::filesystem::path file) {
        capture_frames_ = frames;
        capture_file_ = std::move(file);
    }

    [[nodiscard]] SDL_Window *sdl_window() const noexcept { return window_; }
    // Dark when the system's apps are dark.
    [[nodiscard]] bool dark() const noexcept { return dark_; }
    // The display's scale (1 at 96 dpi).
    [[nodiscard]] float scale() const noexcept { return scale_; }

private:
    struct Vulkan;

    bool init_vulkan(std::string &error);
    void rebuild_swapchain();
    void render(bool &swapchain_stale, bool capture);
    void save_capture();
    [[nodiscard]] bool look_dark() const;
    void apply_look();
    void close();

    int forced_dark_{-1};  // -1: the system's
    class InputScript *script_{};
    int capture_frames_{-1};
    std::filesystem::path capture_file_;

    SDL_Window *window_{};
    Vulkan *vk_{};
    bool imgui_sdl_{};
    bool imgui_vulkan_{};
    bool close_requested_{};
    bool animating_{};
    bool dark_{};
    float scale_{1.0f};
    std::vector<Drop> drops_;
    Drop dropping_;  // the drop being received (one event per file)
    bool dragging_{};
    float drag_x_{};
    float drag_y_{};
};

} // namespace d012::bgm_maker
