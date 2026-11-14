#include "main/auto_region2.h"

/* Sets field_48 to 7 and queues func_0888ABF0 in all three slots. */
void func_0888AC14(void)
{
    D_089D6A10->field_48 = 7;
    func_08889ED4(0, func_0888ABF0, 1);
    func_08889ED4(1, func_0888ABF0, 1);
    func_08889ED4(2, func_0888ABF0, 1);
}
