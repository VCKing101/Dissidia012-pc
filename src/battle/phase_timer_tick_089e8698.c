#include "battle/agent28_types.h"

/* Advances a phase timer and fires the phase's events at the times set for
 * its kind. */
extern FrameClock *D_089D69E0;
extern float D_089E8D18;
extern float D_089E8D1C;
extern float D_089E8D20;
extern int func_089E8BE4(PhaseTimer *timer);
extern void func_089E8CA8(PhaseTimer *timer);

int func_089E8698(PhaseTimer *timer)
{
    float before = timer->time;

    timer->time = before + D_089D69E0->frame_scale;
    switch (timer->kind) {
    case 0:
    case 1:
    case 4:
    case 5:
        if (before < D_089E8D18 && D_089E8D18 <= timer->time) {
            func_089E8BE4(timer);
        }
        if (D_089E8D1C <= timer->time) {
            func_089E8CA8(timer);
        }
        break;
    case 2:
    case 3:
        if (D_089E8D20 <= timer->time) {
            func_089E8CA8(timer);
        }
        break;
    case 6:
    case 7:
    case 8:
        if (D_089E8D1C - D_089E8D18 <= timer->time) {
            func_089E8CA8(timer);
        }
        break;
    default:
        func_089E8CA8(timer);
        break;
    }
    return 0;
}
