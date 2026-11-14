typedef struct ShopState {
    char pad_0000[0x4A4];
    int field_4A4;
    char pad_04A8[0x4DC - 0x4A8];
    int field_4DC[8];
    char pad_04FC[0x1960 - 0x4FC];
    int field_1960;
    char pad_1964[8];
    int field_196C;
    char pad_1970[0x19BC - 0x1970];
    int field_19BC;
    int field_19C0;
} ShopState;

typedef struct ShopOwner {
    char pad_0000[0x23F0];
    ShopState *field_23F0;
} ShopOwner;

extern unsigned short D_08AEFD50[];
extern void func_088D9F5C(int target, int kind, int value);
extern void func_088D9CFC(int handle);

void func_08AEBFDC(ShopOwner *self)
{
    ShopState *state = self->field_23F0;
    int row = state->field_1960 - state->field_196C;
    int kind;
    if (state->field_19BC < state->field_19C0) {
        func_088D9F5C(state->field_4A4, D_08AEFD50[state->field_19BC], 1);
        kind = 9;
    } else {
        func_088D9CFC(state->field_4A4);
        kind = 10;
    }
    func_088D9F5C(self->field_23F0->field_4DC[row], kind, 1);
}
