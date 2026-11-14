typedef struct ShopSlot {
    signed char field_00;
    char pad_01[0xB];
} ShopSlot;

typedef struct ShopState {
    char pad_0000[0x400];
    char field_400[0x195C - 0x400];
    char field_195C[4];
    int field_1960;
    char pad_1964b[0x19B8 - 0x1964];
    int field_19B8;
    int field_19BC;
    int pad_19C0;
    unsigned short field_19C4[0x12];
    ShopSlot field_19E8[0x3E];
    char pad_1CD0[0x1CE0 - 0x1CD0];
    unsigned short field_1CE0[3][5];
    char pad_1CFE[0x1D12 - 0x1CFE];
    unsigned char field_1D12[0x40];
} ShopState;

typedef struct ShopOwner {
    char pad_0000[0x23F0];
    ShopState *field_23F0;
} ShopOwner;

extern void func_08AEC824(ShopOwner *self);

int func_08AEC934(ShopOwner *self)
{
    ShopState *state = self->field_23F0;
    int slot = state->field_1D12[state->field_1960];
    int result = 0;
    if (state->field_19E8[slot].field_00 < 0) {
        state->field_19E8[slot].field_00 = state->field_19BC;
        self->field_23F0->field_19C4[self->field_23F0->field_19BC] = slot;
        func_08AEC824(self);
        result = 1;
    }
    return result;
}
