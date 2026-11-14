#include "menu/auto_region1.h"

extern const short D_08A23538[];

/* Menu root: sets up a screen's layout and clears its 0x120 handles, then
 * plays D_08A23538 on the layout. */
void func_08A1D540(MenuRoot *self, void *layout, void *handles)
{
    func_088D38F0(layout);
    func_088D3B5C(handles, 0x120);
    func_088D3910(9, layout, D_08A23538, 0, 1);
}
