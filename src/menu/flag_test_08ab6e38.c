typedef struct FlagObj {
    char pad_00[0x30];
    unsigned int field_30;
} FlagObj;

unsigned char func_08AB6E38(FlagObj *self)
{
    if ((self->field_30 & 0x4) != 0) {
        return 1;
    }
    return 0;
}
