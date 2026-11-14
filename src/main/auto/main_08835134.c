#include "main/auto_region2.h"

/* func_08811F44 on field_390->field_18; 0 without one. */
int func_08835134(ActorView *self, unsigned int arg1, int arg2)
{
    void *object;

    object = self->field_390->field_18;
    if (object == 0) {
        return 0;
    } else {
        return func_08811F44(object, arg1, arg2);
    }
}
