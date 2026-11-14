struct A6Res {
    char pad_00[0x400];
    int field_400;
    int field_404;
};

struct A6ResOwner {
    A6Res *field_00;
};

extern "C" {
extern unsigned short D_08A88A44[];
void func_088DA0A8(int a, int b, char *c, int *d);
void func_088D9B84(int a, char *b);
void func_088D9AF4(int a, int *b);

void func_08A88340(A6ResOwner *self, int idx) {
    char a[0x40];
    int b;
    func_088DA0A8(self->field_00->field_400, D_08A88A44[idx], a, &b);
    func_088D9B84(self->field_00->field_404, a);
    func_088D9AF4(self->field_00->field_404, &b);
}
}
