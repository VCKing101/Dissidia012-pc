typedef struct PageCtx {
    char pad_0000[0xCACC];
    int field_CACC;
    int field_CAD0;
    int field_CAD4;
    int field_CAD8;
} PageCtx;

typedef struct PageOwner {
    char pad_000[0xDB30];
    PageCtx *field_DB30;
} PageOwner;

void func_08AD030C(PageOwner *self)
{
    int value = 7;
    PageCtx *ctx = self->field_DB30;
    if (ctx->field_CAD4 == ctx->field_CAD8 - 1) {
        value = ctx->field_CAD0 - ctx->field_CAD4 * 7;
    }
    ctx->field_CACC = value;
}
