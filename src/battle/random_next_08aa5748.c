typedef struct RandomState {
    unsigned char active;
    char pad_01[0x3];
    int seed;
} RandomState;

/* Next value of the linear congruential generator, 0-0x7FFF. */
int func_08AA5748(RandomState *self)
{
    int seed;

    if (self->active == 0) {
        return 0;
    }
    seed = self->seed * 0x343FD + 0x269EC3;
    self->seed = seed;
    return (seed >> 16) & 0x7FFF;
}
