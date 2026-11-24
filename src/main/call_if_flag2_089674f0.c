/* If bit 1 of the fifth argument is set, calls func_0896C160 with the word at the fourth. */
extern void func_0896C160(int);
void func_089674F0(int a0, int a1, int a2, int *a3, int flags) {
    if (flags & 2) { func_0896C160(*a3); }
}
