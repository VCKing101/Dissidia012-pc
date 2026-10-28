#pragma once

// Small pieces of the Battle BGM Maker's interface: text fields on
// std::string, a field with its label above and its problems below, notes.
// MIT licence; see port/README.md.

#include "checks.hpp"

#include <imgui.h>

#include <string>
#include <vector>

namespace d012::bgm_maker::widgets {

// With kKeyFlags, the field takes only what an id or key may hold, capitals
// turned into small letters as they are typed.
inline constexpr ImGuiInputTextFlags kKeyFlags = ImGuiInputTextFlags_CallbackCharFilter;

bool input_text(const char *id, std::string &text, ImGuiInputTextFlags flags = 0, const char *hint = nullptr);
bool input_multiline(const char *id, std::string &text, const ImVec2 &size, ImGuiInputTextFlags flags = 0);

// A field as the forms lay it out: the label (and a muted help text) above,
// the input across the column, the problems found at its path below. True
// when the text changed; `left` is set when the field was just left.
bool text_field(const char *label, const char *id, std::string &text, const std::vector<const Problem *> &problems,
                const char *help = nullptr, const char *hint = nullptr, ImGuiInputTextFlags flags = 0,
                bool *left = nullptr);

// Problems, one per line, coloured by level.
void problem_lines(const std::vector<const Problem *> &problems);

void muted(const std::string &text);
void heading(const std::string &text);

// A note on an empty part of the window, centred in the space left.
void centred_note(const std::string &text);

// Hovering the last item shows `text`.
void tooltip(const char *text);

} // namespace d012::bgm_maker::widgets
