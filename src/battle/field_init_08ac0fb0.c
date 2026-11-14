typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

typedef struct FieldBlockC {
    unsigned char field_00;
    char pad_01[0x3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    float field_1C;
    int field_20;
    unsigned char field_24;
    char pad_25[0x3];
    float field_28;
    int field_2C;
    Vec3 field_30;
    Vec3 field_3C;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    Vec3 field_60;
    float field_6C;
    char pad_70[0x60];
    int field_D0;
    int field_D4;
    int field_D8;
} FieldBlockC;

extern void func_089A34E0(Vec3 *v);

void func_08AC0FB0(FieldBlockC *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = 0.0f;
    self->field_20 = 0;
    self->field_24 = 0;
    self->field_28 = 0.0f;
    self->field_2C = -1;
    func_089A34E0(&self->field_30);
    func_089A34E0(&self->field_3C);
    self->field_4C = 0;
    self->field_48 = 0;
    self->field_50 = 0;
    self->field_58 = 0;
    self->field_54 = 0;
    self->field_5C = 0;
    func_089A34E0(&self->field_60);
    self->field_6C = 0.0f;
    self->field_D0 = 0;
    self->field_D4 = 0;
    self->field_D8 = 0;
}
