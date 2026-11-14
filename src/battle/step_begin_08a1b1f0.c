extern int func_08A020A4(void *);
extern void func_089F6208(void *, int, int, float);
extern void func_089F854C(void *);
extern float D_08A1B4AC;

typedef struct StepB1F0 {
    char pad_000[0x8];
    int field_08;
    char pad_00C[8];
    int field_14;
    int field_18;
    char pad_01C[0x4C];
    char field_68[0x50];
    char field_B8[0x38];
    char field_F0[4];
} StepB1F0;

int func_08A1B1F0(StepB1F0 *self)
{
    if (func_08A020A4(self->field_68) == 1) {
        func_089F6208(self->field_F0, self->field_14, self->field_18, D_08A1B4AC);
        func_089F854C(self->field_B8);
        self->field_08 = 5;
    }
    return 0;
}
