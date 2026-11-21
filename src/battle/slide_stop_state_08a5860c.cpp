/* Fighter state callback, slide stop: stops the slide, plays the stop
 * motion and its layer, and while held keeps any remaining push. */
struct StopVec4 {
    float x, y, z, w;
};

struct StopMove {
    float speed;
};

struct StopFighter {
    unsigned char pad_000[0x130];
    unsigned char motion[0x278];
    StopMove *move;
    unsigned char pad_3ac[0x1A4];
    StopVec4 push;
};

extern float D_089C7C2C;

extern "C" {

extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void func_0882FB90(float x, float y, void *motion, int id);
extern int func_0883739C(StopFighter *fighter);
extern float func_089A3BF8(StopVec4 *v);
extern void func_08835BA4(StopFighter *fighter, StopVec4 *v);

void func_08A5860C(void *ctx, StopFighter *fighter)
{
    fighter->move->speed = 0.0f;
    func_0882FA10(D_089C7C2C, 0.0f, fighter->motion, 0x1CC, 0);
    func_0882FB90(D_089C7C2C, 0.0f, fighter->motion, 1);
    if (func_0883739C(fighter) != 0) {
        StopVec4 *push = &fighter->push;
        StopVec4 measured = *push;
        if (func_089A3BF8(&measured) > 0.0f) {
            StopVec4 kept = *push;
            func_08835BA4(fighter, &kept);
        }
    }
}

}
