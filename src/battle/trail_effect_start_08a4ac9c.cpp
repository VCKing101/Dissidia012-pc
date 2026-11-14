#include "battle/agent31_types.h"

/* Starts the trail effect: cues the guard banner when the fighter guards,
 * plays the start motion, clears the seven trail timers and starts the
 * looping sound unless the stage mutes it. */
struct TrailStartItem {
    int timer;
    unsigned char pad_04[0x18];
};

struct TrailStartParts {
    int state;
    unsigned char pad_04[0xC];
    TrailStartItem items[7];
    unsigned char pad_d4[8];
    void *loop;
};

struct TrailStage {
    unsigned char pad_00[0x14];
    unsigned char flags;

    static bool muted(TrailStage *stage) { return (stage->flags & 0x20) != 0; }
};

struct TrailStartEffect {
    A31VActor *actor;
    unsigned char pad_04[8];
    TrailStage *stage;
    unsigned char pad_10[4];
    TrailStartParts *parts;
};

extern void *D_08A405E0;
extern "C" void func_0882FA10(float x, float y, void *motion, int id, int arg);

struct TrailMotion {
    static void play(A31VActor *actor, int id) { func_0882FA10(0.0f, 0.0f, actor->motion, id, 0); }
};

extern "C" {

extern void func_08A3DCA8(void *banner);
extern void *func_088A24D4(int owner, int id, int loop, int flags);
extern void func_08A4AF8C(TrailStartEffect *self);

void func_08A4AC9C(TrailStartEffect *self)
{
    if (self->actor->guard() == 1) {
        func_08A3DCA8(D_08A405E0);
    }
    TrailMotion::play(self->actor, 0x1FA5);
    int none = 0;
    for (int i = 0; i < 7; i++) {
        self->parts->items[i].timer = none;
    }
    if (!TrailStage::muted(self->stage) && self->parts->loop == 0) {
        self->parts->loop = func_088A24D4(0, 0x1437C9F, 1, 0);
    }
    func_08A4AF8C(self);
    self->parts->state = 0;
}

}
