typedef struct GateC04C {
    char pad_000[0x8];
    unsigned char field_08;
    char pad_009[0x1B];
    unsigned char field_24;
} GateC04C;

int func_089EC04C(GateC04C *self)
{
    if (!self->field_08) {
        return -1;
    }
    self->field_24 = 1;
    return 0;
}
