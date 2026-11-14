#include "menu/auto_region2.h"

/* Splits a time in hundredths into minutes, seconds and hundredths. */
void func_08A3CD40(Screen40248 *self, int time, int *minutes, int *seconds, int *hundredths)
{
    *minutes = time / 6000;
    *seconds = (time % 6000) / 100;
    *hundredths = (time % 6000) % 100;
}
