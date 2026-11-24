/* Allocates 0xDB40 bytes with func_08AF1278, constructs the object with func_08AD0E2C and returns it (0 if allocation fails). */
extern void *func_08AF1278(int size);
extern void func_08AD0E2C(void *p);
void *func_08AD0FD4(void) {
    void *r = 0;
    void *p = func_08AF1278(0xDB40);
    if (p != 0) {
        func_08AD0E2C(p);
        r = p;
    }
    return r;
}
