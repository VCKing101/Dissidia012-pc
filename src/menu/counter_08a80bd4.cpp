struct A6CountInner {
    char pad_00[0x620];
    int field_620;
};

struct A6CountOwner {
    char pad_00[0x28];
    A6CountInner *field_28;
};

extern "C" {
void func_08A80BD4(A6CountOwner *self) {
    self->field_28->field_620 = self->field_28->field_620 - 1;
    if (self->field_28->field_620 <= 0) {
        self->field_28->field_620 = 0;
    }
}

bool func_08A80C00(A6CountOwner *self) {
    if (self->field_28->field_620 > 0) {
        return true;
    } else {
        return false;
    }
}
}
