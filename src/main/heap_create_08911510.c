extern void *func_088D0700(int heap, int size, int a, int b);
extern int D_089D7830;

typedef struct HeapPair {
    void *first;
    void *second;
} HeapPair;

void func_08911510(HeapPair *pair)
{
    int size = 0x24;
    pair->second = func_088D0700(D_089D7830, size, 0x8A1A, 0x8803);
    if (pair->second == 0) {
        return;
    }
}
