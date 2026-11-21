/* Dice effect, settle stage: counts the face's frames; at the limit shows
 * the closing face (1 on the last die when it settles, else 9) and restarts
 * the time. */
struct SettleParts {
    unsigned char pad_00[4];
    int current;
    unsigned char pad_08[0x10];
    int state;
    unsigned char pad_1c[0x10];
    int count;
    unsigned char pad_30[4];
    int mode;
    unsigned char pad_38[4];
    float limit;
    float time;
};

struct SettleActor {
    unsigned char pad_000[0x5C4];
    void *dice_display;
};

struct SettleEffect {
    SettleActor *actor;
    unsigned char pad_04[0x10];
    SettleParts *parts;
};

extern float D_089D5EAC;

extern "C" {

extern void func_08A6427C(void *display, int face);

void func_08A44140(SettleEffect *self)
{
    if (self->parts->state == 2) {
        if (!(self->parts->time < self->parts->limit)) {
            int current = self->parts->current;
            if (current + 1 == self->parts->count && (self->parts->mode == 0 || (self->parts->mode == 1 && current == 0))) {
                SettleActor *actor = self->actor;
                func_08A6427C(actor->dice_display, 1);
            } else {
                SettleActor *actor = self->actor;
                func_08A6427C(actor->dice_display, 9);
            }
            self->parts->time = 0.0f;
        } else {
            self->parts->time = self->parts->time + D_089D5EAC;
        }
    }
}

}
