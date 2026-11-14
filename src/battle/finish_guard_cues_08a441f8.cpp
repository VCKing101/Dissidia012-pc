#include "battle/agent31_types.h"

/* Near the end of the finishing motion: cues the guard banners for the
 * current finisher kind while the fighter guards. */
struct CuesCtrl {
    A31VActor *actor;
    unsigned char pad_04[4];
    int kind;
};

extern void *D_08A405E0;
extern "C" void func_08A3DBD0(void *banner, int cue);

struct CueBanner {
    static void show(int cue) { func_08A3DBD0(D_08A405E0, cue); }
};

extern "C" {

extern float func_0882FC20(void *motion, int frame);

void func_08A441F8(CuesCtrl *self)
{
    float end = func_0882FC20(self->actor->motion, -1);
    A31VActor *actor = self->actor;
    if (end - actor->time < 120.0f && actor->guard()) {
        if (self->kind == 0) {
            CueBanner::show(8);
        }
        if (self->kind == 1) {
            CueBanner::show(0xA);
        }
    }
}

}
