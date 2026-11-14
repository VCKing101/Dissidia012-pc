/* Turn phase update: follows the rule step for this turn, and keeps the
 * waiting effect alive until the phase reports done. */
struct TurnStep {
    unsigned char pad_00[8];
    float duration;
    unsigned char pad_0c[8];
    int cost;
    unsigned char pad_18[0xC];
    unsigned char spend;
    unsigned char pad_25[3];
    float progress;
};

struct TurnEffect {
    unsigned char pad_00[0x34];
    unsigned int flags;

    static bool is_ending(TurnEffect *effect)
    {
        return (effect->flags & 2) != 0;
    }
};

extern "C" void func_088A1E44(TurnEffect *effect, float fade);

struct TurnPhase {
    void *vtable;
    unsigned char pad_04[4];
    int state;
    unsigned char pad_0c[8];
    TurnEffect *effect;
    unsigned char pad_18[4];
    unsigned char cue[0x28];
    unsigned char clock[0x30];
    unsigned char gauge[0x34];
    unsigned char bar[0x1C];
    int wins;
    int losses;
    void *rules;

    static void release(TurnEffect *&slot)
    {
        if (slot != 0) {
            func_088A1E44(slot, 0.0f);
            slot = 0;
        }
    }
};

extern "C" {

extern int func_08A22E2C(TurnStep *step, int wins, int losses, int kind);
extern void func_089FE1BC(void *gauge, int amount);
extern float func_08891348(void *rules);
extern void func_08A16CC4(void *clock, float rate);
extern void func_089FCA0C(void *bar, float progress);
extern int func_08A2836C(TurnPhase *phase);
extern void func_088A1E44(TurnEffect *effect, float fade);
extern TurnEffect *func_088A24D4(int owner, int id, int loop, int flags);
extern void func_08A01C74(void *cue, int mode);
extern void func_08A16C58(void *clock);
extern void func_089FC9A0(void *bar);

int func_08A281B0(TurnPhase *phase)
{
    TurnStep step;
    if (func_08A22E2C(&step, phase->wins, phase->losses, 0x1A) == 0) {
        if (step.spend == 1) {
            func_089FE1BC(phase->gauge, -step.cost);
        }
        float length = func_08891348(phase->rules);
        if (length != 0.0f) {
            func_08A16CC4(phase->clock, step.duration / length);
        }
        func_089FCA0C(phase->bar, step.progress);
        int done = func_08A2836C(phase);
        TurnEffect *effect = phase->effect;
        if (done == 1) {
            TurnPhase::release(phase->effect);
        } else {
            if (effect != 0 && TurnEffect::is_ending(effect) == 1) {
                TurnPhase::release(phase->effect);
                effect = phase->effect;
            }
            if (effect == 0) {
                phase->effect = func_088A24D4(0, 0x637CC2, 1, 0);
            }
        }
    } else {
        func_08A01C74(phase->cue, 6);
        func_08A16C58(phase->clock);
        func_089FC9A0(phase->bar);
        TurnEffect *effect = phase->effect;
        if (effect != 0) {
            func_088A1E44(effect, 0.0f);
            phase->effect = 0;
        }
        phase->state = 3;
    }
    return 0;
}

}
