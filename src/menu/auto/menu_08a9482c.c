#include "menu/auto_region3.h"

/* Obj946F0: marks a record (kind 0: flag 1, kind 1: flag 2). */
void func_08A9482C(Obj946F0 *self, Rec2CC *record, int kind)
{
    if (kind == 0) {
        record->flags = record->flags | 1;
    }
    else if (kind == 1) {
        record->flags = record->flags | 2;
    }
}
