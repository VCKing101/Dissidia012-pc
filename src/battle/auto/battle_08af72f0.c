#include "battle/auto_region5.h"

/* Copies the matrix and value of `handle` at `index` to the model
 * `target`. */
void func_08AF72F0(void *self, void *target, void *handle, int index)
{
    char matrix[64];
    int value;

    if (target != 0) {
        func_088DA0A8(handle, index, matrix, &value);
        func_0887B778(target, matrix);
        func_0887B820(target, value);
    }
}
