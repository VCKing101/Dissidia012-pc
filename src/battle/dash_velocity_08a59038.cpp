/* Dash velocity: pushes the fighter along its flat facing at the dash
 * speed, then returns the facing with the dash height, scaled by the
 * frame step. */
struct DashVec4 {
    float x, y, z, w;
};

struct DashVelFighter {
    unsigned char pad_000[0xC0];
    DashVec4 facing;
};

extern float D_08A59344;

extern "C" {

extern DashVec4 *func_089A3868(DashVec4 *out, const DashVec4 *in);
extern DashVec4 *func_089A3A44(DashVec4 *out, const DashVec4 *in, float scale);
extern void func_08835C24(DashVelFighter *fighter, DashVec4 *velocity);
extern float func_08A59244(void *ctx, DashVelFighter *fighter);
extern float func_08838300(DashVelFighter *fighter);

DashVec4 func_08A59038(void *ctx, DashVelFighter *fighter)
{
    DashVec4 velocity;
    DashVec4 dir;
    func_089A3868(&dir, &fighter->facing);
    func_089A3A44(&dir, &dir, D_08A59344);
    dir.w = 0.0f;
    velocity = dir;
    func_08835C24(fighter, &velocity);
    dir.y = func_08A59244(ctx, fighter);
    func_089A3A44(&dir, &dir, func_08838300(fighter));
    return dir;
}

}
