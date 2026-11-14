struct A6BitsInner {
    char pad_00[0x5F0];
    unsigned field_5F0;
};

struct A6BitsOwner {
    char pad_00[0x28];
    A6BitsInner *field_28;
};

extern "C" {
bool func_08A7FA4C(A6BitsOwner *self, unsigned bit) {
    if ((self->field_28->field_5F0 & (1 << bit)) != 0) {
        return true;
    }
    return false;
}
}
