extern void func_08A00ADC(void *);
extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct Phase14B0 {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x7];
    char field_14[0x28];
    char field_3C[0x54];
    char field_90[0x4];
} Phase14B0;

int func_08A014B0(Phase14B0 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_14, 1, -1);
        func_08A16B78(self->field_3C);
        func_08A00ADC(self->field_90);
        self->field_08 = 2;
    }
    return 0;
}
