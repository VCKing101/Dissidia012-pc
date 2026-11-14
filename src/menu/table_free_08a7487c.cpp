/* Frees the shared table at D_08A74AF0 after destroying its 0x1AE entries. */
extern "C" void func_0899372C(void *array, int count, int size, void (*dtor)(void), int a, int b);
extern "C" void func_088DE0BC(void);
extern "C" void func_0885C538(int heap, void *block, int flags);

struct SharedTable {
    char pad_000[0xD70];
    char field_D70[0x1AE * 0x14];

    static void destroy(SharedTable *table)
    {
        if (table != 0) {
            func_0899372C(table->field_D70, 0x1AE, 0x14, func_088DE0BC, 0, 0);
        }
    }
};

extern "C" SharedTable *D_08A74AF0;

extern "C" {
void func_08A7487C(int heap)
{
    if (D_08A74AF0 != 0) {
        SharedTable::destroy(D_08A74AF0);
        func_0885C538(heap, D_08A74AF0, 0);
        D_08A74AF0 = 0;
    }
}
}
