typedef struct Spring {
    float rate;
    float velocity;
} Spring;

typedef struct SpringPair {
    char pad_00[0x10];
    void *owner;
    char pad_14[0x6C];
    Spring first;
    Spring second;
} SpringPair;

extern void func_08AC5578(Spring *self, float rate);

SpringPair *func_08AC55E0(SpringPair *self, void *owner)
{
    self->owner = owner;
    func_08AC5578(&self->first, 0.0225f);
    func_08AC5578(&self->second, 0.01f);
    return self;
}
