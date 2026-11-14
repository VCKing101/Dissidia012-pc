typedef struct TestCtx {
    char pad_000[0x91C];
    unsigned int field_91C;
} TestCtx;

typedef struct TestOwner {
    char pad_000[0x154];
    TestCtx *field_154;
} TestOwner;

unsigned char func_08A9C814(TestOwner *self)
{
    if ((self->field_154->field_91C & 0x40) != 0) {
        return 1;
    }
    return 0;
}
