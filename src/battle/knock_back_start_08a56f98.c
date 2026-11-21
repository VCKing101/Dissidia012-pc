/* Starts a knock-back: plays the front or back knock motion (keeping the
 * hold flag over the motion change when held), pushes the fighter, enters
 * state 2 and marks the hold. */
typedef struct KnockVec {
    float x, y, z, w;
} KnockVec;

typedef struct KnockHold {
    unsigned char active;
} KnockHold;

typedef struct KnockActor {
    unsigned char pad_000[0x130];
    unsigned char motion[0x58];
    unsigned int flags;
    unsigned char pad_18c[0x21C];
    KnockHold *hold;
} KnockActor;

extern void func_08834934(KnockActor *fighter, int from, int to);
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);
extern void func_08A5BF84(KnockActor *fighter, KnockVec *velocity);
extern void func_08834C10(KnockActor *fighter, int state, int arg);

void func_08A56F98(KnockActor *fighter, KnockVec *velocity, unsigned char front, unsigned char held)
{
    KnockHold *hold = fighter->hold;
    if (held) {
        fighter->flags |= 2;
    }
    if (front) {
        func_08834934(fighter, -1, 0x66);
        func_0882FA10(fighter->motion, 0x2BC, 0.0f, 0.0f, 0);
    } else {
        func_08834934(fighter, -1, 0x66);
        func_0882FA10(fighter->motion, 0x2EE, 0.0f, 0.0f, 0);
    }
    if (held) {
        fighter->flags &= ~2;
    }
    func_08A5BF84(fighter, velocity);
    func_08834C10(fighter, 2, 0);
    if (held) {
        fighter->flags |= 0x10;
        hold->active = 1;
    } else {
        hold->active = 0;
    }
}
