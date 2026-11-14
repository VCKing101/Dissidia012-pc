typedef struct LandGuard {
    unsigned char active;
    unsigned char held;
    char pad_02[0x2];
    float time;
    char pad_08[0x8];
    float charge;
    char pad_14[0x3C];
    int count;
} LandGuard;

typedef struct LandFighter {
    char pad_000[0x130];
    char effect[0x250];
    unsigned int state;
    unsigned int flags;
    char pad_388[0x20];
    LandGuard *guard;
} LandFighter;

extern void func_0882FA10(float x, float y, void *effect, int id, int flags);
extern void func_088341A4(LandFighter *fighter);
extern int func_0883739C(LandFighter *fighter);
extern void func_08B308B8(LandFighter *fighter);

/* Lands: sets the landing flags, plays effect 0xD1 and clears the guard state. */
void func_08A59574(void *self, LandFighter *fighter)
{
    LandGuard *guard = fighter->guard;
    float zero;
    fighter->flags |= 0x1000;
    fighter->state |= 2;
    zero = 0.0f;
    func_0882FA10(zero, zero, fighter->effect, 0xD1, 0);
    func_088341A4(fighter);
    if (func_0883739C(fighter) == 1) {
        func_08B308B8(fighter);
    }
    guard->time = zero;
    guard->held = 0;
    guard->count = 0;
    guard->active = 0;
    guard->charge = zero;
    fighter->flags &= ~0x200;
}
