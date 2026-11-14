#include "battle/agent1_types.h"

/* Destructor, reset and release of one motion link. */
extern unsigned char D_089E31C8[];
extern void func_08992F04(void *);
void func_089E2B08(BattleMotionLink *link);
void func_089E2B2C(BattleMotionLink *link);

void func_089E2AB0(BattleMotionLink *link, int flags)
{
    if (link) {
        link->vtable = D_089E31C8;
        func_089E2B2C(link);
        if (flags & 1) func_08992F04(link);
    }
}

void func_089E2B08(BattleMotionLink *link)
{
    link->field_00 = 0;
    link->field_04 = 0.0f;
    link->field_08 = 0.0f;
    link->field_0C = 0.0f;
    link->field_10 = -1;
    link->field_14 = 0;
}

void func_089E2B2C(BattleMotionLink *link)
{
    func_089E2B08(link);
}
