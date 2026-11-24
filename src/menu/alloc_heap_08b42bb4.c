/* Allocates size bytes from the heap held at D_08B42C28->4, aligned to 0x40. */
typedef struct H { int pad; int heap; } H;
extern H *D_08B42C28;
extern int func_0885C6CC(int heap, int size, int a, int b, int c, int d);
int func_08B42BB4(int size)
{
    H *h = D_08B42C28;
    int zero = 0;
    return func_0885C6CC(h->heap, size, 0x40, zero, 1, 0);
}
