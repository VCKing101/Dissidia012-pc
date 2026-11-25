typedef struct H { unsigned char u[0xECDC00]; int heap_a; int heap_b; } H;
extern H *D_089D69A0;
void *func_0885C6CC(int heap, int size, int align, int a, int b, int c);
void *func_088820D8(int size) {
    int zero = 0;
    int heap = D_089D69A0->heap_b;
    int one = 1;
    return func_0885C6CC(heap, size, 0, zero, one, 0);
}
