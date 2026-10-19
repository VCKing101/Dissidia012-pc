extern "C" {
#include "battle/runtime_helpers.h"
#include "battle/oct13_source_views.h"

extern float D_089D5EAC, D_089E68B4;
extern float func_089E6444(void *);
extern int func_089E973C(unsigned int);

void func_089E6450(void *object, int current)
{
    BattleCurveView *state = (BattleCurveView *)object;
    switch (state->field_0C) {
    case 0:
        if (current < state->field_10) {
            float previous = (float)state->field_10;
            state->field_0C = 1;
            state->field_18 = 0.0f;
            state->field_14 = previous;
        }
        break;
    case 1:
        {
            float value = (float)current;
            if (!(value < state->field_14)) state->field_0C = 0;
            if (current < state->field_10) state->field_18 = 0.0f;
            else {
                float delta = D_089D5EAC;
                float progress = state->field_18 + delta;
                state->field_18 = progress;
                if (!(progress <= func_089E6444(state))) {
                    float origin = state->field_14;
                    int limit = func_089E973C(state->field_04);
                    state->field_14 = origin - delta * (float)limit / D_089E68B4;
                    if (!(value < state->field_14)) state->field_0C = 0;
                }
            }
        }
    }
    state->field_10 = current;
}
}
