#pragma once

// The Skill limit cheat's rules (d012_skill_pages.cpp), apart from the game
// so that d012_tests can check them. MIT licence; see port/README.md.
//
// Story Mode keeps the Skills held in the save as a count and six ids
// (save root + 0x3F548): five Skills and, for a moment, a sixth just
// obtained, before the game asks which one to throw away. With the cheat the
// player holds ten, on two pages of five. The game only ever sees the page on
// screen (the visible page, in its own list); the other page is packed into
// the list's sixth slot, which the game leaves alone while it holds five or
// fewer. The save keeps its size and its layout, and the game without the
// cheat (or PPSSPP, or a PSP) reads such a save as five Skills: the packed
// word is neither -1 nor a Skill id the game knows.

#include <array>
#include <cstdint>

namespace d012::skill_pages {

inline constexpr int kPageSize = 5;
inline constexpr std::int32_t kEmpty = -1;
// Five bits a Skill: the game's are 1..24 (dungeon_skill.bin).
inline constexpr std::int32_t kLargestPackable = 31;

struct Page {
    int count = 0;
    std::array<std::int32_t, kPageSize> ids{};
};

// What the sixth slot holds besides a Skill or -1: the other page, and
// whether the page on screen is the second.
struct Packed {
    Page other;
    bool second_on_screen = false;
};

// 31 bits of a word: bit 31 clear (never -1) and bit 15 set (the low half,
// which the game compares with Skill ids as a signed halfword, is negative:
// no Skill matches it). Thirty bits remain: a mark (4), the page on screen
// (1) and five ids (25).
inline constexpr std::uint32_t kMark = 0xBu;

inline std::uint32_t pack(const Packed &packed) {
    std::uint32_t bits = (kMark << 26) | (packed.second_on_screen ? 1u << 25 : 0u);
    for (int i = 0; i < packed.other.count && i < kPageSize; ++i)
        bits |= static_cast<std::uint32_t>(packed.other.ids[static_cast<std::size_t>(i)] & 0x1F) << (5 * i);
    return ((bits >> 15) & 0x7FFFu) << 16 | 0x8000u | (bits & 0x7FFFu);
}

// True and `out` filled when `word` is a packed page.
inline bool unpack(std::uint32_t word, Packed &out) {
    if ((word & 0x80008000u) != 0x00008000u) return false;
    const std::uint32_t bits = ((word >> 16) & 0x7FFFu) << 15 | (word & 0x7FFFu);
    if ((bits >> 26) != kMark) return false;
    Packed packed;
    packed.second_on_screen = ((bits >> 25) & 1u) != 0u;
    for (int i = 0; i < kPageSize; ++i) {
        const auto id = static_cast<std::int32_t>((bits >> (5 * i)) & 0x1Fu);
        if (id == 0) break;
        packed.other.ids[static_cast<std::size_t>(packed.other.count++)] = id;
    }
    out = packed;
    return true;
}

// The sixth slot for `packed`: -1 when there is nothing to keep.
inline std::uint32_t sixth_slot(const Packed &packed) {
    if (packed.other.count == 0 && !packed.second_on_screen) return 0xFFFFFFFFu;
    return pack(packed);
}

// Where a Skill the game adds goes. The game puts it on the page on screen
// while that holds fewer than five; then the other page takes it, while it has
// room; then the game's own choice (keep it, or throw one away) as without
// the cheat.
enum class AddTo { ScreenPage, OtherPage, GameChoice };

inline AddTo add_to(int on_screen, int other, std::int32_t id) {
    if (on_screen < kPageSize) return AddTo::ScreenPage;
    if (other < kPageSize && id >= 1 && id <= kLargestPackable) return AddTo::OtherPage;
    return AddTo::GameChoice;
}

// How far along the page turn is, 0 at the start, 1 at the end, from frames:
// the page on screen slides right and fades out over the first half; the
// other comes in from the right and fades in over the second.
struct TurnFrame {
    float offset = 0.0f;  // of the full slide, to the right
    float alpha = 1.0f;
    bool swap_now = false; // the frame the pages change places
};

inline TurnFrame turn_frame(int frame, int half) {
    TurnFrame out;
    if (half <= 0) return out;
    if (frame < half) {
        const float t = static_cast<float>(frame + 1) / static_cast<float>(half);
        out.offset = t * t;            // eases out of place
        out.alpha = 1.0f - t;
        out.swap_now = frame + 1 == half;
    } else {
        const float t = static_cast<float>(frame - half + 1) / static_cast<float>(half);
        const float u = 1.0f - t;
        out.offset = u * u;            // eases into place
        out.alpha = t;
    }
    return out;
}

} // namespace d012::skill_pages
