/* Stores the argument in global D_09B6F478 then calls func_08949E2C with 1 as the fourth argument. */
extern int D_09B6F478;
extern void func_08949E2C(int, int, int, int);
void func_08949E08(int a0, int a1, int a2) {
    D_09B6F478 = a0;
    func_08949E2C(a0, a1, a2, 1);
}
