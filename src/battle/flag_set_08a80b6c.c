typedef struct FlagSet80B6C {
    unsigned char active;
    char pad_001[0xF];
    unsigned char field_10;
    unsigned char field_11;
} FlagSet80B6C;

int func_08A80B6C(FlagSet80B6C *self, unsigned char value)
{
    if (!self->active) {
        return -1;
    }
    self->field_10 = value;
    return 0;
}

int func_08A80B90(FlagSet80B6C *self, unsigned char value)
{
    if (!self->active) {
        return -1;
    }
    self->field_11 = value;
    return 0;
}
