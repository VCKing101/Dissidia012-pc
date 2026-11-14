typedef struct LabelSetup {
    int field_00;
    unsigned char field_04;
    char pad_05;
    short field_06;
    int field_08;
    char pad_0C[0x10];
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    char pad_2C[0xA];
    unsigned short field_36;
    unsigned short field_38;
    unsigned short field_3A;
    unsigned short field_3C;
} LabelSetup;

void func_08A5BC98(LabelSetup *self, int owner, int text)
{
    self->field_00 = owner;
    self->field_04 = 0x1A;
    self->field_06 = 0;
    self->field_08 = text;
    self->field_1C = 4;
    self->field_20 = 5;
    self->field_24 = 6;
    self->field_28 = 7;
    self->field_36 = 0x6434;
    self->field_38 = 0x6411;
    self->field_3A = 0x6454;
    self->field_3C = 0x6474;
}
