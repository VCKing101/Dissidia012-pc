#include "menu/auto_region3.h"

/* Page76574 destructor. */
void func_08A76710(Page76574 *self)
{
    Row75D14 *row;
    int i;

    func_08A73C7C(&self->field_1D8);
    func_08A76214(&self->list);
    func_08A763A4(&self->field_1CC);
    i = 0;
    row = self->rows;
    do {
        func_08A75E7C(row);
        i = i + 1;
        row = row + 1;
    } while (i < 2);
    func_088D2F30(&self->field_00);
    func_088D3334(&self->field_04);
}
