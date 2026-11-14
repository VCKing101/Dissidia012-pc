extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct Phase084C {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    char field_14[0x28];
    char field_3C[0x4];
} Phase084C;

int func_08A1084C(Phase084C *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_14, 2, -1);
        func_08A16B78(self->field_3C);
        self->field_08 = 2;
        self->field_10 = 0.0f;
    }
    return 0;
}
