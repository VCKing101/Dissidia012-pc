extern "C" {
#include "battle/runtime_helpers.h"
#include "battle/drawing_step.h"
}

/* Stateless accessors operate on the established drawing prefix directly. */
struct BattleDrawingFlagOps {
    static bool has_flag(const MainDrawingRecordPrefix *self, unsigned int mask) { return (self->field_0020 & mask) != 0; }
    static void clear_flag(MainDrawingRecordPrefix *self, unsigned int mask) {
        self->field_0020 &= ~mask;
        func_088D9D24(self);
    }
};

extern "C" {
int func_089E96DC(void *);
void func_088D9CD8(MainDrawingRecordPrefix *);
void func_088D9CFC(MainDrawingRecordPrefix *);
void func_088D9AEC(MainDrawingRecordPrefix *, float);

int func_089E56EC(BattleDrawingStep *state)
{
    int current = func_089E96DC(state->field_04);
    if (current < state->field_0C) {
        if (BattleDrawingFlagOps::has_flag(state->field_D4, 8) == false) {
            func_088D9CD8(state->field_D4);
            func_088D9AEC(state->field_D4, 0.0f);
        }
    }
    if (BattleDrawingFlagOps::has_flag(state->field_D4, 8) == true) {
        if (BattleDrawingFlagOps::has_flag(state->field_D4, 0x20) == true) {
            func_088D9CFC(state->field_D4);
            BattleDrawingFlagOps::clear_flag(state->field_D4, 0x20);
        }
    }
    state->field_0C = current;
    return 0;
}
}
