struct A6IdInner {
    char pad_00[0x2AC];
    int field_2AC;
};

struct A6IdSelf {
    char pad_00[8];
    A6IdInner *field_08;
};

extern "C" {
bool func_08A8EE74(A6IdSelf *self) {
    if (self->field_08->field_2AC == 0xD2) {
        return true;
    }
    return false;
}
}
