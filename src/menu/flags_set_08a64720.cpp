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

void func_08A64720(A6Holder2 *self) {
    for (int i = 0; i < 2; i++) {
        A6Obj *o = self->field_14[i];
        o->flags |= 0x4000;
        func_088D9D24(o);
    }
}
}
