extern void func_088D9F5C(void *, int, unsigned char);

typedef struct Gate1C74 {
    unsigned char field_00;
    char pad_001[3];
    void *field_04;
    void *field_08;
    unsigned char field_0C;
} Gate1C74;

int func_08A01C74(Gate1C74 *self, int mode)
{
    if (!self->field_00) {
        return -1;
    }
    if (!self->field_04 || !self->field_08) {
        return -2;
    }
    func_088D9F5C(self->field_08, mode, 1);
    self->field_0C = 1;
    return 0;
}
