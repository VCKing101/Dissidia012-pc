struct A6Panel {
    char pad_00[0x70];
    int field_70;
};

extern "C" {
void func_08A5318C(A6Panel *self);
int func_0887B3EC(void *a, int b, int c, int d, int e, int f);
void func_088D428C(char *a, char *b, void (*cb)(void), int n);
void func_08A544B8(void);

void func_08A53124(A6Panel *self, void *arg) {
    func_08A5318C(self);
    self->field_70 = func_0887B3EC(arg, 8, 0, 0x19, 0, 0);
    func_088D428C((char *)self + 4, (char *)self + 0x64, func_08A544B8, 1);
}
}
