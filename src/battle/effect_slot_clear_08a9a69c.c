typedef struct EffectSlot {
    unsigned char active;
    char pad_01[0xB];
    char flags[0x4];
} EffectSlot;

extern int func_08A7E82C(void *flags);

int func_08A9A69C(EffectSlot *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (func_08A7E82C(self->flags) != 0) {
        return -2;
    }
    return 0;
}
