typedef struct Heaps08882118 {
    unsigned char unknown_000000[0xECDC00];
    int heap_a;
    int heap_b;
} Heaps08882118;

extern Heaps08882118 *D_089D69A0;
void *func_0885C6CC(int heap, int size, int align, int a, int b, int c);

void *func_08882118(int size, int align) {
    int zero = 0;
    return func_0885C6CC(D_089D69A0->heap_b, size, align, zero, 1, 0);
}
