#include "main/agent30_effect.h"

/* Effect node: switches the node to its sound phase with a sound. */
extern "C" {

int func_08819250(EffectNode *self);
int func_08818B28(EffectNode *self);
void *func_0881BAA8(void);
void func_0881C2CC(void *mgr, EffectNodeDef *def, int flag);
void func_088198F4(EffectNode *self, float time);

int func_088192C8(EffectNode *self, int sound, unsigned char on, unsigned char loop)
{
    if (self->state == 2) {
        return 0;
    }
    if (self->state == 1) {
        if (!func_08819250(self)) {
            return 0;
        }
    }
    func_08818B28(self);
    self->state = 2;
    self->b_value = 1;
    self->c_time = 0.0f;
    self->sound = sound;
    self->sound_on = on;
    self->sound_loop = loop;
    func_0881C2CC(func_0881BAA8(), self->def, 0);
    func_088198F4(self, 0.0f);
    return 0;
}

}
