typedef struct BlinkState {
    char pad_0000[0x1E70];
    unsigned int field_1E70;
    char pad_1E74[0x1E94 - 0x1E74];
    int field_1E94;
} BlinkState;

typedef struct BlinkOwner {
    char pad_0000[0x1EE4];
    BlinkState *field_1EE4;
} BlinkOwner;

void func_08AA4E40(BlinkOwner *self)
{
    self->field_1EE4->field_1E94--;
    if (self->field_1EE4->field_1E94 <= 0) {
        self->field_1EE4->field_1E94 = 0;
    }
    self->field_1EE4->field_1E70 = self->field_1EE4->field_1E70 & ~0x200;
    if (self->field_1EE4->field_1E94 == 0) {
        self->field_1EE4->field_1E70 = self->field_1EE4->field_1E70 | 0x200;
    }
}
