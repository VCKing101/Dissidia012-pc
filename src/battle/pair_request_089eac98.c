typedef struct PairOwner {
    unsigned char active;
    char pad_01[0x82B];
    unsigned char field_82C[2];
} PairOwner;

extern int func_089EB5F8(PairOwner *self);

int func_089EAC98(PairOwner *self, unsigned int index)
{
    if (!self->active) {
        return -1;
    }
    if (index >= 2) {
        return -2;
    }
    self->field_82C[index] = 1;
    if (func_089EB5F8(self) != 0) {
        return -3;
    }
    return 0;
}
