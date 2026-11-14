typedef struct RandomState {
    unsigned char active;
    char pad_01[0x3];
    int seed;
} RandomState;

extern int func_08AA5748(RandomState *self);

/* Random value in [0, 1]. */
float func_08AA57D0(RandomState *self)
{
    if (self->active == 0) {
        return 0.0f;
    }
    return (float)func_08AA5748(self) / 32767.0f;
}
