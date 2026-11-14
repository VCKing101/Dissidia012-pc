typedef struct LinkState {
    unsigned char active;
    char pad_01[0x13];
    unsigned char flag_14;
} LinkState;

int func_08AC068C(LinkState *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->flag_14 = 1;
    return 0;
}
