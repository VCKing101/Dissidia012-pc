#include "menu/auto_region3.h"

/* ScreenAB7CFC: builds a buffer and hands func_08AAA718's result to the five records. */
void func_08AAA6C0(ScreenAB7CFC *self, int arg)
{
    int value;
    char buffer[0x778];

    func_088CB9E8(buffer);
    value = func_08AAA718(self, buffer, arg);
    func_08AB1B8C(value, self->work->records, 5);
}
