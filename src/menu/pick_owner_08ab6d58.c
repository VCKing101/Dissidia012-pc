typedef struct PickObj {
    char pad_00[0x14];
    int field_14;
    char pad_18[0x2C - 0x18];
    unsigned int field_2C;
} PickObj;

extern int *D_08A6D7E0;

int func_08AB6D58(PickObj *self)
{
    if ((self->field_2C & 4) == 0) {
        return *D_08A6D7E0;
    } else {
        return self->field_14;
    }
}
