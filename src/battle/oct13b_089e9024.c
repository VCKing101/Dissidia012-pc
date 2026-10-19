#include "battle/oct13_activation_views.h"

void func_089E9024(void *object)
{
    BattleOct13FourResources *state = (BattleOct13FourResources *)object;
    if (state->field_14) { func_088DA060(state->field_14); state->field_14 = 0; }
    if (state->field_10) { func_088DA060(state->field_10); state->field_10 = 0; }
    if (state->field_0C) { func_088DA060(state->field_0C); state->field_0C = 0; }
    if (state->field_08) { func_088DA060(state->field_08); state->field_08 = 0; }
    func_089E8FF8(state);
}
