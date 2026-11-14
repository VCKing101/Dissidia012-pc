#include "menu/auto_region3.h"

/* View8E3B4: hides the three windows, then shows the one of this kind. */
void func_08A8CE88(View8E3B4 *self, unsigned char shown, int kind)
{
    func_08A8CF34(self, 0);
    func_08A8D098(self, 0);
    func_08A8D0D8(self, 0);
    if (kind == 0) {
        func_08A8CF34(self, shown);
    }
    else if (kind == 1) {
        func_08A8D098(self, shown);
    }
    else if (kind == 2) {
        func_08A8D0D8(self, shown);
    }
}
