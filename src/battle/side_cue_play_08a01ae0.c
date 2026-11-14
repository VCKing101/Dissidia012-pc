#include "battle/agent28_types.h"

/* Plays a motion on the side cue's mark, loading the cue models on first
 * use. */
typedef struct CueMark {
    unsigned char pad_00[0xE];
    short playing;
} CueMark;

extern void *func_088DAA40(void *source, int name, int flags);
extern void *func_088DAA68(void *source);
extern void func_088DA060(void *model);
extern void func_088D9F5C(void *model, int motion, int loop);
extern int func_08A01948(SideCue *cue);

int func_08A01AE0(SideCue *cue, int motion, int playing)
{
    if (!cue->open) {
        return -1;
    }
    if (cue->model == 0) {
        cue->model = func_088DAA40(cue->source, 0x15, 0);
        if (cue->model == 0) {
            return -2;
        }
    }
    if (cue->mark == 0) {
        cue->mark = func_088DAA68(cue->layout);
        if (cue->mark == 0) {
            if (cue->model != 0) {
                func_088DA060(cue->model);
                cue->model = 0;
            }
            return -3;
        }
    }
    func_088D9F5C(cue->mark, motion, 1);
    ((CueMark *)cue->mark)->playing = playing;
    cue->closing = 0;
    func_08A01948(cue);
    return 0;
}
