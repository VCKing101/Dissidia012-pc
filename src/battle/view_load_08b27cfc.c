typedef struct ViewLoader {
    char pad00[0x14];
    char request[4];    /* 0x14 */
} ViewLoader;

extern void func_08B27AE0(ViewLoader *loader);
extern int func_088908E0(void);
extern int func_0887AC70(void *request, int a, int b, int heap);

int func_08B27CFC(ViewLoader *loader, int a, int b)
{
    func_08B27AE0(loader);
    if (func_0887AC70(loader->request, a, b, func_088908E0()) == 0) {
        return -1;
    }
    return 0;
}
