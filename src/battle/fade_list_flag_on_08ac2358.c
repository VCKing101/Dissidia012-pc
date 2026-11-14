typedef struct FadeList {
    unsigned char active;
    char pad_01[0x13];
    unsigned char flag_14;
} FadeList;

int func_08AC2358(FadeList *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->flag_14 = 1;
    return 0;
}
