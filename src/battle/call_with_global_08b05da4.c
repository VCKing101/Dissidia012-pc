/* Forwards its arguments to func_08B1D608 with the global D_08B1D820 as the first one. */
extern int D_08B1D820;
extern void func_08B1D608(int g, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);
void func_08B05DA4(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) {
    func_08B1D608(D_08B1D820, a1, a2, a3, a4, a5, a6, a7, a8);
}
