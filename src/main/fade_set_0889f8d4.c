typedef struct Fade0889F8D4 {
    int field_00;
    int field_04;
    float start;
    float end;
    float step;
} Fade0889F8D4;

void func_0889F8D4(Fade0889F8D4 *self, int frames, float target) {
    if (frames == 0) {
        self->start = target;
        self->end = target;
        self->step = 0.0f;
    } else {
        self->end = target;
        self->step = (target - self->start) / (float)frames;
    }
}
