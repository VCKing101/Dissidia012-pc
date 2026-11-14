struct A6Callback {
    void (*fn)(void);
    void *self;
};

struct A6Screen4 {
    char pad_000[0x100];
    char field_100[0x34];
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    char field_148[0x3C];
    char field_184[0x2C];
    char field_1B0[0x88];
    int field_238;
    void (*field_23C)(void);
    A6Screen4 *field_240;
    void (*field_244)(void);
    A6Screen4 *field_248;
    char pad_24C[4];
    void (*field_250)(void);
    A6Screen4 *field_254;
    char pad_258[0x1C];
    int field_274;
    char field_278;
};

extern "C" {
void func_08864FF0(char *p, int a, int b, int c);
void func_08A74588(char *p);
void func_08A723D8(char *p);
void func_08A79C30(char *p);
void func_08A6D650(void);
void func_08A6D660(void);
void func_08A6D6A8(void);

A6Screen4 *func_08A6CBFC(A6Screen4 *self) {
    func_08864FF0(self->field_100, 1, 0x11188, 0x4D);
    self->field_134 = 0;
    self->field_138 = 0;
    self->field_13C = 0;
    self->field_140 = 0;
    self->field_144 = 0;
    func_08A74588(self->field_148);
    func_08A723D8(self->field_184);
    func_08A79C30(self->field_1B0);
    self->field_238 = 0;
    self->field_23C = func_08A6D650;
    self->field_240 = self;
    self->field_244 = func_08A6D660;
    self->field_248 = self;
    self->field_250 = func_08A6D6A8;
    int m1 = -1;
    self->field_254 = self;
    self->field_274 = m1;
    self->field_278 = 0;
    return self;
}
}
