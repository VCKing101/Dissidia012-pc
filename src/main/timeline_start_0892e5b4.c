#include "main/agent25_timeline.h"
void func_0892E5B4(Timeline *t)
{
    t->flags |= 1;
    t->time = 0.0f;
    t->current = t->next;
    t->next++;
}
