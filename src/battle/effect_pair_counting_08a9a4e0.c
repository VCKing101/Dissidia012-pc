typedef struct EffectPair {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    void *counter;
    void *effect;
} EffectPair;

extern int func_08A65904(void *counter);

int func_08A9A4E0(EffectPair *self)
{
    if (self->active == 0) {
        return 0;
    }
    if (func_08A65904(self->counter) == 0) {
        return 0;
    }
    return 1;
}
