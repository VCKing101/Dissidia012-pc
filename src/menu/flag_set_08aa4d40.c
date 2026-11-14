typedef struct SetCtx {
    unsigned int field_00;
    char pad_04[0x1E90 - 4];
    int field_1E90;
    char pad_1E94[0x1E94 - 0x1E94];
} SetCtx;

typedef struct SetOwner {
    char pad_0000[0x1EE4];
    SetCtx *field_1EE4;
} SetOwner;

void func_08AA4D40(SetOwner *self, int value)
{
    unsigned int *p;
    self->field_1EE4->field_1E90 = value;
    *(unsigned int *)self->field_1EE4 = *(unsigned int *)self->field_1EE4 & ~1;
    p = (unsigned int *)self->field_1EE4;
    if ((int)p[0x7A4] > 0) {
        *p = *p | 1;
    }
}
