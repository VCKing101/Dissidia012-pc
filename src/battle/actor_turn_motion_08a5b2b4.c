/* Plays the turn motion (the air turn 0x1D7 while airborne, else the
 * ground turn 0x1D6) with its blend in, then enters state 0x15. */
typedef struct TurnActor {
    unsigned char pad_000[0x130];
    unsigned char motion[4];
} TurnActor;

extern float D_089C7C2C;
extern int func_088374B0(TurnActor *actor);
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);
extern void func_0882FB90(void *motion, int id, float x, float y);
extern void func_08834C10(TurnActor *actor, int state, int arg);

void func_08A5B2B4(TurnActor *actor)
{
    int airborne = func_088374B0(actor);
    void *motion = actor->motion;
    float zero = 0.0f;
    if (airborne != 0) {
        func_0882FA10(motion, 0x1D7, zero, zero, 0);
        func_0882FB90(motion, 0x65, D_089C7C2C, zero);
    } else {
        func_0882FA10(motion, 0x1D6, zero, zero, 0);
        func_0882FB90(motion, 1, D_089C7C2C, zero);
    }
    func_08834C10(actor, 0x15, 0);
}
