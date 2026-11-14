typedef struct PairFlagOwnerB {
    unsigned char active;
    char pad_001[0x17];
    unsigned char field_18[2];
} PairFlagOwnerB;

extern int func_089ED360(PairFlagOwnerB *self);

int func_089ED28C(PairFlagOwnerB *self, unsigned int index)
{
    if (!self->active) {
        return -1;
    }
    if (index >= 2) {
        return -2;
    }
    self->field_18[index] = 0;
    if (func_089ED360(self)) {
        return -3;
    }
    return 0;
}
