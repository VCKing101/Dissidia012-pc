#include "menu/auto_region2.h"

/* System dialog done: clears D_089D7E88 field_184D and restarts with 1. */
void func_08A4D650(void)
{
    D_089D7E88->field_184D = 0;
    func_08A4D5A8(1);
}
