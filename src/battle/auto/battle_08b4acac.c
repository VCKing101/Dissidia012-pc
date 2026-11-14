#include "battle/auto_region5.h"

/* Runs func_08B4AD7C over D_08B4AE48's second table. */
void func_08B4ACAC(unsigned int arg1, unsigned int arg2)
{
    func_08B4AD7C(D_08B4AE48->table_18, D_08B4AE48->table_18 + D_08B4AE48->count_0C,
                  arg2 | arg1);
}
