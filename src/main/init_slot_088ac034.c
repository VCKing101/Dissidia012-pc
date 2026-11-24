// Initialises a battle-slot object: field 0x210 = 11, 0xEB8 = arg, copies 0x14 bytes of D_089CB3FC to 0x22C, 0x7D0 = 1.
#include "main/agent44_types.h"
extern void func_08935BB4(void *, const void *, int);
extern char D_089CB3FC[];
void func_088AC034(Slot44 *obj, int v)
{
    obj->f210 = 11;
    obj->fEB8 = v;
    func_08935BB4(obj->f22C, D_089CB3FC, 0x14);
    obj->f7D0 = 1;
}
