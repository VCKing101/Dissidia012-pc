#include "battle/agent28_types.h"

/* Fades the duel widget out over the fade time, then ends the phase. */
extern float D_089F4F88;
extern void func_089F3E14(void *widget, float alpha);
extern void func_089F3C34(void *widget);

int func_089F4E30(DuelPhaseData *phase, float step)
{
    phase->time += step;
    if (phase->time > D_089F4F88) {
        phase->time = D_089F4F88;
    }
    func_089F3E14(phase->widget, (D_089F4F88 - phase->time) / D_089F4F88);
    if (phase->time >= D_089F4F88) {
        func_089F3C34(phase->widget);
        phase->state = -1;
        phase->time = 0.0f;
    }
    return 0;
}
