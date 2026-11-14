typedef struct EachObj {
    char pad_00[0xC];
    int *field_0C;
    char field_10;
} EachObj;

extern void func_08AB1D64(EachObj *self, int a, int b, int value);

void func_08AB176C(EachObj *self, int a, int b)
{
    int i = 0;
    int offset;
    if (i < self->field_10 - 1) {
        offset = 0;
        do {
            func_08AB1D64(self, a, b, *(int *)((char *)self->field_0C + offset));
            i++;
            offset += 4;
        } while (i < self->field_10 - 1);
    }
}
