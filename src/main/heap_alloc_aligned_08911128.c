extern void *func_0885C6CC(int heap, int size, int align, int a, int b, int c);
typedef struct HeapSet { int field_00; int heap; } HeapSet;
extern HeapSet *D_089D83A0;
void *func_08911128(int size)
{
    int heap = D_089D83A0->heap;
    int zero = 0;
    int align = 0x40;
    int one = 1;
    return func_0885C6CC(heap, size, align, zero, one, 0);
}
