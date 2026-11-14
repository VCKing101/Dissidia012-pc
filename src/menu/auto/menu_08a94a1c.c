#include "menu/auto_region3.h"

/* Obj946F0: marks a record with flag 4. */
void func_08A94A1C(Obj946F0 *self, Rec2CC *record)
{
    record->flags = record->flags | 4;
}
