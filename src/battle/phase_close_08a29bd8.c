typedef struct PhaseClose29BD8 {
    char pad_000[0x8];
    int state;
    unsigned char field_0C;
    char pad_00D[0xF];
    char field_1C[0x28];
    char field_44[0x68];
    char field_AC[0x4];
} PhaseClose29BD8;

extern void func_08A01BC8(void *sub, short kind, int value);
extern void func_08A16B78(void *sub);
extern void func_089F62E8(void *sub);

int func_08A29BD8(PhaseClose29BD8 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_1C, 2, -1);
        func_08A16B78(self->field_44);
        func_089F62E8(self->field_AC);
        self->state = 3;
    }
    return 0;
}
