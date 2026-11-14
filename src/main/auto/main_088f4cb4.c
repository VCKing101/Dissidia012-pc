#include "main/auto_region4.h"

void func_088F4CB4(Obj8F4C4C *self, int year, int month, int day, int hour, int minute)
{
    unsigned int date;

    date = func_088F4AC8(year, month, day, hour, minute);
    self->date_18 = date;
}
