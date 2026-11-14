#include "menu/auto_region2.h"

/* Splits a time in seconds into hours, minutes and seconds. */
void func_08A3CD08(Screen40248 *self, int time, int *hours, int *minutes, int *seconds)
{
    *hours = time / 3600;
    *minutes = (time % 3600) / 60;
    *seconds = (time % 3600) % 60;
}
