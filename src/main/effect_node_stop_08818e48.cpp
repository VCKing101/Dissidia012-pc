#include "main/agent30_effect.h"

/* Stops an effect node: resets it, unloads its resources and releases its
   owner and lights. */
extern "C" {

int func_08818B28(EffectNode *self);
void *func_0881BAA8(void);
void func_0881C2CC(void *mgr, EffectNodeDef *def, int flag);
void func_0881CAE0(void *mgr, int id, int a, int b);
void func_0881559C(void *owner);
void func_0881A124(char *lights);

int func_08818E48(EffectNode *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->state == 3) {
        return 0;
    }
    func_08818B28(self);
    self->state = 3;
    func_0881C2CC(func_0881BAA8(), self->def, 0);
    func_0881CAE0(func_0881BAA8(), self->def->id, 0, 0);
    if (self->owner != 0) {
        void *owner = self->owner;
        func_0881559C(owner);
    }
    func_0881A124(self->lights);
    return 0;
}

}
