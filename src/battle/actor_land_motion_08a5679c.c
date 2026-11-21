/* State callback: plays the landing motion (0x190) and the blend in, then
 * hands the fighter to the ground state. */
typedef struct LandActor {
    unsigned char pad_000[0x130];
    unsigned char motion[4];
} LandActor;

extern float D_089C7C2C;
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);
extern void func_0882FB90(void *motion, int id, float x, float y);
extern void func_08837A5C(LandActor *actor);

void func_08A5679C(int unused, LandActor *actor)
{
    float zero = 0.0f;
    void *motion = actor->motion;
    func_0882FA10(motion, 0x190, zero, zero, 0);
    func_0882FB90(motion, 1, D_089C7C2C, zero);
    func_08837A5C(actor);
}
