typedef struct FlagBytes {
    unsigned char active;
    unsigned char flags[3];
} FlagBytes;

int func_08AB7520(FlagBytes *self, unsigned int index)
{
    if (self->active == 0) {
        return -1;
    }
    if (index >= 3) {
        return -2;
    }
    self->flags[index] = 1;
    return 0;
}
