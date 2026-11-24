/* Calls func_0895C9DC(*a3, 3) then clears the word the fourth argument points to. */
extern void func_0895C9DC(int, int);
void func_08959F18(int a0, int a1, int a2, int *a3) {
    func_0895C9DC(*a3, 3);
    *a3 = 0;
}
