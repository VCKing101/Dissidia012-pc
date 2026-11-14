extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct Phase3F14 {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x17];
    char field_24[0x28];
    char field_4C[0x4];
} Phase3F14;

int func_08A13F14(Phase3F14 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_24, 3, -1);
        func_08A16B78(self->field_4C);
        self->field_08 = 2;
    }
    return 0;
}
