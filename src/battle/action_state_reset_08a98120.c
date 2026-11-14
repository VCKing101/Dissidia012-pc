typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct ActionState {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    unsigned char field_08;
    char pad_09[0xB];
    Vec3 field_14;
    unsigned char field_20;
    char pad_21[0x3];
    Vec3 field_24;
    Vec3 field_30;
    Vec3 field_3C;
    int field_48;
    int field_4C;
    int field_50;
    void *field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    unsigned char field_70;
    char pad_71[0x3];
    void *field_74;
    int field_78;
    int field_7C;
    float field_80;
    int field_84;
    int field_88;
    int field_8C;
} ActionState;

extern void func_089A34E0(Vec3 *v);

void func_08A98120(ActionState *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    func_089A34E0(&self->field_14);
    self->field_20 = 0;
    func_089A34E0(&self->field_24);
    func_089A34E0(&self->field_30);
    func_089A34E0(&self->field_3C);
    self->field_48 = -1;
    self->field_4C = -1;
    self->field_50 = -1;
    self->field_54 = 0;
    self->field_5C = 0;
    self->field_58 = 0;
    self->field_60 = 0;
    self->field_68 = 0;
    self->field_64 = 0;
    self->field_6C = 0;
    self->field_70 = 0;
    self->field_74 = 0;
    self->field_78 = 0;
    self->field_7C = 0;
    self->field_80 = 0.0f;
    self->field_84 = 0;
    self->field_88 = 0;
    self->field_8C = 0;
}
