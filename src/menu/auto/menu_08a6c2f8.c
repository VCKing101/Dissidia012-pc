#include "menu/auto_region2.h"

/* Panel6C638: shows record index in its Panel58420 (func_08A6C374 when done). */
void func_08A6C2F8(Panel6C638 *self, int index)
{
    self->field_1340 = 0;
    self->record = &(*self->data)->records[index];
    D_08A6CBC0 = self;
    func_08A581F0(&self->view, &self->record, self->field_30, func_08A6C374, 0);
    func_08A577FC(&self->view);
}
