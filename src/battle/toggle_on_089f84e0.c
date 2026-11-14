extern void func_089F7E18(void *);
extern void func_08889F28(int, int);

typedef struct Toggle84E0 {
    unsigned char field_00;
    char pad_001[3];
    int field_04;
    int field_08;
    unsigned char field_0C;
    char pad_00D[7];
    char field_14[4];
} Toggle84E0;

int func_089F84E0(Toggle84E0 *self)
{
    if (!self->field_00) {
        return -1;
    }
    if (self->field_0C == 1) {
        return -2;
    }
    func_089F7E18(self->field_14);
    func_08889F28(self->field_08, self->field_04 + 1);
    self->field_0C = 1;
    return 0;
}
