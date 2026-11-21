/* Fighter state callback, climb jump: plays the climb jump motion, jumps
 * it along its facing at the holder's climb jump speeds, enters state 1 and
 * stops the climb effect. */
struct JumpVec4 {
    float x, y, z, w;
};

struct JumpMove {
    unsigned char pad_00[0x14];
    void *effect;
};

struct JumpHolder {
    unsigned char pad_00[0xF0];
    float rise;
    float drift;
};

struct JumpFighter {
    unsigned char pad_000[0x90];
    JumpVec4 facing;
    unsigned char pad_0a0[0x90];
    unsigned char motion[0x26C];
    JumpHolder *holder;
    unsigned char pad_3a0[8];
    JumpMove *move;
};

extern "C" {

extern void func_08899300(void);
extern JumpVec4 func_08837030(JumpFighter *fighter);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern JumpVec4 *func_089A3868(JumpVec4 *out, const JumpVec4 *in);
extern void func_08A5BF84(JumpFighter *fighter, JumpVec4 *velocity);
extern void func_08834C10(JumpFighter *fighter, int state, int arg);
extern void func_088A6AA4(void *effect);

void func_08A5E248(void *ctx, JumpFighter *fighter)
{
    JumpVec4 velocity;
    JumpMove *move = fighter->move;
    func_08899300();
    func_08837030(fighter);
    JumpHolder *holder = fighter->holder;
    func_0882FA10(6.0f, 0.0f, fighter->motion, 0xC9, 0);
    func_089A3868(&velocity, &fighter->facing);
    velocity.y = holder->rise;
    velocity.w = holder->drift;
    func_08A5BF84(fighter, &velocity);
    func_08834C10(fighter, 1, 0);
    if (move->effect != 0) {
        func_088A6AA4(move->effect);
        move->effect = 0;
    }
}

}
