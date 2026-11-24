/* Calls func_0896023C(*a3, 3) then clears the word the fourth argument points to. */
extern void func_0896023C(int, int);
void func_0895A994(int a0, int a1, int a2, int *a3) {
    func_0896023C(*a3, 3);
    *a3 = 0;
}
