struct A6Pair1 {
    unsigned short key;
    unsigned short pad_02[2];
    unsigned short value;
};

struct A6PairOwner1 {
    char pad_00[0x3C];
    A6Pair1 field_3C[1];
};

extern "C" {
void func_08A54E60(A6PairOwner1 *self, unsigned short key, unsigned short value) {
    for (int i = 0; i < 1; i++) {
        if (self->field_3C[i].key == key) {
            self->field_3C[i].value = value;
        }
    }
}
}
