struct FlagScanD360 {
    char pad_000[0x18];
    unsigned char field_18[2];
    unsigned char field_1A;
};

extern "C" {
int func_089ED360(FlagScanD360 *self)
{
    unsigned char found = 0;
    for (int i = 0; i < 2; i++) {
        if (*((unsigned char *)self + 0x18 + i) == 1) {
            found = 1;
            break;
        }
    }
    self->field_1A = found;
    return 0;
}
}
