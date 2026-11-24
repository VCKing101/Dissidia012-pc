/* Draws an item by kind: kind 0 draws with the alternate id and a label, kind 2 runs its own callback if set. */
#include "menu/agent60_types.h"
extern void func_088D3EA0(int a, int b, int c);
extern void func_088D4664(int a, int b, const char *label);
extern const char D_08B00CB2[];

void func_08AF5858(void *unused, Item51B8 *it)
{
    if (it->kind == 0) {
        func_088D3EA0(it->f8, it->fC, it->f24);
        func_088D4664(it->fC, it->f10, D_08B00CB2);
    } else if (it->kind == 2 && it->cb != 0) {
        it->cb(it);
    } else {
        func_088D3EA0(it->f8, it->fC, it->f20);
    }
}
