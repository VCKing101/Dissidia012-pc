#include "battle/agent27_types.h"

extern void func_088DA060(void *res);
extern int func_08B2A1A8(ResTables *tables);

int func_08B2A220(ResTables *tables)
{
    if (func_08B2A1A8(tables) == 0) {
        return -1;
    }
    {
        int i;
        for (i = 0; i < 50; i++) {
            if (tables->second[i] != 0) {
                func_088DA060(tables->second[i]);
                tables->second[i] = 0;
            }
        }
    }
    {
        int i;
        for (i = 0; i < 50; i++) {
            if (tables->first[i] != 0) {
                func_088DA060(tables->first[i]);
                tables->first[i] = 0;
            }
        }
    }
    {
        int i;
        for (i = 0; i < 4; i++) {
            if (tables->small[i] != 0) {
                func_088DA060(tables->small[i]);
                tables->small[i] = 0;
            }
        }
    }
    tables->field_1A4 = 0;
    return 0;
}
