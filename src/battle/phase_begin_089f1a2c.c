extern void func_089F1E90(void *);

typedef struct Phase1A2C {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    char field_14[0x4];
} Phase1A2C;

int func_089F1A2C(Phase1A2C *self)
{
    if (self->field_0C == 1) {
        func_089F1E90(self->field_14);
        self->field_08 = 2;
        self->field_10 = 0.0f;
    }
    return 0;
}
