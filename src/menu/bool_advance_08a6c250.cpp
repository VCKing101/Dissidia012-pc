struct A6Advance {
    char pad_00[0x2C];
    int field_2C;
};

extern "C" {
int func_08A6C290(A6Advance *self);

bool func_08A6C250(A6Advance *self) {
    int r = func_08A6C290(self);
    if (r != 0) {
        self->field_2C = 4;
        return true;
    }
    return false;
}
}
