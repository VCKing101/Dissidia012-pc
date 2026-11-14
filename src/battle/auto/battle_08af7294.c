#include "battle/auto_region5.h"

/* Copies the matrix and value of `handle` at `index` to the handle
 * `target`. */
void func_08AF7294(void *self, void *target, void *handle, int index)
{
    char matrix[64];
    int value;

    if (target != 0) {
        func_088DA0A8(handle, index, matrix, &value);
        func_088D9B84(target, matrix);
        func_088D9B48(target, value);
    }
}
