#include "battle/auto_region3.h"

typedef struct Result640AC {
    char pad_00[0x30];
    int field_30;
} Result640AC;

/* field_30 of what func_08AB326C returns for field_20. */
int func_08A640AC(Part64E48 *self)
{
    Result640AC *result;

    result = func_08AB326C(self->field_20);
    return result->field_30;
}
