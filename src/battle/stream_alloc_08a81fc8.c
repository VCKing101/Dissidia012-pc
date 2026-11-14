typedef struct ByteStream {
    unsigned char active;
    char pad_01[0x7];
    char *base;
    int pos;
} ByteStream;

extern int func_08A820C8(ByteStream *self, int size, int align);

/* Reserves size bytes at the stream position; returns their address. */
char *func_08A81FC8(ByteStream *self, int size)
{
    char *p;

    if (self->active == 0) {
        return 0;
    }
    if (size <= 0) {
        return 0;
    }
    p = self->base + self->pos;
    if (func_08A820C8(self, size, 1) != 0) {
        return 0;
    }
    return p;
}
