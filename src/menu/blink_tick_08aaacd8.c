typedef struct BlinkState {
    char pad_0000[0x7CC0];
    unsigned int field_7CC0;
    char pad_7CC4[0x7CD0 - 0x7CC4];
    int field_7CD0;
} BlinkState;

typedef struct BlinkOwner {
    char pad_0000[0x7CFC];
    BlinkState *field_7CFC;
} BlinkOwner;

void func_08AAACD8(BlinkOwner *self)
{
    self->field_7CFC->field_7CD0--;
    if (self->field_7CFC->field_7CD0 <= 0) {
        self->field_7CFC->field_7CD0 = 0;
    }
    self->field_7CFC->field_7CC0 = self->field_7CFC->field_7CC0 & ~0x800;
    if (self->field_7CFC->field_7CD0 == 0) {
        self->field_7CFC->field_7CC0 = self->field_7CFC->field_7CC0 | 0x800;
    }
}
