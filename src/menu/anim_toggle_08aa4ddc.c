typedef struct AnimCtx {
    char pad_0000[0x1E70];
    unsigned int field_1E70;
    char pad_1E74[0x1E94 - 0x1E74];
    int field_1E94;
    int field_1E98;
} AnimCtx;

typedef struct AnimOwner {
    char pad_0000[0x1EE4];
    AnimCtx *field_1EE4;
} AnimOwner;

extern short D_08AA8800[];

void func_08AA4DDC(AnimOwner *self)
{
    AnimCtx *ctx;
    self->field_1EE4->field_1E94 = D_08AA8800[self->field_1EE4->field_1E98];
    self->field_1EE4->field_1E70 = self->field_1EE4->field_1E70 & ~0x200;
    ctx = self->field_1EE4;
    if (ctx->field_1E94 == 0) {
        ctx->field_1E70 = ctx->field_1E70 | 0x200;
    }
}
