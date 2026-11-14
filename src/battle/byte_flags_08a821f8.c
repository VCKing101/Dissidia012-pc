typedef struct ByteFlags821F8 {
    unsigned char active;
    unsigned char flags[8];
} ByteFlags821F8;

void func_08A821F8(ByteFlags821F8 *self)
{
    self->active = 0;
    {
        int i;
        for (i = 0; i < 8; i++) {
            *((unsigned char *)self + 1 + i) = 0;
        }
    }
}
