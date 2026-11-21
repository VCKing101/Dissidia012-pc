#include "menu/agent36_types.h"

/* Item list screen: runs the current step (+0x1254) of its three-step
 * member-function table (built in place, called through the real pointer
 * type); at step 3 returns 3, after func_08A41B28 in mode 1, else 1. */

extern "C" {
void func_08A3A0CC(ShopScreen36 *self);
void func_08A3B0EC(ShopScreen36 *self);
void func_08A3A39C(ShopScreen36 *self);
void func_08A41B28(BattleGlobal36 *battle);
}

extern "C" int func_08A3A6B0(ShopScreen36 *self)
{
    Step36 steps[3];
    int done = 3;

    steps[0].delta = 0;
    steps[0].index = 0;
    steps[0].target = (void *)func_08A3A0CC;
    steps[1].delta = 0;
    steps[1].index = 0;
    steps[1].target = (void *)func_08A3B0EC;
    steps[2].delta = 0;
    steps[2].index = 0;
    steps[2].target = (void *)func_08A3A39C;
    (((Stepper36 *)self)->*(*(StepFn36 *)&steps[self->work->state]))();
    if (self->work->state == done) {
        if (self->mode == 1) {
            func_08A41B28(D_08A457EC);
            return done;
        }
        return 1;
    }
    return 0;
}
