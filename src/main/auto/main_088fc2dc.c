#include "main/auto_region4.h"

/* Obj8FBBE0: steps the scripts for flags 2 and 1. */
void func_088FC2DC(Obj8FBBE0 *self)
{
    Work8FBBE0 *work;

    work = self->work;
    if ((work->flags & 2) != 0) {
        func_088D3F00(work->layout, work->handles, D_089CCE9C);
        func_088FBA24(self);
        func_088FBCC0(self);
        func_088FBE60(self);
        work = self->work;
    }
    if ((work->flags & 1) != 0) {
        func_088D3F00(work->layout, work->handles, D_089CCE28);
    }
}
