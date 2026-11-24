// Runs the base setup (arguments passed through), the second setup with the last three arguments, the virtual start hook and sets state 1.
struct PBase {
    char d0[8];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
};
struct Panel : PBase {
    int pad;
    int state;
};
extern "C" {
void func_08A58DD4(Panel *p, int a, int b, int c, int d, int e, int f, int g);
void func_08A58F48(Panel *p, int e, int f, int g);
}
extern "C" void func_08A5890C(Panel *self, int a, int b, int c, int d, int e, int f, int g) {
    Panel *s = self;
    int k = e;
    func_08A58DD4(s, a, b, c, d, k, f, g);
    func_08A58F48(s, k, f, g);
    s->v7();
    s->state = 1;
}
