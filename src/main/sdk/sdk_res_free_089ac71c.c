#include "main/sdk_gcc2_types.h"

extern void func_089AC75C(void *arr, int n);
extern void func_089AC68C(void *arr, int n);

SdkRes16 *func_089AC71C(SdkRes16 *res)
{
    if (res != 0) {
        void *arr;
        int n;
        func_089AC75C(res->data, 1);
        arr = res->f8;
        n = res->fc;
        func_089AC68C(arr, n);
    }
    return res;
}
