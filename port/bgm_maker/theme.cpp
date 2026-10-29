// The Battle BGM Maker's look; see theme.hpp. MIT licence; see
// port/README.md.

#include "theme.hpp"

#include <cstdlib>
#include <filesystem>
#include <initializer_list>
#include <string>

namespace d012::bgm_maker::theme {
namespace {

constexpr float kTextSize = 17.0f;
constexpr float kHeadingSize = 22.0f;

Palette g_palette;
ImFont *g_text{};
ImFont *g_heading{};
ImFont *g_mono{};

std::filesystem::path fonts_folder() {
    const char *windows = std::getenv("WINDIR");
    return std::filesystem::path(windows != nullptr && *windows != '\0' ? windows : "C:\\Windows") / "Fonts";
}

// The first of `names` in Windows' fonts folder, as UTF-8 for ImGui.
std::string first_font(std::initializer_list<const char *> names) {
    const std::filesystem::path folder = fonts_folder();
    std::error_code ec;
    for (const char *name : names) {
        const std::filesystem::path path = folder / name;
        if (std::filesystem::is_regular_file(path, ec)) {
            const std::u8string text = path.u8string();
            return {text.begin(), text.end()};
        }
    }
    return {};
}

// `primary` with the faces that fill its gaps merged in: symbols (✓ ▲ ▼ ●),
// then Japanese, Chinese and Korean, for track titles in those scripts.
ImFont *add_font(const std::string &primary) {
    ImGuiIO &io = ImGui::GetIO();
    if (primary.empty()) return nullptr;
    ImFont *font = io.Fonts->AddFontFromFileTTF(primary.c_str(), 0.0f);
    if (font == nullptr) return nullptr;
    for (const std::string &extra : {first_font({"seguisym.ttf"}), first_font({"YuGothM.ttc", "meiryo.ttc", "msgothic.ttc"}),
                                     first_font({"msyh.ttc", "simsun.ttc"}), first_font({"malgun.ttf"})}) {
        if (extra.empty()) continue;
        ImFontConfig merge;
        merge.MergeMode = true;
        io.Fonts->AddFontFromFileTTF(extra.c_str(), 0.0f, &merge);
    }
    return font;
}

ImVec4 rgb(int r, int g, int b, float a = 1.0f) { return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a); }

void dark_colours(ImGuiStyle &style) {
    ImGui::StyleColorsDark(&style);
    ImVec4 *c = style.Colors;
    c[ImGuiCol_WindowBg] = rgb(30, 31, 34);
    c[ImGuiCol_ChildBg] = rgb(30, 31, 34);
    c[ImGuiCol_PopupBg] = rgb(40, 42, 46);
    c[ImGuiCol_Border] = rgb(62, 65, 71);
    c[ImGuiCol_FrameBg] = rgb(45, 47, 52);
    c[ImGuiCol_FrameBgHovered] = rgb(55, 58, 64);
    c[ImGuiCol_FrameBgActive] = rgb(62, 66, 73);
    c[ImGuiCol_TitleBg] = rgb(30, 31, 34);
    c[ImGuiCol_TitleBgActive] = rgb(36, 38, 42);
    c[ImGuiCol_Button] = rgb(52, 55, 61);
    c[ImGuiCol_ButtonHovered] = rgb(64, 68, 76);
    c[ImGuiCol_ButtonActive] = rgb(53, 116, 196);
    c[ImGuiCol_Header] = rgb(46, 74, 110);
    c[ImGuiCol_HeaderHovered] = rgb(52, 86, 130);
    c[ImGuiCol_HeaderActive] = rgb(53, 116, 196);
    c[ImGuiCol_Tab] = rgb(40, 42, 46);
    c[ImGuiCol_TabHovered] = rgb(58, 92, 138);
    c[ImGuiCol_TabSelected] = rgb(46, 80, 124);
    c[ImGuiCol_TabSelectedOverline] = rgb(86, 156, 240);
    c[ImGuiCol_Separator] = rgb(62, 65, 71);
    c[ImGuiCol_CheckMark] = rgb(110, 170, 245);
    c[ImGuiCol_SliderGrab] = rgb(86, 156, 240);
    c[ImGuiCol_Text] = rgb(225, 227, 231);
    c[ImGuiCol_TextDisabled] = rgb(130, 134, 142);
    g_palette = {rgb(150, 155, 164), rgb(240, 113, 103), rgb(230, 180, 80), rgb(110, 200, 130), rgb(110, 170, 245),
                 rgb(37, 38, 42)};
    // The Python tool's waveform colours.
    g_palette.wave_bg = rgb(13, 15, 18);
    g_palette.ruler_bg = rgb(20, 23, 28);
    g_palette.ruler_text = rgb(125, 133, 148);
    g_palette.grid = rgb(28, 32, 40);
    g_palette.wave = rgb(95, 168, 211);
    g_palette.wave_second = rgb(211, 154, 95);
    g_palette.wave_dim = rgb(47, 80, 104);
    g_palette.ghost = rgb(220, 220, 220, 0.28f);
    g_palette.region = rgb(76, 208, 125, 0.07f);
    g_palette.outside = rgb(0, 0, 0, 0.35f);
    g_palette.start = rgb(76, 208, 125);
    g_palette.end = rgb(255, 107, 107);
    g_palette.cursor = rgb(224, 168, 79);
    g_palette.playhead = rgb(255, 255, 255);
}

void light_colours(ImGuiStyle &style) {
    ImGui::StyleColorsLight(&style);
    ImVec4 *c = style.Colors;
    c[ImGuiCol_WindowBg] = rgb(248, 248, 250);
    c[ImGuiCol_ChildBg] = rgb(248, 248, 250);
    c[ImGuiCol_PopupBg] = rgb(255, 255, 255);
    c[ImGuiCol_Border] = rgb(212, 214, 220);
    c[ImGuiCol_FrameBg] = rgb(255, 255, 255);
    c[ImGuiCol_FrameBgHovered] = rgb(240, 244, 250);
    c[ImGuiCol_FrameBgActive] = rgb(228, 236, 248);
    c[ImGuiCol_TitleBg] = rgb(236, 237, 240);
    c[ImGuiCol_TitleBgActive] = rgb(228, 230, 234);
    c[ImGuiCol_Button] = rgb(232, 234, 238);
    c[ImGuiCol_ButtonHovered] = rgb(218, 226, 238);
    c[ImGuiCol_ButtonActive] = rgb(190, 210, 238);
    c[ImGuiCol_Header] = rgb(214, 228, 248);
    c[ImGuiCol_HeaderHovered] = rgb(200, 218, 244);
    c[ImGuiCol_HeaderActive] = rgb(180, 204, 240);
    c[ImGuiCol_Tab] = rgb(232, 234, 238);
    c[ImGuiCol_TabHovered] = rgb(210, 224, 244);
    c[ImGuiCol_TabSelected] = rgb(255, 255, 255);
    c[ImGuiCol_TabSelectedOverline] = rgb(30, 110, 210);
    c[ImGuiCol_Separator] = rgb(212, 214, 220);
    c[ImGuiCol_CheckMark] = rgb(30, 110, 210);
    c[ImGuiCol_SliderGrab] = rgb(30, 110, 210);
    c[ImGuiCol_Text] = rgb(28, 30, 34);
    c[ImGuiCol_TextDisabled] = rgb(140, 144, 152);
    g_palette = {rgb(100, 106, 116), rgb(196, 43, 28), rgb(160, 104, 0), rgb(30, 130, 60), rgb(30, 110, 210),
                 rgb(238, 239, 243)};
    g_palette.wave_bg = rgb(255, 255, 255);
    g_palette.ruler_bg = rgb(236, 238, 242);
    g_palette.ruler_text = rgb(96, 102, 114);
    g_palette.grid = rgb(226, 229, 235);
    g_palette.wave = rgb(30, 110, 180);
    g_palette.wave_second = rgb(190, 110, 30);
    g_palette.wave_dim = rgb(150, 180, 210);
    g_palette.ghost = rgb(60, 60, 60, 0.25f);
    g_palette.region = rgb(30, 160, 80, 0.07f);
    g_palette.outside = rgb(0, 0, 0, 0.08f);
    g_palette.start = rgb(20, 150, 70);
    g_palette.end = rgb(210, 50, 50);
    g_palette.cursor = rgb(200, 120, 0);
    g_palette.playhead = rgb(20, 20, 24);
}

} // namespace

void load_fonts() {
    g_text = add_font(first_font({"segoeui.ttf", "arial.ttf"}));
    if (g_text == nullptr) g_text = ImGui::GetIO().Fonts->AddFontDefaultVector();
    g_heading = add_font(first_font({"segoeuisb.ttf", "segoeuib.ttf"}));
    g_mono = add_font(first_font({"consola.ttf", "cour.ttf"}));
    ImGui::GetIO().FontDefault = g_text;
}

void apply(bool dark, float scale) {
    ImGuiStyle style;
    if (dark) dark_colours(style);
    else light_colours(style);
    style.WindowPadding = ImVec2(14.0f, 12.0f);
    style.FramePadding = ImVec2(8.0f, 5.0f);
    style.ItemSpacing = ImVec2(10.0f, 8.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
    style.FrameRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 1.0f;
    style.ScrollbarSize = 14.0f;
    style.ScaleAllSizes(scale);
    style.FontSizeBase = kTextSize;
    style.FontScaleDpi = scale;
    ImGui::GetStyle() = style;
}

const Palette &palette() { return g_palette; }
ImFont *heading_font() { return g_heading != nullptr ? g_heading : g_text; }
ImFont *mono_font() { return g_mono != nullptr ? g_mono : g_text; }
float heading_size() { return kHeadingSize; }
float mono_size() { return kTextSize * 0.9f; }

} // namespace d012::bgm_maker::theme
