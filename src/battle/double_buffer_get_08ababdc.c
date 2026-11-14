typedef struct DoubleBuffer {
    unsigned char active;
    char pad_01[0xB];
    int current;
    int slots[2];
    int shown[2];
} DoubleBuffer;

int func_08ABABDC(DoubleBuffer *self)
{
    if (self->active == 0) {
        return 0;
    }
    return self->shown[(unsigned char)(self->current == 0)];
}
