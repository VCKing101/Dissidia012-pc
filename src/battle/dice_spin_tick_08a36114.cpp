/* Dice effect, spinning stage: while the current die is spinning its time
 * runs; at its limit it shows its face (mode 0) or a random face 1 to 3
 * steps on (wrapping at 4), and the time restarts. Mode 2 never stops. */
struct SpinParts {
    unsigned char pad_00[8];
    int current;
    unsigned char pad_0c[8];
    int faces[3];
    unsigned char pad_20[0xC];
    int state;
    unsigned char pad_30[0x78];
    int modes[3];
    unsigned char pad_b4[8];
    float limits[3];
    unsigned char pad_c8[8];
    float time;
};

struct SpinActor {
    unsigned char pad_000[0x5C4];
    void *dice_display;
};

struct SpinEffect {
    SpinActor *actor;
    unsigned char pad_04[0x10];
    SpinParts *parts;
};

struct SpinClock {
    unsigned char pad_00[0x18];
    float frame_scale;
};

extern SpinClock *D_089D69E0;
extern int D_09AF8B00;

extern "C" void func_08A6427C(void *display, int face);

struct SpinRand {
    static int next()
    {
        return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    }
};

extern "C" {

void func_08A36114(SpinEffect *self)
{
    if (self->parts->state == 1) {
        int mode = self->parts->modes[self->parts->current];
        if (mode != 2 && !(self->parts->time < self->parts->limits[self->parts->current])) {
            if (mode == 0) {
                SpinActor *actor = self->actor;
                func_08A6427C(actor->dice_display, self->parts->faces[self->parts->current]);
            } else {
                SpinActor *actor = self->actor;
                func_08A6427C(actor->dice_display, (SpinRand::next() % 3 + self->parts->faces[self->parts->current] + 1) % 4);
            }
            self->parts->time = 0.0f;
        } else {
            self->parts->time = self->parts->time + D_089D69E0->frame_scale;
        }
    } else {
        self->parts->time = 0.0f;
    }
}

}
