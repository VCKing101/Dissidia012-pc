typedef struct Fade088A09BC {
    float start;
    float end;
    float step;
} Fade088A09BC;

typedef struct Owner088A09BC {
    unsigned char unknown_00[0x48];
    Fade088A09BC fade;
} Owner088A09BC;

void func_088A09BC(Owner088A09BC *self, float target, float time) {
    if (0.0f < time) {
        self->fade.end = target;
        self->fade.step = (target - self->fade.start) / time;
    } else {
        self->fade.start = target;
        self->fade.end = target;
        self->fade.step = 0.0f;
    }
}
