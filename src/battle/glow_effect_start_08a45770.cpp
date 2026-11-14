#include "battle/agent31_types.h"

/* Starts the glow effect: cues the guard banner when the fighter guards,
 * loads the glow sprite, plays the start motions and sets the glow up. */
struct GlowStartParts {
    unsigned char state;
    unsigned char pad_001[0x307];
    void *glow;
};

struct GlowStartEffect {
    A31VActor *actor;
    unsigned char pad_04[8];
    void *sheet;
    unsigned char pad_10[4];
    GlowStartParts *parts;
};

extern void *D_08A405E0;

extern "C" {

extern void func_08A3DCA8(void *banner);
extern void *func_088DAA68(void *sheet);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void func_0882FB90(float x, float y, void *motion, int id);
extern void func_08A45A5C(GlowStartEffect *self);

void func_08A45770(GlowStartEffect *self)
{
    if (self->actor->guard() == 1) {
        func_08A3DCA8(D_08A405E0);
    }
    if (self->sheet != 0) {
        self->parts->glow = func_088DAA68(self->sheet);
    }
    float zero = 0.0f;
    {
        int id = 0x1FA5;
        func_0882FA10(zero, zero, self->actor->motion, id, 0);
    }
    {
        int id = 0x1FA6;
        func_0882FB90(zero, zero, self->actor->motion, id);
    }
    func_08A45A5C(self);
    self->parts->state = 0;
}

}
