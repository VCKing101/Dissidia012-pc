#include "battle/auto_region5.h"

/* Looks up the data for (arg1, arg3) with func_088CB164 and hands it to
 * func_08B01498. */
void func_08B01450(void *self, int arg1, int arg2, int arg3)
{
    void *data;

    data = func_088CB164(arg1, arg3);
    func_08B01498(self, data, arg2);
}
