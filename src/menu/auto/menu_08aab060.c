#include "menu/auto_region3.h"

/* ScreenAB7CFC: func_08AA9F78 with 0 for the five records, -1 past them. */
void func_08AAB060(ScreenAB7CFC *self, int index)
{
    if (index < 5) {
        func_08AA9F78(self, index, 0);
    }
    else {
        func_08AA9F78(self, index, -1);
    }
}
