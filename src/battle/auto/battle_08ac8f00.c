#include "battle/auto_region4.h"

/* PartC8F00 constructor (no vtable): resets it, then calls func_08AC9468. */
PartC8F00 *func_08AC8F00(PartC8F00 *self)
{
    func_08AC8F84(self);
    func_08AC9468(self);
    return self;
}
