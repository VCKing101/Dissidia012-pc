typedef struct GateC164 {
    char pad_000[0x8];
    unsigned char field_08;
    char pad_009[0x3];
    int field_0C;
} GateC164;

int func_089EC164(GateC164 *self)
{
    if (!self->field_08) {
        return -1;
    }
    self->field_0C = 0;
    return 0;
}
