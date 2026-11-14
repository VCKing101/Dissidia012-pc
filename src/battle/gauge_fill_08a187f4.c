typedef struct GaugeFill {
    char pad_000[0x8];
    int state;
    char pad_00C[0x4];
    float value;
    char pad_014[0x28];
    char field_3C[0x114];
    int field_150;
} GaugeFill;

extern float func_0889140C(int id);
extern int func_08A22DEC(float remaining);
extern void func_08A19080(void *sub, int value);

int func_08A187F4(GaugeFill *self, float dt)
{
    float limit = func_0889140C(self->field_150);
    int frame;
    self->value += dt;
    if (self->value > limit) {
        self->value = limit;
    }
    frame = func_08A22DEC(limit - self->value);
    func_08A19080(self->field_3C, frame);
    if (!(self->value < limit)) {
        self->state = 3;
        self->value = 0.0f;
    }
    return 0;
}
