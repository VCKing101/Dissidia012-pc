struct A6Obj {
    char pad_00[0x20];
    unsigned short flags;
};

struct A6Holder2 {
    char pad_00[0x14];
    A6Obj *field_14[2];
    A6Obj *field_1C;
};

struct A6Holder3 {
    char pad_00[0x18];
    A6Obj *field_18;
};

extern "C" {
void func_088D9D24(A6Obj *o);

void func_08A64ECC(A6Holder3 *self) {
    A6Obj *o = self->field_18;
    o->flags &= ~0x4000;
    func_088D9D24(o);
}

void func_08A64EF8(A6Holder3 *self) {
    A6Obj *o = self->field_18;
    o->flags |= 0x4000;
    func_088D9D24(o);
}
}
