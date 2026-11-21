/* Fighter state callback, knock-back hit: puts the fighter in the knocked
 * motion pair, plays the knock motion unless it already is, pushes it with
 * the move's velocity scaled by its strength and enters state 0x12. */
struct HitVec4 {
    float x, y, z, w;
};

struct HitMove {
    unsigned char pad_00[0x10];
    float vx, vy, vz;
    unsigned char pad_1c[4];
    float strength;
};

struct HitFighter {
    unsigned char pad_000[0x130];
    unsigned char motion[0x50];
    short motion_id;
    unsigned char pad_182[0x226];
    HitMove *move;
};

extern "C" {

extern void func_08834934(HitFighter *fighter, int from, int to);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern HitVec4 *func_089A3A44(HitVec4 *out, const HitVec4 *in, float scale);
extern void func_08835C24(HitFighter *fighter, HitVec4 *velocity);
extern void func_08834C10(HitFighter *fighter, int state, int arg);

void func_08A551AC(void *ctx, HitFighter *fighter)
{
    HitMove *move = fighter->move;
    int knocked = 0x65;
    func_08834934(fighter, 0x65, 0x66);
    if (fighter->motion_id != knocked) {
        func_0882FA10(6.0f, 0.0f, fighter->motion, 0x65, 0);
    }
    HitVec4 velocity;
    HitVec4 dir;
    dir.x = move->vx;
    dir.y = move->vy;
    dir.z = move->vz;
    dir.w = 1.0f;
    func_089A3A44(&dir, &dir, move->strength);
    velocity = dir;
    func_08835C24(fighter, &velocity);
    func_08834C10(fighter, 0x12, 0);
}

}
