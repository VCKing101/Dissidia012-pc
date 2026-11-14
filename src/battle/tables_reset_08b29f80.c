#include "battle/agent27_types.h"

void func_08B29F80(ResTables *tables)
{
    tables->active = 0;
    {
        int i;
        for (i = 0; i < 4; i++) {
            tables->small[i] = 0;
        }
    }
    {
        int i;
        for (i = 0; i < 50; i++) {
            tables->first[i] = 0;
        }
    }
    {
        int i;
        for (i = 0; i < 50; i++) {
            tables->second[i] = 0;
        }
    }
    tables->field_1A4 = 0;
    tables->firstCount = 0;
    tables->secondCount = 0;
    tables->ctx = 0;
    tables->field_1B4 = 0;
}
