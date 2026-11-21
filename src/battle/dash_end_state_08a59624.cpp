/* Fighter state callback, dash end: stops the dash trail, clears the dash
 * flags, stops the fighter and ends a pending boost while held. */
template <class T> struct DashEndKept {
    T value;
    DashEndKept(T v) : value(v) {}
};

struct DashEndVec4 {
    float x, y, z, w;
};

struct DashEndMove {
    unsigned char pad_00[0x50];
    void *trail;
};

struct DashEndFighter {
    unsigned char pad_000[0x380];
    unsigned int state_flags;
    unsigned int flags;
    unsigned char pad_388[0x20];
    DashEndMove *move;
};

extern DashEndVec4 D_09AF84E0;

extern "C" {

extern void func_088A6A78(void *effect);
extern void func_08835BDC(DashEndFighter *fighter, DashEndVec4 *velocity);
extern int func_0883739C(DashEndFighter *fighter);
extern void func_08B333F0(DashEndFighter *fighter);

void func_08A59624(void *ctx, DashEndFighter *fighter)
{
    DashEndVec4 stop;
    DashEndMove *move = fighter->move;
    void *trail = move->trail;
    if (trail != 0) {
        DashEndKept<DashEndMove *> saved(move);
        func_088A6A78(trail);
        move = saved.value;
    }
    move->trail = 0;
    fighter->flags &= ~0x1000;
    fighter->state_flags &= ~2;
    stop = D_09AF84E0;
    func_08835BDC(fighter, &stop);
    if (func_0883739C(fighter) != 0) {
        func_08B333F0(fighter);
    }
}

}
