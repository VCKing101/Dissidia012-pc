#include "battle/agent31_types.h"

/* Starts the burst effect: cues the guard banner when the fighter guards,
 * plays the start motion and starts the eight burst effects from the
 * table (slot 15 for the table's kind 1, slot 16 otherwise). */
struct BurstSpawn {
    int id;
    int kind;
};

struct BurstStartParts {
    int state;
};

struct BurstStartEffect {
    A31VActor *actor;
    unsigned char pad_04[0x10];
    BurstStartParts *parts;
};

extern void *D_08A405E0;
extern BurstSpawn D_08A4F08C[8];
extern "C" void func_0882FA10(float x, float y, void *motion, int id, int arg);

struct BurstMotion {
    static void play(A31VActor *actor, int id) { func_0882FA10(0.0f, 0.0f, actor->motion, id, 0); }
};

extern "C" {

extern void func_08A3DCA8(void *banner);
extern void *func_08850574(void *effects, int slot, int id, int a, int b, int c, int d, int e, int f);
extern void func_0884F138(void *effect);
extern void func_08A4DF38(BurstStartEffect *self);

void func_08A4DC40(BurstStartEffect *self)
{
    BurstSpawn *table = D_08A4F08C;
    if (self->actor->guard() == 1) {
        func_08A3DCA8(D_08A405E0);
    }
    BurstMotion::play(self->actor, 0x1FA5);
    void *effects = self->actor->effects;
    for (int i = 0; i < 8; i++) {
        BurstSpawn *spawn = &table[i];
        void *effect = func_08850574(effects, spawn->kind == 1 ? 0xF : 0x10, spawn->id, 0, 0, 0, 0, 5, 0);
        if (effect != 0) {
            func_0884F138(effect);
        }
    }
    func_08A4DF38(self);
    self->parts->state = 0;
}

}
