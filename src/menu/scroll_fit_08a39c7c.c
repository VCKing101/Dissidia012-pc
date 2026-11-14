/* Sets up the eight-row scroll bar; on the last page only the remaining rows count. */
typedef struct FitScroll {
    unsigned int field_00;
    int field_04;
} FitScroll;

typedef struct FitState {
    char pad_0000[0xDC];
    FitScroll field_DC;
    char pad_00E4[0x10C - 0xE4];
    int field_10C;
    char pad_0110[0x4];
    int field_114;
    char pad_0118[0x1258 - 0x118];
    int field_1258;
} FitState;

typedef struct FitOwner {
    char pad_0000[0x1274];
    FitState *field_1274;
} FitOwner;

extern void func_088D46E4(FitScroll *scroll, int top, int start, int last, int height, int shown);

void func_08A39C7C(FitOwner *self, int position)
{
    FitState *state = self->field_1274;
    int top = state->field_DC.field_04;
    int last;
    FitScroll *scroll;

    if (position != -1) {
        top = position;
    }
    last = 7;
    if (state->field_10C == state->field_114) {
        last = (state->field_1258 - state->field_10C * 8) % 8 - 1;
    }
    if (last < top) {
        top = last;
    }
    func_088D46E4(&state->field_DC, top, 0, last, 0, 8);
    scroll = &self->field_1274->field_DC;
    scroll->field_00 |= 1;
}
