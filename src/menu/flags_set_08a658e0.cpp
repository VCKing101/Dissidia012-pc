struct A6ObjB {
    char pad_00[0x20];
    unsigned short flags;
};

struct A6Holder4 {
    char pad_00[0x1C];
    A6ObjB *field_1C[5];
};

extern "C" {
void func_088D9D24(A6ObjB *o);

void func_08A658E0(A6Holder4 *self) {
    for (int i = 0; i < 5; i++) {
        A6ObjB *o = self->field_1C[i];
        o->flags |= 0x4000;
        func_088D9D24(o);
    }
}
}
