/* Allocates 0x854 bytes with func_08AF1278, constructs the object with func_08AE2960 and returns it (0 if allocation fails). */
extern void *func_08AF1278(int size);
extern void func_08AE2960(void *p);
void *func_08AE2A9C(void) {
    void *r = 0;
    void *p = func_08AF1278(0x854);
    if (p != 0) {
        func_08AE2960(p);
        r = p;
    }
    return r;
}
