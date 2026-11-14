#include "battle/agent28_types.h"

/* Forwards a selection to one of the display set's two fighter displays. */
extern int func_089E5644(FighterDisplay *display, int value);

int func_089E7E74(DisplaySet *set, unsigned int side, int value)
{
    if (!set->open) {
        return -1;
    }
    if (side >= 2) {
        return -2;
    }
    if (func_089E5644(&set->displays[side], value) != 0) {
        return -3;
    }
    return 0;
}
