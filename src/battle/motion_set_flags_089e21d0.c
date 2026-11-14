#include "battle/agent1_types.h"

/* Recomputes the motion set's any-flag byte. */
int func_089E21D0(BattleMotionSet *set)
{
    unsigned char any = 0;
    int i;

    for (i = 0; i < 3; i++) {
        if (*((unsigned char *)set + 0x114 + i) == 1) {
            any = 1;
            break;
        }
    }
    set->field_117 = any;
    return 0;
}
