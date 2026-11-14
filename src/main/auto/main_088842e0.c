#include "main/auto_region2.h"

/* Deleting destructor: releases the three handles and models, then the base. */
void func_088842E0(Obj8842E0 *self, unsigned int flags)
{
    int i;

    if (self != 0) {
        i = 0;
        do {
            if (self->handles[i] != 0) {
                func_088DA060(self->handles[i]);
                self->handles[i] = 0;
            }
            if (self->models[i] != 0) {
                func_088DAB44(self->models[i]);
                self->models[i] = 0;
            }
            i = i + 1;
        } while (i < 3);
        func_08885B14(self, 0);
        if ((flags & 1) != 0) {
            func_08885B5C(self);
        }
    }
}
