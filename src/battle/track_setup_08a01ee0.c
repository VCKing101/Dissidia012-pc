typedef struct TrackSetup {
    unsigned char active;
    char pad_01[0xB];
    unsigned int side;
    unsigned int slot;
    void *source;
    void *target;
    int start;
} TrackSetup;

extern void func_08A01E88(TrackSetup *self);

/* Records side, slot, source, target and start; -1 for bad arguments. */
int func_08A01EE0(TrackSetup *self, unsigned int side, unsigned int slot, void *source, void *target, int start)
{
    func_08A01E88(self);
    if (side >= 2 || slot >= 2 || source == 0 || target == 0 || start < 0) {
        return -1;
    }
    self->side = side;
    self->slot = slot;
    self->source = source;
    self->target = target;
    self->start = start;
    self->active = 1;
    return 0;
}
