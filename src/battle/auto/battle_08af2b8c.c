#include "battle/auto_region4.h"

/* Frees the object at D_08AF2BD0, if any, and clears D_08AF2BD0 and
 * D_08AF2BD4. */
void func_08AF2B8C(void)
{
    if (D_08AF2BD0 != 0) {
        func_08890970(D_08AF2BD0);
        D_08AF2BD0 = 0;
        D_08AF2BD4 = 0;
    }
}
