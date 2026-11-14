struct A6Panel4 {
    char pad_00[0x60];
    int field_60;
    int field_64;
};

extern "C" {
void func_08A653CC(A6Panel4 *self);
int func_0887B3EC(int a, int b, int c, int d, int e, int f);
void func_088D428C(char *a, char *b, void (*cb)(void), int n);
void func_08A68218(void);

void func_08A65344(A6Panel4 *self, int arg) {
    func_08A653CC(self);
    self->field_60 = func_0887B3EC(arg, 8, 0, 0x40, 0, 0);
    self->field_64 = func_0887B3EC(arg, 8, 0, 0x41, 0, 0);
    func_088D428C((char *)self + 0x18, (char *)self + 0x60, func_08A68218, 1);
}
}
