// Small pieces of the Battle BGM Maker's interface; see widgets.hpp. MIT
// licence; see port/README.md.

#include "widgets.hpp"

#include "theme.hpp"

#include <algorithm>

namespace d012::bgm_maker::widgets {
namespace {

int resize_callback(ImGuiInputTextCallbackData *data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        auto *text = static_cast<std::string *>(data->UserData);
        text->resize(static_cast<std::size_t>(data->BufTextLen));
        data->Buf = text->data();
    }
    // An id or key: capitals become small letters, and what the game does not
    // take in a key is not typed at all.
    if (data->EventFlag == ImGuiInputTextFlags_CallbackCharFilter) {
        ImWchar &c = data->EventChar;
        if (c >= 'A' && c <= 'Z') c = static_cast<ImWchar>(c - 'A' + 'a');
        const bool allowed = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
        return allowed ? 0 : 1;
    }
    return 0;
}

} // namespace

bool input_text(const char *id, std::string &text, ImGuiInputTextFlags flags, const char *hint) {
    flags |= ImGuiInputTextFlags_CallbackResize;
    if (hint != nullptr)
        return ImGui::InputTextWithHint(id, hint, text.data(), text.capacity() + 1u, flags, resize_callback, &text);
    return ImGui::InputText(id, text.data(), text.capacity() + 1u, flags, resize_callback, &text);
}

bool input_multiline(const char *id, std::string &text, const ImVec2 &size, ImGuiInputTextFlags flags) {
    flags |= ImGuiInputTextFlags_CallbackResize;
    return ImGui::InputTextMultiline(id, text.data(), text.capacity() + 1u, size, flags, resize_callback, &text);
}

bool text_field(const char *label, const char *id, std::string &text, const std::vector<const Problem *> &problems,
                const char *help, const char *hint, ImGuiInputTextFlags flags, bool *left) {
    ImGui::TextUnformatted(label);
    if (help != nullptr) {
        ImGui::SameLine();
        muted(help);
    }
    ImGui::SetNextItemWidth(ImGui::CalcItemWidth());
    const bool changed = input_text(id, text, flags, hint);
    if (left != nullptr) *left = ImGui::IsItemDeactivated();
    problem_lines(problems);
    return changed;
}

void problem_lines(const std::vector<const Problem *> &problems) {
    const theme::Palette &colours = theme::palette();
    for (const Problem *problem : problems) {
        const bool error = problem->level == Problem::Level::Error;
        const bool warning = problem->level == Problem::Level::Warning;
        ImGui::PushStyleColor(ImGuiCol_Text, error ? colours.error : warning ? colours.warning : colours.text_muted);
        ImGui::PushTextWrapPos(0.0f);
        ImGui::TextUnformatted((std::string(error ? "\xE2\x9C\x95 " : warning ? "\xE2\x9A\xA0 " : "\xE2\x84\xB9 ") +
                                problem->message)
                                   .c_str());
        ImGui::PopTextWrapPos();
        ImGui::PopStyleColor();
    }
}

void muted(const std::string &text) {
    ImGui::PushStyleColor(ImGuiCol_Text, theme::palette().text_muted);
    ImGui::TextUnformatted(text.c_str());
    ImGui::PopStyleColor();
}

void heading(const std::string &text) {
    ImGui::PushFont(theme::heading_font(), theme::heading_size());
    ImGui::TextUnformatted(text.c_str());
    ImGui::PopFont();
}

void centred_note(const std::string &text) {
    const ImVec2 space = ImGui::GetContentRegionAvail();
    const float width = std::min(space.x, ImGui::GetFontSize() * 28.0f);
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (space.x - width) * 0.5f, ImGui::GetCursorPosY() + space.y * 0.35f));
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + width);
    ImGui::PushStyleColor(ImGuiCol_Text, theme::palette().text_muted);
    ImGui::TextUnformatted(text.c_str());
    ImGui::PopStyleColor();
    ImGui::PopTextWrapPos();
}

void tooltip(const char *text) {
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("%s", text);
}

} // namespace d012::bgm_maker::widgets
