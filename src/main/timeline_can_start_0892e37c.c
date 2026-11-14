#include "main/agent25_timeline.h"
extern int func_0892DCE4(Timeline *t);
int func_0892E37C(Timeline *t)
{
    int hidden = func_0892DCE4(t);
    if ((t->flags & 1) != 0) {
        return 0;
    }
    if (hidden == 0) {
        return 0;
    }
    return 1;
}
