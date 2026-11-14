struct A6Menu2 {
    char pad_000[0x1FC];
    int field_1FC;
    void (*field_200)(void);
    A6Menu2 *field_204;
    int field_208;
    char pad_20C[4];
    int field_210;
    int field_214;
    int field_218;
    char field_21C;
};

extern "C" {
void func_08904C08(A6Menu2 *self, void (**slot)(void));
void func_08A6FFA8(void);

A6Menu2 *func_08A6F940(A6Menu2 *self) {
    func_08904C08(self, &self->field_200);
    self->field_1FC = 0;
    self->field_200 = func_08A6FFA8;
    int m1 = -1;
    self->field_204 = self;
    self->field_208 = 0;
    self->field_210 = m1;
    self->field_214 = 0;
    self->field_218 = 0;
    self->field_21C = 0;
    return self;
}
}
