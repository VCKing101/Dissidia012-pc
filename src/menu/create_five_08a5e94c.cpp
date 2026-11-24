// Runs the virtual hook on self, creates five effects (ids 0x56..0x5A) from the manager, stores their handles and refreshes the panel.
struct Self {
    char d0[0x24];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9();
};
extern "C" {
extern char D_08A5EFD4[];
int func_0887B3EC(int mgr, int a, int b, int c, int d, int e);
void func_088D428C(int a, int b, void *c, int d);
}
extern "C" void func_08A5E94C(Self *self, int mgr) {
    int h;
    self->v9();
    h = func_0887B3EC(mgr, 8, 0, 0x56, 0, 0);
    *(int *)((char *)self + 0x84) = h;
    h = func_0887B3EC(mgr, 8, 0, 0x57, 0, 0);
    *(int *)((char *)self + 0x88) = h;
    h = func_0887B3EC(mgr, 8, 0, 0x58, 0, 0);
    *(int *)((char *)self + 0x8C) = h;
    h = func_0887B3EC(mgr, 8, 0, 0x59, 0, 0);
    *(int *)((char *)self + 0x90) = h;
    h = func_0887B3EC(mgr, 8, 0, 0x5A, 0, 0);
    *(int *)((char *)self + 0x94) = h;
    func_088D428C((int)self + 0x40, (int)self + 0x84, D_08A5EFD4, 1);
}
