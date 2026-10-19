#include "battle/runtime_helpers.h"
#include "battle/oct13_source_views.h"

extern void *func_08935828(void *, int, unsigned int);

void func_089E399C(void *object)
{
    BattleOct13BufferView *state = (BattleOct13BufferView *)object;
    state->field_00 = 0;
    state->field_04 = -1;
    func_08935828(state->field_A4, 0, 0x30);
}
