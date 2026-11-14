typedef struct Fade08875A84 {
    int field_00;
    float value;
    unsigned char unknown_08[0x10];
    float step;
    float start;
} Fade08875A84;

int func_08875978(Fade08875A84 *self);

int func_08875A84(Fade08875A84 *self, float duration) {
    if (func_08875978(self) == 0) {
        return -1;
    }
    self->start = self->value;
    self->value = 0.0f;
    self->step = 1.0f / duration;
    return 0;
}
