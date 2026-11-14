typedef struct PickCtx {
    char pad_0000[0x1E70];
    unsigned int field_1E70;
    char pad_1E74[0x2048 - 0x1E74];
    int field_2048;
    char pad_204C[0x259C - 0x204C];
    int field_259C;
    char pad_25A0[8];
    unsigned char field_25A8;
} PickCtx;

typedef struct PickOwner {
    char pad_000[0x28F0];
    PickCtx *field_28F0;
} PickOwner;

unsigned int func_08A256C8(PickOwner *self)
{
    PickCtx *ctx = self->field_28F0;
    unsigned int id = 0xFFFFFFFF;
    if ((ctx->field_1E70 & 2) != 0) {
        if ((*(unsigned char *)((char *)ctx + ctx->field_2048 * 8 + 0x25AC) & 1) != 0) {
            id = *(unsigned short *)((char *)ctx + ctx->field_259C * 0x10 + 0x1EA0);
        }
    } else if (ctx->field_25A8 != 0) {
        id = *(unsigned short *)((char *)ctx + ctx->field_259C * 0x10 + 0x1EA0);
    }
    return id;
}
