#include "battle/agent27_types.h"

extern void func_088D9B84(void *res, int a);
extern void func_088D9AF4(void *res, int b);

int func_08B2A3FC(ResTables *tables, int a, int b)
{
    {
        int i;
        for (i = 0; i < 4; i++) {
            if (tables->small[i] != 0) {
                func_088D9B84(tables->small[i], a);
                func_088D9AF4(tables->small[i], b);
            }
        }
    }
    {
        int i;
        for (i = 0; i < tables->firstCount; i++) {
            if (tables->first[i] != 0) {
                func_088D9AF4(tables->first[i], b);
            }
        }
    }
    {
        int i;
        for (i = 0; i < tables->secondCount; i++) {
            if (tables->second[i] != 0) {
                func_088D9AF4(tables->second[i], b);
            }
        }
    }
    return 0;
}
