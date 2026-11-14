typedef struct PhaseClose28158 {
    char pad_000[0x8];
    int state;
    unsigned char field_0C;
    char pad_00D[0xF];
    char field_1C[0x28];
    char field_44[0x64];
    char field_A8[0x4];
} PhaseClose28158;

extern void func_08A01BC8(void *sub, short kind, int value);
extern void func_08A16B78(void *sub);
extern void func_089FC89C(void *sub);

int func_08A28158(PhaseClose28158 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_1C, 5, -1);
        func_08A16B78(self->field_44);
        func_089FC89C(self->field_A8);
        self->state = 2;
    }
    return 0;
}
