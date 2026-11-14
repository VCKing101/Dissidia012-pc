typedef struct PairByteOwner {
    unsigned char active;
    char pad_001[0x643];
    unsigned char field_644[2];
} PairByteOwner;

int func_089E7EF0(PairByteOwner *self, unsigned int index, unsigned char value)
{
    if (!self->active) {
        return -1;
    }
    if (index >= 2) {
        return -2;
    }
    self->field_644[index] = value;
    return 0;
}
