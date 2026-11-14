typedef struct Request85B0 {
    unsigned char active;
    char pad_01[0xB];
    unsigned char ready;
    char pad_0D[0x3];
    int value;
    char field_14[0x4];
} Request85B0;

extern int func_089F7EB8(void *request, int value, unsigned char flag);
extern int func_089F83AC(Request85B0 *self);

int func_089F85B0(Request85B0 *self, int value, unsigned char flag)
{
    if (!self->active) {
        return -1;
    }
    if (!self->ready) {
        return -2;
    }
    if (func_089F7EB8(self->field_14, value, flag) != 0) {
        return -3;
    }
    self->value = value;
    if (func_089F83AC(self) != 0) {
        return -4;
    }
    return 0;
}
