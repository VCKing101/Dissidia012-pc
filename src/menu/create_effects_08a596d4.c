/* Stores the owner, runs the setup, creates two effects from formatted names (the second numbered by the field at +0xA4) with their sub-handles and refreshes the panel. */
extern int D_089D6998;
extern char D_08A5B710[];
extern char D_08A5B714[];
extern char D_08A5B678[];
extern void func_08A597B0(void);
extern void func_089347E8();
extern int func_0887D868(int mgr, int a, void *b, int c, int d, int e, int f, int g);
extern int func_0887E534(int h);
extern void func_088D428C(int a, int b, void *c, int d);
void func_08A596D4(int self, int owner) {
    char buf[128];
    int h;
    int n;
    *(int *)(self + 0x14) = owner;
    func_08A597B0();
    func_089347E8(buf, D_08A5B710);
    h = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    *(int *)(self + 0x84) = h;
    h = func_0887E534(h);
    n = *(int *)(self + 0xA4);
    *(int *)(self + 0x90) = h;
    func_089347E8(buf, D_08A5B714, n + 1);
    h = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    *(int *)(self + 0x88) = h;
    h = func_0887E534(h);
    *(int *)(self + 0x94) = h;
    func_088D428C(self + 0x18, self + 0x78, D_08A5B678, 1);
}
