#include "battle/auto_region4.h"

typedef struct ValuesEAF84 {
    char pad_000[0x918];
    int field_918;
    int field_91C;
    int field_920;
    int field_924;
    int field_928;
} ValuesEAF84;

/* Stores the five values at field_918..field_928 (declared in
 * auto_region3.h with a void * parameter). */
void func_08AEAF84(void *object, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    ValuesEAF84 *self = object;

    self->field_918 = arg1;
    self->field_91C = arg2;
    self->field_920 = arg3;
    self->field_924 = arg4;
    self->field_928 = arg5;
}
