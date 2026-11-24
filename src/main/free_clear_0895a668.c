/* Calls func_0895FC98(*a3, 3) then clears the word the fourth argument points to. */
extern void func_0895FC98(int, int);
void func_0895A668(int a0, int a1, int a2, int *a3) {
    func_0895FC98(*a3, 3);
    *a3 = 0;
}
