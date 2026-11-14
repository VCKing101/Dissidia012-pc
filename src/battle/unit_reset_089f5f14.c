typedef struct Unit5F14 {
    char field_00;
    char pad_001[0x3];
    int field_04;
    int field_08;
    int field_0C;
    char pad_010[0x20];
    char field_30;
    char pad_031[0x3];
    float field_34;
    int field_38;
    int field_3C;
    float field_40;
} Unit5F14;

void func_089F5F14(Unit5F14 *self)
{
    self->field_00 = 0;
    self->field_04 = -1;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_30 = 0;
    self->field_34 = 0.0f;
    self->field_38 = 0;
    self->field_3C = 0;
    self->field_40 = 0.0f;
}
