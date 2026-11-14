typedef struct BandEntry {
    unsigned short field_00;
    unsigned short field_02;
    int field_04;
} BandEntry;

typedef struct BandState {
    char pad_0000[0xF80];
    int field_F80;
    char pad_F84[0x122C - 0xF84];
    int field_122C;
    BandEntry field_1230[1];
} BandState;

typedef struct BandOwner {
    char pad_0000[0xDB30];
    BandState *field_DB30;
} BandOwner;

extern int func_08ACD744(BandOwner *self, int id);

void func_08ACBF58(BandOwner *self)
{
    int slot = func_08ACD744(self, self->field_DB30->field_1230[self->field_DB30->field_F80].field_02);
    int odd = 0;
    if (slot < 0 || slot >= 3) {
        if (slot < 6 || slot >= 9) {
            odd = 1;
        }
    }
    self->field_DB30->field_122C = odd;
}
