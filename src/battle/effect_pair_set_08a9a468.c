typedef struct EffectPair {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    void *counter;
    void *effect;
} EffectPair;

extern void func_08A6AC9C(void *effect, int value);

int func_08A9A468(EffectPair *self, int value)
{
    if (self->active == 0) {
        return -1;
    }
    func_08A6AC9C(self->effect, value);
    return 0;
}
