extern int func_089EF7E4(void *);

typedef struct Wait0B38 {
    char pad_000[0x8];
    int field_08;
    char pad_00C[4];
    float field_10;
    char pad_014[8];
    char field_1C[4];
} Wait0B38;

int func_089F0B38(Wait0B38 *self)
{
    if (func_089EF7E4(self->field_1C) == 1) {
        self->field_08 = 10;
        self->field_10 = 0.0f;
    }
    return 0;
}
