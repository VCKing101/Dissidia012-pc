struct A6State {
    char pad_00[0xC];
    int field_0C;
};

extern "C" {
bool func_08A58B7C(A6State *self) {
    if (self->field_0C == 1) {
        return true;
    }
    return false;
}
}
