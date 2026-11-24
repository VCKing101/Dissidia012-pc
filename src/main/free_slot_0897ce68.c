/* Releases the object held in the word the fourth argument points to (func_08985C74(obj, 3)), then clears it. */
extern void func_08985C74(int, int);
void func_0897CE68(int a0, int a1, int a2, int *slot) {
    int *s = slot;
    int obj = *s;
    if (obj) {
        func_08985C74(obj, 3);
        *s = 0;
    }
}
