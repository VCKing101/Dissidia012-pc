/* Image library allocator: release into the owning pool, else call free. */
#include "main/sdk_gcc1_types.h"

void func_0899B920(int heap, void *ptr)
{
    SdkPool *pool;
    SdkPool *prev;

    if (ptr == 0)
        return;
    prev = 0;
    for (pool = D_09AF679C[heap]; pool != 0; pool = pool->next) {
        if ((char *)ptr >= (char *)pool && (char *)ptr < pool->end) {
            if (--pool->count != 0)
                return;
            if (prev)
                prev->next = pool->next;
            else
                D_09AF679C[heap] = pool->next;
            D_089DA4CC[heap](pool);
            return;
        }
        prev = pool;
    }
    D_089DA4CC[heap](ptr);
}
