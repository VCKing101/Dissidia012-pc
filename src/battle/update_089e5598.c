#include "battle/runtime_helpers.h"

/* Observed state/update helpers; original gameplay ownership is unresolved. */
extern int func_089E57C4(void *);

int func_089E5598(unsigned char *object)
{
    if (!object[0]) return -1;
    if (func_089E56EC((struct BattleDrawingStep *)object)) return -2;
    if (func_089E57C4(object)) return -3;
    if (func_089E5988(object)) return -4;
    return 0;
}
