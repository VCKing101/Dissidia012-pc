// Stores the seventh argument, runs two setup calls and the virtual start hook, sets the started flag.
struct Panel {
    char d0[0x54];
    int a54;
    char d1[0xF4 - 0x58];
    int started;
    char d2[0x130 - 0xF8];
    virtual void v0();
};
extern "C" {
void func_08A55364(Panel *p, int a, int b, int c, int d, int e, int f);
void func_08A55560(Panel *p);
}
extern "C" void func_08A54FF4(Panel *p, int a1, int a2, int a3, int a4, int a5, int a6) {
    p->a54 = a6;
    func_08A55364(p, a1, a2, a3, a4, a5, a6);
    func_08A55560(p);
    p->v0();
    p->started = 1;
}
