/* Returns the value of the slot under the cursor in the current set (-1 when
   the cursor is on no slot). */
struct Slot08AE7C94 {
    unsigned short value;
    char pad_2[6];
};

struct Set08AE7C94 {
    struct Slot08AE7C94 slots[0x2BF];
};

struct State08AE7C94 {
    char pad_0000[0x2558];
    unsigned int field_2558;
    char pad_255C[0x25C0 - 0x255C];
    int field_25C0;
    char pad_25C4[0x2694 - 0x25C4];
    int field_2694;
    int field_2698;
    char pad_269C[0x26B8 - 0x269C];
    struct Set08AE7C94 field_26B8[4];
    unsigned short field_7E98[5][4];
    unsigned short field_7EC0[4][0x2BF];
};

struct Screen08AE7C94 {
    char pad_0000[0xA690];
    struct State08AE7C94 *field_A690;
};

unsigned int func_08AE7C94(struct Screen08AE7C94 *self)
{
    unsigned int value = -1;
    unsigned int slot = 0x7FFF;
    if ((self->field_A690->field_2558 & 4) != 0) {
        slot = self->field_A690->field_7EC0[self->field_A690->field_2698][self->field_A690->field_25C0];
    } else if ((self->field_A690->field_2558 & 2) != 0) {
        slot = self->field_A690->field_7E98[self->field_A690->field_2694][self->field_A690->field_2698];
    }
    if (slot != 0x7FFF) {
        value = self->field_A690->field_26B8[self->field_A690->field_2698].slots[slot].value;
    }
    return value;
}
