typedef struct PairOwner {
    unsigned char active;
    char pad_01[0x17];
    unsigned char field_18[2];
} PairOwner;

extern int func_089ED360(PairOwner *self);

int func_089ED2F4(PairOwner *self, unsigned int index)
{
    if (!self->active) {
        return -1;
    }
    if (index >= 2) {
        return -2;
    }
    self->field_18[index] = 1;
    if (func_089ED360(self) != 0) {
        return -3;
    }
    return 0;
}
