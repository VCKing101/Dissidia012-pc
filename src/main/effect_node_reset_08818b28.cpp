#include "main/agent30_effect.h"

/* Effect node: reset and restart. */
extern "C" {

void *func_0881BAA8(void);
void func_0881C148(void *mgr, EffectNodeDef *def, int load);
void *func_08899300(void);
void func_088983C8(void *cache, int id, int flag);
void func_08814FA0(void *owner);
void func_0881C468(void *mgr, int lock);
void func_08818E48(EffectNode *self);
void func_08819624(EffectNode *self, int mode, int a, int b, int c, int d, int f);

/* Puts the node back to its start state, reloads its resources and
   resets its owner. */
int func_08818B28(EffectNode *self)
{
    if (!self->active) {
        return -1;
    }
    self->state = 4;
    self->a_on = 0;
    self->a_time = 0.0f;
    self->a_value = 0;
    self->b_on = 0;
    self->b_time = 0.0f;
    self->b_value = 0;
    self->c_time = 0.0f;
    self->sound = -1;
    self->sound_on = 0;
    self->sound_loop = 0;
    func_0881C148(func_0881BAA8(), self->def, 1);
    if (self->def->flags & 1) {
        void *cache = func_08899300();
        func_088983C8(cache, self->def->id, 0);
    }
    if (self->owner != 0) {
        func_08814FA0(self->owner);
    }
    return 0;
}

/* Restarts a restartable node: a looping track is stopped instead. */
int func_08818BF4(EffectNode *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->def->flags & 2) {
        if (self->track != 0 && self->track->looping == 1) {
            func_08818E48(self);
        } else {
            void *mgr = func_0881BAA8();
            func_0881C468(mgr, 1);
            func_08819624(self, 0, 0, 0, 1, 1000, 1);
            func_0881C468(mgr, 0);
        }
    }
    return 0;
}

}
