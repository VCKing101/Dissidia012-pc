#include "menu/auto_region3.h"

/* Position of an index on its page (twelve per page). */
int func_08ACD8C4(ScreenDB30 *self, int index)
{
    return index % 0xC;
}
