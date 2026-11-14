typedef struct BitFlags {
    unsigned char active;
    char pad_001[0x3];
    unsigned int pending;
    unsigned int current;
} BitFlags;

int func_08A7E82C(BitFlags *self)
{
    if (!self->active) {
        return -1;
    }
    self->current = self->pending;
    self->pending = 0;
    return 0;
}

int func_08A7E858(BitFlags *self, unsigned int bit)
{
    if (!self->active) {
        return -1;
    }
    if (bit >= 32) {
        return -2;
    }
    {
        unsigned int mask = 1 << bit;
        self->pending |= mask;
    }
    return 0;
}

int func_08A7E8A0(BitFlags *self)
{
    if (!self->active) {
        return -1;
    }
    self->pending = 0;
    self->current = 0;
    return 0;
}

int func_08A7E8C8(BitFlags *self, unsigned int mask)
{
    if (!self->active) {
        return -1;
    }
    self->pending &= ~mask;
    self->current &= ~mask;
    return 0;
}

int func_08A7E904(BitFlags *self, unsigned int bit)
{
    unsigned int mask;
    if (!self->active) {
        return 0;
    }
    if (bit >= 32) {
        return 0;
    }
    mask = 1 << bit;
    return (self->current & mask) != 0;
}
