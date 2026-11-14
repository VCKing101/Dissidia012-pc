struct A6Pair {
    unsigned short key;
    unsigned short pad_02[2];
    unsigned short value;
};

struct A6PairOwner {
    char pad_00[0x58];
    A6Pair field_58[2];
};

extern "C" {
void func_08A52C5C(A6PairOwner *self, unsigned short key, unsigned short value) {
    for (int i = 0; i < 2; i++) {
        if (self->field_58[i].key == key) {
            self->field_58[i].value = value;
        }
    }
}
}
