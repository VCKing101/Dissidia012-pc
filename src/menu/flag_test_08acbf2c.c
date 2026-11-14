typedef struct TestCtx {
    char pad_000[0x13F8];
    int field_13F8;
} TestCtx;

typedef struct TestOwner {
    char pad_000[0xDB30];
    TestCtx *field_DB30;
} TestOwner;

unsigned char func_08ACBF2C(TestOwner *self)
{
    if (self->field_DB30->field_13F8 != 0) {
        return 1;
    }
    return 0;
}
