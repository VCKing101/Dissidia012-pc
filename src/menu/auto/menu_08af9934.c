#include "menu/auto_region4.h"

/* Moves the selection of a Block9880 row back and redraws it. */
void func_08AF9934(void *object, void *block)
{
    func_088D9F5C(((Block9880 *)block)->handles[((Block9880 *)block)->index + 1], 2, 1);
    func_088D9F5C(((Block9880 *)block)->handles[((Block9880 *)block)->index], 6, 1);
    ((Block9880 *)block)->state = 2;
    ((Block9880 *)block)->field_F8 = 0;
    func_08AF9880(object, block);
}
