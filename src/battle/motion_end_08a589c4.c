typedef struct MotionTimer {
    float length;
    float time;
    char pad_08[0x4];
    float speed;
} MotionTimer;

typedef struct FighterMotion {
    char pad_00[0x10];
    MotionTimer timer;
} FighterMotion;

typedef struct EndFighter {
    char pad_000[0x188];
    unsigned int input;
    char pad_18C[0x188];
    FighterMotion *motion;
    char pad_318[0x6C];
    unsigned int flags;
} EndFighter;

extern void func_08834934(EndFighter *fighter, int first, int second);
extern void func_08834948(EndFighter *fighter);

/* Clears flag 4 once a playing motion has reached its end; on input bit 0 cancels into motions 0x65/0x66. */
void func_08A589C4(void *self, EndFighter *fighter)
{
    MotionTimer *timer = &fighter->motion->timer;
    if (timer->speed > 0.0f && timer->time <= timer->length) {
        fighter->flags &= ~4;
    }
    if (fighter->input & 1) {
        fighter->flags &= ~0x800;
        func_08834934(fighter, 0x65, 0x66);
        func_08834948(fighter);
    }
}
