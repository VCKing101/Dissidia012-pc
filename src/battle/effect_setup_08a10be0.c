typedef struct EffectSetup {
    unsigned char active;
    char pad_01[0xF];
    void *owner;
    int index;
    int field_18;
    int field_1C;
    int field_20;
    unsigned char looped;
} EffectSetup;

extern void func_08A10B88(EffectSetup *self);

/* Records the effect's owner, index and settings; -1 without an owner or index. */
int func_08A10BE0(EffectSetup *self, void *owner, int index, int a, int b, int c, unsigned char looped)
{
    func_08A10B88(self);
    if (owner == 0 || index < 0) {
        return -1;
    }
    self->owner = owner;
    self->index = index;
    self->field_18 = a;
    self->field_1C = b;
    self->field_20 = c;
    self->looped = looped;
    self->active = 1;
    return 0;
}
