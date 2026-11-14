typedef struct GateC114 {
    char pad_000[0x8];
    unsigned char field_08;
    char pad_009[0x3];
    int field_0C;
} GateC114;

int func_089EC114(GateC114 *self)
{
    if (!self->field_08) {
        return -1;
    }
    self->field_0C = 5;
    return 0;
}
