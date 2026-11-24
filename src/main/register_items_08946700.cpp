/* Resets two subsystems, then registers twelve 2-byte entries with the same group. */
extern short D_089D9278[];
extern char D_089D9218[];
extern "C" void func_089A0C8C(int b);
extern "C" void func_0896FB5C(int a);
extern "C" void func_089A0520(int a, int b, int c, void *p, void *q);
extern "C" int func_08946700(int a, int b)
{
    func_089A0C8C(b);
    func_0896FB5C(a);
    for (int i = 0; i < 12; i++) {
        func_089A0520(1, 0x980, 2, &D_089D9278[i], D_089D9218);
    }
    return 0;
}
