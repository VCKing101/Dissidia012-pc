#include "menu/auto_region3.h"

extern unsigned char D_08A7B9F0[];

/* Deleting destructor of three 0x14-byte rows (row destructor D_08A7B9F0). */
void func_08A7BA60(Rows74E80 *self, unsigned int flags)
{
    if ((self != 0) && (func_0899372C(self, 3, sizeof(Row74B28), D_08A7B9F0, 0, 0), (flags & 1) != 0)) {
        func_08992F04(self);
    }
}
