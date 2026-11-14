extern void func_089F62E8(void *);
extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct Phase04BC {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0x13];
    char field_20[0x28];
    char field_48[0x68];
    char field_B0[0x4];
} Phase04BC;

int func_08A004BC(Phase04BC *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_20, 2, -1);
        func_08A16B78(self->field_48);
        func_089F62E8(self->field_B0);
        self->field_08 = 3;
    }
    return 0;
}
