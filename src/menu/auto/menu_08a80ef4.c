#include "menu/auto_region3.h"

/* Screen7EEDC: sets flag 1 and field_5E4 = 1000. */
void func_08A80EF4(void *object)
{
    ((Screen7EEDC *)object)->work->flags = ((Screen7EEDC *)object)->work->flags | 1;
    ((Screen7EEDC *)object)->work->field_5E4 = 1000;
}
