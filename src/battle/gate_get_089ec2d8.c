typedef struct GateC2D8 {
    char pad_000[0x8];
    unsigned char field_08;
    char pad_009[0x3F];
    int field_48;
} GateC2D8;

int func_089EC2D8(GateC2D8 *self)
{
    if (!self->field_08) {
        return -1;
    }
    return self->field_48;
}
