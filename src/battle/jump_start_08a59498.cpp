struct Vec4 {
    float x;
    float y;
    float z;
    float w;
    static void set(Vec4 *v, float x, float y, float z, float w)
    {
        v->x = x;
        v->y = y;
        v->z = z;
        v->w = w;
    }
};

struct JumpGuard {
    char pad_00[0xC];
    float scale;
    char pad_10[0x30];
    float x;
    char pad_44[0x4];
    float z;
};

struct JumpStats {
    char pad_00[0xCC];
    float lift;
    float power;
};

struct JumpFighter {
    char pad_000[0x384];
    unsigned int flags;
    char pad_388[0x14];
    JumpStats *stats;
    char pad_3A0[0x8];
    JumpGuard *guard;
};

extern "C" {

extern void func_08837A38(JumpFighter *fighter);
extern void func_08834934(JumpFighter *fighter, int first, int second);
extern void func_08A5BF84(JumpFighter *fighter, Vec4 *velocity);
extern void func_08834C10(JumpFighter *fighter, int action, int flags);

/* Starts the jump: sets flag 2, motions 0x65/0x66, the launch velocity and action 1. */
void func_08A59498(void *self, JumpFighter *fighter)
{
    JumpGuard *guard = fighter->guard;
    JumpStats *stats = fighter->stats;
    Vec4 velocity;
    func_08837A38(fighter);
    fighter->flags |= 2;
    func_08834934(fighter, 0x65, 0x66);
    Vec4::set(&velocity, guard->x, stats->lift, guard->z, guard->scale * stats->power);
    func_08A5BF84(fighter, &velocity);
    func_08834C10(fighter, 1, 0);
}

}
