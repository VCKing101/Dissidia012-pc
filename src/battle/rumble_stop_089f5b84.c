#include "battle/agent28_types.h"

/* Stops every running rumble slot and resets the rumble set. */
extern void func_08889FB8(int pad, int mode, int strength);

int func_089F5B84(RumbleSet *rumble)
{
    if (!rumble->open) {
        return -1;
    }
    if (rumble->active != 0) {
        return -2;
    }
    {
        int pad;
        for (pad = 0; pad < 3; pad++) {
            if (rumble->states[pad] == 2) {
                func_08889FB8(pad, 2, 0);
            }
        }
    }
    {
        int pad;
        for (pad = 0; pad < 3; pad++) {
            rumble->states[pad] = -1;
        }
    }
    rumble->active = -1;
    rumble->time = 0.0f;
    rumble->stopping = 0;
    return 0;
}
