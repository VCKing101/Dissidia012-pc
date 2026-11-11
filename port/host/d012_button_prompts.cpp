// The game's button symbols drawn as the buttons of the gamepad in use:
// Xbox, PlayStation or Nintendo (Controls > Button prompts). MIT licence; see
// port/README.md.
//
// The game draws its prompts, inline ones in its hints too, from nine
// textures in its menu and battle files (docs/port/button-prompts.md). As one
// loads, PortableKit hands its pixels here (GameProfile::edited_textures), and
// this draws it again at four times its size: the game's own pixels scaled up
// as the Texture filter samples them, then each PSP symbol painted over. The
// look stays the game's: its dark round buttons and silver keys keep their
// shape and only what is on them changes. Letters are drawn as strokes, not
// from a font, so they look the same on every system. Nothing of the game's
// is kept here but where its symbols sit.
//
// The face buttons follow where the pad's buttons are: the one that sends the
// PSP's cross is shown, which the Confirm button setting moves between the
// bottom and the right (input::ButtonPrompts).
#include "d012_button_prompts.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <string_view>

namespace d012 {
namespace {

namespace input = portablekit::input;

constexpr std::uint32_t kScale = 4u;  // the drawn texture against the game's
constexpr float S = static_cast<float>(kScale);

enum Sheet : std::size_t { kGeneral, kBattle, kLook, kExDefense, kBattleMenu, kSmall, kManual, kLetters, kPages };

// By their keys as Dump textures names them (USA and European discs alike).
constexpr portablekit::EditedTexture kTextures[] = {
    {"00000000c1a4601a0f4147e8", 128u, 128u},  // the icon sheet: SELECT, START, L, R, triangle, circle, square, cross
    {"00000000077b753ce322c2b0", 128u, 128u},  // battle: a large cross
    {"00000000292b5c3bdf76a538", 128u, 128u},  // customization: triangle (LOOK)
    {"00000000f78fbcba7c0a4ec6", 64u, 32u},    // EX defense: small cross, triangle, square, circle
    {"000000004da4c8c7207c3a18", 128u, 32u},   // NEXT: circle, cross
    {"000000002923c51e34372acf", 64u, 64u},    // small cross, triangle, circle, square and an R key
    {"00000000eecbc58d528a24c2", 64u, 32u},    // MANUAL START
    {"00000000f5efd5f2c2280980", 128u, 128u},  // blue and white L and R
    {"000000009e70b4097ca59bf2", 128u, 128u},  // Opponent Info: L R between arrows
};

// --- Colours ------------------------------------------------------------------

struct Colour {
    float r, g, b, a;
};

constexpr Colour rgb(int r, int g, int b, int a = 255) {
    return {static_cast<float>(r), static_cast<float>(g), static_cast<float>(b), static_cast<float>(a)};
}

constexpr Colour kDark = rgb(0x23, 0x1C, 0x3C);     // a round button's face
constexpr Colour kPill = rgb(0x27, 0x28, 0x3F);     // the SELECT and START pills
constexpr Colour kInk = rgb(0xD8, 0xD4, 0xE2);      // light symbols on them
constexpr Colour kOutline = rgb(0x14, 0x10, 0x26);  // round a symbol drawn on its own
constexpr Colour kKeyInk = rgb(0x23, 0x1C, 0x3C);   // letters on a silver key

// --- Pixels --------------------------------------------------------------------

std::uint32_t pack(Colour c) {
    const auto byte = [](float v) { return static_cast<std::uint32_t>(std::clamp(v, 0.0f, 255.0f) + 0.5f); };
    return byte(c.r) | byte(c.g) << 8u | byte(c.b) << 16u | byte(c.a) << 24u;
}

Colour unpack(std::uint32_t p) {
    return rgb(static_cast<int>(p & 0xFFu), static_cast<int>(p >> 8u & 0xFFu), static_cast<int>(p >> 16u & 0xFFu),
               static_cast<int>(p >> 24u));
}

struct Point {
    float x, y;
};

struct Segment {
    Point a, b;
};

float distance_to(Point p, const Segment &s) {
    const float dx = s.b.x - s.a.x, dy = s.b.y - s.a.y;
    const float length2 = dx * dx + dy * dy;
    const float t = length2 > 0.0f ? std::clamp(((p.x - s.a.x) * dx + (p.y - s.a.y) * dy) / length2, 0.0f, 1.0f) : 0.0f;
    const float ex = p.x - (s.a.x + t * dx), ey = p.y - (s.a.y + t * dy);
    return std::sqrt(ex * ex + ey * ey);
}

// Signed distance to a box centred on `c`, half `hw` by `hh`, its corners
// rounded by `r`.
float round_box(Point p, Point c, float hw, float hh, float r) {
    const float qx = std::fabs(p.x - c.x) - hw + r, qy = std::fabs(p.y - c.y) - hh + r;
    const float ox = std::max(qx, 0.0f), oy = std::max(qy, 0.0f);
    return std::sqrt(ox * ox + oy * oy) + std::min(std::max(qx, qy), 0.0f) - r;
}

// The picture being drawn, in its own pixels; boxes are given in the game's
// pixels (native) and scaled.
class Canvas {
public:
    Canvas(std::uint32_t width, std::uint32_t height, std::vector<std::uint32_t> &pixels)
        : width_(static_cast<int>(width)), height_(static_cast<int>(height)), pixels_(pixels) {}

    // Paints `colour` wherever `distance` (signed, in pixels, at a pixel's
    // centre) is inside, smoothed over a pixel, within the box (pixels).
    template <typename Distance>
    void fill(float x0, float y0, float x1, float y1, Colour colour, Distance distance) {
        const int ix0 = std::max(0, static_cast<int>(std::floor(x0)) - 1);
        const int iy0 = std::max(0, static_cast<int>(std::floor(y0)) - 1);
        const int ix1 = std::min(width_, static_cast<int>(std::ceil(x1)) + 1);
        const int iy1 = std::min(height_, static_cast<int>(std::ceil(y1)) + 1);
        for (int y = iy0; y < iy1; ++y)
            for (int x = ix0; x < ix1; ++x) {
                const float d = distance(Point{static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f});
                const float coverage = std::clamp(0.5f - d, 0.0f, 1.0f);
                if (coverage > 0.0f) blend(x, y, colour, coverage);
            }
    }

    // A box of the game's pixels made transparent.
    void clear(float x0, float y0, float x1, float y1) {
        for (int y = std::max(0, static_cast<int>(y0 * S)); y < std::min(height_, static_cast<int>(y1 * S)); ++y)
            for (int x = std::max(0, static_cast<int>(x0 * S)); x < std::min(width_, static_cast<int>(x1 * S)); ++x)
                pixels_[static_cast<std::size_t>(y) * width_ + x] = 0u;
    }

    // `colour` over the pixel at `coverage`, straight alpha as the texture keeps it.
    void blend(int x, int y, Colour colour, float coverage) {
        std::uint32_t &p = pixels_[static_cast<std::size_t>(y) * width_ + x];
        const Colour d = unpack(p);
        const float sa = colour.a / 255.0f * coverage, da = d.a / 255.0f;
        const float oa = sa + da * (1.0f - sa);
        if (oa <= 0.0f) {
            p = 0u;
            return;
        }
        const auto mix = [&](float s, float t) { return (s * sa + t * da * (1.0f - sa)) / oa; };
        p = pack({mix(colour.r, d.r), mix(colour.g, d.g), mix(colour.b, d.b), oa * 255.0f});
    }

private:
    int width_, height_;
    std::vector<std::uint32_t> &pixels_;
};

// Strokes of `radius` along segments (pixels).
void strokes(Canvas &canvas, const std::vector<Segment> &segments, float radius, Colour colour) {
    if (segments.empty()) return;
    float x0 = segments.front().a.x, y0 = segments.front().a.y, x1 = x0, y1 = y0;
    for (const Segment &s : segments) {
        x0 = std::min({x0, s.a.x, s.b.x});
        y0 = std::min({y0, s.a.y, s.b.y});
        x1 = std::max({x1, s.a.x, s.b.x});
        y1 = std::max({y1, s.a.y, s.b.y});
    }
    canvas.fill(x0 - radius, y0 - radius, x1 + radius, y1 + radius, colour, [&](Point p) {
        float d = 1e9f;
        for (const Segment &s : segments) d = std::min(d, distance_to(p, s));
        return d - radius;
    });
}

void disc(Canvas &canvas, Point c, float r, Colour colour) {
    canvas.fill(c.x - r, c.y - r, c.x + r, c.y + r, colour, [&](Point p) {
        return std::sqrt((p.x - c.x) * (p.x - c.x) + (p.y - c.y) * (p.y - c.y)) - r;
    });
}

void ring(Canvas &canvas, Point c, float r, float thickness, Colour colour) {
    const float outer = r + thickness / 2.0f;
    canvas.fill(c.x - outer, c.y - outer, c.x + outer, c.y + outer, colour, [&](Point p) {
        return std::fabs(std::sqrt((p.x - c.x) * (p.x - c.x) + (p.y - c.y) * (p.y - c.y)) - r) - thickness / 2.0f;
    });
}

void box(Canvas &canvas, Point c, float hw, float hh, float r, Colour colour) {
    canvas.fill(c.x - hw, c.y - hh, c.x + hw, c.y + hh, colour, [&](Point p) { return round_box(p, c, hw, hh, r); });
}

void box_outline(Canvas &canvas, Point c, float hw, float hh, float r, float thickness, Colour colour) {
    const float t = thickness / 2.0f;
    canvas.fill(c.x - hw - t, c.y - hh - t, c.x + hw + t, c.y + hh + t, colour,
                [&](Point p) { return std::fabs(round_box(p, c, hw, hh, r)) - t; });
}

// --- Letters ---------------------------------------------------------------------

// A letter as strokes along its centre lines, in units of its height: y 0 at
// the top, 1 at the foot; x from 0 to `width`.
struct Glyph {
    float width;
    std::vector<std::vector<Point>> lines;
};

void arc(std::vector<Point> &line, float cx, float cy, float rx, float ry, float from, float to, int steps = 14) {
    constexpr float kDegrees = 3.14159265f / 180.0f;
    for (int i = 0; i <= steps; ++i) {
        const float t = (from + (to - from) * static_cast<float>(i) / static_cast<float>(steps)) * kDegrees;
        line.push_back({cx + rx * std::cos(t), cy + ry * std::sin(t)});
    }
}

// A bowl on the right of a stem, from (0, top) to (0, bottom).
std::vector<Point> bowl(float top, float bottom, float reach) {
    const float r = (bottom - top) / 2.0f;
    std::vector<Point> line{{0.0f, top}, {reach - r, top}};
    arc(line, reach - r, top + r, r, r, -90.0f, 90.0f);
    line.push_back({0.0f, bottom});
    return line;
}

const Glyph *glyph(char c) {
    static const std::array<Glyph, 17> glyphs = [] {
        std::array<Glyph, 17> g{};
        g[0] = {0.62f, {{{0.0f, 1.0f}, {0.31f, 0.0f}, {0.62f, 1.0f}}, {{0.105f, 0.66f}, {0.515f, 0.66f}}}};  // A
        g[1] = {0.56f, {{{0.0f, 0.0f}, {0.0f, 1.0f}}, bowl(0.0f, 0.47f, 0.52f), bowl(0.47f, 1.0f, 0.56f)}};    // B
        g[2] = {0.46f,
                {{{0.46f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f}, {0.46f, 1.0f}}, {{0.0f, 0.5f}, {0.4f, 0.5f}}}};  // E
        g[3] = {0.56f, {{{0.0f, 0.0f}, {0.0f, 1.0f}}, {{0.56f, 0.0f}, {0.56f, 1.0f}}, {{0.0f, 0.5f}, {0.56f, 0.5f}}}};  // H
        g[4] = {0.0f, {{{0.0f, 0.0f}, {0.0f, 1.0f}}}};                                                   // I
        g[5] = {0.44f, {{{0.0f, 0.0f}, {0.0f, 1.0f}, {0.44f, 1.0f}}}};                                   // L
        g[6] = {0.56f, {{{0.0f, 1.0f}, {0.0f, 0.0f}, {0.56f, 1.0f}, {0.56f, 0.0f}}}};                    // N
        {
            std::vector<Point> o;  // O: a stadium
            arc(o, 0.3f, 0.3f, 0.3f, 0.3f, 180.0f, 360.0f);
            arc(o, 0.3f, 0.7f, 0.3f, 0.3f, 0.0f, 180.0f);
            o.push_back(o.front());
            g[7] = {0.6f, {o}};
        }
        g[8] = {0.52f, {{{0.0f, 1.0f}, {0.0f, 0.0f}}, bowl(0.0f, 0.54f, 0.52f)}};  // P
        g[9] = {0.56f, {{{0.0f, 1.0f}, {0.0f, 0.0f}}, bowl(0.0f, 0.52f, 0.52f), {{0.24f, 0.52f}, {0.56f, 1.0f}}}};
        {
            std::vector<Point> s;  // S: two bowls meeting at the middle
            arc(s, 0.26f, 0.25f, 0.26f, 0.25f, -25.0f, -270.0f, 20);
            arc(s, 0.26f, 0.75f, 0.26f, 0.25f, -90.0f, 155.0f, 20);
            g[10] = {0.52f, {s}};
        }
        g[11] = {0.5f, {{{0.0f, 0.0f}, {0.5f, 0.0f}}, {{0.25f, 0.0f}, {0.25f, 1.0f}}}};                 // T
        g[12] = {0.58f, {{{0.0f, 0.0f}, {0.58f, 1.0f}}, {{0.58f, 0.0f}, {0.0f, 1.0f}}}};                // X
        g[13] = {0.58f, {{{0.0f, 0.0f}, {0.29f, 0.52f}, {0.58f, 0.0f}}, {{0.29f, 0.52f}, {0.29f, 1.0f}}}};  // Y
        g[14] = {0.3f, {{{0.0f, 0.22f}, {0.3f, 0.0f}, {0.3f, 1.0f}}}};                                  // 1
        {
            std::vector<Point> two;  // 2: a hook over the top, then down to the foot
            arc(two, 0.25f, 0.27f, 0.25f, 0.27f, 200.0f, 385.0f, 18);
            two.push_back({0.0f, 1.0f});
            two.push_back({0.52f, 1.0f});
            g[15] = {0.52f, {two}};
        }
        g[16] = {0.5f, {{{0.02f, 0.0f}, {0.5f, 0.0f}, {0.0f, 1.0f}, {0.5f, 1.0f}}}};                    // Z
        return g;
    }();
    switch (c) {
    case 'A': return &glyphs[0];
    case 'B': return &glyphs[1];
    case 'E': return &glyphs[2];
    case 'H': return &glyphs[3];
    case 'I': return &glyphs[4];
    case 'L': return &glyphs[5];
    case 'N': return &glyphs[6];
    case 'O': return &glyphs[7];
    case 'P': return &glyphs[8];
    case 'R': return &glyphs[9];
    case 'S': return &glyphs[10];
    case 'T': return &glyphs[11];
    case 'X': return &glyphs[12];
    case 'Y': return &glyphs[13];
    case '1': return &glyphs[14];
    case '2': return &glyphs[15];
    case 'Z': return &glyphs[16];
    default: return nullptr;
    }
}

struct TextStyle {
    Colour fill;
    float weight = 0.19f;          // stroke thickness against the letters' height
    float outline = 0.0f;          // a border round each stroke, in pixels
    Colour outline_colour = kOutline;
};

// `text` centred on `centre` (pixels), its letters `height` pixels tall from
// stroke edge to stroke edge, squeezed sideways to at most `max_width`.
void text(Canvas &canvas, std::string_view text, Point centre, float height, float max_width, const TextStyle &style) {
    const float radius = style.weight * height / 2.0f;
    const float cap = height - 2.0f * radius;  // between the strokes' centre lines
    if (cap <= 0.0f) return;
    const float gap = 0.2f + 2.0f * radius / cap;
    float total = 0.0f;
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (const Glyph *g = glyph(text[i])) total += g->width;
        if (i + 1u < text.size()) total += gap;
    }
    const float squeeze = total > 0.0f ? std::min(1.0f, (max_width - 2.0f * radius) / (total * cap)) : 1.0f;
    float x = centre.x - total * cap * squeeze / 2.0f;
    const float top = centre.y - cap / 2.0f;
    std::vector<Segment> segments;
    for (const char c : text) {
        const Glyph *g = glyph(c);
        if (g == nullptr) continue;
        for (const std::vector<Point> &line : g->lines) {
            for (std::size_t i = 0; i < line.size(); ++i) {
                const Point a{x + line[i].x * cap * squeeze, top + line[i].y * cap};
                const Point b = i + 1u < line.size() ? Point{x + line[i + 1u].x * cap * squeeze, top + line[i + 1u].y * cap}
                                                     : a;
                segments.push_back({a, b});
            }
        }
        x += (g->width + gap) * cap * squeeze;
    }
    if (style.outline > 0.0f) strokes(canvas, segments, radius + style.outline, style.outline_colour);
    strokes(canvas, segments, radius, style.fill);
}

// --- Buttons -------------------------------------------------------------------

enum class Psp { Cross, Circle, Square, Triangle };

// What a face button looks like in a style: a letter, or a PlayStation shape.
struct Face {
    char letter;  // 0: the shape
    Psp shape;
    Colour colour;
};

input::FaceButton where(const input::ButtonPrompts &prompts, Psp button) {
    switch (button) {
    case Psp::Cross: return prompts.cross;
    case Psp::Circle: return prompts.circle;
    case Psp::Square: return prompts.square;
    case Psp::Triangle: return prompts.triangle;
    }
    return prompts.cross;
}

// The face button that sends the PSP's `button`, as the style shows it.
Face face(const input::ButtonPrompts &prompts, Psp button) {
    const auto at = static_cast<std::size_t>(where(prompts, button));  // South, East, West, North
    static constexpr Face kXbox[] = {{'A', Psp::Cross, rgb(0x72, 0xCC, 0x4E)},
                                     {'B', Psp::Circle, rgb(0xF2, 0x55, 0x4E)},
                                     {'X', Psp::Square, rgb(0x4C, 0xA0, 0xF2)},
                                     {'Y', Psp::Triangle, rgb(0xF7, 0xCA, 0x3C)}};
    static constexpr Face kNintendo[] = {{'B', Psp::Cross, kInk}, {'A', Psp::Circle, kInk},
                                         {'Y', Psp::Square, kInk}, {'X', Psp::Triangle, kInk}};
    static constexpr Face kPlayStation[] = {{0, Psp::Cross, rgb(0x92, 0xB8, 0xF2)},
                                            {0, Psp::Circle, rgb(0xF2, 0x6E, 0x72)},
                                            {0, Psp::Square, rgb(0xEC, 0xA0, 0xD8)},
                                            {0, Psp::Triangle, rgb(0x50, 0xD8, 0xB4)}};
    switch (prompts.style) {
    case input::PromptStyle::Xbox: return kXbox[at];
    case input::PromptStyle::Nintendo: return kNintendo[at];
    default: return kPlayStation[at];
    }
}

// A face symbol centred on `c` (pixels) for a button of radius `r`. One on
// its own (`plain`) gets a dark border so it reads on any background.
void draw_face(Canvas &canvas, const Face &f, Point c, float r, bool plain) {
    if (f.letter != 0) {
        const char letter[2] = {f.letter, 0};
        TextStyle style{f.colour};
        style.weight = 0.22f;
        if (plain) style.outline = 0.12f * r;
        text(canvas, letter, c, 1.3f * r, 4.0f * r, style);
        return;
    }
    const float a = 0.6f * r, w = std::max(1.5f, 0.24f * r);
    switch (f.shape) {
    case Psp::Cross:
        strokes(canvas, {{{c.x - a, c.y - a}, {c.x + a, c.y + a}}, {{c.x - a, c.y + a}, {c.x + a, c.y - a}}}, w / 2.0f,
                f.colour);
        break;
    case Psp::Circle: ring(canvas, c, a - w / 2.0f, w, f.colour); break;
    case Psp::Square: {
        const float b = 0.86f * a - w / 2.0f;
        box_outline(canvas, c, b, b, 0.1f * b, w, f.colour);
        break;
    }
    case Psp::Triangle: {
        const float b = 1.08f * a;
        const Point top{c.x, c.y - b}, right{c.x + 0.98f * b, c.y + 0.72f * b}, left{c.x - 0.98f * b, c.y + 0.72f * b};
        strokes(canvas, {{top, right}, {right, left}, {left, top}}, w / 2.0f, f.colour);
        break;
    }
    }
}

// A round button (centre and face radius in the game's pixels): its face
// painted over, the new symbol on it; the rim and glow stay the game's.
void round_button(Canvas &canvas, const input::ButtonPrompts &prompts, Psp button, Point centre, float r) {
    const Point c{centre.x * S, centre.y * S};
    disc(canvas, c, r * S, kDark);
    draw_face(canvas, face(prompts, button), c, 0.82f * r * S, false);
}

// A symbol on its own, the old one cleared from its box (the game's pixels).
void plain_button(Canvas &canvas, const input::ButtonPrompts &prompts, Psp button, float x0, float y0, float x1,
                  float y1) {
    canvas.clear(x0, y0, x1, y1);
    const float r = std::min(x1 - x0, y1 - y0) / 2.0f * 0.92f;
    draw_face(canvas, face(prompts, button), {(x0 + x1) / 2.0f * S, (y0 + y1) / 2.0f * S}, r * S, true);
}

// The names, in a style, of the buttons that press the PSP's L and R: the
// shoulders, or the triggers with Swap shoulders and triggers.
std::string_view shoulder(const input::ButtonPrompts &prompts, bool right) {
    const bool numbered = prompts.style == input::PromptStyle::PlayStation || prompts.numbered_shoulders;
    if (prompts.l_r_on_triggers) {
        if (prompts.style == input::PromptStyle::Nintendo) return right ? "ZR" : "ZL";
        if (numbered) return right ? "R2" : "L2";
        return right ? "RT" : "LT";
    }
    if (prompts.style == input::PromptStyle::Nintendo) return right ? "R" : "L";
    if (numbered) return right ? "R1" : "L1";
    return right ? "RB" : "LB";
}

// The game's silver L/R key (a box of its pixels), drawn again with `label`.
void silver_key(Canvas &canvas, float x0, float y0, float x1, float y1, std::string_view label) {
    canvas.clear(x0, y0, x1, y1);
    const float w = x1 - x0, h = y1 - y0;
    const Point c{(x0 + x1) / 2.0f * S, (y0 + y1) / 2.0f * S};
    box(canvas, c, (w / 2.0f - 0.4f) * S, (h / 2.0f - 0.4f) * S, 2.2f * S, rgb(0x22, 0x1D, 0x38, 0xC0));
    // The face, light at the top and darker below.
    const float top = (y0 + 1.2f) * S, bottom = (y1 - 1.4f) * S;
    const Point face_c{c.x, (top + bottom) / 2.0f};
    const float hw = (w / 2.0f - 1.3f) * S, hh = (bottom - top) / 2.0f;
    for (int y = static_cast<int>(top); y < static_cast<int>(std::ceil(bottom)); ++y) {
        const float t = std::clamp((static_cast<float>(y) + 0.5f - top) / std::max(1.0f, bottom - top), 0.0f, 1.0f);
        const Colour row = rgb(static_cast<int>(0xE2 + (0x8E - 0xE2) * t), static_cast<int>(0xE0 + (0x8B - 0xE0) * t),
                               static_cast<int>(0xEC + (0xA4 - 0xEC) * t));
        canvas.fill(face_c.x - hw, static_cast<float>(y), face_c.x + hw, static_cast<float>(y + 1), row, [&](Point p) {
            if (p.y < static_cast<float>(y) || p.y > static_cast<float>(y + 1)) return 1.0f;
            return round_box(p, face_c, hw, hh, 1.2f * S);
        });
    }
    strokes(canvas, {{{(x0 + 1.6f) * S, (y1 - 1.6f) * S}, {(x1 - 1.6f) * S, (y1 - 1.6f) * S}}}, 0.3f * S,
            rgb(0xDA, 0xD8, 0xE4));
    TextStyle style{kKeyInk};
    style.weight = 0.17f;
    text(canvas, label, {c.x, c.y - 0.2f * S}, (h - 6.4f) * S, (w - 6.0f) * S, style);
}

// Xbox's Menu button (three lines) and View button (two windows), centred on
// `c` (pixels) for a pill `h` pixels tall.
void menu_icon(Canvas &canvas, Point c, float h, Colour colour) {
    const float a = 0.26f * h, r = std::max(0.75f, 0.05f * h);
    for (int d = -1; d <= 1; ++d) {
        const float y = c.y + static_cast<float>(d) * a * 0.85f;
        strokes(canvas, {{{c.x - 1.05f * a, y}, {c.x + 1.05f * a, y}}}, r, colour);
    }
}

void view_icon(Canvas &canvas, Point c, float h, Colour colour) {
    const float a = 0.2f * h, t = std::max(1.5f, 0.075f * h);
    box_outline(canvas, {c.x - 0.35f * a, c.y - 0.3f * a}, 0.9f * a - t / 2.0f, 0.75f * a - t / 2.0f, 0.2f * a, t, colour);
    const Point front{c.x + 0.35f * a, c.y + 0.3f * a};
    box(canvas, front, 0.9f * a, 0.75f * a, 0.2f * a, kPill);
    box_outline(canvas, front, 0.9f * a - t / 2.0f, 0.75f * a - t / 2.0f, 0.2f * a, t, colour);
}

void plus_minus(Canvas &canvas, Point c, float size, bool plus, Colour colour) {
    const float a = size / 2.0f, r = 0.1f * size;
    std::vector<Segment> bars{{{c.x - a, c.y}, {c.x + a, c.y}}};
    if (plus) bars.push_back({{c.x, c.y - a}, {c.x, c.y + a}});
    strokes(canvas, bars, r, colour);
}

// The game's dark SELECT or START pill (a box of its pixels), drawn again
// with the style's button.
void pill(Canvas &canvas, const input::ButtonPrompts &prompts, float x0, float y0, float x1, float y1, bool start) {
    canvas.clear(x0, y0, x1, y1);
    const float w = x1 - x0, h = y1 - y0;
    const Point c{(x0 + x1) / 2.0f * S, (y0 + y1) / 2.0f * S};
    box(canvas, c, (w / 2.0f - 0.5f) * S, (h / 2.0f - 0.5f) * S, (h - 1.0f) / 2.0f * S, rgb(255, 255, 255, 0x45));
    box(canvas, c, (w / 2.0f - 1.3f) * S, (h / 2.0f - 1.3f) * S, (h - 2.6f) / 2.0f * S, kPill);
    switch (prompts.style) {
    case input::PromptStyle::PlayStation: {
        TextStyle style{kInk};
        style.weight = 0.2f;
        text(canvas, start ? "OPTIONS" : "SHARE", c, 0.42f * h * S, (w - 5.0f) * S, style);
        break;
    }
    case input::PromptStyle::Nintendo: plus_minus(canvas, c, 0.5f * h * S, start, kInk); break;
    default:
        if (start) menu_icon(canvas, c, h * S, kInk);
        else view_icon(canvas, c, h * S, kInk);
        break;
    }
}

// MANUAL START: the START on the badge's silver key replaced by the style's
// button. The key's face behind the old letters is filled first, in the
// game's pixels (fill_manual_face, before scaling).
void fill_manual_face(std::vector<std::uint32_t> &native, std::uint32_t width) {
    for (std::uint32_t y = 8u; y < 16u; ++y) {
        const std::uint32_t face = native[y * width + 19u];
        for (std::uint32_t x = 21u; x < 57u; ++x) native[y * width + x] = face;
    }
}

void manual_badge(Canvas &canvas, const input::ButtonPrompts &prompts) {
    const Point c{39.0f * S, 12.0f * S};
    switch (prompts.style) {
    case input::PromptStyle::PlayStation: {
        TextStyle style{kKeyInk};
        style.weight = 0.2f;
        text(canvas, "OPTIONS", c, 5.6f * S, 33.0f * S, style);
        break;
    }
    case input::PromptStyle::Nintendo: plus_minus(canvas, c, 6.0f * S, true, kKeyInk); break;
    default: menu_icon(canvas, c, 8.8f * S, kKeyInk); break;
    }
}

// The brightest third of a box's solid pixels, averaged: a letter's colour.
Colour letter_colour(const std::uint32_t *native, std::uint32_t width, int x0, int y0, int x1, int y1) {
    std::vector<Colour> solid;
    for (int y = y0; y < y1; ++y)
        for (int x = x0; x < x1; ++x) {
            const Colour c = unpack(native[static_cast<std::size_t>(y) * width + x]);
            if (c.a > 200.0f) solid.push_back(c);
        }
    if (solid.empty()) return kInk;
    std::sort(solid.begin(), solid.end(), [](const Colour &a, const Colour &b) { return a.r + a.g + a.b > b.r + b.g + b.b; });
    const std::size_t n = std::max<std::size_t>(1u, solid.size() / 3u);
    Colour sum{0.0f, 0.0f, 0.0f, 255.0f};
    for (std::size_t i = 0; i < n; ++i) {
        sum.r += solid[i].r;
        sum.g += solid[i].g;
        sum.b += solid[i].b;
    }
    return {sum.r / static_cast<float>(n), sum.g / static_cast<float>(n), sum.b / static_cast<float>(n), 255.0f};
}

// Plain L or R letters (a box of the game's pixels) as the style's shoulders,
// in the old letters' colour.
void shoulder_letters(Canvas &canvas, const std::uint32_t *native, std::uint32_t width, int x0, int y0, int x1, int y1,
                      std::string_view label) {
    const Colour colour = letter_colour(native, width, x0, y0, x1, y1);
    canvas.clear(static_cast<float>(x0), static_cast<float>(y0), static_cast<float>(x1), static_cast<float>(y1));
    const float w = static_cast<float>(x1 - x0), h = static_cast<float>(y1 - y0);
    TextStyle style{colour};
    style.weight = 0.16f;
    style.outline = 0.3f * S;
    style.outline_colour = rgb(0x10, 0x0C, 0x30, 160);
    text(canvas, label, {(static_cast<float>(x0) + w / 2.0f) * S, (static_cast<float>(y0) + h / 2.0f) * S},
         (h - 2.6f) * S, (w - 1.4f) * S, style);
}

// The game's picture at four times its size, sampled as the Texture filter
// samples it: between pixels (Smooth) or the nearest (Sharp).
void scale_up(const std::vector<std::uint32_t> &native, std::uint32_t width, std::uint32_t height, bool sharp,
              std::vector<std::uint32_t> &out) {
    const std::uint32_t w = width * kScale, h = height * kScale;
    out.assign(static_cast<std::size_t>(w) * h, 0u);
    const auto at = [&](int x, int y) {
        x = std::clamp(x, 0, static_cast<int>(width) - 1);
        y = std::clamp(y, 0, static_cast<int>(height) - 1);
        return unpack(native[static_cast<std::size_t>(y) * width + x]);
    };
    for (std::uint32_t y = 0; y < h; ++y)
        for (std::uint32_t x = 0; x < w; ++x) {
            const float u = (static_cast<float>(x) + 0.5f) / S - 0.5f, v = (static_cast<float>(y) + 0.5f) / S - 0.5f;
            Colour c;
            if (sharp) {
                c = at(static_cast<int>(x / kScale), static_cast<int>(y / kScale));
            } else {
                const int ix = static_cast<int>(std::floor(u)), iy = static_cast<int>(std::floor(v));
                const float fx = u - static_cast<float>(ix), fy = v - static_cast<float>(iy);
                const Colour a = at(ix, iy), b = at(ix + 1, iy), d = at(ix, iy + 1), e = at(ix + 1, iy + 1);
                const auto mix = [&](float p, float q, float r, float s) {
                    return (p * (1.0f - fx) + q * fx) * (1.0f - fy) + (r * (1.0f - fx) + s * fx) * fy;
                };
                c = {mix(a.r, b.r, d.r, e.r), mix(a.g, b.g, d.g, e.g), mix(a.b, b.b, d.b, e.b), mix(a.a, b.a, d.a, e.a)};
            }
            out[static_cast<std::size_t>(y) * w + x] = pack(c);
        }
}

} // namespace

std::span<const portablekit::EditedTexture> button_prompt_textures() { return kTextures; }

bool draw_button_prompts(std::size_t index, const std::uint32_t *pixels, std::uint32_t width, std::uint32_t height,
                         const portablekit::TextureEditContext &context, std::vector<std::uint32_t> &out,
                         std::uint32_t &out_width, std::uint32_t &out_height) {
    const input::ButtonPrompts &prompts = context.prompts;
    if (prompts.style == input::PromptStyle::Psp || index >= std::size(kTextures)) return false;
    if (width != kTextures[index].width || height != kTextures[index].height) return false;
    std::vector<std::uint32_t> native(pixels, pixels + static_cast<std::size_t>(width) * height);
    if (index == kManual) fill_manual_face(native, width);
    scale_up(native, width, height, context.sharp_textures, out);
    out_width = width * kScale;
    out_height = height * kScale;
    Canvas canvas(out_width, out_height, out);
    // Nintendo's shoulders are L and R already; its triggers are not.
    const bool plain_l_r = prompts.style == input::PromptStyle::Nintendo && !prompts.l_r_on_triggers;
    switch (index) {
    case kGeneral:
        round_button(canvas, prompts, Psp::Triangle, {63.0f, 117.0f}, 4.7f);
        round_button(canvas, prompts, Psp::Circle, {81.0f, 117.0f}, 4.7f);
        round_button(canvas, prompts, Psp::Square, {99.0f, 117.0f}, 4.7f);
        round_button(canvas, prompts, Psp::Cross, {117.0f, 117.0f}, 4.7f);
        pill(canvas, prompts, 3.0f, 92.0f, 34.0f, 106.0f, false);
        pill(canvas, prompts, 39.0f, 92.0f, 70.0f, 106.0f, true);
        silver_key(canvas, 18.0f, 111.0f, 36.0f, 125.0f, shoulder(prompts, false));
        silver_key(canvas, 36.0f, 111.0f, 54.0f, 125.0f, shoulder(prompts, true));
        break;
    case kBattle: round_button(canvas, prompts, Psp::Cross, {79.0f, 19.0f}, 12.0f); break;
    case kLook: round_button(canvas, prompts, Psp::Triangle, {80.5f, 70.5f}, 5.7f); break;
    case kExDefense:
        plain_button(canvas, prompts, Psp::Cross, 14.0f, 1.0f, 24.0f, 11.0f);
        plain_button(canvas, prompts, Psp::Triangle, 25.0f, 0.0f, 36.0f, 10.0f);
        plain_button(canvas, prompts, Psp::Square, 14.0f, 13.0f, 24.0f, 22.0f);
        plain_button(canvas, prompts, Psp::Circle, 25.0f, 12.0f, 35.0f, 22.0f);
        break;
    case kBattleMenu:
        round_button(canvas, prompts, Psp::Circle, {104.5f, 23.5f}, 5.7f);
        round_button(canvas, prompts, Psp::Cross, {120.5f, 23.5f}, 5.7f);
        break;
    case kSmall:
        plain_button(canvas, prompts, Psp::Cross, 23.0f, 17.0f, 32.0f, 26.0f);
        plain_button(canvas, prompts, Psp::Triangle, 14.0f, 25.0f, 25.0f, 35.0f);
        plain_button(canvas, prompts, Psp::Circle, 25.0f, 25.0f, 34.0f, 34.0f);
        plain_button(canvas, prompts, Psp::Square, 34.0f, 25.0f, 43.0f, 34.0f);
        silver_key(canvas, 46.0f, 30.0f, 64.0f, 42.0f, shoulder(prompts, true));
        break;
    case kManual: manual_badge(canvas, prompts); break;
    case kLetters:
        if (plain_l_r) return false;
        shoulder_letters(canvas, pixels, width, 114, 54, 124, 66, shoulder(prompts, false));
        shoulder_letters(canvas, pixels, width, 114, 68, 125, 80, shoulder(prompts, true));
        shoulder_letters(canvas, pixels, width, 116, 80, 127, 93, shoulder(prompts, false));
        shoulder_letters(canvas, pixels, width, 116, 93, 128, 106, shoulder(prompts, true));
        break;
    case kPages:
        if (plain_l_r) return false;
        shoulder_letters(canvas, pixels, width, 97, 28, 108, 41, shoulder(prompts, false));
        shoulder_letters(canvas, pixels, width, 108, 28, 118, 41, shoulder(prompts, true));
        break;
    default: return false;
    }
    return true;
}

} // namespace d012
