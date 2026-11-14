#include "battle/agent28_types.h"

/* Hold state: counts time while the battle runs and moves on to state 3
 * once the hold time (or, for kind 0 cards, the wait time) has passed. */
extern FrameClock *D_089D69E0;
extern void *D_08A22AB0;
extern float D_089ECEE0;
extern float D_089ECEE4;
extern int D_089ECF00[2];
extern unsigned int func_089E95E0(unsigned int side);
extern void *func_08A22970(void *battle);
extern void *func_08A074FC(void *scene);
extern int func_08A15A70(void *phase);
extern void func_088D9F5C(EffectModel *model, int motion, int loop);

int func_089EC4D8(CardRecord *record)
{
    unsigned int side = func_089E95E0(record->side);

    if (side >= 2) {
        return -1;
    }
    if (func_08A15A70(func_08A074FC(func_08A22970(D_08A22AB0))) == 1) {
        record->time += D_089D69E0->frame_scale;
    }
    if ((record->held == 1 && record->time >= D_089ECEE0) ||
        (record->kind == 0 && record->time >= D_089ECEE4 && record->skip == 0)) {
        if (record->model != 0) {
            func_088D9F5C(record->model, D_089ECF00[side], 1);
        }
        record->state = 3;
    }
    record->skip = 0;
    return 0;
}
