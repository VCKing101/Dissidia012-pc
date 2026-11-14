#include "menu/auto_region3.h"

/* View8E3B4: steps the prompt; true while func_08A8E7E8 is nonzero. */
unsigned char func_08A8EE44(View8E3B4 *self)
{
    int result;

    func_08A8E74C(self);
    result = func_08A8E7E8(self);
    return result != 0;
}
