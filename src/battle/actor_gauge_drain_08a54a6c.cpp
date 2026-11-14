/* State callback: while the fighter's gauge is below its cap and the drain
 * flag is set, takes the drain amount from the reserve; then ends the hold
 * when holding, or drops to state 0x12 once the gauge is used up and the
 * fighter may fall. */
struct DrainGauge {
    float value;
    float cap;
    float reserve;
};

struct DrainStats {
    unsigned char pad_00[0x10];
    DrainGauge gauge;
};

struct DrainActor {
    unsigned char pad_000[0x188];
    unsigned int hold;
    unsigned char pad_18c[0x188];
    DrainStats *stats;
    unsigned char pad_318[0x6C];
    unsigned int flags;

    static bool draining(DrainActor *actor) { return (actor->flags & 0x10) != 0; }
};

extern "C" {

extern float func_08A5B724(void *ctx, DrainActor *actor);
extern void func_08834A40(DrainActor *actor);
extern int func_088377C0(DrainActor *actor);
extern void func_08834C10(DrainActor *actor, int state, int arg);

void func_08A54A6C(void *ctx, DrainActor *actor)
{
    DrainGauge *gauge = &actor->stats->gauge;
    if (gauge->value < gauge->cap && DrainActor::draining(actor)) {
        gauge->reserve -= func_08A5B724(ctx, actor);
    }
    if (actor->hold & 1) {
        func_08834A40(actor);
        return;
    }
    if (gauge->value > 0.0f && func_088377C0(actor)) {
        func_08834C10(actor, 0x12, 0);
    }
}

}
