#include "battle/auto_region5.h"

/* PartF6008: copies the matrix and value of handles[0] at `index` to the
 * model `target`. */
void func_08AF654C(PartF6008 *self, void *target, int index)
{
    char matrix[64];
    char value[4];

    if (target != 0) {
        func_088DA0A8(self->handles[0], index, matrix, value);
        func_0887B970(target);
        func_0887B9C4(target);
        func_0887B778(target, matrix);
        func_0887B828(target, value);
    }
}
