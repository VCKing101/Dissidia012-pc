#include "battle/agent31_types.h"

/* Starts the spark effect: cues the guard banner when the fighter guards,
 * stops the fighter's first five effects, loads the two spark sprites,
 * plays the start motion and sets the sparks up. */
struct SparkStartParts {
    int state;
    unsigned char pad_04[0x28];
    void *sparks[2];
};

struct SparkStartEffect {
    A31VActor *actor;
    unsigned char pad_04[8];
    void *sheet;
    unsigned char pad_10[4];
    SparkStartParts *parts;
};

extern void *D_08A405E0;
extern "C" void func_0882FA10(float x, float y, void *motion, int id, int arg);

struct SparkMotion {
    static void play(A31VActor *actor, int id) { func_0882FA10(0.0f, 0.0f, actor->motion, id, 0); }
};

extern "C" {

extern void func_08A3DCA8(void *banner);
extern void *func_0884F75C(void *effects, int index);
extern void func_0884EEF4(void *effect, int flags);
extern void *func_088DAA68(void *sheet);
extern void func_08A4CB14(SparkStartEffect *self);

void func_08A4C830(SparkStartEffect *self)
{
    if (self->actor->guard() == 1) {
        func_08A3DCA8(D_08A405E0);
    }
    void *effects = self->actor->effects;
    for (int i = 0; i < 5; i++) {
        void *effect = func_0884F75C(effects, i);
        if (effect != 0) {
            func_0884EEF4(effect, 0);
        }
    }
    if (self->sheet != 0) {
        for (int i = 0; i < 2; i++) {
            self->parts->sparks[i] = func_088DAA68(self->sheet);
        }
    }
    SparkMotion::play(self->actor, 0x1FA5);
    func_08A4CB14(self);
    self->parts->state = 0;
}

}
