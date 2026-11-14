extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct PhaseE068 {
    char pad_000[0x8];
    int field_08;
    char pad_00C[0x4];
    unsigned char field_10;
    char pad_011[0x7];
    char field_18[0x28];
    char field_40[0x4];
} PhaseE068;

int func_08A0E068(PhaseE068 *self)
{
    if (self->field_10 == 1) {
        func_08A01BC8(self->field_18, 4, -1);
        func_08A16B78(self->field_40);
        self->field_08 = 2;
    }
    return 0;
}
