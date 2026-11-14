#include "menu/auto_region3.h"

extern char *D_089D85E8;

/* Runs func_08933C00 on D_089D85E8 and starts the screen in D_08A8B7A8. */
void func_08A969D4(void *arg)
{
    func_08933C00(D_089D85E8);
    func_08A88EDC(D_08A8B7A8, 0, -1);
}
