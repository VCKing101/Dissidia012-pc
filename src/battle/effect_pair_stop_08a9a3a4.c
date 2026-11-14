typedef struct EffectPair {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    void *counter;
    void *effect;
} EffectPair;

extern void func_08A66094(void *effect);

int func_08A9A3A4(EffectPair *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_08A66094(self->effect);
    return 0;
}
