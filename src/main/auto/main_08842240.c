#include "main/auto_region2.h"

/* Calls func_08842284 on every item func_08841FF4 walks. */
void func_08842240(void)
{
    void *item;

    item = 0;
    while (item = func_08841FF4(item), item != 0) {
        func_08842284(item);
    }
}
