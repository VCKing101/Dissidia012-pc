#include "battle/agent1_types.h"

/* Constructor, destructor, reset and release of the motion set. */
extern unsigned char D_089E2210[];
extern void *D_089D7830;
extern void func_089E2A78(void *);
extern void func_089E2AB0(void *);
extern void func_089E2220(void *);
extern void func_089E2258(void *);
extern void func_089E22E8(BattleMotionTrack *);
extern void func_089E2B2C(BattleMotionLink *);
extern void func_08993560(void *, unsigned int, unsigned int, void (*)(void *));
extern void func_0899372C(void *, unsigned int, unsigned int, void (*)(void *), int, int);
extern void func_08992F04(void *);
extern void func_088D0A1C(void *, void *);
void func_089E1E00(BattleMotionSet *set);
void func_089E1E30(BattleMotionSet *set);

BattleMotionSet *func_089E1D00(BattleMotionSet *set)
{
    set->vtable = D_089E2210;
    func_08993560(set->field_0C, 2, sizeof(BattleMotionLink), func_089E2A78);
    func_08993560(set->field_44, 2, sizeof(BattleMotionTrack), func_089E2220);
    func_089E1E00(set);
    return set;
}

void func_089E1D68(BattleMotionSet *set, int flags)
{
    if (set) {
        set->vtable = D_089E2210;
        func_089E1E30(set);
        func_0899372C(set->field_44, 2, sizeof(BattleMotionTrack), func_089E2258, 0, 0);
        func_0899372C(set->field_0C, 2, sizeof(BattleMotionLink), func_089E2AB0, 0, 0);
        if (flags & 1) func_08992F04(set);
    }
}

void func_089E1E00(BattleMotionSet *set)
{
    int i;

    set->field_00 = 0;
    set->field_04 = 0;
    set->field_08 = 0;
    for (i = 0; i < 3; i++) {
        *((unsigned char *)set + 0x114 + i) = 0;
    }
    set->field_117 = 0;
}

void func_089E1E30(BattleMotionSet *set)
{
    {
        int i;
        for (i = 0; i < 2; i++) {
            func_089E22E8(&set->field_44[i]);
            func_089E2B2C(&set->field_0C[i]);
        }
    }
    if (set->field_08) {
        func_088D0A1C(D_089D7830, set->field_08);
    }
    func_089E1E00(set);
}
