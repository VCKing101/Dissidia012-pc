struct A6Copy {
    char pad_00[4];
    int field_04;
    char pad_08[4];
    char field_0C[8];
};

extern "C" {
void func_089359A4(char *dst, char *src);
void func_0890AB4C(char *dst, char *src, int n);
extern char D_08A6DB00[];

void func_08A6DA08(A6Copy *self, char *other) {
    char *src = self->field_0C;
    char *dst = other + 8;
    func_089359A4(src, D_08A6DB00);
    func_0890AB4C(dst, src, 8);
    self->field_04 = 2;
}
}
