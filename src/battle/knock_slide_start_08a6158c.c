/* Starts a knock-back along a given velocity: plays the front or back
 * knock motion, sets the slide track to the velocity with its climb rate
 * (length over w), clears the slide stage, enters state 3 and records the
 * hold. */
typedef struct SlideKnockVec {
    float x, y, z, w;
} SlideKnockVec;

typedef struct SlideKnockHold {
    unsigned char pad_00[0x20];
    unsigned char held;
} SlideKnockHold;

typedef struct SlideKnockHolder {
    float x, y, z, w;
    SlideKnockVec rate;
    unsigned char pad_20[0xD4];
    int stage;
} SlideKnockHolder;

typedef struct SlideKnockActor {
    unsigned char pad_000[0x130];
    unsigned char motion[0x1E4];
    SlideKnockHolder *holder;
    unsigned char pad_318[0x90];
    SlideKnockHold *hold;
} SlideKnockActor;

extern void func_08834934(SlideKnockActor *fighter, int from, int to);
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);
extern float func_089A3BF8(SlideKnockVec *v);
extern void func_08834C10(SlideKnockActor *fighter, int state, int arg);

void func_08A6158C(SlideKnockActor *fighter, SlideKnockVec *velocity, unsigned char front, unsigned char held)
{
    SlideKnockHold *hold = fighter->hold;
    SlideKnockVec *rate = &fighter->holder->rate;
    SlideKnockVec measured;
    float length;
    if (front) {
        func_08834934(fighter, -1, 0x66);
        func_0882FA10(fighter->motion, 0x2BC, 0.0f, 0.0f, 0);
    } else {
        func_08834934(fighter, -1, 0x66);
        func_0882FA10(fighter->motion, 0x2EE, 0.0f, 0.0f, 0);
    }
    measured = *velocity;
    length = func_089A3BF8(&measured);
    {
        float climb = velocity->w != 0.0f ? length / velocity->w : 0.0f;
        rate->x = 0.0f;
        rate->z = 0.0f;
        rate->w = 0.0f;
        rate->y = climb;
    }
    fighter->holder->x = velocity->x;
    fighter->holder->y = velocity->y;
    fighter->holder->z = velocity->z;
    fighter->holder->w = velocity->w;
    fighter->holder->stage = 0;
    func_08834C10(fighter, 3, 0);
    hold->held = held;
}
