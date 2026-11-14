typedef struct AnimHandle {
    char pad_00[0xE];
    short field_0E;
} AnimHandle;

typedef struct AnimCtx {
    char pad_000[0x404];
    AnimHandle *field_404;
} AnimCtx;

typedef struct AnimOwner {
    char pad_000[0x138];
    AnimCtx *field_138;
} AnimOwner;

extern void func_088D9F5C(AnimHandle *handle, int anim, unsigned char flag);

void func_08AC5D70(AnimOwner *self)
{
    func_088D9F5C(self->field_138->field_404, 0, 1);
    self->field_138->field_404->field_0E = 1;
}
