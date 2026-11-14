extern int func_089EF7E4(void *);

typedef struct Wait09FC {
    char pad_000[0x8];
    int field_08;
    char pad_00C[4];
    float field_10;
    char pad_014[8];
    char field_1C[4];
} Wait09FC;

int func_089F09FC(Wait09FC *self)
{
    if (func_089EF7E4(self->field_1C) == 1) {
        self->field_08 = 7;
        self->field_10 = 0.0f;
    }
    return 0;
}
