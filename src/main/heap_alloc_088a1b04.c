/* Allocates size bytes from the sound heap. */
typedef struct Heaps088A1B04 {
    void *heap;
} Heaps088A1B04;

extern Heaps088A1B04 *D_089D6C88;

void *func_0885C6CC(void *heap, int size, int a, int b, int one, int c);

void *func_088A1B04(int size)
{
    void *heap = D_089D6C88->heap;
    int one = 1;
    return func_0885C6CC(heap, size, 0, 0, one, 0);
}
