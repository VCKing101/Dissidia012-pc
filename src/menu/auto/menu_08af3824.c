#include "menu/auto_region3.h"

/* ScreenB01CF8: plays a script on a layout with func_088D5638. */
void func_08AF3824(ScreenB01CF8 *self, MenuLayout *layout, MenuModel **handles, unsigned char arg,
                   const unsigned short *script)
{
    func_088D5638(self->work->field_00, layout, handles, arg, script, 0, 1,
                  self->work->field_2BFC, 2);
}
