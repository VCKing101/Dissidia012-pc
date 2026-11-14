#include "menu/auto_region2.h"

/* Tests bit 0x10000000 of D_089D726C->field_A8. */
unsigned char func_08A41098(void)
{
    return (D_089D726C->field_A8 & 0x10000000) != 0;
}
