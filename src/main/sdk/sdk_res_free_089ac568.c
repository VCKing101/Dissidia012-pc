#include "main/sdk_gcc2_types.h"

extern void func_089BB4A0(void *data);

SdkRes16 *func_089AC568(SdkRes16 *res)
{
    if (res != 0) {
        func_089BB4A0(res->data);
    }
    return res;
}
