typedef struct RangeSet {
    unsigned char active;
    char pad_001[0xB];
    int start;
    int count;
} RangeSet;

int func_08A19038(RangeSet *self, int start, int count)
{
    if (!self->active) {
        return -1;
    }
    if (start < 0 || count == 0) {
        return -2;
    }
    self->start = start;
    self->count = count;
    return 0;
}
