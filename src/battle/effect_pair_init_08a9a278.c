typedef struct EffectPair {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    void *counter;
    void *effect;
} EffectPair;

extern void func_08A9A25C(EffectPair *self);

int func_08A9A278(EffectPair *self, void *owner, void *counter, void *effect)
{
    func_08A9A25C(self);
    if (owner == 0 || counter == 0 || effect == 0) {
        return -1;
    }
    self->counter = counter;
    self->effect = effect;
    self->owner = owner;
    self->active = 1;
    return 0;
}
