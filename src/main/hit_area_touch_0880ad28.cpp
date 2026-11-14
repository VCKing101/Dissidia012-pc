#include "main/agent30_types.h"

/* Records that the area touched an actor: reports whether the actor is
   inside and whether this is the first touch, starts the owner's cooldown,
   runs the hit's timed flag and marks the actor in both sets. Returns the
   actor's current hit definition. */
struct HitDef0880AD28 {
    char pad_00[0x12];
    unsigned char timed;
};

struct Actor0880AD28 {
    char pad_00[0x26];
    short slot;
    char pad_28[0x34 - 0x28];
    HitDef0880AD28 *hit;
};

extern "C" {

extern float D_089C6CA0;

bool func_0880ACBC(HitArea *self, Actor0880AD28 *actor);
int func_0883739C(void *owner);
void func_0880AAE0(HitArea *self, Actor0880AD28 *actor, HitDef0880AD28 *hit);

HitDef0880AD28 *func_0880AD28(HitArea *self, Actor0880AD28 *actor, unsigned char *inside, unsigned char *first)
{
    *inside = func_0880ACBC(self, actor);
    *first = !BitSet256::test(&self->seen, actor->slot);
    if (*inside && self->owner != 0 && func_0883739C(self->owner)) {
        self->cooldown = D_089C6CA0;
    }
    HitDef0880AD28 *hit = actor->hit;
    if (hit->timed > 0) {
        func_0880AAE0(self, actor, hit);
    }
    BitSet256::add(&self->hit, actor->slot);
    BitSet256::add(&self->seen, actor->slot);
    return hit;
}

}
