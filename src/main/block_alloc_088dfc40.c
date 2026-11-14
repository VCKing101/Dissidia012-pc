/* Allocates a 64-byte aligned block from the scene heap. */
typedef struct HeapOwner {
    int field_00;
    int heap;
} HeapOwner;

extern HeapOwner *D_089D7E78;
extern void *func_0885C6CC(int heap, int size, int align, int zero, int one, int flags);

void *func_088DFC40(int size)
{
    int zero = 0;

    return func_0885C6CC(D_089D7E78->heap, size, 0x40, zero, 1, 0);
}
