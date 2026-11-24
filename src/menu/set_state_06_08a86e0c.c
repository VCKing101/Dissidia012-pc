/* Runs the base routine, calls the manager routine, then sets the owner's state word to 6. */
extern int D_089D69E8;
extern void func_088853CC(int a, int b);
extern void func_08A87040(int p);
void func_08A86E0C(int self) {
    int g;
    int zero;
    func_08A87040(self);
    g = D_089D69E8;
    zero = 0;
    func_088853CC(g, zero);
    *(int *)(*(int *)(self + 0x14C) + 0x910) = 6;
}
