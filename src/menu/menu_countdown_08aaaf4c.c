typedef struct CountState {
    char pad_0000[0x7CC0];
    unsigned int field_7CC0;
    char pad_7CC4[0x7CD8 - 0x7CC4];
    int field_7CD8;
} CountState;

typedef struct CountOwner {
    char pad_0000[0x7CFC];
    CountState *field_7CFC;
} CountOwner;

extern void func_08AAAFC0(CountOwner *self);

void func_08AAAF4C(CountOwner *self)
{
    if ((self->field_7CFC->field_7CC0 & 2) == 0) {
        return;
    }
    self->field_7CFC->field_7CD8--;
    if (self->field_7CFC->field_7CD8 <= 0) {
        self->field_7CFC->field_7CD8 = 0;
    }
    if (self->field_7CFC->field_7CD8 == 1) {
        func_08AAAFC0(self);
    }
}
