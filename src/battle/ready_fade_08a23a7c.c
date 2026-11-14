#include "battle/agent31_types.h"

typedef struct A31Options {
    unsigned char pad_000[0x278];
    int skip;
} A31Options;

extern A31Options *D_08A405E0;
extern A31FrameClock *D_089D69E0;
extern float D_08A24BB0;

int func_08A23A7C(ReadyScreen *self)
{
    if ((unsigned char)(D_08A405E0->skip != 0) == 1) {
        self->fade = D_08A24BB0;
    } else if (self->fade > 0.0f) {
        self->fade -= D_089D69E0->frame_scale;
        if (self->fade < 0.0f) {
            self->fade = 0.0f;
        }
    }
    return 0;
}
