/* Fighter state callback, launch: marks the fighter launched, plays the
 * launch motion and lifts it at the holder's launch speed, scaled up while
 * boosted (bits 0x40, 0x80 of its status) and clearing the pending boost,
 * then enters the airborne state. */
struct LaunchVec4 {
    float x, y, z, w;
};

struct LaunchHolder {
    unsigned char pad_00[0x68];
    float launch_speed;
};

struct LaunchBits {
    unsigned char b0;
    unsigned char pad_1[2];
    unsigned char b3;
};

struct LaunchFighter {
    unsigned char pad_000[0x130];
    unsigned char motion[0x254];
    unsigned int flags;
    unsigned char pad_388[0x14];
    LaunchHolder *holder;
    unsigned char pad_3a0[0x210];
    LaunchBits bits;

    static bool flag(int bits, int mask) { return (bits & mask) != 0; }
};

extern float D_08A54D8C;
extern float D_08A54D90;

extern "C" {

extern void func_08834934(LaunchFighter *fighter, int from, int to);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern int func_0883739C(LaunchFighter *fighter);
extern int func_08B3459C(LaunchFighter *fighter);
extern void func_08B333F0(LaunchFighter *fighter);
extern void func_08A5BF84(LaunchFighter *fighter, LaunchVec4 *velocity);
extern void func_08A5B40C(void *ctx, LaunchFighter *fighter);

void func_08A54934(void *ctx, LaunchFighter *fighter)
{
    fighter->flags |= 4;
    func_08834934(fighter, 0x65, 0x66);
    void *motion = fighter->motion;
    int id = 0x1B8;
    func_0882FA10(0.0f, 0.0f, motion, id, 0);
    float speed = fighter->holder->launch_speed;
    if (func_0883739C(fighter) != 0) {
        LaunchFighter *boosted = fighter;
        LaunchBits *bits = &boosted->bits;
        unsigned char status = bits->b0;
        if (LaunchFighter::flag(status, 0x40)) {
            speed = D_08A54D8C * speed;
        }
        if (LaunchFighter::flag(status, 0x80)) {
            speed = D_08A54D90 * speed;
        }
        if (func_08B3459C(boosted) != 0) {
            func_08B333F0(boosted);
        }
        bits->b3 &= ~4;
    }
    LaunchVec4 velocity;
    velocity.x = 0.0f;
    velocity.y = speed;
    velocity.z = 0.0f;
    velocity.w = 0.0f;
    func_08A5BF84(fighter, &velocity);
    func_08A5B40C(ctx, fighter);
}

}
