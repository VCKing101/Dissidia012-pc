typedef struct FlagBytes {
    unsigned char active;
    unsigned char flags[3];
} FlagBytes;

extern "C" {

int func_08AB74E4(FlagBytes *self)
{
    if (self->active == 0) {
        return -1;
    }
    for (int i = 0; i < 3; i++) {
        *((unsigned char *)self + 1 + i) = 0;
    }
    return 0;
}

}
