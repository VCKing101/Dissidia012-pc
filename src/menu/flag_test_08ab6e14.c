typedef struct FlagObj {
    char pad_00[0x30];
    unsigned int field_30;
} FlagObj;

unsigned char func_08AB6E14(FlagObj *self)
{
    if ((self->field_30 & 0x1A) != 0) {
        return 1;
    }
    return 0;
}
