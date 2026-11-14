extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct PhaseCC88 {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x7];
    char field_14[0x28];
    char field_3C[0x4];
} PhaseCC88;

int func_08A0CC88(PhaseCC88 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_14, 1, -1);
        func_08A16B78(self->field_3C);
        self->field_08 = 2;
    }
    return 0;
}
