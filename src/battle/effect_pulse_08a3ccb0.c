/* Hit effect pulses: in stage 0, each of the two pulses fires (shown on
 * the dice display as 1 for the first, 5 for the second) every period of
 * the effect time. */
typedef struct PulseParts {
    int stage;
    unsigned char pad_04[0x18];
    float time;
    unsigned char pad_20[0x34];
    int counts[2];
    float periods[2];
} PulseParts;

typedef struct PulseActor {
    unsigned char pad_000[0x5C4];
    void *dice_display;
} PulseActor;

typedef struct PulseEffect {
    PulseActor *actor;
    unsigned char pad_04[0x10];
    PulseParts *parts;
} PulseEffect;

extern void func_08A6427C(void *display, int face);

void func_08A3CCB0(PulseEffect *self)
{
    int i;
    if (self->parts->stage != 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        if (self->parts->counts[i] < (int)(self->parts->time / self->parts->periods[i])) {
            if (i == 0) {
                PulseActor *actor = self->actor;
                func_08A6427C(actor->dice_display, 1);
            } else {
                PulseActor *actor = self->actor;
                func_08A6427C(actor->dice_display, 5);
            }
            self->parts->counts[i]++;
        }
    }
}
