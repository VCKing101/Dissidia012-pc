/* Allocates 0x46B0 bytes with func_08AF1278, constructs the object with func_08ADCB50 and returns it (0 if allocation fails). */
extern void *func_08AF1278(int size);
extern void func_08ADCB50(void *p);
void *func_08ADCCF0(void) {
    void *r = 0;
    void *p = func_08AF1278(0x46B0);
    if (p != 0) {
        func_08ADCB50(p);
        r = p;
    }
    return r;
}
