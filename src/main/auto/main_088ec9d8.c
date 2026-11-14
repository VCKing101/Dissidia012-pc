#include "main/auto_region4.h"

/* Obj9CCB08: releases the handle sets, the handle and the text. */
void func_088EC9D8(Obj9CCB08 *self)
{
    func_088EC544(&self->base.field_04);
    func_088EC2F8(&self->base.field_18);
    if (self->handle != 0) {
        func_088DA060(self->handle);
        self->handle = 0;
    }
    if (self->text != 0) {
        func_0887BA5C(self->text);
        self->text = 0;
    }
}
