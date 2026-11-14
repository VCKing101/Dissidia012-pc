typedef struct EffectPair {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    void *counter;
    void *effect;
} EffectPair;

extern void func_08AAE22C(EffectPair *self);

int func_08AAE248(EffectPair *self, void *owner, void *counter, void *effect)
{
    func_08AAE22C(self);
    if (owner == 0 || counter == 0 || effect == 0) {
        return -1;
    }
    self->owner = owner;
    self->counter = counter;
    self->effect = effect;
    self->active = 1;
    return 0;
}
