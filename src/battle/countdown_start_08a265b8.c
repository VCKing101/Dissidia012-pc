typedef struct Countdown265B8 {
    unsigned char active;
    char pad_01[0x3];
    int state;
    float elapsed;
} Countdown265B8;

int func_08A265B8(Countdown265B8 *self)
{
    int result = -1;
    if (!self->active) {
        return result;
    }
    if (self->state != result) {
        return -2;
    }
    self->state = 3;
    self->elapsed = 0.0f;
    return 0;
}
