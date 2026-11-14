typedef struct FlagBytes {
    unsigned char active;
    unsigned char flags[3];
} FlagBytes;

extern "C" {

void func_08AB7470(FlagBytes *self)
{
    self->active = 0;
    for (int i = 0; i < 3; i++) {
        *((unsigned char *)self + 1 + i) = 0;
    }
}

}
