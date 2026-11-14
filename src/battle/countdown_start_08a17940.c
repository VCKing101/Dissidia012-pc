typedef struct Countdown {
    unsigned char active;
    char pad_01[0x3];
    int state;
    float elapsed;
    unsigned char done;
    char pad_0D[0x7];
    int frames;
} Countdown;

int func_08A17940(Countdown *self, float frames)
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
    self->done = 0;
    self->frames = (int)frames;
    return 0;
}
