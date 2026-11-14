/* Allocates size bytes from the stream heap. */
extern void *D_089D6D68;

void *func_0885C6CC(void *heap, int size, int a, int b, int one, int c);

void *func_088A5270(int size)
{
    void *heap = D_089D6D68;
    int one = 1;
    return func_0885C6CC(heap, size, 0, 0, one, 0);
}
