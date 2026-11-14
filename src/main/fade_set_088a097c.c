typedef struct Fade088A097C {
    float start;
    float end;
    float step;
} Fade088A097C;

typedef struct Owner088A097C {
    unsigned char unknown_00[0x3C];
    Fade088A097C fade;
} Owner088A097C;

void func_088A097C(Owner088A097C *self, float target, float time) {
    if (0.0f < time) {
        self->fade.end = target;
        self->fade.step = (target - self->fade.start) / time;
    } else {
        self->fade.start = target;
        self->fade.end = target;
        self->fade.step = 0.0f;
    }
}
