#include "main/agent30_types.h"

/* Starts the effect's motion for a key: looks the motion up in the
   resource's table and plays it from the start. */
extern "C" {

void func_0886F0B4(char *anim, int motion, float start, float speed);
float func_0886B344(void *player, int motion);

int func_0880FD34(EffectObject *self, int key, int arg, float speed)
{
    if (self->resource == 0) {
        return 0;
    }
    if (self->resource->table == 0) {
        return 0;
    }
    EffectMotionTable *t0 = self->resource->table;
    EffectMotionTable *t = t0 != 0 ? t0 : 0;
    EffectMotionEntry *e = func_08934944(key, t->entries, t->count, sizeof(EffectMotionEntry), func_0880FB60);
    if (e != 0) {
    int motion = e->motion;
    func_0886F0B4(self->anim, motion, 0.0f, speed);
    self->arg = arg;
    self->length = func_0886B344(self->player, motion);
    self->state = 2;
    return 1;
    }
    return 0;
}

}
