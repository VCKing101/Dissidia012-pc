/* Fighter state callback, dashing: while dash-held counts the move time;
 * ends the dash (back to the knock motion pair) when the hold is released
 * or the dash speed has dropped under its floor. */
struct DashRunMove {
    float time;
};

struct DashRunFighter {
    unsigned char pad_000[0x130];
    unsigned char motion[0x258];
    unsigned long long mask;
    unsigned char pad_390[0x18];
    DashRunMove *move;
    unsigned char pad_3ac[0x14];
    float speed;

    static bool held(DashRunFighter *fighter) { return (fighter->mask & 0x100000ULL) != 0; }
};

extern float D_08A59348;
extern float D_089C7C30;

extern "C" {

extern float func_08838300(DashRunFighter *fighter);
extern void func_08834934(DashRunFighter *fighter, int from, int to);
extern void func_08834948(DashRunFighter *fighter);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);

void func_08A59154(void *ctx, DashRunFighter *fighter)
{
    DashRunMove *move = fighter->move;
    if (DashRunFighter::held(fighter)) {
        move->time = move->time + func_08838300(fighter);
    }
    if (!DashRunFighter::held(fighter) || fighter->speed < D_08A59348) {
        func_08834934(fighter, 0x65, 0x66);
        func_08834948(fighter);
        func_0882FA10(D_089C7C30, 0.0f, fighter->motion, 0x65, 0);
    }
}

}
