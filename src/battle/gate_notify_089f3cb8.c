extern void func_088D9F5C(void *, int, int);
extern void func_089F39E4(void *);

typedef struct Gate3CB8 {
    unsigned char field_00;
    char pad_001[0xB];
    void *field_0C;
} Gate3CB8;

int func_089F3CB8(Gate3CB8 *self)
{
    if (!self->field_00) {
        return -1;
    }
    if (!self->field_0C) {
        return -2;
    }
    func_088D9F5C(self->field_0C, 9, 1);
    func_089F39E4(self);
    return 0;
}
