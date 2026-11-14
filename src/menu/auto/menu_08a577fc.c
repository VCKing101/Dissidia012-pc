#include "menu/auto_region2.h"

/* Panel58420: shows its record in the six items, state 6. */
void func_08A577FC(Panel58420 *self)
{
    if (self->handles[4] != 0) {
        ((MenuModelView *)self->handles[4])->field_0E = 1;
    }
    if (self->handles[2] != 0) {
        func_088D9CFC(self->handles[2]);
    }
    func_08A566D4(&self->field_100, (*self->record)->field_08);
    func_08A566D4(&self->field_33C, (*self->record)->field_0C);
    func_08A5613C(&self->field_AB0, (*self->record)->field_00);
    func_08A55B58(&self->field_578, (*self->record)->field_06);
    func_08A56B78(&self->field_DFC, (*self->record)->field_02);
    func_08A56F60(&self->field_F54, (*self->record)->field_04, (*self->record)->field_10);
    self->state = 6;
}
