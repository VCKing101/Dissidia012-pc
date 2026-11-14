#include "battle/agent28_types.h"

/* Closes the display set: both fighter displays and their gauges, the
 * shared effect, the held texture and the record pool. */
extern void *D_089D7830;
extern void func_089E9024(DisplayGauge *gauge);
extern void func_089E5314(FighterDisplay *display);
extern void func_089E6EFC(void *effect);
extern void func_088D0A1C(void *cache, void *texture);
extern void func_089DE1D8(void *pool);
extern void func_089E7AD0(DisplaySet *set);

void func_089E7AFC(DisplaySet *set)
{
    {
        int i;
        for (i = 0; i < 2; i++) {
            func_089E9024(&set->gauges[i]);
            func_089E5314(&set->displays[i]);
        }
    }
    func_089E6EFC(set->effect);
    if (set->texture != 0) {
        func_088D0A1C(D_089D7830, set->texture);
    }
    func_089DE1D8(set->pool);
    func_089E7AD0(set);
}
