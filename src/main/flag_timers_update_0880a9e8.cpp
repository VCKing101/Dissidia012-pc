/* Counts down the object's cooldown and its four flag timers; a timer that
   runs out clears its flag bit. Timers tied to an actor use the actor's time
   step, the others the global one. */
struct FlagTimer0880A9E8 {
    unsigned int bit;
    float time;
    void *actor;
};

struct FlagSet0880A9E8 {
    unsigned int words[2];

    static void clear(FlagSet0880A9E8 *set, unsigned int bit)
    {
        unsigned int *w = &set->words[bit >> 5];
        *w &= ~(1 << (bit & 0x1F));
    }
};

struct Flags0880A9E8 {
    char pad_00[4];
    FlagSet0880A9E8 set;
    char pad_0C[0x44 - 0xC];
    float cooldown;
    FlagTimer0880A9E8 timers[4];
};

extern "C" {

extern float D_089D5EAC;

float func_08838300(void *actor);

void func_0880A9E8(Flags0880A9E8 *self)
{
    if (self->cooldown > 0.0f) {
        self->cooldown -= D_089D5EAC;
    }
    FlagTimer0880A9E8 *t = self->timers;
    for (int i = 0; i < 4; i++, t++) {
        if (t->bit != 0) {
            if (t->actor != 0) {
                t->time -= func_08838300(t->actor);
            } else {
                t->time -= D_089D5EAC;
            }
            if (t->time <= 0.0f) {
                FlagSet0880A9E8::clear(&self->set, t->bit);
                t->bit = 0;
            }
        }
    }
}

}
