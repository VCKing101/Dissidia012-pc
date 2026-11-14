typedef struct A6Table {
    char pad_00[0x20];
    unsigned short field_20;
    char pad_22[6];
    unsigned short field_28;
} A6Table;

typedef struct A6Holder {
    char pad_00[0xC];
    int field_0C;
    int field_10;
} A6Holder;

extern A6Table D_08A5863C;
void func_088D9F5C(int handle, int value, int one);

void func_08A558E0(A6Holder *self) {
    int v = D_08A5863C.field_20;
    func_088D9F5C(self->field_0C, v, 1);
}

void func_08A5590C(A6Holder *self) {
    int v = D_08A5863C.field_28;
    func_088D9F5C(self->field_10, v, 1);
}
