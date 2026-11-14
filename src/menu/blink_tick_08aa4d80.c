typedef struct BlinkState {
    unsigned int field_00;
    char pad_04[0x1E8C];
    int field_1E90;
} BlinkState;

typedef struct BlinkOwner {
    char pad_0000[0x1EE4];
    BlinkState *field_1EE4;
} BlinkOwner;

void func_08AA4D80(BlinkOwner *self)
{
    self->field_1EE4->field_1E90--;
    if (self->field_1EE4->field_1E90 <= 0) {
        self->field_1EE4->field_1E90 = 0;
    }
    self->field_1EE4->field_00 = self->field_1EE4->field_00 & ~1;
    if (self->field_1EE4->field_1E90 > 0) {
        self->field_1EE4->field_00 = self->field_1EE4->field_00 | 1;
    }
}
