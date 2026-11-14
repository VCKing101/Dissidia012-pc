extern void func_08A01BC8(void *, int, int);
extern void func_08A16B78(void *);

typedef struct Phase5098 {
    char pad_000[0x8];
    int field_08;
    unsigned char field_0C;
    char pad_00D[0xF];
    char field_1C[0x28];
    char field_44[0x4];
} Phase5098;

int func_08A15098(Phase5098 *self)
{
    if (self->field_0C == 1) {
        func_08A01BC8(self->field_1C, 1, -1);
        func_08A16B78(self->field_44);
        self->field_08 = 2;
    }
    return 0;
}
