extern void func_089FC89C(void *);
extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct PhaseD1D4 {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x7];
    char field_14[0x28];
    char field_3C[0x64];
    char field_A0[0x4];
} PhaseD1D4;

int func_089FD1D4(PhaseD1D4 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_14, 5, -1);
        func_08A16B78(self->field_3C);
        func_089FC89C(self->field_A0);
        self->field_08 = 2;
    }
    return 0;
}
