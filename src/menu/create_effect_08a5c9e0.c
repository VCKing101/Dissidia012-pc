/* Runs the base routine, looks up a record by the stored (signed byte, short) ids and, when found, creates and shows its effect. */
extern int D_089D8530;
extern char D_08A5DBD8[];
extern void func_08A5CA8C(int self);
extern int func_08925C90(int g, int a, int b);
extern int func_0887B3EC(int mgr, int a, int b, int c, int d, int e);
extern int func_0887E534(int h);
extern void func_088D428C(int a, int b, void *c, int d);
void func_08A5C9E0(int self, int mgr) {
    int rec;
    int h;
    func_08A5CA8C(self);
    rec = func_08925C90(D_089D8530, (char)*(int *)(self + 0x4C), (short)*(int *)(self + 0x50));
    if (rec == 0) {
        return;
    }
    h = func_0887B3EC(mgr, 8, 0, *(short *)(rec + 8), 0, 0);
    *(int *)(self + 0x40) = h;
    h = func_0887E534(h);
    *(int *)(self + 0x44) = h;
    func_088D428C(self + 4, self + 0x40, D_08A5DBD8, 1);
}
