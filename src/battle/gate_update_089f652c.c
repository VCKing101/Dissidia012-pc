extern void func_08AEAF28(void *);

typedef struct Gate652C {
    unsigned char field_00;
    char pad_001[3];
    char field_04[4];
} Gate652C;

int func_089F652C(Gate652C *self)
{
    if (!self->field_00) {
        return -1;
    }
    func_08AEAF28(self->field_04);
    return 0;
}
