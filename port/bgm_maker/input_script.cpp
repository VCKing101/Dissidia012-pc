// Scripted input for the Battle BGM Maker's own checks; see input_script.hpp.
// MIT licence; see port/README.md.

#include "input_script.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>

namespace d012::bgm_maker {
namespace {

std::optional<ImGuiKey> key_named(const std::string &name) {
    static const std::map<std::string, ImGuiKey> keys = {
        {"enter", ImGuiKey_Enter},   {"escape", ImGuiKey_Escape}, {"tab", ImGuiKey_Tab},
        {"backspace", ImGuiKey_Backspace}, {"delete", ImGuiKey_Delete}, {"space", ImGuiKey_Space},
        {"left", ImGuiKey_LeftArrow}, {"right", ImGuiKey_RightArrow}, {"up", ImGuiKey_UpArrow},
        {"down", ImGuiKey_DownArrow}, {"home", ImGuiKey_Home},     {"end", ImGuiKey_End},
        {"equal", ImGuiKey_Equal},   {"minus", ImGuiKey_Minus},   {"leftbracket", ImGuiKey_LeftBracket},
        {"rightbracket", ImGuiKey_RightBracket},
    };
    if (const auto found = keys.find(name); found != keys.end()) return found->second;
    if (name.size() == 1 && name[0] >= 'a' && name[0] <= 'z') return static_cast<ImGuiKey>(ImGuiKey_A + (name[0] - 'a'));
    if (name.size() == 1 && name[0] >= '0' && name[0] <= '9') return static_cast<ImGuiKey>(ImGuiKey_0 + (name[0] - '0'));
    return std::nullopt;
}

} // namespace

bool InputScript::load(const std::filesystem::path &file, std::string &error) {
    std::ifstream in(file);
    if (!in) {
        error = "cannot read the input script";
        return false;
    }
    std::string line;
    int number = 0;
    while (std::getline(in, line)) {
        ++number;
        if (number == 1 && line.starts_with("\xEF\xBB\xBF")) line.erase(0, 3);  // a UTF-8 byte order mark
        if (!line.empty() && line.back() == '\r') line.pop_back();
        const auto first = line.find_first_not_of(" \t");
        if (first == std::string::npos || line[first] == '#') continue;
        std::istringstream words(line.substr(first));
        Step step;
        words >> step.verb;
        if (step.verb == "frames" || step.verb == "ms") {
            words >> step.count;
        } else if (step.verb == "move" || step.verb == "click" || step.verb == "down" || step.verb == "up") {
            words >> step.x >> step.y;
        } else if (step.verb == "drop") {
            words >> step.x >> step.y;
            std::getline(words, step.text);
            if (!step.text.empty() && step.text.front() == ' ') step.text.erase(0, 1);
        } else if (step.verb == "type" || step.verb == "key" || step.verb == "capture") {
            std::getline(words, step.text);
            if (!step.text.empty() && step.text.front() == ' ') step.text.erase(0, 1);
            if (step.verb == "key") {
                std::string chord = step.text;
                for (const char *modifier : {"ctrl+", "shift+", "alt+"})
                    if (const auto at = chord.find(modifier); at != std::string::npos) chord.erase(at, std::string(modifier).size());
                if (!key_named(chord)) {
                    error = "line " + std::to_string(number) + ": no key \"" + chord + "\"";
                    return false;
                }
            }
        } else if (step.verb != "close" && step.verb != "quit") {
            error = "line " + std::to_string(number) + ": no step \"" + step.verb + "\"";
            return false;
        }
        steps_.push_back(std::move(step));
    }
    return true;
}

void InputScript::before_frame() {
    ImGuiIO &io = ImGui::GetIO();
    // The SDL back end gives ImGui the real pointer every frame; the script's
    // stays where the script put it, or a drag would jump.
    if (has_pointer_) io.AddMousePosEvent(pointer_x_, pointer_y_);
    // A real pointer is over an item a frame before it presses it, and ImGui
    // counts on that (an item that allows overlap is not hovered on the frame
    // the pointer arrives): the press comes the frame after the move.
    if (press_ >= 0) {
        io.AddMouseButtonEvent(0, press_ == 1);
        release_ = click_;
        press_ = -1;
        click_ = false;
        return;
    }
    if (release_) {
        io.AddMouseButtonEvent(0, false);
        release_ = false;
        return;  // one frame for the release, as a real click has
    }
    if (wait_ > 0) {
        --wait_;
        return;
    }
    if (wait_until_ > 0) {
        if (SDL_GetTicks() < wait_until_) return;
        wait_until_ = 0;
    }
    // Steps up to the next one that needs a frame of its own.
    while (next_ < steps_.size()) {
        const Step &step = steps_[next_++];
        if (step.verb == "move" || step.verb == "click" || step.verb == "down" || step.verb == "up") {
            pointer_x_ = step.x;
            pointer_y_ = step.y;
            has_pointer_ = true;
        }
        if (step.verb == "frames") {
            wait_ = step.count;
            return;
        }
        if (step.verb == "ms") {
            wait_until_ = SDL_GetTicks() + static_cast<Uint64>(std::max(step.count, 1));
            return;
        }
        if (step.verb == "move") {
            io.AddMousePosEvent(step.x, step.y);
            return;
        }
        if (step.verb == "click" || step.verb == "down" || step.verb == "up") {
            io.AddMousePosEvent(step.x, step.y);
            press_ = step.verb == "up" ? 0 : 1;
            click_ = step.verb == "click";
            return;
        }
        if (step.verb == "type") {
            io.AddInputCharactersUTF8(step.text.c_str());
            return;
        }
        if (step.verb == "key") {
            const std::string &chord = step.text;
            const bool ctrl = chord.find("ctrl+") != std::string::npos;
            const bool shift = chord.find("shift+") != std::string::npos;
            const bool alt = chord.find("alt+") != std::string::npos;
            const std::string name = chord.substr(chord.rfind('+') == std::string::npos ? 0 : chord.rfind('+') + 1);
            const ImGuiKey key = *key_named(name);
            io.AddKeyEvent(ImGuiMod_Ctrl, ctrl);
            io.AddKeyEvent(ImGuiMod_Shift, shift);
            io.AddKeyEvent(ImGuiMod_Alt, alt);
            io.AddKeyEvent(key, true);
            io.AddKeyEvent(key, false);
            io.AddKeyEvent(ImGuiMod_Ctrl, false);
            io.AddKeyEvent(ImGuiMod_Shift, false);
            io.AddKeyEvent(ImGuiMod_Alt, false);
            return;
        }
        if (step.verb == "close") {
            close_ = true;
            return;
        }
        if (step.verb == "drop") {
            drop_ = Dropped{std::filesystem::path(std::u8string(step.text.begin(), step.text.end())), step.x, step.y};
            return;
        }
        if (step.verb == "capture") {
            capture_ = std::filesystem::path(std::u8string(step.text.begin(), step.text.end()));
            return;
        }
        if (step.verb == "quit") break;
    }
    finished_ = true;
}

std::optional<std::filesystem::path> InputScript::take_capture() {
    auto file = std::move(capture_);
    capture_.reset();
    return file;
}

std::optional<InputScript::Dropped> InputScript::take_drop() {
    auto drop = std::move(drop_);
    drop_.reset();
    return drop;
}

bool InputScript::close_requested() {
    const bool close = close_;
    close_ = false;
    return close;
}

} // namespace d012::bgm_maker
