struct A6FlInner {
    char pad_000[0x2A8];
    unsigned field_2A8;
    char pad_2AC[0x4C];
    int field_2F8;
};

struct A6FlSelf {
    char pad_00[8];
    A6FlInner *field_08;
};

extern "C" {
void func_08A8EF90(A6FlSelf *self, int v) {
    self->field_08->field_2F8 = v;
    self->field_08->field_2A8 = self->field_08->field_2A8 & 0xFFFDFFFF;
    A6FlInner *in = self->field_08;
    if (in->field_2F8 > 0) {
        in->field_2A8 = in->field_2A8 | 0x20000;
    }
}

bool func_08A8EFD8(A6FlSelf *self) {
    if ((self->field_08->field_2A8 & 0x20000) != 0) {
        return true;
    }
    return false;
}

bool func_08A8F004(A6FlSelf *self) {
    if ((self->field_08->field_2A8 & 0x40000) != 0) {
        return true;
    }
    return false;
}
}
