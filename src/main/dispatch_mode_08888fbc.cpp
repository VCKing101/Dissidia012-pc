// Call the handler for the current mode (0, 1, 3, 2) with the argument 1.
struct Glob { char pad[0x740]; int mode; };
extern "C" Glob *D_089D6A10;
extern "C" void func_088FEB60(int v);
extern "C" void func_088FEAEC(int v);
extern "C" void func_088FEC30(int v);
extern "C" void func_088FEA84(int v);
struct E {
    static bool is(int a, int b) { return a == b; }
};
extern "C" void func_08888FBC(void)
{
    int m = D_089D6A10->mode;
    if (E::is(m, 0)) {
        func_088FEB60(1);
    } else if (E::is(m, 1)) {
        func_088FEAEC(1);
    } else if (E::is(m, 3)) {
        func_088FEC30(1);
    } else if (E::is(m, 2)) {
        func_088FEA84(1);
    }
}
