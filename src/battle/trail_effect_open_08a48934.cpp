#include "battle/agent31_types.h"

/* Trail effect start: cues the guard banner when the fighter guards,
 * frees the five trails, loads the six trail sprites, plays the attack
 * motion and the target's held reaction, shakes the screen and starts the
 * trail stage. */
struct TrailOpenParts {
    int state;
    unsigned char pad_04[0x28];
    void *trails[5];
    unsigned char pad_40[0x18];
    void *sprites[6];
};

struct TrailOpenEffect {
    A31VActor *actor;
    A31VActor *target;
    unsigned char pad_08[4];
    void *sheet;
    unsigned char pad_10[4];
    TrailOpenParts *parts;
};

extern void *D_08A405E0;

extern "C" {

extern void func_08A3DCA8(void *cue);
extern void func_088A6C70(void *trail);
extern void *func_088DAA68(void *source);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void func_08B3AE14(float delay, void *display, int time, int a, int b);
extern void func_08A48CC8(TrailOpenEffect *self);
extern void func_08A4A2A0(TrailOpenEffect *self);

void func_08A48934(TrailOpenEffect *self)
{
    int i;
    if (self->actor->guard() == 1) {
        func_08A3DCA8(D_08A405E0);
    }
    for (i = 0; i < 5; i++) {
        if (self->parts->trails[i] != 0) {
            func_088A6C70(self->parts->trails[i]);
            self->parts->trails[i] = 0;
        }
    }
    if (self->sheet != 0) {
        for (i = 0; i < 6; i++) {
            self->parts->sprites[i] = func_088DAA68(self->sheet);
        }
    }
    {
        A31VActor *actor = self->actor;
        int id = 0x1FA5;
        func_0882FA10(0.0f, 0.0f, actor->motion, id, 0);
    }
    { A31ActorBits *bits = &self->target->bits; bits->b8 |= 0x80; }
    func_0882FA10(0.0f, 0.0f, self->target->motion, 0x226, 0);
    { A31ActorBits *bits = &self->target->bits; bits->b8 &= ~0x80; }
    func_08B3AE14(0.0f, self->actor->guard_display, 10000, 0, 5);
    func_08A48CC8(self);
    self->parts->state = 0;
    func_08A4A2A0(self);
}

}
