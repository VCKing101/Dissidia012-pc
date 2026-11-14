typedef struct UnitHolder {
    unsigned char active;
    char pad_01[0x2F];
    unsigned int mode;
} UnitHolder;

int func_08A976E4(UnitHolder *self, unsigned int mode)
{
    if (self->active == 0) {
        return -1;
    }
    if (mode >= 6) {
        return -2;
    }
    self->mode = mode;
    return 0;
}
