#include "main/auto_region4.h"

/* Releases the entry D_089D7EC8 (if any) from D_089D6C88. */
void func_088EB370(void)
{
    if (D_089D7EC8 != -1) {
        func_088A1638(D_089D6C88, D_089D7EC8);
        D_089D7EC8 = -1;
        func_088A015C();
    }
}
