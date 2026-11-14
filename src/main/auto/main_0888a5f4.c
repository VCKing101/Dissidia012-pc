#include "main/auto_region2.h"

/* func_0891BA74 on D_089D8508, sets field_48 to 7 and queues func_0888A5B0 in all three slots. */
void func_0888A5F4(void)
{
    func_0891BA74(D_089D8508);
    D_089D6A10->field_48 = 7;
    func_08889ED4(0, func_0888A5B0, 1);
    func_08889ED4(1, func_0888A5B0, 1);
    func_08889ED4(2, func_0888A5B0, 1);
}
