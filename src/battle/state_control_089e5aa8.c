#include "battle/runtime_helpers.h"

/* Partial state record used by the 0x089E56xx wrappers. Names are neutral:
 * no particular combat mechanic has yet been assigned to these fields. */

void func_089E5AA8(BattleControlState *state)
{
    state->field_00 = 0;
    state->field_01 = 0;
    state->field_02 = 0;
    state->field_04 = 0;
    state->field_08 = 0.0f;
    state->field_0C = -1;
    state->field_10 = 0;
}
