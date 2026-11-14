#include "battle/agent28_types.h"

/* After the reveal delay, shows each side's result: the score parts for a
 * side still playing, the badge for a finished one; then moves to state 5. */
extern float D_089F4F84;
extern int func_089E99C0(int side, int score);
extern int func_089F370C(DuelPartSet *parts, int kind, int flag);
extern void func_089F854C(DuelGauge *gauge);
extern void func_08A08350(DuelBadge *badge);
extern void func_089F3C34(void *widget);

int func_089F4C54(DuelPhaseData *phase, float step)
{
    phase->time += step;
    if (phase->time >= D_089F4F84) {
        int side;
        for (side = 0; side < 2; side++) {
            if (phase->done[side] == 0) {
                int turn = (phase->wins + phase->losses + side) % 2;
                int score = phase->scores[side];
                func_089F370C(&phase->parts[side], score, func_089E99C0(turn, score));
                func_089F854C(&phase->gauges[side]);
            } else {
                func_08A08350(&phase->badges[side]);
            }
        }
        func_089F3C34(phase->widget);
        phase->state = 5;
    }
    return 0;
}
