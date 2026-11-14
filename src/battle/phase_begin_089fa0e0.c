extern void func_08891D84(int, int, int, int, int, int);
extern void func_08A00ADC(void *);
extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct PhaseA0E0 {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x7];
    char field_14[0x28];
    char field_3C[0x54];
    char field_90[0x78];
    int field_108;
} PhaseA0E0;

int func_089FA0E0(PhaseA0E0 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_14, 1, -1);
        func_08A16B78(self->field_3C);
        func_08A00ADC(self->field_90);
        func_08891D84(self->field_108, 0, 0, 0, 0, 0);
        self->field_08 = 2;
    }
    return 0;
}
