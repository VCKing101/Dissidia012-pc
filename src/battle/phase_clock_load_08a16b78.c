#include "battle/agent28_types.h"

/* Loads the phase clock's face and hand models; the hand starts at zero. */
extern void *func_088DAA40(void *source, int name, int flags);
extern void func_088DA060(void *model);
extern void func_088D9F00(void *model, float frame);
extern void func_08A169DC(PhaseClock *clock);
extern int D_08A16D2C[];

int func_08A16B78(PhaseClock *clock)
{
    if (!clock->open) {
        return -1;
    }
    if (clock->face == 0) {
        clock->face = func_088DAA40(clock->source, 0x15, 0);
        if (clock->face == 0) {
            return -2;
        }
    }
    if (clock->hand == 0) {
        clock->hand = func_088DAA40(clock->source, D_08A16D2C[clock->kind], 0);
        if (clock->hand == 0) {
            if (clock->face != 0) {
                func_088DA060(clock->face);
                clock->face = 0;
            }
            return -3;
        }
        func_088D9F00(clock->hand, 0.0f);
    }
    func_08A169DC(clock);
    return 0;
}
