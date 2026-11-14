typedef struct Heaps088909C8 {
    int field_00;
    int field_04;
    int heap;
} Heaps088909C8;

extern Heaps088909C8 *D_089D6AC0;
void *func_0885C6CC(int heap, int size, int align, int a, int b, int c);

void *func_088909C8(int size) {
    int one = 1;
    return func_0885C6CC(D_089D6AC0->heap, size, 0, 0, one, 0);
}
