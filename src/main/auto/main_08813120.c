#include "main/auto_region1.h"

/* Creates an object with func_088130F0 and sets it up with the three arguments. */
void *func_08813120(int arg1, int arg2, int arg3)
{
    void *object;

    object = func_088130F0();
    func_088131BC(object, arg1, arg2, arg3);
    return object;
}
