#include "main/sdk_gcc2_types.h"
typedef struct SdkRes12 { unsigned short ref; unsigned short pad2; void *data; unsigned short n; } SdkRes12;
extern void func_089AC884(void *arr, int n);
SdkRes12 *func_089AC914(SdkRes12 *res)
{
    if (res != 0) {
        void *arr = res->data;
        int n = res->n;
        func_089AC884(arr, n);
    }
    return res;
}
