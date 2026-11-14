/* Writes the cursor target for the current mode (0: by id, 1: by handle). */
typedef struct TargetInfo {
    unsigned char field_00;
    char pad_01;
    short field_02;
    int field_04;
} TargetInfo;

typedef struct TargetPanel {
    char pad_000[0x14];
    int field_14;
    char pad_018[0x78 - 0x18];
    int field_78;
    char pad_07C[0xA0 - 0x7C];
    TargetInfo *field_A0;
    char pad_0A4[0x4A4 - 0xA4];
    int field_4A4[8];
} TargetPanel;

void func_08A63C74(TargetPanel *self)
{
    int index = self->field_78;
    switch (self->field_14) {
    case 0:
        self->field_A0->field_00 = 0;
        self->field_A0->field_02 = self->field_4A4[index];
        break;
    case 1:
        self->field_A0->field_00 = 2;
        self->field_A0->field_02 = -1;
        self->field_A0->field_04 = self->field_4A4[index];
        break;
    }
}
