extern void func_08A020C8(void *);

typedef struct Phase2944 {
    char pad_000[0x8];
    int field_08;
    char pad_00C[0x1];
    unsigned char field_0D;
    char pad_00E[0x2];
    float field_10;
    char pad_014[0x60];
    char field_74[0x4];
} Phase2944;

int func_08A12944(Phase2944 *self)
{
    if (self->field_0D == 1) {
        func_08A020C8(self->field_74);
        self->field_08 = 10;
        self->field_10 = 0.0f;
    }
    return 0;
}
