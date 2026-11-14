struct A6FlagObj {
    char pad_00[0x20];
    unsigned short flags;
};

struct A6FlagHolder {
    char pad_00[0xC];
    A6FlagObj *field_0C;
};

extern "C" {
bool func_08A7304C(A6FlagHolder *self) {
    A6FlagObj **pp = &self->field_0C;
    bool r = false;
    if (*pp != 0) {
        r = ((*pp)->flags & 0x20) != 0;
    }
    return r;
}
}
