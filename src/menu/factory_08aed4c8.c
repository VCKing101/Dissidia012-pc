/* Allocates 0x23F4 bytes with func_08AF1278, constructs the object with func_08AED360 and returns it (0 if allocation fails). */
extern void *func_08AF1278(int size);
extern void func_08AED360(void *p);
void *func_08AED4C8(void) {
    void *r = 0;
    void *p = func_08AF1278(0x23F4);
    if (p != 0) {
        func_08AED360(p);
        r = p;
    }
    return r;
}
