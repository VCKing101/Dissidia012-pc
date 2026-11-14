typedef struct AnimHandle {
    char pad_00[0xE];
    short field_0E;
} AnimHandle;

typedef struct AnimStartC {
    char pad_00[0x14];
    AnimHandle *field_14;
    char pad_18[0x20 - 0x18];
    int field_20;
} AnimStartC;

extern void func_088D9F5C(AnimHandle *handle, int anim, char flag);

void func_089EEB08(AnimStartC *self)
{
    func_088D9F5C(self->field_14, 0x1F, 1);
    self->field_14->field_0E = 0x22;
    int one = 1;
    self->field_20 = one;
}
