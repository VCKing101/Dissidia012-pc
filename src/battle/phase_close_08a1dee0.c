typedef struct PhaseCloseDEE0 {
    char pad_000[0x8];
    int state;
    unsigned char field_0C;
    char pad_00D[0x77];
    char field_84[0x48];
    char field_CC[0x28];
    char field_F4[0x64];
    char field_158[0x4];
} PhaseCloseDEE0;

extern void func_08A01BC8(void *sub, int kind, int value);
extern void func_08A16B78(void *sub);
extern void func_089FC2A0(void *sub);
extern void func_089F62E8(void *sub);

int func_08A1DEE0(PhaseCloseDEE0 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_CC, 7, -1);
        func_08A16B78(self->field_F4);
        func_089FC2A0(self->field_158);
        func_089F62E8(self->field_84);
        self->state = 4;
    }
    return 0;
}
