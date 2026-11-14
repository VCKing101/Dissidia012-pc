typedef struct TestCtx {
    char pad_000[0x1E70];
    unsigned int field_1E70;
} TestCtx;

typedef struct TestOwner {
    char pad_000[0x1EE4];
    TestCtx *field_1EE4;
} TestOwner;

unsigned char func_08AA5684(TestOwner *self)
{
    if ((self->field_1EE4->field_1E70 & 0x10) != 0) {
        return 1;
    }
    return 0;
}
