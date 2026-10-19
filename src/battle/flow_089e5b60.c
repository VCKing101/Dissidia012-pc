#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */
extern void *D_089D69E0;
extern float D_089E6078;

int func_089E5B60(BattleControlState *state)
{
    if (!state->field_00) return -1;
    if (state->field_04 == 2) {
        state->field_08 += D_089E6078 * ((BattleScale18 *)D_089D69E0)->field_18 * 0.5f;
        if (!(state->field_08 <= 1.0f)) state->field_08 = 1.0f;
    }
    return 0;
}
