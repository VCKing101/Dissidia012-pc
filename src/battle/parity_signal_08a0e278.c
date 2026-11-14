typedef struct ParitySignal {
    char pad_000[0xC];
    int field_0C;
    char pad_010[0x84];
    char field_94[0x14];
    int field_A8;
    int field_AC;
} ParitySignal;

extern int func_08A0D888(void *sub);
extern void func_08889F70(int parity, int value);

int func_08A0E278(ParitySignal *self)
{
    if (func_08A0D888(self->field_94) == 1) {
        func_08889F70((self->field_A8 + self->field_AC) % 2, self->field_A8 + 5);
        self->field_0C = -1;
    }
    return 0;
}
