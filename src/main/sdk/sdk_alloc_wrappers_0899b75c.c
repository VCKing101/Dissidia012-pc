/* Image library allocator: per-heap malloc and free wrappers. */
#include "main/sdk_gcc1_types.h"

void *func_0899B75C(unsigned int size)
{
    if (size == 0)
        return 0;
    return func_0899B888(0, size);
}

void *func_0899B794(unsigned int size)
{
    if (size == 0)
        return 0;
    return func_0899B888(1, size);
}

void *func_0899B7CC(unsigned int size)
{
    if (size == 0)
        return 0;
    return func_0899B888(2, size);
}

void func_0899B804(void *ptr)
{
    if (ptr)
        func_0899B920(0, ptr);
}

void func_0899B830(void *ptr)
{
    if (ptr)
        func_0899B920(1, ptr);
}

void func_0899B85C(void *ptr)
{
    if (ptr)
        func_0899B920(2, ptr);
}
