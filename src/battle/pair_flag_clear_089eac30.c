typedef struct PairFlagOwner {
    unsigned char active;
    char pad_001[0x82B];
    unsigned char field_82C[2];
} PairFlagOwner;

extern int func_089EB5F8(PairFlagOwner *self);

int func_089EAC30(PairFlagOwner *self, unsigned int index)
{
    if (!self->active) {
        return -1;
    }
    if (index >= 2) {
        return -2;
    }
    self->field_82C[index] = 0;
    if (func_089EB5F8(self)) {
        return -3;
    }
    return 0;
}
