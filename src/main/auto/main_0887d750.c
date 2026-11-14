#include "main/auto_region2.h"

/* Frees block (with its 16-byte header) to the heap stored in that header. */
void func_0887D750(void *owner, char *block)
{
    void *heap;

    func_08935524(&heap, block + -0x10, 4);
    func_0885C538(heap, block + -0x10, 0);
}
