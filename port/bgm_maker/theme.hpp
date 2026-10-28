#pragma once

// The Battle BGM Maker's look: a light and a dark theme following the
// system, Windows' own fonts (Segoe UI, with symbols and CJK faces merged in
// for titles in other scripts), sizes scaled to the display. MIT licence; see
// port/README.md.

#include <imgui.h>

namespace d012::bgm_maker::theme {

// Colours the interface uses beyond ImGui's own, for the current theme.
struct Palette {
    ImVec4 text_muted;
    ImVec4 error;
    ImVec4 warning;
    ImVec4 good;
    ImVec4 accent;
    ImVec4 panel;  // a slightly set-off background
};

// Loads the fonts once, after ImGui's context exists.
void load_fonts();

// Sets the style for `dark` at `scale`; again whenever either changes.
void apply(bool dark, float scale);

[[nodiscard]] const Palette &palette();
[[nodiscard]] ImFont *heading_font();  // bold, or the text font
[[nodiscard]] ImFont *mono_font();     // for samples and times, or the text font
[[nodiscard]] float heading_size();    // in the style's units (FontSizeBase)

} // namespace d012::bgm_maker::theme
