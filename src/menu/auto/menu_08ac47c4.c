#include "menu/auto_region3.h"

/* ScreenC47C4: sets its task's callbacks and block. */
void func_08AC47C4(void *object)
{
    func_0886503C(&((ScreenC47C4 *)object)->task, func_08AC6540, func_08AC6560, 0);
    func_0886504C(&((ScreenC47C4 *)object)->task, object, 0x100);
}
