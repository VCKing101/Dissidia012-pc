extern void func_08AEAD7C(void *);

typedef struct Gate64F8 {
    unsigned char field_00;
    char pad_001[3];
    char field_04[4];
} Gate64F8;

int func_089F64F8(Gate64F8 *self)
{
    if (!self->field_00) {
        return -1;
    }
    func_08AEAD7C(self->field_04);
    return 0;
}
