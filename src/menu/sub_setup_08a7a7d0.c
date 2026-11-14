typedef struct A6Sub {
    char pad_00[4];
    char field_04[0x10];
    int field_14;
} A6Sub;

void func_08A742A0(char *p, char *q, int a, int b);
void func_08A744BC(char *p, char *q, int a, int b, int c, int d);
void func_08A74390(char *p);

void func_08A7A7D0(A6Sub *self, char *other) {
    char *p = self->field_04;
    char *q = other + 0x148;
    char *r = q + 0x34;
    func_08A742A0(p, q, 0, 0);
    func_08A744BC(p, r, 0, 0xDA, 0, 0);
    func_08A74390(p);
    self->field_14 = 0;
}
