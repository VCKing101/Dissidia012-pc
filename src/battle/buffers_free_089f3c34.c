typedef struct BufferSet {
    unsigned char active;
    char pad_01[0x3];
    void *first;
    void *second;
    void *third;
} BufferSet;

extern void func_088DA060(void *buffer);

/* Frees the three buffers that are still held. */
int func_089F3C34(BufferSet *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->third != 0) {
        func_088DA060(self->third);
        self->third = 0;
    }
    if (self->second != 0) {
        func_088DA060(self->second);
        self->second = 0;
    }
    if (self->first != 0) {
        func_088DA060(self->first);
        self->first = 0;
    }
    return 0;
}
