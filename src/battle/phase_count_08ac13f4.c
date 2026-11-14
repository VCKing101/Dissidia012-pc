typedef struct PhaseState {
    unsigned char active;
    char pad_01[0x1F];
    int count;
} PhaseState;

int func_08AC13F4(PhaseState *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->count++;
    return 0;
}
