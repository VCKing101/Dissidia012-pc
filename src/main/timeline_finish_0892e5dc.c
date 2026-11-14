#include "main/agent25_timeline.h"
int func_0892E5DC(Timeline *t)
{
    int i;
    if (t->next >= t->count) {
        for (i = 0; i < 5; i++) {
            TimelineKey *k = &t->keys[i];
            k->kind = 0;
            k->a = 0;
            k->b = -1;
            k->c = -1;
            k->d = -1;
            k->e = 0;
            k->time = 0.0f;
        }
        t->count = 0;
        t->next = 0;
        return 0;
    }
    return 1;
}
