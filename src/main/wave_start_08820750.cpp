/* Starts a wave: resets its state and sizes, then sets its first value
   for the wave kind. */
struct WaveDef08820750 {
    char pad_00[0x34];
    int kind;
};

struct Wave08820750 {
    unsigned char ready;
    char pad_01[3];
    WaveDef08820750 *def;
    int state;
    float time;
    float length;
    float rise;
    float fall;
    unsigned char looped;
    char pad_1D[3];
    float speed;
    float rate;
    float phase;
    float angle;
    float value;
    float target;
};

extern "C" {

float func_08820C50(Wave08820750 *self);
float func_08820B54(Wave08820750 *self);
float func_08820B9C(Wave08820750 *self);
float func_08820C2C(Wave08820750 *self);
float func_08820C08(Wave08820750 *self);
float func_08820BE4(Wave08820750 *self);
float func_08820CBC(Wave08820750 *self, float time, float length);
int func_088208AC(Wave08820750 *self, float time);

int func_08820750(Wave08820750 *self)
{
    if (!self->ready) {
        return -1;
    }
    float zero = 0.0f;
    self->state = 1;
    self->time = zero;
    self->length = func_08820C50(self);
    self->rise = func_08820B54(self);
    self->fall = func_08820B9C(self);
    self->looped = 0;
    self->speed = 1.0f;
    self->rate = 1.0f;
    self->phase = zero;
    self->angle = zero;
    self->value = zero;
    self->target = zero;
    switch (self->def->kind) {
    case 0:
        func_088208AC(self, zero);
        break;
    case 1:
        self->state = 3;
        self->length = func_08820C2C(self);
        func_088208AC(self, zero);
        break;
    case 2: {
        float a = func_08820C50(self) + func_08820C08(self);
        float b = func_08820C2C(self) * func_08820BE4(self);
        func_088208AC(self, func_08820CBC(self, zero, a + b));
    }
    }
    return 0;
}

}
