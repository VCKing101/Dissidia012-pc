#include "main/auto_region1.h"

/* Frees the buffer D_089D4790 and clears it and the three words after it. */
void func_0881160C(void)
{
    if (D_089D4790 != 0) {
        func_08882158((void *)D_089D4790);
        D_089D4790 = 0;
        D_089D4794 = 0;
        D_089D4798 = 0;
        D_089D479C = 0;
    }
}
