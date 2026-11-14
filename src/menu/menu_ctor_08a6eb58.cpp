struct A6Menu {
    int field_00;
    int field_04;
    char field_08[0x1C0];
    void (*field_1C8)(void);
    A6Menu *field_1CC;
    int field_1D0;
    char field_1D4[0x54];
    char field_228[0x970];
    char field_B98[0x10];
};

extern "C" {
void func_0890A470(char *p);
void func_08A6F8B0(void);
void func_08A6E168(char *p);
void func_08993560(char *p, int n, int size, void (*cb)(void));
void func_08A6EAF8(void);
void func_08A6EB20(void);

A6Menu *func_08A6EB58(A6Menu *self, int arg) {
    self->field_00 = 0;
    self->field_04 = arg;
    func_0890A470(self->field_08);
    self->field_1C8 = func_08A6F8B0;
    int m1 = -1;
    self->field_1CC = self;
    self->field_1D0 = m1;
    func_08A6E168(self->field_1D4);
    func_08993560(self->field_228, 10, 0xF0, func_08A6EAF8);
    func_08993560(self->field_B98, 0x10, 0x970, func_08A6EB20);
    return self;
}
}
