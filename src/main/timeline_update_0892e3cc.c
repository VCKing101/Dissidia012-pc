#include "main/agent25_timeline.h"
extern void func_0892DE18(Timeline *t);
extern void func_0892E488(Timeline *t, int a, int b, int c, int d, int e, int kind);
extern int func_0892E5DC(Timeline *t);
void func_0892E3CC(Timeline *t)
{
    unsigned int flags;
    func_0892DE18(t);
    flags = t->flags;
    if ((flags & 1) != 0) {
        if (t->keys[t->current].time <= t->time) {
            t->flags = flags & ~1;
            func_0892E488(t, t->keys[t->current].a, t->keys[t->current].b, t->keys[t->current].c,
                          t->keys[t->current].d, t->keys[t->current].e, t->keys[t->current].kind);
            func_0892E5DC(t);
        }
        t->time += 1.0f;
    }
}
