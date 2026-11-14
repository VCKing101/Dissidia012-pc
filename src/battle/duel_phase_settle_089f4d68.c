#include "battle/agent28_types.h"

/* Waits until every side's result display has finished, then moves to
 * state 6. */
extern int func_089F37F8(DuelPartSet *parts);
extern int func_08A0836C(DuelBadge *badge);

int func_089F4D68(DuelPhaseData *phase)
{
    unsigned char settled = 1;

    {
        int side;
        for (side = 0; side < 2; side++) {
        if (phase->done[side] == 0) {
            if (func_089F37F8(&phase->parts[side]) == 0) {
                settled = 0;
            }
        } else {
            if (func_08A0836C(&phase->badges[side]) == 0) {
                settled = 0;
            }
        }
        }
    }
    if (settled == 1) {
        phase->state = 6;
    }
    return 0;
}
