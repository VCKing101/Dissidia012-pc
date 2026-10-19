extern "C" {
#include "main/drawing_record.h"
}

/* This adds methods, not object fields, to the established drawing prefix. */
struct BattleDrawingFlagView : MainDrawingRecordPrefix {
    bool has_flag(unsigned int mask) { return (field_0020 & mask) != 0; }
    void clear_flag(unsigned int mask) {
        field_0020 &= ~mask;
        func_088D9D24(this);
    }
};
struct BattleDrawingStep {
    unsigned char padding_00[4];
    void *field_04;
    unsigned char padding_08[4];
    int field_0C;
    unsigned char padding_10[0xC4];
    MainDrawingRecordPrefix *field_D4;
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
        if (((BattleDrawingFlagView *)state->field_D4)->has_flag(8) == false) {
            func_088D9CD8(state->field_D4);
            func_088D9AEC(state->field_D4, 0.0f);
        }
    }
    if (((BattleDrawingFlagView *)state->field_D4)->has_flag(8) == true) {
        if (((BattleDrawingFlagView *)state->field_D4)->has_flag(0x20) == true) {
            func_088D9CFC(state->field_D4);
            ((BattleDrawingFlagView *)state->field_D4)->clear_flag(0x20);
        }
    }
    state->field_0C = current;
    return 0;
}
}
