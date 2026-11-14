extern void func_088DAB7C(int);

typedef struct Gate1D60 {
    unsigned char field_00;
    char pad_001[7];
    int field_08;
    char pad_00C[0x10];
    int field_1C;
} Gate1D60;

int func_08A01D60(Gate1D60 *self)
{
    if (!self->field_00) {
        return -1;
    }
    if (self->field_08) {
        func_088DAB7C(self->field_1C);
    }
    return 0;
}
