#include "battle/auto_region5.h"

/* Releases the shared D_08B422A8. */
void func_08B41BA4(void)
{
    if (D_08B422A8 != 0) {
        func_088DAB44(D_08B422A8);
        D_08B422A8 = 0;
    }
}
