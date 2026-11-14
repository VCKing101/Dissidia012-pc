typedef struct SlotReset {
    unsigned char active;
    char pad_01[0x3];
    int field_04;
    int field_08;
    int values[2];
    int field_14;
    int field_18;
    int field_1C;
} SlotReset;

extern "C" {

void func_08B052F0(SlotReset *self)
{
    self->active = 0;
    self->field_04 = -1;
    self->field_08 = 0;
    for (int i = 0; i < 2; i++) {
        self->values[i] = 0;
    }
    self->field_14 = -1;
    self->field_18 = 0;
    self->field_1C = 0;
}

}
