#include "battle/agent28_types.h"

/* Loads the score tag's models for the owner's side and attaches the tag. */
extern int D_089FCB20[2];
extern unsigned int func_08A22CA8(int owner);
extern void *func_088DAA40(void *source, int name, int flags);
extern void func_088D9F00(void *model, float frame);
extern void func_088DA060(void *model);
extern int func_089FC7E0(ScoreTag *tag);

int func_089FC89C(ScoreTag *tag)
{
    unsigned int side;

    if (!tag->open) {
        return -1;
    }
    side = func_08A22CA8(tag->owner);
    if (side >= 2) {
        return -2;
    }
    if (tag->model == 0) {
        tag->model = func_088DAA40(tag->source, D_089FCB20[side], 0);
        if (tag->model == 0) {
            return -3;
        }
        func_088D9F00(tag->model, 0.0f);
    }
    if (tag->tag == 0) {
        tag->tag = func_088DAA40(tag->source, 0, 0);
        if (tag->tag == 0) {
            if (tag->model != 0) {
                func_088DA060(tag->model);
                tag->model = 0;
            }
            return -4;
        }
    }
    tag->fade = 0.0f;
    func_089FC7E0(tag);
    return 0;
}
