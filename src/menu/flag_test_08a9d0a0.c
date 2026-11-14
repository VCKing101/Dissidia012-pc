typedef struct TestCtx {
    char pad_000[0x81B4];
    unsigned int field_81B4;
} TestCtx;

typedef struct TestOwner {
    char pad_000[0x154];
    TestCtx *field_154;
} TestOwner;

int func_08A9D0A0(TestOwner *self)
{
    if (self->field_154 == 0) {
        return 0;
    } else if ((self->field_154->field_81B4 & 0x40) != 0) {
        return 1;
    } else {
        return 0;
    }
}
