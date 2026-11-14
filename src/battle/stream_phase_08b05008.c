typedef struct StreamSet {
    unsigned char active;
    char pad_01[0x3];
    int phase;
} StreamSet;

extern int func_08B04F80(StreamSet *self);

int func_08B05008(StreamSet *self)
{
    if (func_08B04F80(self) == 0) {
        return -1;
    }
    return self->phase;
}
