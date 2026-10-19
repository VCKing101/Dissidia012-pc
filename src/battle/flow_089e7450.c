#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */
extern void func_088DA060(void *);
extern void func_089E7430(void *);

void func_089E7450(BattleRelease7450 *state)
{
    if (state->field_0C) {
        func_088DA060(state->field_0C);
        state->field_0C = 0;
    }
    if (state->field_08) {
        func_088DA060(state->field_08);
        state->field_08 = 0;
    }
    func_089E7430(state);
}
