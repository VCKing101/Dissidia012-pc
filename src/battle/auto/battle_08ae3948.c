#include "battle/auto_region4.h"

/* Shuts the manager at D_08AE7CA0 down and clears the pointer. */
void func_08AE3948(void)
{
    func_08ADFA28();
    func_08AE7FA0();
    func_08AFE308(D_08AE7CA0);
    func_08AFE300();
    D_08AE7CA0 = 0;
}
