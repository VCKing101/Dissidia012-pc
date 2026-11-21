/* Fighter state callback, burst knock start: plays the knock motion unless
 * the fighter is already knocked or airborne, pushes it with the move's
 * velocity scaled by its strength, turns it along that velocity and enters
 * state 1. */
struct StartVec4 {
    float x, y, z, w;
};

struct StartMove {
    unsigned char pad_00[0x10];
    float vx, vy, vz;
    unsigned char pad_1c[4];
    float strength;
};

struct StartStatus {
    unsigned char pad_0[3];
    unsigned char bits;

    static bool airborne(StartStatus *status) { return (status->bits & 0x10) != 0; }
};

struct StartFighter {
    unsigned char pad_000[0x130];
    unsigned char motion[0x60];
    StartStatus status;
    unsigned char pad_194[0x1F0];
    unsigned int flags;
    unsigned char pad_388[0x20];
    StartMove *move;

    static bool knocked(StartFighter *fighter) { return (fighter->flags & 0x800) != 0; }
    static bool airborne(StartFighter *fighter)
    {
        StartStatus *status = &fighter->status;
        return StartStatus::airborne(status);
    }
};

extern float D_089C7C30;

extern "C" {

extern void func_0882FAC4(float x, float y, void *motion, int id, int flags);
extern StartVec4 *func_089A3A44(StartVec4 *out, const StartVec4 *in, float scale);
extern void func_08835C24(StartFighter *fighter, StartVec4 *velocity);
extern void func_08A5BF84(StartFighter *fighter, StartVec4 *dir);
extern void func_08834C10(StartFighter *fighter, int state, int arg);

void func_08A52DD0(void *ctx, StartFighter *fighter)
{
    if (!StartFighter::knocked(fighter) && !StartFighter::airborne(fighter)) {
        func_0882FAC4(D_089C7C30, 0.0f, fighter->motion, 0x65, 0);
    }
    StartVec4 velocity;
    StartMove *move = fighter->move;
    StartVec4 dir;
    dir.x = move->vx;
    dir.y = move->vy;
    dir.z = move->vz;
    dir.w = 1.0f;
    func_089A3A44(&dir, &dir, move->strength);
    velocity = dir;
    func_08835C24(fighter, &velocity);
    func_08A5BF84(fighter, &dir);
    func_08834C10(fighter, 1, 0);
}

}
