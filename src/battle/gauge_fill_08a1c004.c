typedef struct GaugeFillC004 {
    char pad_000[0x8];
    int state;
    char pad_00C[0x4];
    float value;
    char pad_014[0x2C];
    char field_40[0x134];
    int field_174;
} GaugeFillC004;

extern float func_0889140C(int id);
extern int func_08A22DEC(float remaining);
extern void func_08A19080(void *sub, int value);

int func_08A1C004(GaugeFillC004 *self, float dt)
{
    float limit = func_0889140C(self->field_174);
    int frame;
    self->value += dt;
    if (self->value > limit) {
        self->value = limit;
    }
    frame = func_08A22DEC(limit - self->value);
    func_08A19080(self->field_40, frame);
    if (!(self->value < limit)) {
        self->state = 3;
        self->value = 0.0f;
    }
    return 0;
}
