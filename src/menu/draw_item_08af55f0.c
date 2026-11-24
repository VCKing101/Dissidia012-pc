/* Draws an item by kind (callback for kind 2), clearing its three trailing words in the fallback case. */
#include "menu/agent60_types.h"
extern void func_088D3CF0(int a, int b, int c);
extern void func_08AF55C4(void *owner, Item510C *it);
extern void func_08AF58E8(void *owner, Item510C *it);

void func_08AF55F0(void *owner, Item510C *it)
{
    if (it->kind == 0) {
        func_088D3CF0(it->f8, it->fC, it->f24);
    } else if (it->kind == 2 && it->cb != 0) {
        it->cb(it);
    } else {
        func_088D3CF0(it->f8, it->fC, it->f20);
        it->f3C = 0;
        it->f40 = 0;
        it->f44 = 0;
        func_08AF55C4(owner, it);
    }
    func_08AF58E8(owner, it);
}
