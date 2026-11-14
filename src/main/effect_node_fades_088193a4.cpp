#include "main/agent30_effect.h"

/* Effect node phase timers. */
struct Owner088193A4 {
    char pad_000[0x130];
    char motion[0x50];
    short motion_id;
};

extern "C" {

void func_0882FA10(char *motion, int id, int mode, float time, float start);
void *func_0881BAA8(void);
void func_0881C2CC(void *mgr, EffectNodeDef *def, int flag);
void func_0881C518(void *mgr, EffectNodeDef *def);
void func_08815314(Owner088193A4 *owner);

/* Counts down phase A; when it runs out, ends the owner's 0x3E9 motion and
   unloads the node's resources. */
void func_088193A4(EffectNode *self, float dt)
{
    if (self->a_on == 1) {
        return;
    }
    if (self->a_time > 0.0f) {
        self->a_time -= dt;
        if (self->a_time > 0.0f) {
            return;
        }
    }
    Owner088193A4 *o = (Owner088193A4 *)self->owner;
    if (o != 0 && o->motion_id == 0x3E9) {
        func_0882FA10(o->motion, 0x3EA, 0, 0.0f, 0.0f);
    }
    void *mgr = func_0881BAA8();
    func_0881C2CC(mgr, self->def, 0);
    func_0881C518(mgr, self->def);
    self->a_on = 1;
}

/* Counts down phase B (held while the owner plays motion 0x3E9); when it
   runs out, releases the owner. */
void func_08819480(EffectNode *self, float dt)
{
    if (self->b_on == 1) {
        return;
    }
    Owner088193A4 *o = (Owner088193A4 *)self->owner;
    if (o != 0 && o->motion_id == 0x3E9) {
        return;
    }
    if (self->b_time > 0.0f) {
        self->b_time -= dt;
        if (self->b_time > 0.0f) {
            return;
        }
    }
    if (o != 0) {
        func_08815314(o);
    }
    self->b_on = 1;
}

}
