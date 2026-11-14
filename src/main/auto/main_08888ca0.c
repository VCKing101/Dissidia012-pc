#include "main/auto_region2.h"

/* When func_08B12C48 reports D_08B12DE0 done, calls func_08B12BD4 and sets field_48 to 7. */
void func_08888CA0(Flow9D6A10 *self)
{
    int done;

    done = func_08B12C48(D_08B12DE0);
    if (done != 0) {
        func_08B12BD4(D_08B12DE0);
        self->field_48 = 7;
    }
}
