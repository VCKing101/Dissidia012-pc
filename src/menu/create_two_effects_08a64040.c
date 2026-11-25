/* Runs the base routine, creates two effects from formatted names (the second numbered by the field at +0x80) and refreshes the panel. */
extern int D_089D6998;
extern char D_08A682BC[];
extern char D_08A682C0[];
extern char D_08A681D8[];
extern void func_08A64100(void);
extern void func_089347E8(void *buf, const void *fmt, ...);
extern int func_0887D868(int mgr, int a, void *b, int c, int d, int e, int f, int g);
extern void func_088D428C(int a, int b, void *c, int d);
void func_08A64040(int self) {
    char buf[128];
    int h;
    int n;
    func_08A64100();
    func_089347E8(buf, D_08A682BC);
    h = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    n = *(int *)(self + 0x80);
    *(int *)(self + 0x6C) = h;
    func_089347E8(buf, D_08A682C0, n + 1);
    h = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    *(int *)(self + 0x70) = h;
    func_088D428C(self + 0x1C, self + 0x64, D_08A681D8, 1);
}
