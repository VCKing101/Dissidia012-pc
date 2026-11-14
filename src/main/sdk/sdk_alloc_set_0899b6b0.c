/* Image library allocator: set heap 0 and heap 1 malloc/free. */
#include "main/sdk_gcc1_types.h"

void *func_08934AF4(unsigned int size);
void func_08934B18(void *ptr);

void func_0899B6B0(SdkAllocFn alloc, SdkFreeFn release)
{
    if (alloc == 0)
        alloc = func_08934AF4;
    if (release == 0)
        release = func_08934B18;
    D_089DA4C0[0] = alloc;
    D_089DA4CC[0] = release;
    D_089DA4C0[1] = alloc;
    D_089DA4CC[1] = release;
}
