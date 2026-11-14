#include "menu/auto_region4.h"

/* Panel26C88: clears every entry of both lists. */
void func_08B255A0(Panel26C88 *self)
{
    int i;

    func_08B257AC(self, 0x33, -1);
    func_08B257AC(self, 0x2B, -1);
    func_08B257AC(self, 0x1F, -1);
    func_08B257AC(self, 0x24, -1);
    func_08B257AC(self, 0x25, -1);
    func_08B257AC(self, 0x26, -1);
    func_08B257AC(self, 0x27, -1);
    func_08B257AC(self, 0x2C, -1);
    func_08B257AC(self, 0x20, -1);
    func_08B257AC(self, 0x21, -1);
    func_08B257AC(self, 0x22, -1);
    func_08B257AC(self, 0x23, -1);
    func_08B257AC(self, 0x28, -1);
    func_08B257AC(self, 0x29, -1);
    func_08B257AC(self, 0x2D, -1);
    func_08B257AC(self, 0x2E, -1);
    func_08B257AC(self, 0x2F, -1);
    func_08B257AC(self, 0x30, -1);
    func_08B257AC(self, 0x31, -1);
    func_08B257AC(self, 0x32, -1);
    func_08B257AC(self, 0x35, -1);
    func_08B257AC(self, 0x36, -1);
    func_08B257AC(self, 0x37, -1);
    func_08B257AC(self, 0x38, -1);
    func_08B27A98(self, 2, -1);
    func_08B27A98(self, 0x29, -1);
    for (i = 0; i < 0x24; i++) {
        func_08B27A98(self, i + 3, -1);
    }
    for (i = 0; i < 4; i++) {
        func_08B27A98(self, i + 0x2A, -1);
    }
}
