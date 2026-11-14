#include "menu/auto_region3.h"

extern const unsigned short D_08B009FC[];

/* ScreenB01CF8: plays D_08B009FC on a layout and updates it. */
void func_08AF371C(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles)
{
    func_088D5638(self->work->field_00, layout, handles, 5, D_08B009FC, 0, 1,
                  self->work->field_2BFC, 2);
    func_08AF385C(self, layout, handles, -1);
    func_08AF3930(self, layout, handles);
}
