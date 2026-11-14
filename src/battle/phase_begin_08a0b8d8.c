extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct PhaseB8D8 {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x3];
    float field_10;
    char pad_014[0x2C];
    char field_40[0x28];
    char field_68[0x4];
} PhaseB8D8;

int func_08A0B8D8(PhaseB8D8 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_40, 5, -1);
        func_08A16B78(self->field_68);
        self->field_08 = 3;
        self->field_10 = 0.0f;
    }
    return 0;
}
