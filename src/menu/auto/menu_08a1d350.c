#include "menu/auto_region1.h"

/* Bit 1 of D_089D8140's field_1C. */
unsigned int func_08A1D350(void)
{
    return D_089D8140->field_1C & 1;
}
