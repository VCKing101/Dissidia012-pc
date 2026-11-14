typedef struct AnimHandle {
    char pad_00[0xE];
    short field_0E;
} AnimHandle;

typedef struct AnimStartA {
    char pad_00[0x2C];
    AnimHandle *field_2C;
    char pad_30[0x158 - 0x30];
    int field_158;
} AnimStartA;

extern void func_088D9F5C(AnimHandle *handle, int anim, unsigned char flag);

void func_089ECBFC(AnimStartA *self)
{
    func_088D9F5C(self->field_2C, 0, 1);
    self->field_2C->field_0E = 1;
    self->field_158 = 1;
}
