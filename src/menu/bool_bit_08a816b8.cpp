struct A6BitInner {
    char pad_00[0x5E8];
    unsigned field_5E8;
};

struct A6BitSelf {
    char pad_00[0x28];
    A6BitInner *field_28;
};

extern "C" {
bool func_08A816B8(A6BitSelf *self) {
    if ((self->field_28->field_5E8 & 0x100) != 0) {
        return true;
    }
    return false;
}
}
