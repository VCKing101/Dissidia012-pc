typedef struct TestCtx {
    char pad_0000[0x7CC0];
    unsigned int field_7CC0;
} TestCtx;

typedef struct TestOwner {
    char pad_0000[0x7CFC];
    TestCtx *field_7CFC;
} TestOwner;

unsigned char func_08AAB88C(TestOwner *self)
{
    if ((self->field_7CFC->field_7CC0 & 0x100) != 0) {
        return 1;
    }
    return 0;
}
