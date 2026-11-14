#include "menu/auto_region3.h"

/* Page of an index (twelve per page), 0 past the third page. */
int func_08ACD768(ScreenDB30 *self, int index)
{
    int page;

    page = 0;
    if (index < 0x24) {
        page = index / 0xC;
    }
    return page;
}
