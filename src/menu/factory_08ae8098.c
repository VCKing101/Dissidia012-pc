/* Allocates 0xA694 bytes with func_08AF1278, constructs the object with func_08AE7ED4 and returns it (0 if allocation fails). */
extern void *func_08AF1278(int size);
extern void func_08AE7ED4(void *p);
void *func_08AE8098(void) {
    void *r = 0;
    void *p = func_08AF1278(0xA694);
    if (p != 0) {
        func_08AE7ED4(p);
        r = p;
    }
    return r;
}
