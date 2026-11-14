#include "battle/agent27_types.h"

extern void func_088DA060(void *res);
extern void func_08B29F80(ResTables *tables);

void func_08B29FF0(ResTables *tables)
{
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
    func_08B29F80(tables);
}
