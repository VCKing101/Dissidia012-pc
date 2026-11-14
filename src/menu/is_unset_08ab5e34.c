typedef struct IdObj {
    char pad_00[0x24];
    int field_24;
} IdObj;

unsigned char func_08AB5E34(IdObj *self)
{
    if (self->field_24 == -1) {
        return 1;
    }
    return 0;
}
