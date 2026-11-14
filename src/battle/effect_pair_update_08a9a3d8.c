typedef struct EffectPair {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    void *counter;
    void *effect;
} EffectPair;

extern int func_08A65904(void *counter);
extern int func_08A663E4(void *effect);
extern int func_08A65928(void *counter);
extern void func_08A65A84(void *counter);
extern void func_08A66390(void *effect);
extern void func_08A66234(void *effect);

int func_08A9A3D8(EffectPair *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (func_08A65904(self->counter) == 1 || func_08A663E4(self->effect) == 1 || func_08A65928(self->counter) == 1) {
        func_08A65A84(self->counter);
        func_08A66390(self->effect);
    }
    func_08A66234(self->effect);
    return 0;
}
