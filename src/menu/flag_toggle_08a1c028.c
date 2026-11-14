typedef struct ToggleCtx {
    char pad_0000[0x2BB8];
    unsigned int field_2BB8;
    char pad_2BBC[0x652C - 0x2BBC];
    unsigned short field_652C;
} ToggleCtx;

typedef struct ToggleOwner {
    char pad_000[0x154];
    ToggleCtx *field_154;
} ToggleOwner;

void func_08A1C028(ToggleOwner *self, unsigned char on)
{
    ToggleCtx *ctx = self->field_154;
    if (on != 0) {
        ctx->field_2BB8 = ctx->field_2BB8 | 1;
        self->field_154->field_652C = self->field_154->field_652C | 0x20;
    } else {
        ctx->field_2BB8 = ctx->field_2BB8 & ~1;
        self->field_154->field_652C = self->field_154->field_652C & ~0x20;
    }
}
