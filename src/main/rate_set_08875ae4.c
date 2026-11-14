typedef struct Fade08875AE4 {
    int field_00;
    float value;
    unsigned char unknown_08[0x10];
    float step;
    float start;
} Fade08875AE4;

int func_08875978(Fade08875AE4 *self);

int func_08875AE4(Fade08875AE4 *self, float duration) {
    if (func_08875978(self) == 0) {
        return -1;
    }
    self->start = 0.0f;
    self->step = -(1.0f / duration);
    return 0;
}
