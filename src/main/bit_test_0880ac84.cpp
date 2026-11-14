struct Agent4BitSet {
    int field_00;
    unsigned int bits[1];

    static unsigned char test(Agent4BitSet *set, int index) {
        unsigned int *bits = set->bits;
        return (bits[(unsigned int)index >> 5] & (1 << (index & 0x1F))) != 0;
    }
};

struct Agent4BitUser {
    unsigned char unknown_0000[0x26];
    short index;
};

extern "C" {
bool func_0880AC84(Agent4BitSet *set, Agent4BitUser *user) {
    return !Agent4BitSet::test(set, user->index);
}
}
