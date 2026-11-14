typedef struct GateC0EC {
    char pad_000[0x8];
    unsigned char field_08;
    char pad_009[0x23];
    unsigned char field_2C;
} GateC0EC;

int func_089EC0EC(GateC0EC *self)
{
    if (!self->field_08) {
        return -1;
    }
    self->field_2C = 1;
    return 0;
}
