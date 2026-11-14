#include "menu/agent29_types.h"

/* Screen0B6A0: whether the pull's models have finished (kinds 0 and 3
   need nothing; kind 2 also waits for the second model); when they have,
   moves on with func_089FC05C. */
extern "C" void func_089FC05C(Screen0B6A0 *self);

struct PullModels29 {
    static bool Done(MenuModel *model) { return (model->field_20 & 0x20) != 0; }
};

extern "C" int func_089FBF94(Screen0B6A0 *self)
{
    Work0B6A0 *work = self->work;
    int kind = work->field_38A0;
    int ready = 0;

    if (kind == 0) {
        ready = 1;
    } else if (kind == 3) {
        ready = 1;
    } else {
        if (PullModels29::Done(work->handles2[0]) && PullModels29::Done(work->handles2[7])) {
            if (kind == 2) {
                if (PullModels29::Done(work->handles2[1])) {
                    ready = 1;
                }
            } else {
                ready = 1;
            }
        }
        if (ready) {
            func_089FC05C(self);
        }
    }
    return ready;
}
