#include "battle/agent31_types.h"

/* Burst effect, hit stage: on a miss plays the fighter's idle motion and
 * ends; on a hit plays the strike and its two layers, puts the target in
 * its held state, plays the target's reaction and moves to stage 2. */
struct BurstHitParts {
    int state;
};

struct BurstHitEffect {
    A31VActor *actor;
    A31VActor *target;
    unsigned char pad_08[0xC];
    BurstHitParts *parts;
};

extern "C" {

extern int func_08A50140(BurstHitEffect *self);
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);
extern void func_0882FB90(void *motion, int id, float x, float y);
extern void func_08834C10(A31VActor *actor, int state, int arg);
extern void func_08834934(A31VActor *actor, int a, int b);

void func_08A4F26C(BurstHitEffect *self)
{
    if (func_08A50140(self) == 0) {
        A31VActor *actor = self->actor;
        func_0882FA10(actor->motion, 0x1FA5, 0.0f, 0.0f, 0);
        self->parts->state = 0;
    } else {
        A31VActor *actor = self->actor;
        func_0882FA10(actor->motion, 0x1FA7, 0.0f, 0.0f, 0);
        func_0882FB90(self->actor->motion, 0x1FA8, 0.0f, 0.0f);
        func_0882FB90(self->actor->motion, 0x2134, 0.0f, 0.0f);
        func_08834C10(self->target, 0x12, 0);
        func_08834934(self->target, -1, 0x66);
        { A31ActorBits *bits = &self->target->bits; bits->b8 |= 0x80; }
        func_0882FA10(self->target->motion, 0x227, 0.0f, 0.0f, 0);
        { A31ActorBits *bits = &self->target->bits; bits->b8 &= ~0x80; }
        self->parts->state = 2;
    }
}

}
