typedef struct A6ShowInner {
    char pad_000[0x1B0];
    int field_1B0[8];
} A6ShowInner;

typedef struct A6ShowSelf {
    char pad_00[8];
    A6ShowInner *field_08;
} A6ShowSelf;

extern short D_08A93B1E[];
void func_088D9F5C(int h, int v, int one);
void func_088D9CFC(int h);

void func_08A8DFA4(A6ShowSelf *self, int idx, int kind, unsigned char on) {
    func_088D9F5C(self->field_08->field_1B0[idx], D_08A93B1E[kind], 1);
    if (on == 0) {
        func_088D9CFC(self->field_08->field_1B0[idx]);
    }
}
