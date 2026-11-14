#include "battle/agent28_types.h"

/* Waits out the flag delay, then loads the side's flag model and moves to
 * state 1. */
extern FrameClock *D_089D69E0;
extern float D_08A06B1C;
extern unsigned int func_08A22CA8(int owner);
extern void func_088DA060(void *model);
extern void *func_088DAA40(void *source, int name, int flags);
extern int D_08A06B68[2];

int func_08A06684(SideFlag *flag)
{
    if (flag->state == 0) {
        flag->time += D_089D69E0->frame_scale;
        if (flag->time >= D_08A06B1C) {
            unsigned int side = func_08A22CA8(flag->owner);
            if (side >= 2) {
                return -1;
            }
            if (flag->model != 0) {
                func_088DA060(flag->model);
                flag->model = 0;
            }
            flag->model = func_088DAA40(flag->source, D_08A06B68[side], 0);
            if (flag->model == 0) {
                return -2;
            }
            flag->state = 1;
            flag->time = 0.0f;
        }
    }
    return 0;
}
