typedef struct GateC13C {
    char pad_000[0x8];
    unsigned char field_08;
    char pad_009[0x3];
    int field_0C;
} GateC13C;

int func_089EC13C(GateC13C *self)
{
    if (!self->field_08) {
        return -1;
    }
    self->field_0C = 6;
    return 0;
}
