extern void func_089F3CB8(void *);

typedef struct Phase4920 {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0xC];
    char field_20[0x4];
} Phase4920;

int func_089F4920(Phase4920 *self)
{
    if (self->field_0C == 1) {
        func_089F3CB8(self->field_20);
        self->field_08 = 2;
        self->field_10 = 0.0f;
    }
    return 0;
}
