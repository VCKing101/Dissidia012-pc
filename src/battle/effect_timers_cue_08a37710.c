/* Shows a dice face (9 for the first side, 1 for the second) for every
 * running, unfinished timer of the hit effect whose scaled time is past its
 * limit. */
typedef struct CueTimer {
    unsigned int flags;
    float time;
} CueTimer;

typedef struct CueTimerParts {
    unsigned char pad_00[0x14];
    int count;
    unsigned char pad_18[4];
    float limits[12];
    CueTimer timers[12];
} CueTimerParts;

typedef struct CueTimerActor {
    unsigned char pad_000[0x5C4];
    void *dice_display;
} CueTimerActor;

typedef struct CueTimerEffect {
    CueTimerActor *actor;
    unsigned char pad_04[4];
    int second;
    unsigned char pad_0c[8];
    CueTimerParts *parts;
} CueTimerEffect;

extern float D_08A38480;
extern void func_08A6427C(void *display, int face);

void func_08A37710(CueTimerEffect *self)
{
    int i;
    for (i = 0; i < self->parts->count; i++) {
        CueTimer *timer = &self->parts->timers[i];
        if ((timer->flags & 1) && !(timer->flags & 2)) {
            if (!(timer->time / D_08A38480 <= self->parts->limits[i])) {
                func_08A6427C(self->actor->dice_display, self->second == 0 ? 9 : 1);
            }
        }
    }
}
