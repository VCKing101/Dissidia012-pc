#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */
extern void *D_089D6A10;

BattleLookupState *func_089E95B0(unsigned int index)
{
    if (index >= 2) return 0;
    return ((BattleLookupController *)D_089D6A10)->field_38[index];
}
