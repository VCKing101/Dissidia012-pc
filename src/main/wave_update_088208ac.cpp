/* Advances a wave: updates both sides, steps the phase by speed * rate,
   keeps its fractional part and sets the angle. */
struct Wave088208AC {
    unsigned char ready;
    char pad_01[0x28 - 1];
    float speed;
    float rate;
    float phase;
    float angle;
};

extern "C" {

extern float D_089C7678;

void func_08820DD0(Wave088208AC *self, float time);
void func_08820D50(Wave088208AC *self, float time);

int func_088208AC(Wave088208AC *self, float time)
{
    if (!self->ready) {
        return -1;
    }
    func_08820DD0(self, time);
    func_08820D50(self, time);
    self->phase = self->phase + self->speed * self->rate;
    self->phase = self->phase - (int)self->phase;
    self->angle = D_089C7678 * 2.0f * self->phase;
    return 0;
}

}
