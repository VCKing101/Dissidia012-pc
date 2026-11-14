#include "battle/agent1_types.h"

/* Destructor, reset and release of one motion track. */
extern unsigned char D_089E2A48[];
extern void func_08992F04(void *);
void func_089E22B0(BattleMotionTrack *track);
void func_089E22E8(BattleMotionTrack *track);

void func_089E2258(BattleMotionTrack *track, int flags)
{
    if (track) {
        track->vtable = D_089E2A48;
        func_089E22E8(track);
        if (flags & 1) func_08992F04(track);
    }
}

void func_089E22B0(BattleMotionTrack *track)
{
    track->field_00 = 0;
    {
        int i;
        for (i = 0; i < 10; i++) {
            BattleMotionEntry *entry = &track->entries[i];
            entry->id = -1;
            entry->value = 0.0f;
        }
    }
    track->field_5C = -1;
    track->field_60 = 0;
}

void func_089E22E8(BattleMotionTrack *track)
{
    func_089E22B0(track);
}
