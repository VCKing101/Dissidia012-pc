typedef struct RangeClear {
    unsigned char active;
    char pad_01[0x17];
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
} RangeClear;

int func_08AC05E8(RangeClear *self)
{
    if (!self->active) {
        return -1;
    }
    self->field_1C = 0;
    self->field_18 = 0;
    self->field_20 = 0;
    self->field_28 = 0;
    self->field_24 = 0;
    self->field_2C = 0;
    return 0;
}
