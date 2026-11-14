typedef struct AnimHandle {
    char pad_00[0xE];
    short field_0E;
} AnimHandle;

typedef struct AnimStartB {
    char pad_00[8];
    AnimHandle *field_08;
    char pad_0C[0x60 - 0xC];
    int field_60;
} AnimStartB;

extern void func_088D9F5C(AnimHandle *handle, int anim, unsigned char flag);

void func_089EE59C(AnimStartB *self)
{
    func_088D9F5C(self->field_08, 0, 1);
    self->field_08->field_0E = 1;
    self->field_60 = 1;
}
