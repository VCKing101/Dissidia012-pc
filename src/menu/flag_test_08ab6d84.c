typedef struct FlagObj {
    char pad_00[0x2C];
    unsigned int field_2C;
} FlagObj;

unsigned char func_08AB6D84(FlagObj *self)
{
    if ((self->field_2C & 0x10000000) != 0) {
        return 1;
    }
    return 0;
}
