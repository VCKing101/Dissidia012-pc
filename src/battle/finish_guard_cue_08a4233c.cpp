#include "battle/agent31_types.h"

/* Near the end of the finishing motion: cues the guard banner (2 in the
 * fighter's second form, 3 otherwise) while the fighter guards. */
struct FormCueCtrl {
    A31VActor *actor;
};

extern void *D_08A405E0;
extern "C" void func_08A3DBD0(void *banner, int cue);

struct FormBanner {
    static void show(int cue) { func_08A3DBD0(D_08A405E0, cue); }
};

extern "C" {

extern float func_0882FC20(void *motion, int frame);

void func_08A4233C(FormCueCtrl *self)
{
    float end = func_0882FC20(self->actor->motion, -1);
    A31VActor *actor = self->actor;
    if (end - actor->time < 60.0f && actor->guard()) {
        FormBanner::show(self->actor->form == 1 ? 2 : 3);
    }
}

}
