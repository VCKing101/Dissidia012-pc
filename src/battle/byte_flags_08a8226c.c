typedef struct ByteFlags8226C {
    unsigned char active;
    unsigned char flags[8];
} ByteFlags8226C;

int func_08A8226C(ByteFlags8226C *self)
{
    if (!self->active) {
        return -1;
    }
    {
        int i;
        for (i = 0; i < 8; i++) {
            *((unsigned char *)self + 1 + i) = 0;
        }
    }
    return 0;
}
