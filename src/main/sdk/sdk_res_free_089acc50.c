#include "main/sdk_gcc2_types.h"
extern void func_089ACBC0(void *arr, int n);
SdkRes16 *func_089ACC50(SdkRes16 *res)
{
    if (res != 0) {
        void *arr = res->data;
        int n = res->fc;
        func_089ACBC0(arr, n);
        func_089ABF70(0, res->f8);
    }
    return res;
}
