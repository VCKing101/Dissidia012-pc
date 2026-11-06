// The game's message, save-data and keyboard dialogs in the game's own menu
// look (d012_dialogs.hpp).
//
// On a PSP the game hands these to the system (sceUtilityMsgDialog,
// sceUtilitySavedata, sceUtilityOsk), which draws them in its own look;
// PortableKit draws them in its interface's look. Here they are drawn like the
// game's Data Config page: the page title in a silver serif, a dark band with
// an instruction line, the choices on the game's silver plates, the selected
// one on its blue plate with the pointing hand, and the hints in the game's
// own button symbols. The keys of the keyboard are the game's bevelled cells.
//
// The pieces come from the game's menu pictures on the player's disc, read
// when the first dialog opens: three entries of PACKAGE.BIN, found through
// PACKAGE_INFO.BIN by their name hash, each a "menu pak" of GIM pictures.
// Each picture is recognised by a fingerprint of its decoded pixels, so a
// disc whose pictures differ keeps PortableKit's look rather than drawing the
// wrong pieces. Nothing of the game's is stored here: the hashes, sizes and
// where each piece sits in its picture.
//
// Every place below is in the PSP's 480x272 screen, drawn over the game's
// picture (ui::game_picture()).
#include "d012_dialogs.hpp"
#include "d012_decomp_harness.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "hle/hle_common.hpp"
#include "psprecomp/runtime.hpp"
#include "kernel/iso_image.hpp"
#include "profile.hpp"
#include "ui/dialog_skin.hpp"
#include "ui/ui.hpp"

#include <imgui.h>
#include <imgui_internal.h>

// The save icons are PNGs; PortableKit keeps its own copy of the decoder to
// itself (STB_IMAGE_STATIC), as this one is.
#define STBI_ONLY_PNG
#define STBI_NO_STDIO
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace d012 {
// The title serif, generated from port/fonts (port/CMakeLists.txt).
extern const unsigned char kTitleFont[];
extern const std::size_t kTitleFontSize;
} // namespace d012

namespace d012::dialogs {

#if defined(PORTABLEKIT_HAS_RENDERER)
namespace {

namespace ui = portablekit::ui;

// --- The game's pictures ---------------------------------------------------

constexpr std::uint32_t kSector = 2048u;

struct Picture {
    int width{};
    int height{};
    std::vector<std::uint8_t> rgba;
};

// The four pictures and where they are: the PACKAGE.BIN entry (by the hash of
// its name) and the fingerprint of the decoded picture (FNV-1a 64 of its RGBA).
enum Sheet { kPlates, kSymbols, kCells, kFeather, kSheetCount };

struct SheetSource {
    const char *what;
    std::uint32_t entry;
    int width;
    int height;
    std::uint64_t fingerprint;
};

constexpr std::array<SheetSource, kSheetCount> kSources{{
    {"the cursor, plates and arrows", 0x09478AE5u, 128, 128, 0x2147E6885DAB0C04ull},
    {"the button symbols", 0x09478AE5u, 128, 128, 0x1D66DFA8877D95E2ull},
    {"the key cells", 0x4AA1BF07u, 128, 128, 0x64BF5F4243ECCAACull},
    {"the feather", 0x9B9D2137u, 256, 256, 0x74824A454E463661ull},
}};

std::uint64_t fingerprint(const std::vector<std::uint8_t> &bytes) {
    std::uint64_t hash = 0xCBF29CE484222325ull;
    for (const std::uint8_t byte : bytes) hash = (hash ^ byte) * 0x100000001B3ull;
    return hash;
}

std::uint16_t u16(const std::vector<std::uint8_t> &d, std::size_t at) {
    return at + 2u <= d.size() ? static_cast<std::uint16_t>(d[at] | d[at + 1u] << 8u) : 0u;
}

std::uint32_t u32(const std::vector<std::uint8_t> &d, std::size_t at) {
    return at + 4u <= d.size() ? static_cast<std::uint32_t>(d[at] | d[at + 1u] << 8u | d[at + 2u] << 16u |
                                                            static_cast<std::uint32_t>(d[at + 3u]) << 24u)
                               : 0u;
}

// A GIM's picture block's image and palette: {u16 type, u16, u32 size, u32
// next, u32 data offset} blocks, the root (2) and picture (3) holding the
// image (4) and the palette (5). Each data header has the format at +4, the
// order (0: not swizzled) at +6, width and height at +8 and +0x0A, the pitch
// alignment at +0x0E and the pixels' offset from the header at +0x1C.
struct GimBlock {
    std::size_t header{};
};

void find_blocks(const std::vector<std::uint8_t> &d, std::size_t at, std::size_t end, std::optional<GimBlock> &image,
                 std::optional<GimBlock> &palette, int depth) {
    while (at + 16u <= end && depth < 4) {
        const std::uint16_t type = u16(d, at);
        const std::uint32_t size = u32(d, at + 4u);
        const std::uint32_t next = u32(d, at + 8u);
        const std::uint32_t data = u32(d, at + 12u);
        if (size < 16u || at + size > end) return;
        if (type == 0x04u && !image) image = GimBlock{at + data};
        if (type == 0x05u && !palette) palette = GimBlock{at + data};
        if (type == 0x02u || type == 0x03u) find_blocks(d, at + data, at + size, image, palette, depth + 1);
        if (next == 0u) return;
        at += next;
    }
}

// An 8-bit or 4-bit indexed GIM with an RGBA8888 palette, as the menus' are.
std::optional<Picture> decode_gim(const std::vector<std::uint8_t> &d, std::size_t start, std::size_t end) {
    std::optional<GimBlock> image;
    std::optional<GimBlock> palette;
    find_blocks(d, start + 16u, end, image, palette, 0);
    if (!image || !palette) return std::nullopt;
    const std::size_t ih = image->header;
    const std::size_t ph = palette->header;
    const std::uint16_t format = u16(d, ih + 4u);
    const int width = u16(d, ih + 8u);
    const int height = u16(d, ih + 10u);
    const std::uint16_t align = std::max<std::uint16_t>(1u, u16(d, ih + 14u));
    const std::size_t pixels = ih + u32(d, ih + 0x1Cu);
    const std::uint16_t palette_format = u16(d, ph + 4u);
    const int colours = u16(d, ph + 8u);
    const std::size_t palette_at = ph + u32(d, ph + 0x1Cu);
    if ((format != 4u && format != 5u) || u16(d, ih + 6u) != 0u || palette_format != 3u || width <= 0 ||
        height <= 0 || colours <= 0)
        return std::nullopt;
    const std::size_t bits = format == 5u ? 8u : 4u;
    const std::size_t row = ((static_cast<std::size_t>(width) * bits + 7u) / 8u + align - 1u) / align * align;
    if (pixels + row * static_cast<std::size_t>(height) > end ||
        palette_at + static_cast<std::size_t>(colours) * 4u > end)
        return std::nullopt;
    Picture out;
    out.width = width;
    out.height = height;
    out.rgba.resize(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4u);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const std::size_t at = pixels + static_cast<std::size_t>(y) * row;
            std::size_t index = bits == 8u ? d[at + static_cast<std::size_t>(x)]
                                           : (d[at + static_cast<std::size_t>(x) / 2u] >> ((x & 1) * 4)) & 0x0Fu;
            index %= static_cast<std::size_t>(colours);
            std::memcpy(&out.rgba[(static_cast<std::size_t>(y) * static_cast<std::size_t>(width) +
                                   static_cast<std::size_t>(x)) * 4u],
                        &d[palette_at + index * 4u], 4u);
        }
    }
    return out;
}

// The four pictures from the disc, or why they could not be read.
struct Pictures {
    std::array<Picture, kSheetCount> sheets;
    std::string problem;
};

Pictures read_pictures() {
    Pictures out;
    const std::filesystem::path &path = portablekit::disc_image_path();
    if (path.empty()) {
        out.problem = "no disc image";
        return out;
    }
    try {
        portablekit::IsoImage disc(path);
        const auto index = disc.find("PSP_GAME/USRDIR/DATA/PACKAGE_INFO.BIN");
        const auto package = disc.find("PSP_GAME/USRDIR/DATA/PACKAGE.BIN");
        if (!index || !package) {
            out.problem = "the disc has no PACKAGE.BIN";
            return out;
        }
        std::vector<std::uint8_t> records(index->size);
        records.resize(disc.read(std::uint64_t(index->lba) * kSector, records));
        // PACKAGE_INFO.BIN: a 16-byte header ("pack" at +4, the count at +8),
        // then {name hash, start sector in the low 20 bits, size in the low
        // 28 bits} for each file.
        const auto entry = [&](std::uint32_t hash) -> std::optional<std::vector<std::uint8_t>> {
            const std::uint32_t count = u32(records, 8u);
            for (std::uint32_t i = 0; i < count; ++i) {
                const std::size_t at = 16u + static_cast<std::size_t>(i) * 12u;
                if (at + 12u > records.size()) break;
                if (u32(records, at) != hash) continue;
                const std::uint32_t sector = u32(records, at + 4u) & 0xFFFFFu;
                const std::uint32_t size = u32(records, at + 8u) & 0x0FFFFFFFu;
                if (std::uint64_t(sector) * kSector + size > package->size) return std::nullopt;
                std::vector<std::uint8_t> bytes(size);
                bytes.resize(disc.read((std::uint64_t(package->lba) + sector) * kSector, bytes));
                return bytes;
            }
            return std::nullopt;
        };
        std::map<std::uint32_t, std::vector<std::uint8_t>> entries;
        for (int s = 0; s < kSheetCount; ++s) {
            const SheetSource &source = kSources[static_cast<std::size_t>(s)];
            if (!entries.contains(source.entry)) {
                auto bytes = entry(source.entry);
                if (!bytes) {
                    out.problem = std::string("the entry with ") + source.what + " is not on the disc";
                    return out;
                }
                entries.emplace(source.entry, std::move(*bytes));
            }
            // Every GIM in the entry, until the one with this fingerprint.
            const std::vector<std::uint8_t> &bytes = entries.at(source.entry);
            static constexpr std::string_view kMagic{"MIG.00.1PSP", 11u};
            bool found = false;
            for (std::size_t at = 0; at + 16u <= bytes.size() && !found; ++at) {
                if (std::memcmp(&bytes[at], kMagic.data(), kMagic.size()) != 0) continue;
                const std::size_t end = std::min(bytes.size(), at + 16u + u32(bytes, at + 20u));
                auto picture = decode_gim(bytes, at, end);
                if (!picture || picture->width != source.width || picture->height != source.height ||
                    fingerprint(picture->rgba) != source.fingerprint)
                    continue;
                out.sheets[static_cast<std::size_t>(s)] = std::move(*picture);
                found = true;
            }
            if (!found) {
                out.problem = std::string(source.what) + " are not the expected picture";
                return out;
            }
        }
    } catch (const std::exception &e) {
        out.problem = std::string("the disc cannot be read (") + e.what() + ")";
    }
    return out;
}

// --- Textures --------------------------------------------------------------

struct Texture {
    ImTextureData *data{};
    int width{};
    int height{};
};

Texture make_texture(int width, int height, const std::uint8_t *rgba) {
    Texture t;
    t.data = new ImTextureData();
    t.data->Create(ImTextureFormat_RGBA32, width, height);
    std::memcpy(t.data->GetPixels(), rgba, static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4u);
    ImGui::RegisterUserTexture(t.data);
    t.width = width;
    t.height = height;
    return t;
}

// A piece of a sheet, in the sheet's pixels.
struct Piece {
    Sheet sheet;
    int x, y, w, h;
};

// Where each piece sits in its picture.
constexpr Piece kHand{kPlates, 2, 50, 31, 24};
constexpr Piece kSilverHalf{kPlates, 77, 54, 14, 26};  // 20 rows of plate, then 6 of its shadow
constexpr Piece kBlueHalf{kPlates, 2, 77, 16, 20};
constexpr Piece kArrowUp{kPlates, 39, 50, 13, 8};
constexpr Piece kArrowDown{kPlates, 39, 65, 13, 7};
constexpr Piece kCell{kCells, 6, 93, 18, 26};  // a key, its bevelled frame and face
constexpr Piece kCross{kSymbols, 111, 111, 12, 12};
constexpr Piece kCircle{kSymbols, 75, 111, 12, 12};
constexpr Piece kTriangle{kSymbols, 57, 111, 12, 12};
constexpr Piece kSquare{kSymbols, 93, 111, 12, 12};
constexpr Piece kStart{kSymbols, 40, 93, 29, 12};
// A plate's rows: 0-3 its top bevel, 4-15 its flat face, 16-19 its bottom
// bevel. A cell's frame: 3 pixels at the left and top, 6 at the right and
// bottom (the inner shading and the lower bevel).
constexpr float kPlateRows = 20.0f;
constexpr float kFaceTop = 4.0f;
constexpr float kFaceBottom = 16.0f;
constexpr float kCellLeft = 3.0f, kCellTop = 3.0f, kCellRight = 6.0f, kCellBottom = 6.0f;

struct Art {
    bool tried{};
    bool ready{};
    std::array<Texture, kSheetCount> sheets;
    Texture blue_cell;  // the key cell in the blue plate's colours, for the selected key
};

Art &art() {
    static Art value;
    return value;
}

// The selected key: the silver cell (its face one colour) in the blue plate's
// colours. The game's own blue cell has a dark stripe down its face, which a
// key's width stretches into a dark block behind the letter. Silver shades
// map to the blue plate's: its dark lower edge, its face, its light rim.
std::vector<std::uint8_t> blue_cell(const Picture &cells) {
    struct Anchor {
        float shade;
        float r, g, b;
    };
    static constexpr std::array<Anchor, 4> kAnchors{{
        {111.0f, 15.0f, 18.0f, 98.0f},
        {160.0f, 49.0f, 34.0f, 171.0f},
        {208.0f, 58.0f, 46.0f, 200.0f},
        {248.0f, 186.0f, 201.0f, 254.0f},
    }};
    std::vector<std::uint8_t> out(static_cast<std::size_t>(kCell.w) * kCell.h * 4u);
    for (int y = 0; y < kCell.h; ++y)
        for (int x = 0; x < kCell.w; ++x) {
            const std::uint8_t *p = &cells.rgba[(static_cast<std::size_t>(kCell.y + y) * cells.width + kCell.x + x) * 4u];
            const float v = (p[0] + p[1] + p[2]) / 3.0f;
            Anchor c = kAnchors.front();
            if (v >= kAnchors.back().shade) {
                c = kAnchors.back();
            } else if (v > kAnchors.front().shade) {
                for (std::size_t i = 0; i + 1u < kAnchors.size(); ++i) {
                    const Anchor &a = kAnchors[i];
                    const Anchor &b = kAnchors[i + 1u];
                    if (v > b.shade) continue;
                    const float t = (v - a.shade) / (b.shade - a.shade);
                    c = {v, a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t};
                    break;
                }
            }
            std::uint8_t *q = &out[(static_cast<std::size_t>(y) * kCell.w + x) * 4u];
            q[0] = static_cast<std::uint8_t>(std::lround(c.r));
            q[1] = static_cast<std::uint8_t>(std::lround(c.g));
            q[2] = static_cast<std::uint8_t>(std::lround(c.b));
            q[3] = p[3];
        }
    return out;
}

// Asked by PortableKit before a dialog draws: the pictures are read with the
// first dialog (a moment's read from the disc), once.
bool available() {
    Art &a = art();
    if (a.tried) return a.ready;
    a.tried = true;
    const Pictures pictures = read_pictures();
    if (!pictures.problem.empty()) {
        std::cerr << "[dialogs] the game's menu pictures could not be read (" << pictures.problem
                  << "); the dialogs keep PortableKit's look\n";
        return false;
    }
    for (int s = 0; s < kSheetCount; ++s) {
        const Picture &p = pictures.sheets[static_cast<std::size_t>(s)];
        a.sheets[static_cast<std::size_t>(s)] = make_texture(p.width, p.height, p.rgba.data());
    }
    const std::vector<std::uint8_t> cell = blue_cell(pictures.sheets[kCells]);
    a.blue_cell = make_texture(kCell.w, kCell.h, cell.data());
    a.ready = true;
    std::cout << "[dialogs] the game's message, save and keyboard dialogs drawn in its menu look\n";
    return true;
}

// --- Drawing in the PSP's screen -------------------------------------------

constexpr ImU32 rgba(int r, int g, int b, int a = 255) {
    return IM_COL32(r, g, b, a);
}

constexpr ImU32 kNavy = rgba(28, 26, 70);
constexpr ImU32 kDarkText = rgba(34, 32, 64);
constexpr ImU32 kLightRim = rgba(236, 236, 248);
constexpr ImU32 kBlueRim = rgba(24, 20, 96);
constexpr ImU32 kBandRim = rgba(30, 26, 80);
constexpr ImU32 kWhite = rgba(255, 255, 255);

struct Canvas {
    ImDrawList *draw{};
    float left{};
    float top{};
    float s{1.0f};  // window pixels per PSP pixel

    [[nodiscard]] ImVec2 at(float x, float y) const { return {left + x * s, top + y * s}; }
};

Canvas canvas() {
    const ui::GamePicture picture = ui::game_picture();
    Canvas c;
    c.draw = ImGui::GetForegroundDrawList();
    c.left = picture.left;
    c.top = picture.top;
    c.s = picture.height / 272.0f;
    return c;
}

const Texture &sheet(Sheet s) { return art().sheets[static_cast<std::size_t>(s)]; }

// A piece at (x, y) w x h; `flip` mirrors it left to right.
void image(const Canvas &c, const Texture &t, float sx, float sy, float sw, float sh, float x, float y, float w,
           float h, bool flip = false, ImU32 tint = kWhite) {
    const ImVec2 uv0{sx / t.width, sy / t.height};
    const ImVec2 uv1{(sx + sw) / t.width, (sy + sh) / t.height};
    c.draw->AddImage(t.data->GetTexRef(), c.at(x, y), c.at(x + w, y + h), flip ? ImVec2{uv1.x, uv0.y} : uv0,
                     flip ? ImVec2{uv0.x, uv1.y} : uv1, tint);
}

void piece(const Canvas &c, const Piece &p, float x, float y, float h, bool flip = false) {
    const float w = p.w * h / p.h;
    image(c, sheet(p.sheet), static_cast<float>(p.x), static_cast<float>(p.y), static_cast<float>(p.w),
          static_cast<float>(p.h), x, y, w, h, flip);
}

float piece_width(const Piece &p, float h) { return p.w * h / p.h; }

// A plate `height` tall (its plate rows; the silver one's shadow hangs below):
// the half at that scale, mirrored at the right, the middle stretched from
// its innermost column.
void plate(const Canvas &c, float x, float y, float width, float height, bool blue) {
    const Piece &half = blue ? kBlueHalf : kSilverHalf;
    const Texture &t = sheet(half.sheet);
    const float k = height / kPlateRows;
    const float cap = half.w * k;
    const float tall = half.h * k;
    const float hx = static_cast<float>(half.x), hy = static_cast<float>(half.y);
    image(c, t, hx, hy, static_cast<float>(half.w), static_cast<float>(half.h), x, y, cap, tall);
    image(c, t, hx, hy, static_cast<float>(half.w), static_cast<float>(half.h), x + width - cap, y, cap, tall, true);
    // The middle: the inner column's centre, so the filter takes nothing beside
    // it, laid a little over both ends, whose inner edge the filter blends with
    // the next piece on the sheet.
    const float column = hx + half.w - 0.5f;
    const float over = 0.75f * k;
    c.draw->AddImage(t.data->GetTexRef(), c.at(x + cap - over, y), c.at(x + width - cap + over, y + tall),
                     {column / t.width, hy / t.height}, {column / t.width, (hy + half.h) / t.height});
}

struct Face {
    float l, t, r, b;
};

Face plate_face(float x, float y, float width, float height) {
    const float k = height / kPlateRows;
    const float cap = std::round(kSilverHalf.w * k * 4.0f) / 4.0f;
    return {x + cap + 2.0f, y + kFaceTop * k, x + width - cap - 2.0f, y + kFaceBottom * k};
}

// A key: the cell's frame at one cell pixel to the PSP pixel, its face stretched.
void cell(const Canvas &c, float x, float y, float w, float h, bool blue) {
    const Texture &t = blue ? art().blue_cell : sheet(kCell.sheet);
    const float ox = blue ? 0.0f : static_cast<float>(kCell.x);
    const float oy = blue ? 0.0f : static_cast<float>(kCell.y);
    const float sx[4] = {0.0f, kCellLeft, kCell.w - kCellRight, static_cast<float>(kCell.w)};
    const float sy[4] = {0.0f, kCellTop, kCell.h - kCellBottom, static_cast<float>(kCell.h)};
    const float dx[4] = {0.0f, kCellLeft, w - kCellRight, w};
    const float dy[4] = {0.0f, kCellTop, h - kCellBottom, h};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            image(c, t, ox + sx[i], oy + sy[j], sx[i + 1] - sx[i], sy[j + 1] - sy[j], x + dx[i], y + dy[j],
                  dx[i + 1] - dx[i], dy[j + 1] - dy[j]);
}

Face cell_face(float x, float y, float w, float h) {
    return {x + kCellLeft + 0.5f, y + kCellTop + 0.5f, x + w - kCellRight - 0.5f, y + h - kCellBottom - 0.5f};
}

// --- Text ------------------------------------------------------------------

// Text as the game's menus draw it: the face the game's text uses, `size` PSP
// pixels, with a rim around each letter, heavier with `bold`, and `track`
// PSP pixels between letters.
struct TextStyle {
    float size{10.0f};
    ImU32 fill{kWhite};
    ImU32 rim{kNavy};
    float rim_width{0.5f};
    bool bold{};
    float track{};
};

std::vector<std::string> characters(std::string_view text) {
    std::vector<std::string> out;
    for (std::size_t i = 0; i < text.size();) {
        const auto byte = static_cast<unsigned char>(text[i]);
        const std::size_t n = byte >= 0xF0u ? 4u : byte >= 0xE0u ? 3u : byte >= 0xC0u ? 2u : 1u;
        out.emplace_back(text.substr(i, n));
        i += n;
    }
    return out;
}

// Width of the text's advance, and its ink's top and bottom from the line's top,
// in PSP pixels.
struct Extent {
    float width{};
    float ink_top{};
    float ink_bottom{};
};

Extent measure(const Canvas &c, std::string_view text, const TextStyle &style) {
    ImFont *font = ui::game_text_font();
    const float px = style.size * c.s;
    ImFontBaked *baked = font->GetFontBaked(px);
    Extent e;
    e.ink_top = 1.0e9f;
    e.ink_bottom = -1.0e9f;
    const std::vector<std::string> chars = characters(text);
    for (std::size_t i = 0; i < chars.size(); ++i) {
        unsigned int cp = 0;
        ImTextCharFromUtf8(&cp, chars[i].c_str(), nullptr);
        if (const ImFontGlyph *g = baked->FindGlyph(static_cast<ImWchar>(cp)); g != nullptr) {
            if (g->Visible) {
                e.ink_top = std::min(e.ink_top, g->Y0 / c.s);
                e.ink_bottom = std::max(e.ink_bottom, g->Y1 / c.s);
            }
            e.width += g->AdvanceX / c.s;
        }
        if (i + 1u < chars.size()) e.width += style.track;
    }
    if (e.ink_top > e.ink_bottom) e.ink_top = e.ink_bottom = 0.0f;
    return e;
}

// Draws the text with its line's top-left at (x, y); returns the advance.
float text_at(const Canvas &c, float x, float y, std::string_view text, const TextStyle &style) {
    ImFont *font = ui::game_text_font();
    const float px = style.size * c.s;
    const std::vector<std::string> chars = characters(text);
    std::vector<float> pen;
    float advance = 0.0f;
    ImFontBaked *baked = font->GetFontBaked(px);
    for (std::size_t i = 0; i < chars.size(); ++i) {
        pen.push_back(advance);
        unsigned int cp = 0;
        ImTextCharFromUtf8(&cp, chars[i].c_str(), nullptr);
        if (const ImFontGlyph *g = baked->FindGlyph(static_cast<ImWchar>(cp))) advance += g->AdvanceX / c.s;
        if (i + 1u < chars.size()) advance += style.track;
    }
    const auto pass = [&](float dx, float dy, ImU32 colour) {
        for (std::size_t i = 0; i < chars.size(); ++i)
            c.draw->AddText(font, px, c.at(x + pen[i] + dx, y + dy), colour, chars[i].c_str());
    };
    // The rim: the text drawn around the letters in twelve directions.
    if (style.rim_width > 0.0f)
        for (int k = 0; k < 12; ++k) {
            const float angle = static_cast<float>(k) * 3.14159265f / 6.0f;
            pass(std::cos(angle) * style.rim_width, std::sin(angle) * style.rim_width, style.rim);
        }
    if (style.bold) {
        // The game's menu text is heavy: the face once more, a hair to each side.
        pass(-0.15f, 0.0f, style.fill);
        pass(0.15f, 0.0f, style.fill);
    }
    pass(0.0f, 0.0f, style.fill);
    return advance;
}

enum class Align { Left, Centre, Right };

// Text with its ink centred on (x, y) (a letter with a descender sits low on
// its line, so the ink, not the line, is centred), aligned on x as asked.
void text_centred(const Canvas &c, float x, float y, std::string_view text, const TextStyle &style,
                  Align align = Align::Centre) {
    const Extent e = measure(c, text, style);
    const float left = align == Align::Left ? x : align == Align::Right ? x - e.width : x - e.width * 0.5f;
    text_at(c, left, y - (e.ink_top + e.ink_bottom) * 0.5f, text, style);
}

// Text centred inside a face, made smaller until the ink and its rim fit it.
void face_text(const Canvas &c, const Face &f, std::string_view text, bool selected, float size, float drop = 0.0f,
               float track = 0.9f, std::optional<ImU32> fill = std::nullopt) {
    TextStyle style;
    style.bold = true;
    style.track = track;
    style.fill = fill.value_or(selected ? kWhite : kDarkText);
    style.rim = selected ? kBlueRim : kLightRim;
    style.rim_width = 1.0f;
    for (style.size = size; style.size > 5.0f; style.size -= 0.5f) {
        const Extent e = measure(c, text, style);
        const float spare = 2.0f * style.rim_width + 0.5f;
        if (e.width + spare <= f.r - f.l && e.ink_bottom - e.ink_top + spare <= f.b - f.t) break;
    }
    text_centred(c, (f.l + f.r) * 0.5f, (f.t + f.b) * 0.5f + drop, text, style);
}

// --- The page --------------------------------------------------------------

// The page title like the game's: a wide serif, lavender blue at the top of
// the letters shading to near white at the bottom, a thin near-black rim and
// a dark soft shadow down and to the right. The serif is Goudy Bookletter
// 1911, compiled in (port/CMakeLists.txt); D012_TITLE_FONT=<file> tries
// another face.
// How much heavier the title is drawn than the face, in PSP pixels to each
// side: the face has one weight, a little lighter than the game's titles
// (D012_TITLE_HEAVIER tries others).
float g_title_heavier = 0.3f;

ImFont *title_font() {
    static ImFont *font = nullptr;
    static bool tried = false;
    if (!tried) {
        tried = true;
        if (const char *heavier = portablekit::env("TITLE_HEAVIER")) g_title_heavier = std::strtof(heavier, nullptr);
        if (const char *path = portablekit::env("TITLE_FONT"); path != nullptr && *path != '\0') {
            font = ImGui::GetIO().Fonts->AddFontFromFileTTF(path, 0.0f);
            if (font == nullptr) std::cerr << "[dialogs] D012_TITLE_FONT could not be loaded\n";
        }
        if (font == nullptr) {
            ImFontConfig config;
            config.FontDataOwnedByAtlas = false;  // the program's own bytes
            font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<unsigned char *>(kTitleFont),
                                                              static_cast<int>(kTitleFontSize), 0.0f, &config);
        }
    }
    return font != nullptr ? font : ui::game_text_font();
}

void title(const Canvas &c, std::string_view text) {
    ImFont *font = title_font();
    const float px = 22.0f * c.s;
    constexpr float kWiden = 1.12f;
    const ImVec2 origin = c.at(8.0f, 1.0f);
    ImDrawList *d = c.draw;
    // Each pass's vertices, widened about the title's left edge afterwards.
    const auto widen = [&](int from) {
        for (int i = from; i < d->VtxBuffer.Size; ++i) d->VtxBuffer[i].pos.x = origin.x + (d->VtxBuffer[i].pos.x - origin.x) * kWiden;
    };
    // The shadow: the letters many times over, faint, spread over 1.6 PSP pixels.
    const std::string s(text);
    {
        const int from = d->VtxBuffer.Size;
        constexpr int kTaps = 16;
        for (int k = 0; k < kTaps; ++k) {
            const float angle = static_cast<float>(k) * 6.2831853f / kTaps;
            const float r = (k % 2 == 0 ? 1.6f : 0.8f) * c.s;
            d->AddText(font, px, {origin.x + 2.5f * c.s + std::cos(angle) * r, origin.y + 2.5f * c.s + std::sin(angle) * r},
                       rgba(0, 0, 0, 26), s.c_str());
        }
        widen(from);
    }
    {
        const int from = d->VtxBuffer.Size;
        for (int k = 0; k < 8; ++k) {
            const float angle = static_cast<float>(k) * 3.14159265f / 4.0f;
            for (const float shift : {-g_title_heavier, g_title_heavier})
                d->AddText(font, px,
                           {origin.x + (std::cos(angle) * 0.35f + shift) * c.s, origin.y + std::sin(angle) * 0.35f * c.s},
                           rgba(26, 27, 33), s.c_str());
        }
        widen(from);
    }
    const int from = d->VtxBuffer.Size;
    // Heavier: the letters drawn again a little to each side.
    for (const float shift : {-g_title_heavier, 0.0f, g_title_heavier})
        d->AddText(font, px, {origin.x + shift * c.s, origin.y}, kWhite, s.c_str());
    widen(from);
    // The fill's colour by height within the letters.
    float top = 1.0e9f, bottom = -1.0e9f;
    for (int i = from; i < d->VtxBuffer.Size; ++i) {
        top = std::min(top, d->VtxBuffer[i].pos.y);
        bottom = std::max(bottom, d->VtxBuffer[i].pos.y);
    }
    const auto colour = [&](float y) {
        const float t = std::clamp((y - top) / std::max(1.0f, bottom - top), 0.0f, 1.0f);
        const float base_r = t < 0.1f ? 158.0f : 163.0f, base_g = t < 0.1f ? 166.0f : 172.0f, base_b = t < 0.1f ? 214.0f : 210.0f;
        const float u = t < 0.55f ? 0.0f : std::pow((t - 0.55f) / 0.45f, 1.3f);
        return rgba(static_cast<int>(base_r + (214.0f - base_r) * u), static_cast<int>(base_g + (230.0f - base_g) * u),
                    static_cast<int>(base_b + (240.0f - base_b) * u));
    };
    for (int i = from; i < d->VtxBuffer.Size; ++i) d->VtxBuffer[i].col = colour(d->VtxBuffer[i].pos.y);
}

// A rectangle filled top to bottom from one colour to another.
void vertical(const Canvas &c, float x0, float y0, float x1, float y1, ImU32 top, ImU32 bottom) {
    c.draw->AddRectFilledMultiColor(c.at(x0, y0), c.at(x1, y1), top, top, bottom, bottom);
}

void line(const Canvas &c, float y, ImU32 colour, float width) {
    c.draw->AddLine(c.at(0.0f, y), c.at(480.0f, y), colour, std::max(1.0f, width * c.s));
}

// The page behind the save list and the keyboard: the lavender ground, the
// feather at the right, the title band and its rules, as at the top of Data
// Config. The game shows through it only faintly.
void page(const Canvas &c, std::string_view heading) {
    vertical(c, 0.0f, 0.0f, 480.0f, 272.0f, rgba(236, 236, 248, 248), rgba(196, 196, 228, 250));
    const Texture &feather = sheet(kFeather);
    image(c, feather, 0.0f, 0.0f, 256.0f, 256.0f, 480.0f - 260.0f + 40.0f, 10.0f, 260.0f, 260.0f, false,
          rgba(255, 255, 255, 115));
    vertical(c, 0.0f, 0.0f, 480.0f, 34.0f, rgba(255, 255, 255, 200), rgba(255, 255, 255, 0));
    line(c, 33.0f, rgba(120, 116, 170), 1.0f);
    line(c, 35.0f, rgba(255, 255, 255, 200), 0.5f);
    title(c, heading);
}

// The dark band under the title with the instruction line ("Select a category
// to edit." on Data Config): strongest from a little in from the left, fading
// out after the middle.
void instruction(const Canvas &c, std::string_view text) {
    const float y0 = 42.0f, y1 = 66.0f;
    const auto alpha = [](float t) {
        return t > 0.55f ? 230.0f * (1.0f - (t - 0.55f) / 0.45f) : 230.0f * std::min(1.0f, t / 0.12f + 0.35f);
    };
    constexpr int kSteps = 24;
    for (int i = 0; i < kSteps; ++i) {
        const float t0 = static_cast<float>(i) / kSteps, t1 = static_cast<float>(i + 1) / kSteps;
        const ImU32 a = rgba(70, 66, 150, static_cast<int>(alpha(t0)));
        const ImU32 b = rgba(70, 66, 150, static_cast<int>(alpha(t1)));
        c.draw->AddRectFilledMultiColor(c.at(480.0f * t0, y0), c.at(480.0f * t1, y1), a, b, b, a);
    }
    c.draw->AddLine(c.at(0.0f, y0), c.at(480.0f, y0), rgba(200, 200, 240, 160), std::max(1.0f, 0.5f * c.s));
    TextStyle style;
    style.bold = true;
    style.track = 0.9f;
    style.rim = kBandRim;
    style.rim_width = 1.0f;
    text_centred(c, 200.0f, 54.0f, text, style);
}

// The game's button symbols for what the buttons do: confirm and back follow
// the menu's "Confirm button" setting (south: cross confirms).
const Piece &confirm_symbol(bool south) { return south ? kCross : kCircle; }
const Piece &back_symbol(bool south) { return south ? kCircle : kCross; }

struct Hint {
    const Piece *symbol;  // null: the label alone
    std::string label;
};

// Hints right-aligned at `right`, at height y (their middle), in `style`.
void hints_at(const Canvas &c, const std::vector<Hint> &items, float right, float y, const TextStyle &style,
              float symbol_height = 12.0f) {
    float x = right;
    for (auto it = items.rbegin(); it != items.rend(); ++it) {
        const Extent e = measure(c, it->label, style);
        x -= e.width;
        text_centred(c, x, y, it->label, style, Align::Left);
        if (it->symbol != nullptr) {
            const float w = piece_width(*it->symbol, symbol_height);
            x -= w + 3.0f;
            piece(c, *it->symbol, x, y - symbol_height * 0.5f, symbol_height);
        }
        x -= 11.0f;
    }
}

// The bar at the foot of the page with the hints at its right.
void hint_bar(const Canvas &c, const std::vector<Hint> &items) {
    vertical(c, 0.0f, 248.0f, 480.0f, 268.0f, rgba(255, 255, 255, 215), rgba(255, 255, 255, 155));
    c.draw->AddLine(c.at(0.0f, 248.0f), c.at(480.0f, 248.0f), rgba(140, 136, 190), std::max(1.0f, 0.5f * c.s));
    TextStyle style;
    style.fill = kNavy;
    style.rim = kWhite;
    hints_at(c, items, 474.0f, 258.0f, style);
}

bool hovering(const Canvas &c, float x0, float y0, float x1, float y1) {
    return ImGui::IsMouseHoveringRect(c.at(x0, y0), c.at(x1, y1), false);
}

void pointer_over(const Canvas &c, float x0, float y0, float x1, float y1, int index, ui::SkinPointer &pointer) {
    if (!hovering(c, x0, y0, x1, y1)) return;
    pointer.hovered = index;
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) pointer.clicked = index;
}

// How far the hand pokes toward its choice this frame. The game's does every
// half second: 2, 3, 4, 3, 2 PSP pixels over five frames, then rests for 25
// (measured frame by frame on its Data Config page).
float poke() {
    static constexpr float kPoke[] = {2.0f, 3.0f, 4.0f, 3.0f, 2.0f};
    const auto frame = static_cast<long long>(ImGui::GetTime() * 60.0) % 30;
    return frame < 5 ? kPoke[frame] : 0.0f;
}

// The pointing hand beside a choice, its tip on the choice's left end.
void hand(const Canvas &c, float x, float y, float h = 19.0f) { piece(c, kHand, x - 16.0f + poke(), y - 1.0f, h); }

// --- The message -----------------------------------------------------------

// The message broken into lines no wider than `width`: at its own line
// breaks, and between words where a line is too long.
std::vector<std::string> wrap(const Canvas &c, const std::string &text, float width, const TextStyle &style) {
    std::vector<std::string> lines;
    std::size_t start = 0;
    for (;;) {
        const std::size_t end = text.find('\n', start);
        const std::string paragraph = text.substr(start, end == std::string::npos ? std::string::npos : end - start);
        std::string current;
        std::size_t at = 0;
        while (at <= paragraph.size()) {
            const std::size_t space = paragraph.find(' ', at);
            const std::string word = paragraph.substr(at, space == std::string::npos ? std::string::npos : space - at);
            const std::string candidate = current.empty() ? word : current + " " + word;
            if (!current.empty() && measure(c, candidate, style).width > width) {
                lines.push_back(current);
                current = word;
            } else {
                current = candidate;
            }
            if (space == std::string::npos) break;
            at = space + 1u;
        }
        lines.push_back(current);
        if (end == std::string::npos) break;
        start = end + 1u;
    }
    while (lines.size() > 1u && lines.back().empty()) lines.pop_back();
    return lines;
}

void message(const ui::MessageSkinView &view, ui::SkinPointer &pointer) {
    const Canvas c = canvas();
    const ImVec2 display = ImGui::GetIO().DisplaySize;
    c.draw->AddRectFilled({0.0f, 0.0f}, display, rgba(20, 18, 50, 120));
    TextStyle style;
    style.bold = true;
    style.track = 0.6f;
    style.rim = kBandRim;
    style.rim_width = 1.0f;
    const std::vector<std::string> lines = wrap(c, view.text, 440.0f, style);
    constexpr float kLine = 14.0f;
    const float band = 22.0f + kLine * static_cast<float>(lines.size()) + 46.0f;
    const float top = std::round((272.0f - band) * 0.5f);
    // The band, strongest in its middle.
    vertical(c, 0.0f, top, 480.0f, top + band * 0.5f, rgba(66, 62, 146, 198), rgba(66, 62, 146, 238));
    vertical(c, 0.0f, top + band * 0.5f, 480.0f, top + band, rgba(66, 62, 146, 238), rgba(66, 62, 146, 198));
    c.draw->AddLine(c.at(0.0f, top), c.at(480.0f, top), rgba(210, 210, 250, 200), std::max(1.0f, 0.5f * c.s));
    c.draw->AddLine(c.at(0.0f, top + band), c.at(480.0f, top + band), rgba(210, 210, 250, 200),
                    std::max(1.0f, 0.5f * c.s));
    for (std::size_t n = 0; n < lines.size(); ++n)
        text_centred(c, 240.0f, top + 12.0f + kLine * static_cast<float>(n) + 5.0f, lines[n], style);

    // The choices on the game's plates.
    const float y = top + 18.0f + kLine * static_cast<float>(lines.size());
    const std::vector<const char *> labels = view.question ? std::vector<const char *>{"Yes", "No"}
                                                           : std::vector<const char *>{"OK"};
    constexpr float kWidth = 80.0f, kGap = 20.0f, kHeight = 18.0f;
    const float left = 240.0f - (static_cast<float>(labels.size()) * kWidth + (labels.size() - 1u) * kGap) * 0.5f;
    for (std::size_t n = 0; n < labels.size(); ++n) {
        const bool selected = view.question ? (n == 1u) == view.no_selected : true;
        const float x = left + static_cast<float>(n) * (kWidth + kGap);
        plate(c, x, y, kWidth, kHeight, selected);
        face_text(c, plate_face(x, y, kWidth, kHeight), labels[n], selected, 9.0f);
        if (selected) hand(c, x, y);
        pointer_over(c, x, y, x + kWidth, y + kHeight, static_cast<int>(n), pointer);
    }

    std::vector<Hint> items;
    if (!view.question) {
        items.push_back({&confirm_symbol(view.confirm_south), "OK"});
    } else {
        items.push_back({&confirm_symbol(view.confirm_south), "Choose"});
        if (view.allow_back) items.push_back({&back_symbol(view.confirm_south), "Back"});
    }
    TextStyle hint;
    hint.size = 9.0f;
    hint.rim = kBandRim;
    hints_at(c, items, 470.0f, top + band - 8.0f, hint);
}

// --- The save list -----------------------------------------------------------

struct Icons {
    std::map<std::uint64_t, Texture> textures;  // by the PNG's fingerprint
    std::vector<std::uint64_t> order;           // oldest first
};

Icons &icons() {
    static Icons value;
    return value;
}

// A save's icon as a texture: decoded once, kept for the next lists; at most
// 40 kept, the oldest's texture reused.
const Texture *icon(const std::vector<std::uint8_t> &png) {
    if (png.empty()) return nullptr;
    Icons &cache = icons();
    const std::uint64_t key = fingerprint(png);
    if (const auto found = cache.textures.find(key); found != cache.textures.end()) return &found->second;
    int width = 0, height = 0, channels = 0;
    stbi_uc *pixels = stbi_load_from_memory(png.data(), static_cast<int>(png.size()), &width, &height, &channels, 4);
    if (pixels == nullptr) return nullptr;
    Texture t;
    if (cache.order.size() >= 40u) {
        const auto oldest = cache.textures.find(cache.order.front());
        if (oldest != cache.textures.end() && oldest->second.width == width && oldest->second.height == height) {
            t = oldest->second;
            std::memcpy(t.data->GetPixels(), pixels, static_cast<std::size_t>(width) * height * 4u);
            if (t.data->Status == ImTextureStatus_OK || t.data->Status == ImTextureStatus_WantUpdates)
                ImTextureDataQueueUpload(t.data, 0, 0, width, height);
            cache.textures.erase(oldest);
            cache.order.erase(cache.order.begin());
        }
    }
    if (t.data == nullptr) t = make_texture(width, height, pixels);
    stbi_image_free(pixels);
    cache.order.push_back(key);
    return &cache.textures.emplace(key, t).first->second;
}

std::string slot_name(const ui::SaveSlot &slot) {
    return slot.exists ? (slot.title.empty() ? slot.name : slot.title) : "New Data";
}

// The first slot listed: kept from one frame to the next, moved only as far
// as the selected slot needs.
std::size_t g_first_listed = 0;

void save_list(const ui::SaveListSkinView &view, ui::SkinPointer &pointer) {
    const Canvas c = canvas();
    const std::vector<ui::SaveSlot> &slots = *view.slots;
    const char *heading = "Load", *line_text = "Select the data to load.", *verb = "Load";
    if (view.action == ui::SaveAction::Save) heading = "Save", line_text = "Select where to save.", verb = "Save";
    if (view.action == ui::SaveAction::Delete)
        heading = "Delete", line_text = "Select the data to delete.", verb = "Delete";
    page(c, heading);
    instruction(c, line_text);

    // The slots as plates down the left, the name in each plate's face.
    constexpr float kLeft = 40.0f, kWidth = 226.0f, kTop = 72.0f, kRow = 21.0f, kHeight = 18.0f;
    constexpr std::size_t kVisible = 8u;
    const auto at = std::find(view.shown.begin(), view.shown.end(), view.selected);
    const std::size_t position = at == view.shown.end() ? 0u : static_cast<std::size_t>(at - view.shown.begin());
    if (position < g_first_listed) g_first_listed = position;
    if (position >= g_first_listed + kVisible) g_first_listed = position + 1u - kVisible;
    if (g_first_listed + kVisible > view.shown.size())
        g_first_listed = view.shown.size() > kVisible ? view.shown.size() - kVisible : 0u;
    for (std::size_t n = 0; n < kVisible && g_first_listed + n < view.shown.size(); ++n) {
        const std::size_t index = view.shown[g_first_listed + n];
        const bool selected = index == view.selected;
        const float y = kTop + static_cast<float>(n) * kRow;
        plate(c, kLeft, y, kWidth, kHeight, selected);
        face_text(c, plate_face(kLeft, y, kWidth, kHeight), slot_name(slots[index]), selected, 9.0f);
        if (selected) hand(c, kLeft, y);
        pointer_over(c, kLeft, y, kLeft + kWidth, y + kHeight, static_cast<int>(index), pointer);
    }
    if (g_first_listed > 0u) piece(c, kArrowUp, kLeft + kWidth * 0.5f - 6.0f, kTop - 8.0f, 6.0f);
    if (g_first_listed + kVisible < view.shown.size())
        piece(c, kArrowDown, kLeft + kWidth * 0.5f - 6.0f, kTop + kVisible * kRow - 1.0f, 6.0f);

    // The selected save on the right: its icon, title and the game's detail.
    constexpr float kPanelX = 290.0f, kPanelY = 70.0f, kPanelW = 180.0f, kPanelH = 170.0f;
    c.draw->AddRectFilled(c.at(kPanelX, kPanelY), c.at(kPanelX + kPanelW, kPanelY + kPanelH), rgba(255, 255, 255, 150),
                          3.0f * c.s);
    c.draw->AddRect(c.at(kPanelX, kPanelY), c.at(kPanelX + kPanelW, kPanelY + kPanelH), rgba(150, 146, 200), 3.0f * c.s,
                    0, std::max(1.0f, 0.6f * c.s));
    const ui::SaveSlot &shown = slots[view.selected < slots.size() ? view.selected : 0u];
    const float ix = kPanelX + (kPanelW - 144.0f) * 0.5f, iy = kPanelY + 8.0f;
    if (const Texture *t = shown.exists ? icon(shown.icon) : nullptr; t != nullptr) {
        c.draw->AddRectFilled(c.at(ix - 2.0f, iy - 2.0f), c.at(ix + 146.0f, iy + 82.0f), rgba(90, 86, 150));
        c.draw->AddImage(t->data->GetTexRef(), c.at(ix, iy), c.at(ix + 144.0f, iy + 80.0f));
    } else {
        c.draw->AddRectFilled(c.at(ix - 2.0f, iy - 2.0f), c.at(ix + 146.0f, iy + 82.0f), rgba(210, 208, 235));
        c.draw->AddRect(c.at(ix - 2.0f, iy - 2.0f), c.at(ix + 146.0f, iy + 82.0f), rgba(150, 146, 200), 0.0f, 0,
                        std::max(1.0f, 0.5f * c.s));
        TextStyle faint;
        faint.size = 12.0f;
        faint.fill = rgba(110, 106, 160);
        faint.rim = kWhite;
        text_centred(c, ix + 72.0f, iy + 40.0f, shown.exists ? slot_name(shown) : "New Data", faint);
    }
    TextStyle name;
    name.size = 12.0f;
    name.fill = kNavy;
    name.rim = kWhite;
    TextStyle detail = name;
    detail.size = 10.0f;
    detail.fill = rgba(50, 48, 100);
    c.draw->PushClipRect(c.at(kPanelX, kPanelY), c.at(kPanelX + kPanelW - 4.0f, kPanelY + kPanelH), true);
    text_at(c, kPanelX + 10.0f, kPanelY + 96.0f, slot_name(shown), name);
    std::vector<std::string> lines;
    if (shown.exists) {
        lines = wrap(c, shown.detail, kPanelW - 20.0f, detail);
    } else if (view.action == ui::SaveAction::Save) {
        lines = {"Save the game here."};
    }
    for (std::size_t n = 0; n < lines.size() && n < 8u; ++n)
        text_at(c, kPanelX + 10.0f, kPanelY + 114.0f + 13.0f * static_cast<float>(n), lines[n], detail);
    c.draw->PopClipRect();

    hint_bar(c, {{&confirm_symbol(view.confirm_south), verb}, {&back_symbol(view.confirm_south), "Back"}});
}

// --- The keyboard -----------------------------------------------------------

void keyboard(const ui::KeyboardSkinView &view, ui::SkinPointer &pointer) {
    const Canvas c = canvas();
    page(c, view.title.empty() ? std::string_view("Enter Text") : std::string_view(view.title));
    instruction(c, view.prompt.empty() ? std::string_view("Enter text.") : std::string_view(view.prompt));

    // The text on a wide silver plate, the count at its right.
    constexpr float kFieldX = 60.0f, kFieldY = 72.0f, kFieldW = 360.0f, kFieldH = 20.0f;
    plate(c, kFieldX, kFieldY, kFieldW, kFieldH, false);
    const Face field = plate_face(kFieldX, kFieldY, kFieldW, kFieldH);
    TextStyle typed;
    typed.bold = true;
    typed.track = 0.6f;
    typed.fill = kDarkText;
    typed.rim = kLightRim;
    typed.rim_width = 1.0f;
    const float middle = (field.t + field.b) * 0.5f;
    const Extent all = measure(c, view.text.empty() ? std::string_view("A") : std::string_view(view.text), typed);
    const float line_top = middle - (all.ink_top + all.ink_bottom) * 0.5f;
    c.draw->PushClipRect(c.at(field.l, field.t - 2.0f), c.at(field.r - 40.0f, field.b + 2.0f), true);
    text_at(c, field.l + 6.0f, line_top, view.text, typed);
    if (view.caret_on) {
        const float x = field.l + 6.0f + measure(c, std::string_view(view.text).substr(0, view.cursor_byte), typed).width +
                        (view.cursor_byte > 0 ? typed.track : 0.0f);
        c.draw->AddRectFilled(c.at(x, field.t + 1.0f), c.at(x + 0.8f, field.b - 1.0f), rgba(60, 56, 160));
    }
    c.draw->PopClipRect();
    TextStyle count;
    count.size = 8.0f;
    count.fill = view.refused ? rgba(190, 30, 50) : view.full ? rgba(60, 56, 160) : rgba(90, 88, 130);
    count.rim = kLightRim;
    text_centred(c, field.r - 4.0f, middle,
                 std::to_string(view.length) + " / " + std::to_string(view.max_length), count, Align::Right);

    // The keys: the game's cells, the selected one blue. No hand: beside a key
    // inside a row it would cover the key before it.
    constexpr float kKeysX = 52.0f, kKeysWidth = 376.0f, kKeyH = 22.0f, kGap = 4.0f, kFirstRow = 100.0f;
    constexpr float kTextDrop = 1.25f;  // the face's middle sits above the key's: its bottom bevel is the thicker
    const float unit = (kKeysWidth + kGap) / 10.0f;
    for (std::size_t r = 0; r < view.rows.size(); ++r) {
        const bool actions = r + 1u == view.rows.size();
        const float y = kFirstRow + static_cast<float>(r) * (kKeyH + 3.0f) + (actions ? 2.0f : 0.0f);
        for (std::size_t k = 0; k < view.rows[r].size(); ++k) {
            const ui::KeyboardSkinKey &key = view.rows[r][k];
            const float x = kKeysX + key.left * unit;
            const float w = key.width * unit - kGap;
            cell(c, x, y, w, kKeyH, key.selected);
            std::optional<ImU32> ink;
            if (!key.enabled) ink = rgba(150, 148, 172);
            else if (key.lit && !key.selected && !key.character && key.label != "OK") ink = rgba(60, 56, 160);
            face_text(c, cell_face(x, y, w, kKeyH), key.label, key.selected, key.character ? 10.0f : 9.0f, kTextDrop,
                      key.character ? 0.0f : 0.6f, ink);
            pointer_over(c, x, y, x + w, y + kKeyH, static_cast<int>(r * 100u + k), pointer);
        }
    }

    const bool south = view.confirm_south;
    if (view.gamepad) {
        hint_bar(c, {{&confirm_symbol(south), "Type"},
                     {&back_symbol(south), "Delete"},
                     {&kSquare, "Shift"},
                     {&kTriangle, "Space"},
                     {&kStart, "OK"}});
    } else {
        hint_bar(c, {{nullptr, "Enter: OK"}, {nullptr, "Esc: Cancel"}, {nullptr, "Backspace: Delete"}});
    }
}

// --- The game's menu sounds ------------------------------------------------
//
// A dialog's sounds are the game's own menu sounds, played by the game's
// menu-sound function as its menus play them (main 0x088A24D4(0, sound, 1, 0),
// sounds from the menu table at 0x089CBE60: 0 the cursor, 1 a choice, 2
// back, 4 not allowed). The interface reports them (sound() below, on its
// thread); the game's thread plays them the next time it polls the dialog
// (the six Update and GetStatus calls, wrapped in install()). The function
// itself does nothing until the game's sound is set up (its flag byte), and
// the port checks the same flag first. D012_NO_DIALOG_SOUNDS=1: silent.

constexpr std::uint32_t kPlaySound = 0x088A24D4u;
constexpr std::uint32_t kSoundReady = 0x089D6C98u;
constexpr std::uint32_t kSoundManager = 0x09B4D2A0u;  // -> the sound manager
constexpr std::uint32_t kCursorSound = 10000u, kChoiceSound = 10001u, kBackSound = 10002u, kRefusedSound = 10003u;

struct SoundQueue {
    std::mutex lock;
    std::vector<std::uint32_t> sounds;
    bool failed = false;
};

SoundQueue &sound_queue() {
    static SoundQueue q;
    return q;
}

void sound(ui::DialogSound what) {
    std::uint32_t id = kCursorSound;
    switch (what) {
    case ui::DialogSound::Move: id = kCursorSound; break;
    case ui::DialogSound::Confirm: id = kChoiceSound; break;
    case ui::DialogSound::Back: id = kBackSound; break;
    case ui::DialogSound::Refused: id = kRefusedSound; break;
    }
    SoundQueue &q = sound_queue();
    const std::lock_guard<std::mutex> hold(q.lock);
    if (q.sounds.size() < 8u) q.sounds.push_back(id);
}

// On the game's thread, from a dialog's poll: plays what the interface asked for.
void play_sounds(psprecomp::Runtime &rt, const psprecomp::AllegrexContext &ctx) {
    std::vector<std::uint32_t> sounds;
    {
        SoundQueue &q = sound_queue();
        const std::lock_guard<std::mutex> hold(q.lock);
        if (q.sounds.empty() || q.failed) {
            q.sounds.clear();
            return;
        }
        sounds.swap(q.sounds);
    }
    psprecomp::GuestMemory &memory = rt.memory();
    if (memory.load8(kSoundReady) == 0u || memory.load32(kSoundManager) == 0u) return;
    for (const std::uint32_t id : sounds) {
        GuestRegs regs{};
        regs.a[0] = 0u;
        regs.a[1] = id;
        regs.a[2] = 1u;
        regs.a[3] = 0u;
        try {
            decomp::call_game(rt, ctx, kPlaySound, regs);
            // D012_TRACE_DIALOG_SOUNDS=1: each sound played, with what the game answered.
            static const bool trace = portablekit::env("TRACE_DIALOG_SOUNDS") != nullptr;
            if (trace) std::cout << "[dialogs] sound " << id << " -> " << regs.v0 << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "[dialogs] the game's menu sound failed (" << e.what() << "); the dialogs stay silent\n";
            SoundQueue &q = sound_queue();
            const std::lock_guard<std::mutex> hold(q.lock);
            q.failed = true;
            return;
        }
    }
}

// The polls the game makes every frame while a dialog is up.
constexpr std::uint32_t kDialogPolls[] = {
    0x95FC253Bu,  // sceUtilityMsgDialogUpdate
    0x9A1C91D7u,  // sceUtilityMsgDialogGetStatus
    0xD4B95FFBu,  // sceUtilitySavedataUpdate
    0x8874DBE0u,  // sceUtilitySavedataGetStatus
    0x4B85C861u,  // sceUtilityOskUpdate
    0xF3F76017u,  // sceUtilityOskGetStatus
};

void install_sounds(portablekit::HleRegistrar &hle) {
    if (portablekit::env("NO_DIALOG_SOUNDS") != nullptr) {
        std::cout << "[dialogs] D012_NO_DIALOG_SOUNDS: the dialogs are silent\n";
        return;
    }
    for (const std::uint32_t nid : kDialogPolls) {
        const portablekit::HleFunction poll = hle.bound_function("sceUtility", nid);
        if (!poll) {
            std::cerr << "[dialogs] a dialog poll is not bound; the dialogs are silent\n";
            return;
        }
    }
    for (const std::uint32_t nid : kDialogPolls) {
        const portablekit::HleFunction poll = hle.bound_function("sceUtility", nid);
        hle.bind("sceUtility", nid, [poll](psprecomp::Runtime &rt, psprecomp::AllegrexContext &ctx) {
            play_sounds(rt, ctx);
            poll(rt, ctx);
        });
    }
}

} // namespace

void install(portablekit::HleRegistrar &hle) {
    if (portablekit::env("NO_NATIVE_DIALOGS") != nullptr) {
        std::cout << "[dialogs] D012_NO_NATIVE_DIALOGS: the dialogs keep PortableKit's look\n";
        return;
    }
    install_sounds(hle);
    ui::DialogSkin skin;
    skin.available = available;
    skin.sound = sound;
    skin.message = message;
    skin.save_list = save_list;
    skin.keyboard = keyboard;
    ui::set_dialog_skin(skin);
}

#else

void install(portablekit::HleRegistrar &) {}

#endif

} // namespace d012::dialogs
