typedef struct EffectNode {
    char pad_000[0x124];
    void *effect;
} EffectNode;

extern int func_088A751C(void *effect);
extern void func_08AD8284(EffectNode *self, int arg);
extern void func_08AD2ABC(EffectNode *self, int arg);

void func_08AD7CFC(EffectNode *self)
{
    if (self->effect != 0 && func_088A751C(self->effect) == 1) {
        func_08AD8284(self, 0);
    } else {
        func_08AD2ABC(self, 0);
    }
}
