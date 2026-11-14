#include "main/agent30_effect.h"

/* Effect node: puts the node back to its start state and reloads it from
   the cache (or loads it when it is not cached). */
extern "C" {

void *func_08899300(void);
void func_088983C8(void *cache, int id, int flag);
void func_088981B4(void *cache, int id, int flag, int load);
void func_08814FA0(void *owner);
void *func_0881BAA8(void);
void func_0881CB3C(void *mgr, int id, int flag);

int func_08818F84(EffectNode *self)
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
    void *cache = func_08899300();
    EffectNodeDef *def = self->def;
    int id = def->id;
    if (def != 0 && (def->flags & 1)) {
        func_088983C8(cache, id, 0);
    } else {
        func_088981B4(cache, id, 1, 1);
    }
    if (self->owner != 0) {
        func_08814FA0(self->owner);
    }
    func_0881CB3C(func_0881BAA8(), self->def->id, 1);
    return 0;
}

}
