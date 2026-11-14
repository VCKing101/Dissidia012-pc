struct A6Flag {
    char pad_00[0x49];
    unsigned char field_49;
};

extern "C" {
bool func_08A5C764(A6Flag *self) {
    if (self->field_49 == 0) {
        return false;
    }
    return true;
}
}
