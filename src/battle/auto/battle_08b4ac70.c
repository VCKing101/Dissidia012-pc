#include "battle/auto_region5.h"

/* Runs func_08B4AD38 over D_08B4AE48's first table. */
void func_08B4AC70(unsigned int arg1, unsigned int arg2)
{
    func_08B4AD38(D_08B4AE48->table_14, D_08B4AE48->table_14 + D_08B4AE48->count_08,
                  arg2 | arg1);
}
