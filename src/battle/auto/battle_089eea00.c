#include "battle/auto_region1.h"

/* PartEF178 constructor: the member at +4, then seven cells. */
PartEF178 *func_089EEA00(PartEF178 *self)
{
    self->vtable = D_089EF178;
    func_08A21C40(self->field_04);
    self->cells[0].vtable = D_089EF188;
    self->cells[0].field_00 = 0;
    self->cells[0].field_04 = 0;
    self->cells[1].vtable = D_089EF188;
    self->cells[1].field_00 = 0;
    self->cells[1].field_04 = 0;
    self->cells[2].vtable = D_089EF198;
    self->cells[2].field_00 = 0;
    self->cells[2].field_04 = 0;
    self->cells[3].vtable = D_089EF1A8;
    self->cells[3].field_00 = 0;
    self->cells[3].field_04 = 0;
    self->cells[4].vtable = D_089EF1B8;
    self->cells[4].field_00 = 0;
    self->cells[4].field_04 = 0;
    self->cells[5].vtable = D_089EF188;
    self->cells[5].field_00 = 0;
    self->cells[5].field_04 = 0;
    self->cells[6].vtable = D_089EF1C8;
    self->cells[6].field_00 = 0;
    self->cells[6].field_04 = 0;
    func_089EECCC(self);
    return self;
}
