struct A6Panel3 {
    char pad_00[0x44];
    int field_44;
    int field_48;
};

extern "C" {
void func_08A64A58(A6Panel3 *self);
int func_0887B3EC(int a, int b, int c, int d, int e, int f);
int func_0887E534(int handle);
void func_088D428C(char *a, char *b, void (*cb)(void), int n);
void func_08A68200(void);

void func_08A649E4(A6Panel3 *self, int arg) {
    func_08A64A58(self);
    self->field_44 = func_0887B3EC(arg, 8, 0, 0x3F, 0, 0);
    self->field_48 = func_0887E534(self->field_44);
    func_088D428C((char *)self + 0x14, (char *)self + 0x44, func_08A68200, 1);
}
}
