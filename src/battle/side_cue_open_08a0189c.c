#include "battle/agent28_types.h"

/* Opens a side cue. */
extern void func_08A01844(SideCue *cue);

int func_08A0189C(SideCue *cue, unsigned int side, unsigned int kind, void *source, void *layout,
                  unsigned char shared)
{
    func_08A01844(cue);
    if (side >= 2 || kind >= 2 || source == 0 || layout == 0) {
        return -1;
    }
    cue->side = side;
    cue->kind = kind;
    cue->source = source;
    cue->layout = layout;
    cue->shared = shared;
    cue->open = 1;
    return 0;
}
