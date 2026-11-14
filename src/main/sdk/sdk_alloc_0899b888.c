/* Image library allocator: carve from the heap's pool, else call its malloc. */
#include "main/sdk_gcc1_types.h"

void *func_0899B888(int heap, unsigned int size)
{
    SdkPool *pool;
    char *ptr = 0;

    if (size == 0)
        return ptr;
    pool = D_09AF6790[heap];
    if (pool != 0) {
        size = (size + pool->align) & ~pool->align;
        ptr = pool->cur;
        if (ptr + size <= pool->end) {
            pool->cur = ptr + size;
            pool->count++;
            return ptr;
        }
    }
    return D_089DA4C0[heap](size);
}
