// The 10 Skills cheat (the Cheats page's row, d012_cheats.cpp): Story Mode
// lets the player hold ten Skills, on two pages of five, and L2 or R2 turns
// the page. MIT licence; see port/README.md.
//
// The Skills held are in the save (root + 0x3F548): a count and six ids. The
// game holds five; a sixth, just obtained, waits in the last slot while the
// game asks which one to throw away (main 0x08920244: more than five). Its
// lists of Skills are all built for that: the Skill bar (main 0x0893xxxx,
// on the world map in the battle overlay, in a gateway in the menu overlay)
// has six icons at six fixed places; a gateway's table of the Skills' state
// (menu 0x08B44F40) has six rows.
//
// So the game only ever sees the page on screen. The other page is packed
// into the sixth slot (d012_skill_pages_rules.hpp), which the game leaves
// alone while it holds five or fewer. Overrides of the list's functions (main
// 0x089200F0 add, 0x08920130 remove one, 0x0892017C remove several) keep it
// there, and put a Skill obtained with the page on screen full on the other
// page while that has room. Turning the page swaps the two: the list's five,
// the icons, and in a gateway the rows of the state table; the page slides
// right and fades out, the other comes in from the right (the Skill bar and
// the Moogle shop's throw-away window). A hint over the game names the buttons
// and the page. Skills in effect on the page turned away still count at the
// next battle.
//
// Without the cheat the overrides only keep the packed page in its slot, so
// the second page waits for the cheat to be turned on again; until the game
// makes a sixth Skill wait there.
//
// D012_TRACE_SKILLS=1 logs the list's changes and the page turns (=2 also the
// Skill bar's states, L2/R2 and the hint's place).
// D012_TEST_SKILLS=ID,... and D012_TEST_SKILLS_OTHER=ID,... put those Skills
// on the page on screen and on the other page when the world map or a
// gateway first lists them.
#include "d012_skill_pages.hpp"

#include "d012_button_prompts.hpp"
#include "d012_cheats.hpp"
#include "d012_decomp_harness.hpp"
#include "d012_input.hpp"
#include "d012_skill_pages_rules.hpp"

#include "input/bindings.hpp"
#include "portablekit/hle_extension.hpp"
#include "settings/settings.hpp"

#if defined(PORTABLEKIT_HAS_RENDERER)
#include "imgui.h"
#include "ui/ui.hpp"
#endif

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <string>
#include <vector>

namespace d012 {
namespace {

namespace decomp = d012::decomp;
namespace sp = skill_pages;
namespace ext = portablekit::hle_extension;

// The list, in the save.
constexpr std::uint32_t kSaveRoot = 0x089D712Cu;  // main: the save manager
constexpr std::uint32_t kSkillList = 0x3F548u;    // count, then six ids
constexpr std::uint32_t kSkillRow = 0x088BB88Cu;  // main: (id) -> its dungeon_skill.bin row, or 0
constexpr std::uint32_t kAdd = 0x089200F0u;       // main: (list, id)
constexpr std::uint32_t kRemoveAt = 0x08920130u;  // main: (list, index)
constexpr std::uint32_t kRemoveSet = 0x0892017Cu; // main: (list, five indices, -1 ends)

// A gateway's state table: six rows of 0x14 bytes (a state, then the Skill's
// row), in the field (main 0x089D8518 + 0x6C0), built by menu 0x08B44F40.
constexpr std::uint32_t kField = 0x089D8518u;
constexpr std::uint32_t kStateTable = 0x6C0u;
constexpr std::uint32_t kStateRow = 0x14u;
constexpr std::uint32_t kBuildStateTable = 0x08B44F40u; // menu
constexpr std::uint32_t kMapFillBar = 0x08B49B5Cu;      // battle: the world map lists the Skills
constexpr std::uint32_t kInEffect = 1u;                 // a row's state: in effect
// Skills used that last until the next battle stay in the list, in effect
// (main 0x08920718). As a battle starts, menu 0x08B39F78 hands each such row
// to menu 0x08B3FD7C, which sets the battle's terms; after it, menu
// 0x08B3A008 removes them (main 0x08920750 ends the effect).
constexpr std::uint32_t kApplyInEffect = 0x08B39F78u;  // menu: (gateway)
constexpr std::uint32_t kApplyOne = 0x08B3FD7Cu;       // menu: (gateway, row, 0)
constexpr std::uint32_t kEndInEffect = 0x08B3A008u;    // menu: (gateway, animate)
constexpr std::uint32_t kEndOne = 0x08920750u;         // main: (row) -> 1 when it was in effect

// The Skill bar (main).
constexpr std::uint32_t kBarUpdate = 0x089315E4u;     // (bar): each frame
constexpr std::uint32_t kBarSetIcon = 0x08930E10u;    // (bar, index, id)
constexpr std::uint32_t kBarMarkInEffect = 0x0893116Cu; // (bar, index)
constexpr std::uint32_t kIconRelease = 0x08930690u;   // (icon)
constexpr std::uint32_t kIconClear = 0x08930328u;     // (icon)
constexpr std::uint32_t kIconShowHand = 0x089306E4u;  // (icon)
constexpr std::uint32_t kIconHideHand = 0x08930700u;  // (icon)
constexpr std::uint32_t kCursorInit = 0x088D46E4u;    // (cursor, index, 0, last, 0, -1)
constexpr std::uint32_t kLayoutAlpha = 0x088D9B28u;   // (object, alpha)
constexpr std::uint32_t kPlaceParts = 0x088D3F00u;    // (0, objects, placements)
constexpr std::uint32_t kPlaceTexts = 0x088D44ECu;    // (objects, texts, table)
constexpr std::uint32_t kIconTextTable = 0x089CF1A0u;
constexpr std::uint32_t kCursorSound = 0x088A24D4u;   // (0, 10000, 1, 0)
constexpr std::uint32_t kBarState = 0x28u;            // 1 shown, 2 choosing, 3 icons moving
constexpr std::uint32_t kBarIcons = 0x38u;
constexpr std::uint32_t kIconSize = 0x5Cu;
constexpr std::uint32_t kBarCount = 0x260u;
constexpr std::uint32_t kBarCursor = 0x268u;
constexpr std::uint32_t kBarCursorIndex = 0x26Cu;
constexpr std::uint32_t kBarResult = 0x294u;          // 1: the cursor moved
constexpr std::uint32_t kIconState = 0x0u;            // 1 in place
constexpr std::uint32_t kIconRoot = 0xCu;             // four layout objects
constexpr std::uint32_t kIconPlacements = 0x1Cu;
constexpr std::uint32_t kIconTexts = 0x44u;
constexpr std::uint32_t kIconSkill = 0x4Cu;
constexpr std::uint32_t kLayoutX = 0x10u;             // a layout object's place, in PSP pixels
constexpr std::uint32_t kLayoutY = 0x14u;
constexpr std::uint32_t kLayoutAlphaByte = 0x2Bu;

// The Moogle shop (menu). Buying a Skill with five held, menu 0x08B0D27C says
// one has to go: the shop adds it (a sixth, waiting) and opens a window of the
// five (built by 0x08B0D364, run each frame by 0x08B0D578) to throw one away.
// Each Skill's row shows how many are held (menu 0x08B19C14's seventh
// argument, counted by 0x08B0CA14 from the list; after a throw-away 0x08B0CAD0
// counts them again into the rows).
constexpr std::uint32_t kShopMustDiscard = 0x08B0D27Cu;  // (shop) -> 1
constexpr std::uint32_t kShopDiscardWindow = 0x08B0D578u; // (shop) -> 0 once one is thrown away
constexpr std::uint32_t kShopRow = 0x08B19C14u;          // (shop, item, kind, ..., held, ...)
constexpr std::uint32_t kShopRedraw = 0x08B05024u;       // (shop)
constexpr std::uint32_t kShopWindowRefresh = 0x08B0D710u; // (shop): the hand and the help
constexpr std::uint32_t kShopSkills = 3u;                // the rows' kind for Skills
constexpr std::uint32_t kShopData = 0x138u;
constexpr std::uint32_t kShopBuying = 0x1F464u;          // s16: the Skill being bought
constexpr std::uint32_t kShopSkillRowCount = 0x31C8u;
constexpr std::uint32_t kShopSkillRows = 0xEDE4u;        // s16 id, ..., +0xC held; 0x14 each
constexpr std::uint32_t kShopRowHeld = 0xCu;
constexpr std::uint32_t kShopRowSize = 0x14u;
constexpr std::uint32_t kWindowNames = 0xCB0u;           // five texts
constexpr std::uint32_t kWindowNameViews = 0xCC4u;       // and what draws each
constexpr std::uint32_t kWindowIcons = 0x7A0u;           // five layout objects
constexpr std::uint32_t kWindowObjects = 0x404u;
constexpr std::uint32_t kWindowTexts = 0x884u;
constexpr std::uint32_t kWindowTextTable = 0x08B1C498u;
constexpr std::uint32_t kWindowIconAnimations = 0x08B1CA32u; // u16 per Skill kind
constexpr std::uint32_t kSkillNames = 0x09B4E838u;       // dungeon_skill.bin's names
constexpr std::uint32_t kTextMake = 0x0887AD78u;         // (texts, 0, index, 0, 0, 0)
constexpr std::uint32_t kTextView = 0x0887E534u;         // (text)
constexpr std::uint32_t kTextRelease = 0x0887BA5Cu;      // (text)
constexpr std::uint32_t kLayoutPlay = 0x088D9F5Cu;       // (object, animation, 1)
constexpr std::uint32_t kAttachTexts = 0x088D428Cu;      // (objects, texts, table, 1)
constexpr std::uint32_t kRowName = 0xCu;                 // in a dungeon_skill.bin row
constexpr std::uint32_t kRowKind = 0xFu;
constexpr int kMostShown = 10;
// The throw-away window's layout objects (shop data + 0x404, a word each),
// placed by menu 0x08B1C430's table: the window, its five rows (at the
// window's anchors 4 to 0), each row's icon (at the row's anchor 5, or 8 for
// the row under the hand: menu 0x08B0D8CC), and the hand's highlight.
constexpr std::uint32_t kWindowRow = 0xE2u;
constexpr std::uint32_t kWindowRowIcon = 0xE7u;
constexpr std::uint32_t kWindowHighlight = 0xECu;
constexpr std::uint32_t kWindowCursor = 0x24768u;
constexpr std::uint32_t kAttach = 0x088D3AE8u;           // (objects, parent, anchor, child)
constexpr std::uint32_t kShopPlaceHandRow = 0x08B0D8CCu; // menu: (shop) the row under the hand

// The page turn: frames per half, and how far right the page slides.
constexpr float kSlide = 40.0f;
// The page hint's right end and middle from its list's anchor object, in PSP
// pixels: the "Skill" plate's, and the throw-away window's.
constexpr float kHintBarDx = -51.0f;
constexpr float kHintBarDy = -21.0f;
constexpr float kHintShopDx = 0.0f;
constexpr float kHintShopDy = 0.0f;

bool flag(const char *name) {
    const char *value = std::getenv(name);
    return value != nullptr && *value != '\0' && std::strcmp(value, "0") != 0;
}
bool tracing() {
    static const bool on = flag("D012_TRACE_SKILLS");
    return on;
}
// D012_TRACE_SKILLS=2: the Skill bar's states too.
bool verbose() {
    static const bool on = [] {
        const char *value = std::getenv("D012_TRACE_SKILLS");
        return value != nullptr && std::strcmp(value, "2") == 0;
    }();
    return on;
}
int half_turn() {
    static const int frames = [] {
        const char *value = std::getenv("D012_SKILL_PAGE_FRAMES");
        const int n = value != nullptr ? std::atoi(value) : 0;
        return n > 0 ? n : 8;
    }();
    return frames;
}
bool ram(std::uint32_t a) { return a >= 0x08800000u && a < 0x09FF0000u; }

// The page hint, from the game's thread for the interface's: where the list
// on screen is, how faded, which page it shows.
enum class HintBy { Bar, ShopWindow };
struct Hint {
    std::uint64_t serial = 0;  // one more each frame a list shows it
    HintBy by = HintBy::Bar;
    float x = 0.0f, y = 0.0f;  // the list's anchor object, in PSP pixels
    float alpha = 0.0f;
    bool second = false;       // the second page is on screen
    bool other = false;        // the other page holds Skills
};
std::mutex &hint_lock() {
    static std::mutex lock;
    return lock;
}
Hint &hint() {
    static Hint value;
    return value;
}

void show_hint(HintBy by, float x, float y, float alpha, bool second, bool other) {
    const std::lock_guard<std::mutex> hold(hint_lock());
    Hint &h = hint();
    if (verbose() && (h.by != by || h.serial % 60u == 0u))
        std::printf("[skills] hint by %s at %.1f, %.1f\n", by == HintBy::Bar ? "the bar" : "the shop window", x, y);
    ++h.serial;
    h.by = by;
    h.x = x;
    h.y = y;
    h.alpha = alpha;
    h.second = second;
    h.other = other;
}

using Row = std::array<std::uint8_t, kStateRow>;

struct State {
    // The other page as last seen, for while the sixth slot holds a waiting
    // Skill instead; and its rows of the state table.
    sp::Packed packed;
    std::array<Row, sp::kPageSize> rows{};
    // The packed page while a waiting Skill holds its slot.
    bool held = false;
    std::uint32_t held_word = 0u;
    bool in_gateway = false;
    // The cheat's switch, read every so often.
    bool on = false;
    int on_age = 1000;
    // Input: the last read seen, and L2/R2 then.
    std::uint64_t read = ~0ull;
    std::uint32_t extra = 0u;
    // The page turn under way.
    std::uint32_t turning_bar = 0u;
    int turn = -1;
    std::uint64_t turn_read = 0u;
    bool tested = false;
};
State &state() {
    static State s;
    return s;
}

bool cheat_on() {
    State &s = state();
    if (++s.on_age >= 30) {
        s.on_age = 0;
        s.on = skill_pages_on();
    }
    return s.on;
}

std::uint32_t list_address(Guest *g) {
    const std::uint32_t root = g_ld32(g, kSaveRoot);
    return ram(root) ? root + kSkillList : 0u;
}
std::int32_t count_of(Guest *g, std::uint32_t list) { return g_ld32s(g, list); }
std::uint32_t slot(std::uint32_t list, int i) { return list + 4u + 4u * static_cast<std::uint32_t>(i); }

std::string describe(Guest *g, std::uint32_t list) {
    char text[200];
    int n = std::snprintf(text, sizeof text, "%d on screen:", count_of(g, list));
    for (int i = 0; i < 6 && n < static_cast<int>(sizeof text); ++i)
        n += std::snprintf(text + n, sizeof text - n, " %d", g_ld32s(g, slot(list, i)));
    const sp::Packed &p = state().packed;
    n += std::snprintf(text + n, sizeof text - n, " | other page (%s on screen):",
                       p.second_on_screen ? "2" : "1");
    for (int i = 0; i < p.other.count && n < static_cast<int>(sizeof text); ++i)
        n += std::snprintf(text + n, sizeof text - n, " %d", p.other.ids[static_cast<std::size_t>(i)]);
    return text;
}

void trace(Guest *g, std::uint32_t list, const char *what) {
    if (!tracing()) return;
    std::printf("[skills] %s: %s\n", what, describe(g, list).c_str());
    std::fflush(stdout);
}

// A state table row for a Skill: in no effect, then its row of
// dungeon_skill.bin, as menu 0x08B44F40 makes them (main 0x08920660).
Row fresh_row(Guest *g, std::int32_t id) {
    Row row{};
    const std::uint32_t data = g_call1(g, kSkillRow, static_cast<std::uint32_t>(id));
    if (ram(data))
        for (std::uint32_t k = 0; k < 0x10u; ++k) row[4u + k] = g_ld8(g, data + k);
    return row;
}

// The other page, from the sixth slot (or from what was kept while a Skill
// waits there). Rows are made again for Skills that changed.
sp::Packed read_other(Guest *g, std::uint32_t list) {
    State &s = state();
    sp::Packed now;
    if (count_of(g, list) > sp::kPageSize) {
        if (!s.held || !sp::unpack(s.held_word, now)) now = sp::Packed{};
    } else if (!sp::unpack(g_ld32(g, slot(list, 5)), now)) {
        now = sp::Packed{};
    }
    for (int i = 0; i < now.other.count; ++i) {
        const auto k = static_cast<std::size_t>(i);
        if (i >= s.packed.other.count || s.packed.other.ids[k] != now.other.ids[k]) s.rows[k] = fresh_row(g, now.other.ids[k]);
    }
    s.packed = now;
    return now;
}

void write_other(Guest *g, std::uint32_t list, const sp::Packed &packed) {
    State &s = state();
    s.packed = packed;
    const std::uint32_t word = sp::sixth_slot(packed);
    if (count_of(g, list) <= sp::kPageSize) {
        g_st32(g, slot(list, 5), word);
        s.held = false;
    } else {
        s.held = word != 0xFFFFFFFFu;
        s.held_word = word;
    }
}

// Runs one of the game's list functions with the packed page kept in the
// sixth slot: a removal moves it down with the Skills; a Skill that has to
// wait there takes it, and it comes back once the game settles on five.
GuestRegs around(Guest *g, std::uint32_t list, std::uint32_t address, const decomp::CallState &in) {
    State &s = state();
    sp::Packed seen;
    std::uint32_t word = 0u;
    bool packed = false;
    if (count_of(g, list) <= sp::kPageSize && sp::unpack(g_ld32(g, slot(list, 5)), seen)) {
        word = g_ld32(g, slot(list, 5));
        packed = true;
    } else if (s.held) {
        word = s.held_word;
        packed = true;
    }
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, address, regs);
    if (!packed) return regs;
    const std::int32_t count = count_of(g, list);
    for (int i = std::max<std::int32_t>(count, 0); i < 6; ++i)
        if (g_ld32(g, slot(list, i)) == word) g_st32(g, slot(list, i), 0xFFFFFFFFu);
    if (count <= sp::kPageSize) {
        g_st32(g, slot(list, 5), word);
        s.held = false;
    } else {
        s.held = true;
        s.held_word = word;
    }
    return regs;
}

void run_add(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const std::uint32_t list = in.gpr[4];
    const auto id = static_cast<std::int32_t>(in.gpr[5]);
    if (cheat_on() && list == list_address(g)) {
        sp::Packed other = read_other(g, list);
        if (sp::add_to(count_of(g, list), other.other.count, id) == sp::AddTo::OtherPage) {
            const auto k = static_cast<std::size_t>(other.other.count);
            other.other.ids[k] = id;
            ++other.other.count;
            state().rows[k] = fresh_row(g, id);
            write_other(g, list, other);
            trace(g, list, "a Skill goes on the other page");
            return;
        }
    }
    around(g, list, kAdd, in);
    if (list == list_address(g)) trace(g, list, "a Skill added");
}

void run_remove_at(Guest *g, const decomp::CallState &in, GuestRegs &) {
    around(g, in.gpr[4], kRemoveAt, in);
    if (in.gpr[4] == list_address(g)) trace(g, in.gpr[4], "a Skill removed");
}

void run_remove_set(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    out.v0 = around(g, in.gpr[4], kRemoveSet, in).v0;
    if (in.gpr[4] == list_address(g)) trace(g, in.gpr[4], "Skills removed");
}

std::vector<std::int32_t> ids_from(const char *name) {
    std::vector<std::int32_t> ids;
    const char *text = std::getenv(name);
    if (text == nullptr) return ids;
    while (*text != '\0' && ids.size() < static_cast<std::size_t>(sp::kPageSize)) {
        char *end = nullptr;
        const long id = std::strtol(text, &end, 0);
        if (end == text) break;
        ids.push_back(static_cast<std::int32_t>(id));
        text = *end == ',' ? end + 1 : end;
    }
    return ids;
}

// D012_TEST_SKILLS / D012_TEST_SKILLS_OTHER, once.
void put_test_skills(Guest *g) {
    State &s = state();
    if (s.tested) return;
    const std::uint32_t list = list_address(g);
    if (list == 0u) return;
    s.tested = true;
    const std::vector<std::int32_t> screen = ids_from("D012_TEST_SKILLS");
    const std::vector<std::int32_t> other = ids_from("D012_TEST_SKILLS_OTHER");
    if (screen.empty() && other.empty()) return;
    g_st32(g, list, static_cast<std::uint32_t>(screen.size()));
    for (int i = 0; i < 6; ++i)
        g_st32(g, slot(list, i), static_cast<std::size_t>(i) < screen.size() ? static_cast<std::uint32_t>(screen[static_cast<std::size_t>(i)]) : 0xFFFFFFFFu);
    sp::Packed packed;
    for (const std::int32_t id : other) packed.other.ids[static_cast<std::size_t>(packed.other.count++)] = id;
    g_st32(g, slot(list, 5), sp::sixth_slot(packed));
    trace(g, list, "test Skills");
}

void pass(Guest *g, std::uint32_t address, const decomp::CallState &in) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    for (int i = 0; i < 4; ++i) regs.f[i] = in.fpr[12 + i];
    decomp::call_original(g, address, regs);
}

void run_map_fill_bar(Guest *g, const decomp::CallState &in, GuestRegs &) {
    put_test_skills(g);
    State &s = state();
    s.in_gateway = false;
    pass(g, kMapFillBar, in);
    const std::uint32_t list = list_address(g);
    if (list == 0u) return;
    // Out of a gateway nothing is in effect.
    const sp::Packed other = read_other(g, list);
    for (int i = 0; i < other.other.count; ++i)
        s.rows[static_cast<std::size_t>(i)] = fresh_row(g, other.other.ids[static_cast<std::size_t>(i)]);
    trace(g, list, "the world map lists them");
}

void run_build_state_table(Guest *g, const decomp::CallState &in, GuestRegs &) {
    put_test_skills(g);
    state().in_gateway = true;
    pass(g, kBuildStateTable, in);
    const std::uint32_t list = list_address(g);
    if (list == 0u) return;
    read_other(g, list);
    trace(g, list, "a gateway lists them");
}

// Runs `fn` on each row of the other page in effect, copied onto the guest
// stack and back.
template <typename Fn>
void for_other_in_effect(Guest *g, Fn fn) {
    State &s = state();
    const std::uint32_t list = list_address(g);
    if (list == 0u || !cheat_on()) return;
    const sp::Packed other = read_other(g, list);
    const std::uint32_t row = g_push(g, 0x20u);
    for (int i = 0; i < other.other.count; ++i) {
        Row &r = s.rows[static_cast<std::size_t>(i)];
        std::uint32_t row_state;
        std::memcpy(&row_state, r.data(), 4);
        if (row_state != kInEffect) continue;
        for (std::uint32_t k = 0; k < kStateRow; ++k) g_st8(g, row + k, r[k]);
        fn(row, i);
        for (std::uint32_t k = 0; k < kStateRow; ++k) r[k] = g_ld8(g, row + k);
    }
    g_pop(g, 0x20u);
}

void run_apply_in_effect(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    decomp::call_original(g, kApplyInEffect, regs);
    out.v0 = regs.v0;
    for_other_in_effect(g, [&](std::uint32_t row, int i) {
        g_call3(g, kApplyOne, in.gpr[4], row, 0u);
        out.v0 = 1u;
        if (tracing()) std::printf("[skills] other page's Skill %d in effect for this battle\n", i);
    });
}

void run_end_in_effect(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    decomp::call_original(g, kEndInEffect, regs);
    out.v0 = regs.v0;
    std::vector<int> ended;
    for_other_in_effect(g, [&](std::uint32_t row, int i) {
        if (g_call1(g, kEndOne, row) != 0u) ended.push_back(i);
    });
    if (ended.empty()) return;
    State &s = state();
    const std::uint32_t list = list_address(g);
    sp::Packed packed = read_other(g, list);
    sp::Packed kept = packed;
    kept.other.count = 0;
    std::array<Row, sp::kPageSize> rows{};
    for (int i = 0; i < packed.other.count; ++i) {
        if (std::find(ended.begin(), ended.end(), i) != ended.end()) continue;
        rows[static_cast<std::size_t>(kept.other.count)] = s.rows[static_cast<std::size_t>(i)];
        kept.other.ids[static_cast<std::size_t>(kept.other.count++)] = packed.other.ids[static_cast<std::size_t>(i)];
    }
    write_other(g, list, kept);
    s.rows = rows;
    out.v0 = 1u;
    trace(g, list, "the other page's Skills in effect ended");
}

std::uint32_t state_table(Guest *g) {
    if (!state().in_gateway) return 0u;
    const std::uint32_t field = g_ld32(g, kField);
    return ram(field) ? field + kStateTable : 0u;
}

std::uint32_t icon_at(std::uint32_t bar, int i) { return bar + kBarIcons + kIconSize * static_cast<std::uint32_t>(i); }

// The bar's icons for the list as it is now; the hand on the same row, or the
// last, while the player is choosing.
void refresh_bar(Guest *g, std::uint32_t bar, std::uint32_t list) {
    const std::int32_t count = std::clamp<std::int32_t>(count_of(g, list), 0, 6);
    const std::int32_t before = std::clamp<std::int32_t>(g_ld32s(g, bar + kBarCount), 0, 6);
    for (int i = 0; i < count; ++i) {
        // Made again even for the same Skill: the in-effect mark goes with it.
        if (i < before && i < sp::kPageSize) g_st32(g, icon_at(bar, i) + kIconSkill, 0xFFFFFFFEu);
        g_call3(g, kBarSetIcon, bar, static_cast<std::uint32_t>(i), g_ld32(g, slot(list, i)));
    }
    for (int i = before - 1; i >= count; --i) {
        g_call1(g, kIconRelease, icon_at(bar, i));
        g_call1(g, kIconClear, icon_at(bar, i));
        g_st32(g, bar + kBarCount, static_cast<std::uint32_t>(i));
    }
    const std::uint32_t table = state_table(g);
    if (table != 0u)
        for (int i = 0; i < count; ++i)
            if (g_ld32(g, table + kStateRow * static_cast<std::uint32_t>(i)) == kInEffect)
                g_call2(g, kBarMarkInEffect, bar, static_cast<std::uint32_t>(i));
    if (g_ld32(g, bar + kBarState) == 2u && count > 0) {
        const std::int32_t index = std::clamp<std::int32_t>(g_ld32s(g, bar + kBarCursorIndex), 0, count - 1);
        g_call8(g, kCursorInit, bar + kBarCursor, static_cast<std::uint32_t>(index), 0u,
                static_cast<std::uint32_t>(count - 1), 0u, 0xFFFFFFFFu, 0u, 0u);
        for (int i = 0; i < count; ++i) g_call1(g, kIconHideHand, icon_at(bar, i));
        g_call1(g, kIconShowHand, icon_at(bar, index));
        g_st32(g, bar + kBarResult, 1u);  // the owner shows the Skill under the hand
    }
}

// Swaps the page on screen with the other: the list's first five, and in a
// gateway the state table's rows. False when there is nothing to turn to.
bool swap_pages(Guest *g, std::uint32_t list) {
    State &s = state();
    const std::int32_t count = count_of(g, list);
    const bool waiting = count > sp::kPageSize;
    sp::Packed packed = read_other(g, list);
    if (packed.other.count == 0 && count == 0) return false;
    if (waiting && packed.other.count != sp::kPageSize) return false;
    const int shown = std::min<int>(std::max<std::int32_t>(count, 0), sp::kPageSize);

    sp::Page leaving;
    std::array<Row, sp::kPageSize> leaving_rows{};
    const std::uint32_t table = state_table(g);
    for (int i = 0; i < shown; ++i) {
        leaving.ids[static_cast<std::size_t>(leaving.count++)] = g_ld32s(g, slot(list, i));
        if (table != 0u)
            for (std::uint32_t k = 0; k < kStateRow; ++k)
                leaving_rows[static_cast<std::size_t>(i)][k] = g_ld8(g, table + kStateRow * static_cast<std::uint32_t>(i) + k);
    }
    for (int i = 0; i < sp::kPageSize; ++i) {
        const auto k = static_cast<std::size_t>(i);
        g_st32(g, slot(list, i), i < packed.other.count ? static_cast<std::uint32_t>(packed.other.ids[k]) : 0xFFFFFFFFu);
        if (table != 0u && i < packed.other.count)
            for (std::uint32_t b = 0; b < kStateRow; ++b)
                g_st8(g, table + kStateRow * static_cast<std::uint32_t>(i) + b, s.rows[k][b]);
    }
    g_st32(g, list, static_cast<std::uint32_t>(packed.other.count + (waiting ? 1 : 0)));
    sp::Packed next;
    next.other = leaving;
    next.second_on_screen = !packed.second_on_screen;
    s.rows = leaving_rows;
    if (table == 0u)
        for (int i = 0; i < leaving.count; ++i) s.rows[static_cast<std::size_t>(i)] = fresh_row(g, leaving.ids[static_cast<std::size_t>(i)]);
    write_other(g, list, next);
    trace(g, list, "page turned");
    return true;
}

bool can_turn(Guest *g, std::uint32_t list) {
    const std::int32_t count = count_of(g, list);
    const sp::Packed packed = read_other(g, list);
    if (count > sp::kPageSize) return packed.other.count == sp::kPageSize;
    return packed.other.count > 0;
}

// Moves the page's icons `offset` of the slide right and fades them.
void place_turning(Guest *g, std::uint32_t bar, const sp::TurnFrame &frame) {
    const std::int32_t count = std::clamp<std::int32_t>(g_ld32s(g, bar + kBarCount), 0, sp::kPageSize);
    for (int i = 0; i < count; ++i) {
        const std::uint32_t icon = icon_at(bar, i);
        if (g_ld32(g, icon + kIconState) != 1u) continue;
        const std::uint32_t root = g_ld32(g, icon + kIconRoot);
        if (!ram(root)) continue;
        float x;
        std::memcpy(&x, g_ptr(g, root + kLayoutX, 4), 4);
        g_stf(g, root + kLayoutX, x + frame.offset * kSlide);
        const float alpha = static_cast<float>(g_ld8(g, root + kLayoutAlphaByte)) * std::clamp(frame.alpha, 0.0f, 1.0f);
        g_call2(g, kLayoutAlpha, root, static_cast<std::uint32_t>(alpha + 0.5f));
        g_call3(g, kPlaceParts, 0u, icon + kIconRoot, icon + kIconPlacements);
        g_call3(g, kPlaceTexts, icon + kIconRoot, icon + kIconTexts, kIconTextTable);
    }
}

bool page_button_pressed() {
    State &s = state();
    const input::Frame &f = input::current();
    if (f.read == s.read) return false;
    s.read = f.read;
    const std::uint32_t pressed = f.player1.extra & ~s.extra & (ext::kExtraL2 | ext::kExtraR2);
    if (verbose() && f.player1.extra != s.extra)
        std::printf("[skills] extra buttons 0x%X at read %llu\n", f.player1.extra, static_cast<unsigned long long>(f.read));
    s.extra = f.player1.extra;
    return pressed != 0u;
}

void run_bar_update(Guest *g, const decomp::CallState &in, GuestRegs &) {
    const std::uint32_t bar = in.gpr[4];
    pass(g, kBarUpdate, in);
    State &s = state();
    const bool pressed = page_button_pressed();
    if (!cheat_on()) {
        s.turn = -1;
        return;
    }
    const std::uint32_t list = list_address(g);
    if (list == 0u) return;
    const std::uint32_t bar_state = g_ld32(g, bar + kBarState);
    if (verbose()) {
        static std::uint32_t last_bar = 0u, last_state = 0u;
        if (bar != last_bar || bar_state != last_state)
            std::printf("[skills] bar 0x%08X state %u at read %llu\n", bar, bar_state,
                        static_cast<unsigned long long>(input::current().read));
        last_bar = bar;
        last_state = bar_state;
    }
    // The hint, by the "Skill" plate, while the bar shows Skills: placed from
    // the first row's icon as the game placed it this frame (before a turn
    // moves it), and faded as it is.
    if ((bar_state == 1u || bar_state == 2u) && g_ld32s(g, bar + kBarCount) > 0) {
        const sp::Packed other = read_other(g, list);
        const std::uint32_t root = g_ld32(g, icon_at(bar, 0) + kIconRoot);
        if (ram(root)) {
            float x, y;
            std::memcpy(&x, g_ptr(g, root + kLayoutX, 4), 4);
            std::memcpy(&y, g_ptr(g, root + kLayoutY, 4), 4);
            const float alpha = static_cast<float>(g_ld8(g, root + kLayoutAlphaByte)) / 255.0f;
            show_hint(HintBy::Bar, x, y, alpha, other.second_on_screen, other.other.count > 0);
        }
    }
    // A turn whose bar stopped (closed, or another took its place) ends.
    if (s.turn >= 0 && input::current().read > s.turn_read + 8u) s.turn = -1;
    if (s.turn < 0) {
        if (pressed && tracing())
            std::printf("[skills] L2/R2 on bar 0x%08X in state %u (icons %d)\n", bar, bar_state,
                        g_ld32s(g, bar + kBarCount));
        if (!pressed || (bar_state != 1u && bar_state != 2u) || !can_turn(g, list)) return;
        s.turn = 0;
        s.turning_bar = bar;
        g_call4(g, kCursorSound, 0u, 10000u, 1u, 0u);
    }
    if (bar != s.turning_bar) return;
    s.turn_read = input::current().read;
    if (bar_state != 1u && bar_state != 2u) {
        s.turn = -1;
        return;
    }
    const sp::TurnFrame frame = sp::turn_frame(s.turn, half_turn());
    if (frame.swap_now && swap_pages(g, list)) refresh_bar(g, bar, list);
    place_turning(g, bar, frame);
    if (++s.turn >= 2 * half_turn()) s.turn = -1;
}

// How many of Skill `id` are held, on both pages (and waiting).
int held_of(Guest *g, std::uint32_t list, std::int32_t id) {
    int held = 0;
    const std::int32_t count = std::clamp<std::int32_t>(count_of(g, list), 0, 6);
    for (int i = 0; i < count; ++i)
        if (g_ld32s(g, slot(list, i)) == id) ++held;
    const sp::Packed other = read_other(g, list);
    for (int i = 0; i < other.other.count; ++i)
        if (other.other.ids[static_cast<std::size_t>(i)] == id) ++held;
    return std::min(held, kMostShown);
}

void run_shop_row(Guest *g, const decomp::CallState &in, GuestRegs &) {
    GuestRegs regs{};
    for (int i = 0; i < 8; ++i) regs.a[i] = in.gpr[4 + i];
    const std::uint32_t list = list_address(g);
    if (regs.a[2] == kShopSkills && list != 0u && cheat_on() && ram(regs.a[1]))
        regs.a[6] = static_cast<std::uint32_t>(held_of(g, list, g_ld16s(g, regs.a[1] + 6u)));
    decomp::call_original(g, kShopRow, regs);
}

void run_shop_must_discard(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    GuestRegs regs{};
    regs.a[0] = in.gpr[4];
    decomp::call_original(g, kShopMustDiscard, regs);
    out.v0 = regs.v0;
    const std::uint32_t list = list_address(g);
    if (out.v0 == 0u || list == 0u || !cheat_on()) return;
    const std::uint32_t data = g_ld32(g, in.gpr[4] + kShopData);
    const std::int32_t id = ram(data) ? g_ld16s(g, data + kShopBuying) : -1;
    if (sp::add_to(count_of(g, list), read_other(g, list).other.count, id) == sp::AddTo::OtherPage) out.v0 = 0u;
}

// The throw-away window's five rows, for the list as it is now.
void refresh_shop_window(Guest *g, std::uint32_t shop, std::uint32_t list) {
    const std::uint32_t data = g_ld32(g, shop + kShopData);
    if (!ram(data)) return;
    const std::int32_t shown = std::clamp<std::int32_t>(count_of(g, list), 0, sp::kPageSize);
    for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(sp::kPageSize); ++i) {
        for (const std::uint32_t at : {data + kWindowNames + 4u * i, data + kWindowNameViews + 4u * i}) {
            const std::uint32_t text = g_ld32(g, at);
            if (text != 0u) g_call1(g, kTextRelease, text);
            g_st32(g, at, 0u);
        }
        if (static_cast<std::int32_t>(i) >= shown) continue;
        const std::uint32_t row = g_call1(g, kSkillRow, g_ld32(g, slot(list, static_cast<int>(i))));
        if (!ram(row)) continue;
        const std::uint32_t name = g_call8(g, kTextMake, kSkillNames, 0u, g_ld8(g, row + kRowName), 0u, 0u, 0u, 0u, 0u);
        g_st32(g, data + kWindowNames + 4u * i, name);
        g_st32(g, data + kWindowNameViews + 4u * i, g_call1(g, kTextView, name));
        const auto animation = static_cast<std::uint16_t>(g_ld16s(g, kWindowIconAnimations + 2u * g_ld8(g, row + kRowKind)));
        g_call3(g, kLayoutPlay, g_ld32(g, data + kWindowIcons + 4u * i), animation, 1u);
    }
    g_call4(g, kAttachTexts, data + kWindowObjects, data + kWindowTexts, kWindowTextTable, 1u);
    g_call1(g, kShopWindowRefresh, shop);
}

std::uint32_t window_object(Guest *g, std::uint32_t data, std::uint32_t index) {
    return g_ld32(g, data + kWindowObjects + 4u * index);
}

// Moves the window's rows `offset` of the slide right and fades them, after
// the game placed them this frame; then places their icons and texts again.
void place_shop_turning(Guest *g, std::uint32_t shop, const sp::TurnFrame &frame) {
    const std::uint32_t data = g_ld32(g, shop + kShopData);
    if (!ram(data)) return;
    const std::uint32_t objects = data + kWindowObjects;
    const std::int32_t under_hand = g_ld32s(g, data + kWindowCursor);
    const float shift = frame.offset * kSlide;
    const float fade = std::clamp(frame.alpha, 0.0f, 1.0f);
    const auto move = [&](std::uint32_t object) {
        if (!ram(object)) return;
        float x;
        std::memcpy(&x, g_ptr(g, object + kLayoutX, 4), 4);
        g_stf(g, object + kLayoutX, x + shift);
        const float alpha = static_cast<float>(g_ld8(g, object + kLayoutAlphaByte)) * fade;
        g_call2(g, kLayoutAlpha, object, static_cast<std::uint32_t>(alpha + 0.5f));
    };
    for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(sp::kPageSize); ++i) {
        move(window_object(g, data, kWindowRow + i));
        g_call4(g, kAttach, objects, kWindowRow + i, static_cast<std::int32_t>(i) == under_hand ? 8u : 5u,
                kWindowRowIcon + i);
        const std::uint32_t icon = window_object(g, data, kWindowRowIcon + i);
        if (ram(icon)) {
            const float alpha = static_cast<float>(g_ld8(g, icon + kLayoutAlphaByte)) * fade;
            g_call2(g, kLayoutAlpha, icon, static_cast<std::uint32_t>(alpha + 0.5f));
        }
    }
    // The names, then the row under the hand again, as the game places them
    // each frame (menu 0x08B0D578): that row's name goes to its highlighted
    // anchors, its icon to anchor 8, the highlight to the window. Without the
    // second step its name stays at the plain anchor, drawn large.
    g_call3(g, kPlaceTexts, objects, data + kWindowTexts, kWindowTextTable);
    g_call1(g, kShopPlaceHandRow, shop);
    move(window_object(g, data, kWindowHighlight));
}

void run_shop_discard_window(Guest *g, const decomp::CallState &in, GuestRegs &out) {
    const std::uint32_t shop = in.gpr[4];
    const std::uint32_t list = list_address(g);
    State &s = state();
    const bool pressed = page_button_pressed();
    const bool on = list != 0u && cheat_on();
    if (!on) s.turn = -1;
    if (on && pressed && s.turn < 0 && can_turn(g, list)) {
        s.turn = 0;
        s.turning_bar = shop;
        g_call4(g, kCursorSound, 0u, 10000u, 1u, 0u);
    }
    GuestRegs regs{};
    regs.a[0] = shop;
    decomp::call_original(g, kShopDiscardWindow, regs);
    out.v0 = regs.v0;
    const std::uint32_t data = g_ld32(g, shop + kShopData);
    // The hint, above the window, placed from its first row as the game placed
    // it this frame (before a turn moves it), and faded as the window is.
    const std::uint32_t first_row = ram(data) ? window_object(g, data, kWindowRow) : 0u;
    if (on && out.v0 != 0u && ram(first_row)) {
        const sp::Packed other = read_other(g, list);
        float x, y;
        std::memcpy(&x, g_ptr(g, first_row + kLayoutX, 4), 4);
        std::memcpy(&y, g_ptr(g, first_row + kLayoutY, 4), 4);
        const float alpha = static_cast<float>(g_ld8(g, first_row + kLayoutAlphaByte)) / 255.0f;
        show_hint(HintBy::ShopWindow, x, y, alpha, other.second_on_screen, other.other.count > 0);
    }
    if (on && out.v0 != 0u && s.turn >= 0 && s.turning_bar == shop) {
        s.turn_read = input::current().read;
        const sp::TurnFrame frame = sp::turn_frame(s.turn, half_turn());
        if (frame.swap_now && swap_pages(g, list)) refresh_shop_window(g, shop, list);
        place_shop_turning(g, shop, frame);
        if (++s.turn >= 2 * half_turn()) s.turn = -1;
    }
    // Thrown away: the rows' counts again, with the other page's.
    if (out.v0 != 0u) return;
    if (s.turning_bar == shop) s.turn = -1;
    if (list == 0u || !cheat_on() || !ram(data)) return;
    const std::int32_t rows = std::clamp<std::int32_t>(g_ld32s(g, data + kShopSkillRowCount), 0, 64);
    for (std::int32_t j = 0; j < rows; ++j) {
        const std::uint32_t row = data + kShopSkillRows + kShopRowSize * static_cast<std::uint32_t>(j);
        g_st8(g, row + kShopRowHeld, static_cast<std::uint8_t>(held_of(g, list, g_ld16s(g, row))));
    }
    g_call1(g, kShopRedraw, shop);
}

#if defined(PORTABLEKIT_HAS_RENDERER)
// The page hint: "L2/R2  Page 1/2", the buttons named as the game's prompts
// are drawn (Controls > Button prompts), in the style of the port's other
// hints over the game (the BGM selector's). It shows while a Skill list draws
// it each frame, faded as the list is, and dimmer while the other page is
// empty.

// The pad's buttons that turn the page: the extra buttons L2 and R2, on the
// shoulders with Swap shoulders and triggers.
std::string page_keys() {
    namespace pki = portablekit::input;
    const pki::ButtonPrompts prompts = current_button_prompts();
    const bool shoulders = portablekit::settings::current().swap_triggers;
    switch (prompts.style) {
    case pki::PromptStyle::Xbox:
        if (prompts.numbered_shoulders) return shoulders ? "L1/R1" : "L2/R2";
        return shoulders ? "LB/RB" : "LT/RT";
    case pki::PromptStyle::Nintendo:
        return shoulders ? "L/R" : "ZL/ZR";
    case pki::PromptStyle::Keyboard: {
        const auto &bindings = portablekit::settings::current().bindings;
        const pki::Binding left = bindings[static_cast<std::size_t>(pki::Action::L2)][0];
        const pki::Binding right = bindings[static_cast<std::size_t>(pki::Action::R2)][0];
        if (left == pki::kNone || right == pki::kNone) return "L2/R2";
        return "[" + pki::name(left) + "]/[" + pki::name(right) + "]";
    }
    default:
        return shoulders ? "L1/R1" : "L2/R2";
    }
}

struct HintSeen {
    std::uint64_t serial = 0;
    int idle_frames = 1000;
};
HintSeen &hint_seen() {
    static HintSeen value;
    return value;
}

bool hint_visible() {
    Hint now;
    {
        const std::lock_guard<std::mutex> hold(hint_lock());
        now = hint();
    }
    HintSeen &seen = hint_seen();
    if (now.serial != seen.serial) {
        seen.serial = now.serial;
        seen.idle_frames = 0;
    } else if (seen.idle_frames < 1000) {
        ++seen.idle_frames;
    }
    return seen.idle_frames <= 4 && now.alpha > 0.02f;
}

void hint_draw() {
    Hint now;
    {
        const std::lock_guard<std::mutex> hold(hint_lock());
        now = hint();
    }
    const portablekit::ui::GamePicture picture = portablekit::ui::game_picture();
    const float sx = picture.width / 480.0f;
    const float sy = picture.height / 272.0f;
    const auto at = [&](float x, float y) { return ImVec2(picture.left + x * sx, picture.top + y * sy); };
    ImDrawList *draw = ImGui::GetForegroundDrawList();
    ImFont *font = portablekit::ui::game_text_font();
    const std::string text = page_keys() + "  Page " + (now.second ? "2/2" : "1/2");
    const float size = 6.5f;
    const float width = font->CalcTextSizeA(size * sy, 1.0e9f, 0.0f, text.c_str()).x / sx;
    // Right of the hint and its middle, from the list's anchor object: left
    // of the "Skill" plate, above its spear; above the window's top right.
    float right = now.x;
    float middle = now.y;
    if (now.by == HintBy::Bar) {
        right += kHintBarDx;
        middle += kHintBarDy;
    } else {
        right += kHintShopDx;
        middle += kHintShopDy;
    }
    const float a = std::clamp(now.alpha, 0.0f, 1.0f) * (now.other ? 1.0f : 0.55f);
    const auto colour = [](int r, int gr, int b, float alpha) {
        return IM_COL32(r, gr, b, static_cast<int>(std::clamp(alpha, 0.0f, 1.0f) * 255.0f));
    };
    draw->AddRectFilled(at(right - width - 6.0f, middle - 4.5f), at(right, middle + 4.5f), colour(20, 30, 80, 0.45f * a),
                        2.0f * sy);
    const ImVec2 extent = font->CalcTextSizeA(size * sy, 1.0e9f, 0.0f, text.c_str());
    draw->AddText(font, size * sy, ImVec2(at(right - 3.0f, middle).x - extent.x, at(0.0f, middle).y - extent.y * 0.5f),
                  colour(255, 255, 255, 0.95f * a), text.c_str());
}
#endif

struct Install {
    Install() {
        static const decomp::Override overrides[] = {
            {kAdd, "skill_pages_add", decomp::Module::Main, decomp::Returns::Nothing, &run_add},
            {kRemoveAt, "skill_pages_remove_at", decomp::Module::Main, decomp::Returns::Nothing, &run_remove_at},
            {kRemoveSet, "skill_pages_remove_set", decomp::Module::Main, decomp::Returns::Integer, &run_remove_set},
            {kBarUpdate, "skill_pages_bar", decomp::Module::Main, decomp::Returns::Nothing, &run_bar_update},
            {kBuildStateTable, "skill_pages_gateway", decomp::Module::MenuOverlay, decomp::Returns::Nothing,
             &run_build_state_table},
            {kMapFillBar, "skill_pages_world_map", decomp::Module::BattleOverlay, decomp::Returns::Nothing,
             &run_map_fill_bar},
            {kApplyInEffect, "skill_pages_apply", decomp::Module::MenuOverlay, decomp::Returns::Integer,
             &run_apply_in_effect},
            {kEndInEffect, "skill_pages_end", decomp::Module::MenuOverlay, decomp::Returns::Integer,
             &run_end_in_effect},
            {kShopRow, "skill_pages_shop_row", decomp::Module::MenuOverlay, decomp::Returns::Nothing, &run_shop_row},
            {kShopMustDiscard, "skill_pages_shop_full", decomp::Module::MenuOverlay, decomp::Returns::Integer,
             &run_shop_must_discard},
            {kShopDiscardWindow, "skill_pages_shop_window", decomp::Module::MenuOverlay, decomp::Returns::Integer,
             &run_shop_discard_window},
        };
        decomp::add_overrides(overrides, sizeof overrides / sizeof overrides[0], true);
    }
} install_overrides;

} // namespace

namespace skill_pages {

void install() {
#if defined(PORTABLEKIT_HAS_RENDERER)
    static bool installed = false;
    if (installed) return;
    installed = true;
    portablekit::ui::add_game_overlay(&hint_visible, &hint_draw);
#endif
}

} // namespace skill_pages
} // namespace d012
