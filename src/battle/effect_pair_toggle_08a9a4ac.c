typedef struct EffectPair {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    void *counter;
    void *effect;
} EffectPair;

extern void func_08A6ADB4(void *effect, unsigned char on);

int func_08A9A4AC(EffectPair *self, unsigned char on)
{
    if (self->active == 0) {
        return -1;
    }
    func_08A6ADB4(self->effect, on);
    return 0;
}
