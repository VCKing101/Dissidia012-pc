extern void func_088D9F5C(void *, int, int);

typedef struct Gate11C4 {
    unsigned char field_00;
    char pad_001[7];
    void *field_08;
} Gate11C4;

int func_089F11C4(Gate11C4 *self)
{
    if (!self->field_00) {
        return -1;
    }
    if (!self->field_08) {
        return -2;
    }
    func_088D9F5C(self->field_08, 2, 1);
    return 0;
}
