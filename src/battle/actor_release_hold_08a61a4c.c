/* State callback: while the fighter holds (flag 1 at 0x188), ends the hold
 * (with the standard rate when the hold can end), clears the hold flag and
 * resets the fighter's pose. */
typedef struct HoldActor {
    unsigned char pad_000[0x188];
    unsigned int hold;
    unsigned char pad_18c[0x1F8];
    unsigned int flags;
} HoldActor;

extern float D_089C7C2C;
extern int func_08A5BE58(void *ctx, HoldActor *actor);
extern void func_08A5BBC8(void *ctx, HoldActor *actor, float rate);
extern void func_0883497C(HoldActor *actor, float time);

void func_08A61A4C(void *ctx, HoldActor *actor)
{
    if (actor->hold & 1) {
        if (func_08A5BE58(ctx, actor)) {
            func_08A5BBC8(ctx, actor, D_089C7C2C);
        }
        actor->flags &= ~4;
        func_0883497C(actor, 0.0f);
    }
}
