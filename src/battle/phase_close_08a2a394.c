typedef struct PhaseClose2A394 {
    char pad_000[0x8];
    int state;
    unsigned char field_0C;
    char pad_00D[0x7];
    char field_14[0x28];
    char field_3C[0x4];
} PhaseClose2A394;

extern void func_08A01BC8(void *sub, short kind, int value);
extern void func_08A16B78(void *sub);

int func_08A2A394(PhaseClose2A394 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_14, 1, -1);
        func_08A16B78(self->field_3C);
        self->state = 2;
    }
    return 0;
}
