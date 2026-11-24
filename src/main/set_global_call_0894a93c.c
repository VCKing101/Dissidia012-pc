/* Stores the argument in global D_09B6F478 then calls func_0894A960 with 1 as the fourth argument. */
extern int D_09B6F478;
extern void func_0894A960(int, int, int, int);
void func_0894A93C(int a0, int a1, int a2) {
    D_09B6F478 = a0;
    func_0894A960(a0, a1, a2, 1);
}
