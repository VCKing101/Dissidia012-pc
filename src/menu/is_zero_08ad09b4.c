typedef struct ZeroCtx {
    char pad_0000[0xDB08];
    int field_DB08;
} ZeroCtx;

typedef struct ZeroOwner {
    char pad_000[0xDB30];
    ZeroCtx *field_DB30;
} ZeroOwner;

unsigned char func_08AD09B4(ZeroOwner *self)
{
    if (self->field_DB30->field_DB08 == 0) {
        return 1;
    }
    return 0;
}
