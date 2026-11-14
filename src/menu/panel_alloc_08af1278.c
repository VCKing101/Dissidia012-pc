typedef struct AllocGlobal {
    char pad_000[0x138];
    int field_138;
} AllocGlobal;

extern AllocGlobal *D_08B01CF8;
extern void *func_0885C6CC(int heap, int size, int a, int b, int c, int d);

void *func_08AF1278(int size)
{
    int zero = 0;
    return func_0885C6CC(D_08B01CF8->field_138, size, 0, 0, zero, 0);
}
