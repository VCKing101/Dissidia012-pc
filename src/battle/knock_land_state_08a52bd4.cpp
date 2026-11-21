/* Fighter state callback, landing from a burst knock: when the fighter is
 * held, plays the knocked-down motion and pushes it along its facing scaled
 * by the holder's knock speed, then enters state 0x12; otherwise plays the
 * landing for the fall motions and enters state 0. */
struct KnockVec4 {
    float x, y, z, w;
};

extern "C" KnockVec4 *func_089A3A44(KnockVec4 *out, const KnockVec4 *in, float scale);


struct KnockHolder {
    unsigned char pad_000[0x104];
    KnockVec4 speed;

    static float knock_speed(KnockHolder *holder)
    {
        KnockVec4 *speed = &holder->speed;
        return speed->y;
    }
};

struct KnockFighter {
    unsigned char pad_000[0x90];
    KnockVec4 facing;
    unsigned char pad_0a0[0x90];
    unsigned char motion[0x50];
    short motion_id;
    unsigned char pad_182[0x21A];
    KnockHolder *holder;
};

extern float D_089C7C30;
extern float D_089C7C2C;

extern "C" {

extern int func_088374B0(KnockFighter *fighter);
extern void func_08834934(KnockFighter *fighter, int from, int to);
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);
extern void func_0882FAC4(void *motion, int id, float x, float y, int flags);
extern void func_08835C24(KnockFighter *fighter, KnockVec4 *velocity);
extern void func_08834C10(KnockFighter *fighter, int state, int arg);

void func_08A52BD4(void *ctx, KnockFighter *fighter, unsigned char forced)
{
    if (func_088374B0(fighter) | forced) {
        KnockHolder *holder = fighter->holder;
        func_08834934(fighter, 0x65, 0x66);
        func_0882FA10(fighter->motion, 0x65, D_089C7C30, 0.0f, 0);
        KnockVec4 velocity;
        KnockVec4 scaled;
        func_089A3A44(&scaled, &fighter->facing, KnockHolder::knock_speed(holder));
        velocity = scaled;
        func_08835C24(fighter, &velocity);
        func_08834C10(fighter, 0x12, 0);
    } else {
        short id = fighter->motion_id;
        if (id == 0xF0 || id == 0xF2 || id == 0xF4 || id == 0xF3 || id == 0x65) {
            func_0882FAC4(fighter->motion, 1, D_089C7C2C, 0.0f, 0);
        }
        func_08834C10(fighter, 0, 0);
    }
}

}
