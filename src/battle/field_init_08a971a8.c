typedef struct FieldBlockB {
    unsigned char field_00;
    char pad_01[0x3];
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    float field_24;
    float field_28;
    int field_2C;
    int field_30;
    unsigned char field_34;
    char pad_35[0x3];
    char field_38[0x118];
    int field_150;
    int field_154;
    int field_158;
    char field_15C[0x28];
    int field_184;
    unsigned char field_188;
    char pad_189[0x3];
    int field_18C;
} FieldBlockB;

extern void func_088C01C0(void *sub);
extern void func_08935828(void *dest, int value, int size);

void func_08A971A8(FieldBlockB *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = -1;
    self->field_10 = -1;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = 0;
    self->field_20 = -1;
    self->field_24 = 0.0f;
    self->field_28 = 0.0f;
    self->field_2C = 0;
    self->field_30 = -1;
    self->field_34 = 0;
    func_088C01C0(self->field_38);
    self->field_150 = -1;
    self->field_154 = 0;
    self->field_158 = 0;
    func_08935828(self->field_15C, 0, 0x28);
    self->field_184 = -1;
    self->field_188 = 0;
    self->field_18C = -1;
}
