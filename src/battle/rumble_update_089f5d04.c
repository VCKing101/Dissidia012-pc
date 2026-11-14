#include "battle/agent28_types.h"

/* Advances the rumble set: starts the waiting slots when the delay passes and
 * stops the running ones once stopping is requested. */
extern FrameClock *D_089D69E0;
extern void func_089F5380(RumbleSlot *slot, int strength);
extern void func_08889FB8(int pad, int mode, int strength);

int func_089F5D04(RumbleSet *rumble)
{
    float before = rumble->time;

    rumble->time = before + D_089D69E0->frame_scale;
    if (before < rumble->delay && rumble->delay <= rumble->time) {
        int pad;
        for (pad = 0; pad < 3; pad++) {
            if (rumble->states[pad] == 1) {
                func_089F5380(&rumble->slots[pad], rumble->strengths[pad]);
            }
        }
    }
    if (rumble->stopping == 1) {
        int pad;
        for (pad = 0; pad < 3; pad++) {
            if (rumble->states[pad] == 2) {
                func_08889FB8(pad, 2, 0);
            }
        }
    }
    if (rumble->time >= rumble->delay && rumble->stopping == 1) {
        int pad;
        for (pad = 0; pad < 3; pad++) {
            rumble->states[pad] = -1;
        }
        rumble->active = -1;
        rumble->time = 0.0f;
        rumble->stopping = 0;
    }
    return 0;
}
