typedef struct Heaps088954C4 {
    int field_00;
    int field_04;
    int heap;
} Heaps088954C4;

extern Heaps088954C4 *D_089D6AD0;
void *func_0885C6CC(int heap, int size, int align, int a, int b, int c);

void *func_088954C4(int size) {
    int one = 1;
    return func_0885C6CC(D_089D6AD0->heap, size, 0, 0, one, 0);
}
