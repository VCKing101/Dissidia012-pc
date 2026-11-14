extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct Phase25BC {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0xF];
    char field_1C[0x28];
    char field_44[0x4];
} Phase25BC;

int func_08A125BC(Phase25BC *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_1C, 5, -1);
        func_08A16B78(self->field_44);
        self->field_08 = 2;
    }
    return 0;
}
