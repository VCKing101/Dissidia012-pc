/* Picks the target entry for the cursor (from the slot list in mode 4, or the
   grid in mode 2; 0x7FFF is none), stores its id and sends it. */
struct Target08ADB784 {
    short field_0;
    char pad_2[6];
};

struct State08ADB784 {
    char pad_0000[0x1B7C];
    unsigned int field_1B7C;
    char pad_1B80[0x1BC4 - 0x1B80];
    int field_1BC4;
    char pad_1BC8[0x1BF0 - 0x1BC8];
    int field_1BF0;
    char pad_1BF4[0x1CA4 - 0x1BF4];
    int field_1CA4;
    char pad_1CA8[0x1CB4 - 0x1CA8];
    struct Target08ADB784 field_1CB4[(0x3064 - 0x1CB4) / 8];
    unsigned short field_3064[5][10];
    unsigned short field_30C8[(0x3630 - 0x30C8) / 2];
    int field_3630;
    int field_3634;
};

struct Screen08ADB784 {
    char pad_0000[0x46AC];
    struct State08ADB784 *field_46AC;
};

extern void *D_08B01CF8;
extern void func_08AF58E8(void *queue, int *message);

void func_08ADB784(struct Screen08ADB784 *self)
{
    unsigned int slot = 0x7FFF;
    int target;
    if ((self->field_46AC->field_1B7C & 4) != 0) {
        slot = self->field_46AC->field_30C8[self->field_46AC->field_1BF0];
        if (slot == 0x7FFF) {
            slot = 0x7FFF;
        }
    } else if ((self->field_46AC->field_1B7C & 2) != 0) {
        slot = self->field_46AC->field_3064[self->field_46AC->field_1CA4][self->field_46AC->field_1BC4];
    }
    target = -1;
    if (slot != 0x7FFF) {
        target = self->field_46AC->field_1CB4[slot].field_0;
    }
    self->field_46AC->field_3634 = target;
    func_08AF58E8(D_08B01CF8, &self->field_46AC->field_3630);
}
