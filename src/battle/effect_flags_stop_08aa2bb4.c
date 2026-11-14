typedef struct FlagEffect {
    unsigned char active;
    char pad_01[0x7];
    char flags[0x20];
    void *effect;
} FlagEffect;

extern int func_08A7E82C(void *flags);
extern void func_08A66094(void *effect);

int func_08AA2BB4(FlagEffect *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_08A7E82C(self->flags);
    func_08A66094(self->effect);
    return 0;
}
