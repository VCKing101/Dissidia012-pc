// Advances a panel selection: after the cursor object changes, calls the virtual hook then the next/previous handler.
struct Panel {
    char d0[0xF8];
    unsigned char active;
    char d1[0x104 - 0xF9];
    int index;
    char d2[0x130 - 0x108];
    virtual void v0();
};
extern "C" {
void func_088D688C(void *a, int b);
void func_08A558E0(Panel *p);
void func_08A5590C(Panel *p);
}
extern "C" void func_08A55284(Panel *p) {
    if (p->active == 0) {
        return;
    }
    int old = p->index;
    func_088D688C((char *)p + 0x100, 0);
    if (old != p->index) {
        p->v0();
        if (old + 1 == p->index) {
            func_08A5590C(p);
        } else if (old - 1 == p->index) {
            func_08A558E0(p);
        } else if (p->index < old) {
            func_08A5590C(p);
        } else if (old < p->index) {
            func_08A558E0(p);
        }
    }
}
