typedef struct TableCtx {
    char pad_000[0x400];
    char field_400[1];
    char pad_401[0x1BF0 - 0x401];
    int field_1BF0;
    char pad_1BF4[0x1BFC - 0x1BF4];
    int field_1BFC;
} TableCtx;

typedef struct TableOwner {
    char pad_000[0x46AC];
    TableCtx *field_46AC;
} TableOwner;

extern void func_088D3AE8(void *a, int b, int c, int d);
extern unsigned short D_08ADFAC0[];

void func_08ADAAF0(TableOwner *self)
{
    TableCtx *ctx = self->field_46AC;
    func_088D3AE8(ctx->field_400, 0x55, D_08ADFAC0[ctx->field_1BF0 - ctx->field_1BFC], 0x21);
}
