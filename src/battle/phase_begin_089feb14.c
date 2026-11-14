extern void func_089FC2A0(void *);
extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct PhaseEB14 {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x7];
    char field_14[0x28];
    char field_3C[0x64];
    char field_A0[0x4];
} PhaseEB14;

int func_089FEB14(PhaseEB14 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_14, 2, -1);
        func_08A16B78(self->field_3C);
        func_089FC2A0(self->field_A0);
        self->field_08 = 2;
    }
    return 0;
}
