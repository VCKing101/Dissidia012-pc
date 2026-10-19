#include "battle/runtime_helpers.h"

/* Partial state record used by the 0x089E56xx wrappers. Names are neutral:
 * no particular combat mechanic has yet been assigned to these fields. */

int func_089E5BDC(BattleControlState *state)
{
    if (!state->field_00) return -1;
    if (state->field_04 == 1) {
        state->field_04 = 2;
        state->field_08 = 0.0f;
    }
    state->field_01 = 0;
    return 0;
}

int func_089E5C28(BattleControlState *state)
{
    if (!state->field_00) return -1;
    state->field_01 = 1;
    state->field_04 = 1;
    state->field_08 = 0.0f;
    return 0;
}

int func_089E5C60(BattleControlState *state)
{
    if (!state->field_00) return -1;
    state->field_02 = 1;
    return 0;
}

int func_089E5C88(BattleControlState *state)
{
    if (!state->field_00) return -1;
    state->field_02 = 0;
    return 0;
}
