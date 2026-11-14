#include "menu/auto_region3.h"

/* View8E3B4: shows its second set of entries. */
void func_08A8C298(View8E3B4 *self)
{
    int i;

    func_08A8D480(self, 0x12, -1);
    func_08A8D480(self, 10, 0x34);
    func_08A8D480(self, 0x10, 0x35);
    func_08A8D480(self, 0x7F, -1);
    func_08A8D480(self, 0x80, -1);
    func_08A8D480(self, 0x81, -1);
    func_08A8D480(self, 0x82, -1);
    i = 0;
    do {
        func_08A8D480(self, i + 0x6B, -1);
        i = i + 1;
    } while (i < 0x14);
    func_08A8D480(self, 0x86, 0x71);
    func_08A8D480(self, 0x87, 0x72);
    func_08A8DC60(self, 0x17, -1);
    func_08A8DC60(self, 0x10, -1);
    func_08A8DC60(self, 0x11, -1);
}
