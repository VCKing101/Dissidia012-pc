/* Hold-check velocity for a held fighter: while the hold runs, counts its
 * time (a longer limit against two holder kinds) and releases the hold at
 * the limit, returning no push; otherwise the normal velocity. */
struct HoldVec4 {
    float x, y, z, w;
};

struct HoldMove {
    unsigned char active;
};

struct HoldTimer {
    unsigned char pad_00[0x10];
    float time;
};

struct HoldKind {
    unsigned char kind;
};

struct HoldTarget {
    unsigned char pad_000[0x5A4];
    HoldKind *kind;
};

struct HoldFighter {
    unsigned char pad_000[0x188];
    unsigned int flags;
    unsigned char pad_18c[0x178];
    HoldTarget *holder;
    unsigned char pad_308[0xC];
    HoldTimer *timer;
    unsigned char pad_318[0x90];
    HoldMove *move;
};

extern float D_08A5712C;
extern float D_08A57130;
extern float D_08A57134;
extern HoldVec4 D_09AF84C0;

extern "C" {

extern float func_08838300(HoldFighter *fighter);
extern int func_0883739C(HoldTarget *target);
extern HoldVec4 func_08A5B59C(void *ctx, HoldFighter *fighter);

HoldVec4 func_08A56E10(void *ctx, HoldFighter *fighter)
{
    HoldMove *move = fighter->move;
    if (move->active != 0) {
        float *time = &fighter->timer->time;
        *time = *time + func_08838300(fighter);
        float limit = D_08A5712C;
        if (fighter->holder != 0 && func_0883739C(fighter->holder) != 0) {
            unsigned char kind = fighter->holder->kind->kind;
            if (kind == 0x10) {
                limit = D_08A57130;
            } else if (kind == 0x21) {
                limit = D_08A57134;
            }
        }
        if (limit <= *time) {
            fighter->flags &= ~0x10;
            move->active = 0;
            *time = 0.0f;
        } else {
            fighter->flags |= 0x10;
        }
        return D_09AF84C0;
    }
    return func_08A5B59C(ctx, fighter);
}

}
