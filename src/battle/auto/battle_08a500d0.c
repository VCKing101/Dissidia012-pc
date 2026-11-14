#include "battle/auto_region3.h"

void func_0886BB44(void *target, int value, unsigned char arg2, unsigned char arg3, int arg4);
void *func_08895420(void *object);
unsigned int func_088985F0(unsigned int value, int arg);
unsigned int func_08899300(void);

/* Passes func_088985F0's value and !flag to func_0886BB44 for the object
 * func_08895420 finds at field_3A0 of D_089D6AC8. */
void func_08A500D0(unsigned char flag)
{
    unsigned int value;
    void *target;

    value = func_08899300();
    value = func_088985F0(value, 0);
    target = func_08895420(D_089D6AC8->field_3A0);
    if (target != 0) {
        func_0886BB44(target, value, flag == 0, 1, 1);
    }
}
