typedef struct FlagEffect {
    unsigned char active;
    char pad_01[0x7];
    char flags[0x1C];
    void *counter;
    void *effect;
} FlagEffect;

extern void func_08A65A84(void *counter);
extern void func_08A66390(void *effect);
extern void func_08A7E8A0(void *flags);

int func_08AA2E04(FlagEffect *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_08A65A84(self->counter);
    func_08A66390(self->effect);
    func_08A7E8A0(self->flags);
    return 0;
}
