// Runs the virtual hook on self, then looks up 0x27 ids from a table and stores each handle in consecutive slots, registering it with the global.
struct Self {
    char d0[0x24];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
};
extern "C" {
extern int D_089D7830;
extern unsigned short D_08A5855C[];
int func_088D0700(int g, int a, int b, int c);
void func_088CF970(int g, int h);
}
extern "C" void func_08A57D28(Self *self) {
    int i;
    unsigned short *t;
    self->v7();
    i = 0;
    t = D_08A5855C;
    do {
        unsigned short a = t[0];
        unsigned short b = t[1];
        int h = func_088D0700(D_089D7830, 0x1A, a, b);
        *(int *)((char *)self + 0x34) = h;
        func_088CF970(D_089D7830, h);
        i = i + 1;
        t = t + 2;
        self = (Self *)((char *)self + 4);
    } while (i < 0x27);
}
