#include "battle/auto_region4.h"

/* Part98058: sets field_184 (declared in auto_region3.h with a void *
 * parameter). */
int func_08A978F4(void *object, int arg)
{
    Part98058 *self = object;

    self->field_184 = arg;
    return 0;
}
