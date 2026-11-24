/* Allocates 0x680 bytes, constructs it with func_08B036C8 and returns it (0 when allocation fails). */
extern void *func_08AF1278(int size);
extern void func_08B036C8(void *p);
void *func_08B03804(void)
{
    void *r = 0;
    void *p = func_08AF1278(0x680);
    if (p) { func_08B036C8(p); r = p; }
    return r;
}
