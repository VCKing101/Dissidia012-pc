#include "battle/agent28_types.h"

/* Sets how fast the phase clock's hand turns. */
extern void func_088D9AEC(void *model, float speed);
extern float D_08A16D28;

int func_08A16CC4(PhaseClock *clock, float rate)
{
    if (!clock->open) {
        return -1;
    }
    if (clock->hand != 0) {
        func_088D9AEC(clock->hand, D_08A16D28 * rate);
    }
    return 0;
}
