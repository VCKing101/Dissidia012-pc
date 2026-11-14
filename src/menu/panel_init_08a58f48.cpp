struct A6Panel2 {
    char pad_00[0x50];
    int field_50;
    int field_54;
};

extern "C" {
void func_08A58FD4(A6Panel2 *self);
int func_0887B3EC(int a, int b, int c, int d, int e, int f);
int func_0887E534(int handle);
void func_088D428C(char *a, char *b, void (*cb)(void), int n);
void func_08A5B660(void);

void func_08A58F48(A6Panel2 *self, int a, int b, int c) {
    func_08A58FD4(self);
    self->field_50 = func_0887B3EC(a, b, 0, c, 0, 0);
    self->field_54 = func_0887E534(self->field_50);
    func_088D428C((char *)self + 0x14, (char *)self + 0x50, func_08A5B660, 1);
}
}
