#include "menu/auto_region1.h"

/* Menu root: plays motion 0x17 on handle 0x19, then func_088D4F9C on its
 * prompt. */
void func_08A1EC00(MenuRoot *self, int arg, MenuModel **handles)
{
    func_088D9F5C(handles[0x19], 0x17, 1);
    func_08A1E188(self);
}
