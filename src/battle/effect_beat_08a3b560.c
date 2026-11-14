/* Hit effect beats: while running, fires a beat (dice display 1) each
 * period of the effect time (a fixed period once past the limit), then
 * advances the time. */
typedef struct BeatParts {
    unsigned char running;
    unsigned char pad_01[0xF];
    int beats;
    float period;
    float limit;
    float time;
} BeatParts;

typedef struct BeatActor {
    unsigned char pad_000[0x5C4];
    void *dice_display;
} BeatActor;

typedef struct BeatEffect {
    BeatActor *actor;
    unsigned char pad_04[0x10];
    BeatParts *parts;
} BeatEffect;

extern float D_089D5EAC;
extern void func_08A6427C(void *display, int face);

void func_08A3B560(BeatEffect *self)
{
    BeatParts *parts = self->parts;
    int due;
    if (parts->running != 1) {
        return;
    }
    if (parts->limit >= 0.0f && parts->time > parts->limit) {
        due = (int)(parts->time / 10.4347830f);
    } else {
        due = (int)(parts->time / parts->period);
    }
    if (parts->beats < due) {
        func_08A6427C(self->actor->dice_display, 1);
        self->parts->beats++;
        parts = self->parts;
    }
    parts->time += D_089D5EAC;
}
