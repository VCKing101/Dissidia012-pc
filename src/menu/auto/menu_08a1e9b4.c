#include "menu/auto_region1.h"

/* Menu root: plays motions 4/5 and 2/3 (by field_25B94) on handles 0x20 and
 * 0x21, then func_08A1EA3C. */
void func_08A1E9B4(MenuRoot *self, MenuModel **handles)
{
    int first;
    int second;

    first = 4;
    second = 2;
    if (self->work->field_25B94 == 0) {
        first = 5;
    } else {
        second = 3;
    }
    func_088D9F5C(handles[0x20], first, 1);
    func_088D9F5C(handles[0x21], second, 1);
    func_08A1EA3C(self, handles);
}
