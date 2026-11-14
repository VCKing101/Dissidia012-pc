#include "main/auto_region4.h"

/* Unpacks a date (year << 20 | month << 16 | day << 11 | hour << 6 | minute). */
void func_088F4AEC(unsigned int *year, unsigned int *month, unsigned int *day,
                   unsigned int *hour, unsigned int *minute, unsigned int date)
{
    *year = date >> 0x14;
    *month = date >> 0x10 & 0xf;
    *day = date >> 0xb & 0x1f;
    *hour = date >> 6 & 0x1f;
    *minute = date & 0x3f;
}
