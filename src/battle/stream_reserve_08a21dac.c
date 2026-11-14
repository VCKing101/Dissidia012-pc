typedef struct StreamBuffer {
    unsigned char active;
    char pad_01[0x7];
    int base;
    int used;
} StreamBuffer;

extern int func_08A21EAC(StreamBuffer *self, int size, int flag);

int func_08A21DAC(StreamBuffer *self, int size)
{
    int pos;
    if (!self->active) {
        return 0;
    }
    if (size <= 0) {
        return 0;
    }
    pos = self->base + self->used;
    if (func_08A21EAC(self, size, 1) != 0) {
        return 0;
    }
    return pos;
}
