typedef struct ModeState {
    char pad_0000[0x1FD4];
    short field_1FD4[0x16];
    int field_2000;
    char pad_2004[0x259C - 0x2004];
    int field_259C;
    int field_25A0;
    int field_25A4;
} ModeState;

typedef struct ModeOwner {
    char pad_0000[0x28F0];
    ModeState *field_28F0;
} ModeOwner;

extern void func_08A282CC(ModeOwner *self, int value);
extern void func_08A243C4(ModeOwner *self);
extern void func_08A247AC(ModeOwner *self);

void func_08A24330(ModeOwner *self, unsigned char direct)
{
    ModeState *state = self->field_28F0;
    int value = state->field_1FD4[state->field_2000];
    state->field_259C = value;
    func_08A282CC(self, value);
    if (direct) {
        self->field_28F0->field_25A0 = 1;
        func_08A243C4(self);
        self->field_28F0->field_25A4 = 1;
        func_08A247AC(self);
    } else {
        self->field_28F0->field_25A0 = 2;
        self->field_28F0->field_25A4 = 1;
    }
}
