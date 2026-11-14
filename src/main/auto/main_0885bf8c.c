#include "main/auto_region2.h"

/* Deleting destructor: releases the object. */
void func_0885BF8C(ActiveByte *self, unsigned int flags)
{
    if (self != 0) {
        func_0885BFE0(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
