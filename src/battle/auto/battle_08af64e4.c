#include "battle/auto_region5.h"

/* PartF6008: copies the matrix and value of handles[0] at `index` to the
 * handle `target`. */
void func_08AF64E4(PartF6008 *self, void *target, int index)
{
    char matrix[64];
    char value[4];

    if (target != 0) {
        func_088DA0A8(self->handles[0], index, matrix, value);
        func_088D9B84(target, matrix);
        func_088D9AF4(target, value);
    }
}
