#include "battle/agent1_types.h"

/* Opens a motion track for one side: copies the motion kind of each of the
 * fighter's ten current deck entries. */
struct DeckPage {
    unsigned char pad_00[0xA2];
    short ids[10];
    unsigned char pad_b6[0x10];
};

struct MotionDeck {
    DeckPage pages[9];
    unsigned char pad_6f6[0x6D];
    signed char page;

    static short *current(MotionDeck *deck) { DeckPage *p = deck->pages; return p[deck->page].ids; }
};

struct DeckOwner {
    unsigned char pad_000[0x10];
    MotionDeck deck;
};

struct SideFighter {
    unsigned char pad_000[0x30C];
    DeckOwner *owner;
};

struct MotionInfo {
    unsigned char pad_00[0x13];
    signed char kind;
};

extern "C" {

extern void func_089E22E8(BattleMotionTrack *track);
extern SideFighter *func_089E95B0(unsigned int side);
extern MotionInfo *func_088B29D8(int id);

int func_089E2304(BattleMotionTrack *track, unsigned int side, int context)
{
    func_089E22E8(track);
    if (side >= 2 || context == 0) {
        return -1;
    }
    SideFighter *fighter = func_089E95B0(side);
    if (fighter == 0) {
        return -2;
    }
    DeckOwner *owner = fighter->owner;
    if (owner != 0) {
        MotionDeck *deck = &owner->deck;
        short *ids = MotionDeck::current(deck);
        for (int i = 0; i < 10; i++) {
            MotionInfo *info = func_088B29D8(ids[i]);
            if (info == 0) {
                continue;
            }
            track->entries[i].id = info->kind;
        }
    }
    track->field_5C = side;
    track->field_60 = context;
    track->field_00 = 1;
    return 0;
}

}
