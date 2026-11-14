typedef struct A6Rec {
    char pad_00[0x1A];
    short field_1A;
    char pad_1C[2];
} A6Rec;

A6Rec *func_08A7038C(A6Rec *p, int key) {
    A6Rec *r = 0;
    int i;
    for (i = 0; i < 10; i++) {
        if (p->field_1A == key) {
            r = p;
            break;
        }
        p = (A6Rec *)((char *)p + 0x1C);
    }
    return r;
}
