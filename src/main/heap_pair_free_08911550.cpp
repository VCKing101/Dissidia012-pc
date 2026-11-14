extern "C" {
extern void func_088DAB44(void *heap);
typedef struct HeapPair { void *heaps[2]; } HeapPair;
void func_08911550(HeapPair *pair)
{
    for (int i = 0; i < 2; i++) {
        if (pair->heaps[i] != 0) {
            func_088DAB44(pair->heaps[i]);
            pair->heaps[i] = 0;
        }
    }
}
}
